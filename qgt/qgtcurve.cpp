#include "qgtplot.h"
#include "qgtcurve.h"

QgtCurve::QgtCurve(QObject* parent) 
	: QObject(parent),
	  visible_(true), 
	  style_(LINE), color_(Qt::black), width_(1.0),
	  pPlot_(nullptr)
{

}

QgtCurve::~QgtCurve()
{
	detach();
}

void QgtCurve::show()
{
	setVisible(true);
}

void QgtCurve::hide()
{
	setVisible(false);
}

bool QgtCurve::isVisible() const
{
	return visible_;
}

void QgtCurve::setVisible(bool b)
{
	visible_ = b;

	emit redisplay();
}

QgtCurve::CurveStyle QgtCurve::type() const
{
	return style_;
}

void QgtCurve::setType(CurveStyle style)
{
	style_ = style;

	emit redisplay();
}

const QColor& QgtCurve::color() const
{
	return color_;
}

void QgtCurve::setColor(const QColor& color)
{
	color_ = color;

	emit redisplay();
}

qreal QgtCurve::width() const
{
	return width_;
}

void QgtCurve::setWidth(qreal width)
{
	width_ = width;

	emit redisplay();
}

const QVector<QPointF>& QgtCurve::points() const
{
	return points_;
}

void QgtCurve::setPoints(const QVector<QPointF>& points)
{
	points_ = points;
	if(signal_replot_)
		emit redisplay();
}

void QgtCurve::setPoints(qreal* pXData, qreal* pYData, size_t n)
{
	points_.clear();

	for(int i = 0; i < n; ++i) {
		points_.append(QPointF(pXData[i], pYData[i]));
	}
	if(signal_replot_)
		emit redisplay();
}
void QgtCurve::setSignalReplot(bool b) {
	signal_replot_ =b;
}

QgtPlot* QgtCurve::plot()
{
    return pPlot_;
}

const QgtPlot* QgtCurve::plot() const
{
	return pPlot_;
}

// QgtPlot::replot()
void QgtCurve::replot()
{
	if(nullptr != pPlot_) pPlot_->replot();
}

/*
	This method will attach a QgtCurve to the QgtPlot argument. It will first
	detach the QwtCurve from any plot from a previous call to attach (if
	necessary). If a NULL argument is passed, it will detach from any QwtPlot it
	was attached to.
*/
void QgtCurve::attach(QgtPlot* pPlot)
{
	if(pPlot_ == pPlot) return;

	if(nullptr != pPlot_) {
		detach();
	}

	if(nullptr != pPlot) {
		pPlot_ = pPlot;
		pPlot_->attach(this);
	}
}

/*
	This method detaches a QgtCurve from any 
	QgtPlot it has been associated with.

	detach() is equivalent to calling attach( NULL )
*/
void QgtCurve::detach()
{
	if(nullptr != pPlot_) {
		pPlot_->detach(this);
		pPlot_ = nullptr;
	}
}

