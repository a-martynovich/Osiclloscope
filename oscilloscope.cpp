#include "oscilloscope.h"
#include <qmath.h>
#include <boost/circular_buffer.hpp>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>
#include <QtDebug>
#include <QThread>
#include <QFile>
#define USE_QWT
#ifdef USE_QWT
#include <qwt/qwt_plot.h>
#include <qwt/qwt_plot_curve.h>
#include <qwt/qwt_series_data.h>
#include <qwt/qwt_curve_fitter.h>
#endif

template<typename T>
class MyDataStream {
	QMutex m;
public:
	class MyListener {
	public:
		virtual void onData(MyDataStream<T>*, int)=0;
	};
	boost::circular_buffer<T> buf;
	MyListener* listener;
	MyDataStream(MyListener* l=0, unsigned s=100000) {
		buf.set_capacity(s);
		listener = l;
	}
	void push(const QVector<T>& p) {
		//QMutexLocker locker(&m);
		int _j=0;
		for(int i=0; i<p.size(); i++) {
			if(!buf.full()) {
				buf.push_back(p[i]);
				_j++;
			}
			else {
				qDebug()<<"MyDataStream is full"<<endl;
				break;
			}
		}
		if(listener)
			listener->onData(this, buf.size());
	}
	int avail() {
		//QMutexLocker locker(&m);
		return buf.size();
	}
	int pop(QVector<T>& p) {
		//QMutexLocker locker(&m);
		int _j=0;
		for(int i=0; i<p.size(); i++) {
			if(!buf.empty()) {
				p[i] = buf.front();
				buf.pop_front();
				_j++;
			}
			else {
				qDebug()<<"MyDataStream is empty"<<endl;
				break;
			}
		}
		return _j;
	}
};

class MyThread: public QThread {
	QMutex m;
	double Fd, F, sin_phase;
	unsigned np,period,cnt;
	QVector<qreal> points;
public:
	MyDataStream<qreal>* ds;
	MyThread(QObject* p=0): QThread(p) {
		sin_phase=0;
		cnt=0;
	}
	void set_Fd(double v) {
		//QMutexLocker locker(&m);
		Fd = v;
	}
	void set_F(double v) {
		//QMutexLocker locker(&m);
		F = v;
	}
	void set_np(unsigned v) {
		//QMutexLocker locker(&m);
		np = v;
		points.resize(np);
	}
	void set_period(unsigned v) {
		period = v;
	}
	void run() {
		while(true) {
			//m.lock();
			//m.unlock();
			for(int i=0; i<points.size(); i++) {
				qreal x = cnt++, y = qSin(2*M_PI*F/Fd*x/*+sin_phase*/);
				points[i] = y;
			}
			sin_phase += 2*M_PI*F/Fd*(points.size());
			if(sin_phase >= 2*M_PI) sin_phase-=2*M_PI;
			ds->push(points);
			usleep(period);
//			qDebug()<<"gen";
		}
	}
};

enum AverageMode {
	PICK_FROM_THE_MIDDLE, SUM_AND_DIVIDE
};
struct Oscilloscope::Private: public MyDataStream<qreal>::MyListener {
#ifdef USE_QWT
	QwtPlotCurve* curve;
#else
	QgtCurve* curve;
#endif
	double sin_phase, sample_total_time, t;
	int fps, real_fps, samplerate, Nm, No,counter, chunksize;
	QVector<qreal> points_x, points_y, chunk;
	boost::circular_buffer<qreal> cbuffer;
	MyDataStream<qreal>* ds_generator;
	qreal average(const QVector<qreal>& v, AverageMode mode) {
		switch(mode) {
			case PICK_FROM_THE_MIDDLE: {
				return v[v.size()/2];
			} break;
			case SUM_AND_DIVIDE: {
				qreal a=0;
				for(int i=0; i<v.size(); i++) {
					a+=v[i];
				}
				return a/(qreal)v.size();
			} break;
		}
	}
	void onData(MyDataStream<qreal>* ds, int) {
		QMutexLocker ml(&m);
		while(ds->avail() >= chunksize) {
			//qDebug()<<"Oscilloscope::onData";
			chunk.resize(chunksize);
			ds->pop(chunk);
			qreal new_sample = average(chunk, PICK_FROM_THE_MIDDLE);// PICK_FROM_THE_MIDDLE);
			cbuffer.push_back(new_sample);
		}
	}
	MyThread* th;
	QFile* f;
	QTextStream* fs;
	QMutex m;
};

Oscilloscope::Oscilloscope(QWidget *parent)
    : QWidget(parent) {
	d = new Private;
	d->f = new QFile("out.log",this);
	d->f->open(QFile::WriteOnly);
	d->fs = new QTextStream(d->f);
	ui.setupUi(this);
#ifdef USE_QWT
	d->curve = new QwtPlotCurve();
	d->curve->setRenderHint(QwtPlotCurve::RenderAntialiased);
	d->curve->setCurveFitter(new QwtSplineCurveFitter());
	d->curve->attach(ui.plotWidget);
#else
	d->curve = new QgtCurve(this);
	d->curve->setSignalReplot(false);
	ui.plotWidget->setAntiAliasing(true);
	ui.plotWidget->attach(d->curve);
	//ui.plotWidget->setAutoReplot(true);
#endif
	ui.toolButton->setIcon(QIcon::fromTheme("edit-undo"));

	d->fps = 20;
	d->No = 800;
	d->samplerate = 8000;
	d->t = 1;
	d->counter = 0;
	d->sample_total_time = 0.0;
	d->chunksize = d->samplerate/(qreal)d->No*d->t;
	d->points_x.resize(d->No);
	d->points_y.resize(d->No);
	int chunksize_rmdr = d->samplerate%(d->No*(int)d->t);
	d->Nm = d->No-ceil(chunksize_rmdr/((qreal)d->No*d->t));	// FIXME: если chunksize_rmdr!=0, Nm должно быть дургим?
	qDebug()<<"Nm"<< d->Nm << "chunksize"<< d->chunksize
			<<"remainder"<< chunksize_rmdr/(qreal)d->samplerate;

	d->real_fps = 1000./(qFloor(1000.0/(double)d->fps));
	startTimer(qFloor(1000.0/(double)d->fps));

	d->sin_phase = 0;
	d->cbuffer.set_capacity(d->Nm);
	//d->points.resize(d->Nm);
	d->th = new MyThread(this);
	d->th->set_F(600);
	d->th->set_Fd(d->samplerate);
	d->th->set_np(300);
	d->th->set_period(1000*100);
	d->ds_generator = new MyDataStream<qreal>(d);
	d->th->ds = d->ds_generator;
#ifdef USE_QWT
	//ui.plotWidget->setAxisAutoScale(QwtPlot::xBottom, false);
	//ui.plotWidget->setAxisScale(QwtPlot::xBottom,0,d->t);
	ui.plotWidget->setAxisScale(QwtPlot::yLeft,-1,1);
#else
	ui.plotWidget->horizontalAxis()->setMinValue(0);
	ui.plotWidget->horizontalAxis()->setMaxValue(d->t);
	ui.plotWidget->verticalAxis()->setMinValue(-1);
	ui.plotWidget->verticalAxis()->setMaxValue(1);
	ui.plotWidget->verticalAxis()->setPrecision(2);
#endif
	d->th->start();
}

Oscilloscope::~Oscilloscope() {

}

void Oscilloscope::timerEvent(QTimerEvent* e) {
	QMutexLocker ml(&d->m);
	//d->curve->replot();
	//(*d->fs)<<"<cut>"<<"\n";
	//qDebug()<<"update";
	for(int i=0; i<d->cbuffer.size(); i++) {
		d->points_y[i] = d->cbuffer[i];
		d->points_x[i] = d->t/(qreal)d->Nm*(qreal)i;
		//qDebug()<<d->points[i];
		//(*d->fs)<<d->points_x[i]<<" "<<d->points_y[i]<<"\n";
	}
#ifdef USE_QWT
	d->curve->setSamples(d->points_x.data(), d->points_y.data(), d->cbuffer.size());
#else
	d->curve->setPoints(d->points_x.data(), d->points_y.data(), d->cbuffer.size());
#endif
	ui.plotWidget->replot();
}
