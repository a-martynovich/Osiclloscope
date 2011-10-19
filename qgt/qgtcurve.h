#ifndef QGTCURVE_H
#define QGTCURVE_H

#include <QPointF>
#include <QVector>
#include <QColor>
#include <QObject>

#include "qgtrendergraphicitem.h"

class QgtPlot;

/*
	A QgtCurve is the representation of a series of points or lines in the x-y plane.
*/
class QgtCurve : public QObject
{
	Q_OBJECT

public:
	enum CurveStyle
	{ 
		/*
			Draws a connected group of line segments from the first point to the last.
		*/
		LINE, 

		/*
			Draw dots at the locations of the data points.
		*/
		POINTS 
	};
	
	QgtCurve(QObject* parent = nullptr);
	~QgtCurve();

	void show();
	void hide();

	bool isVisible() const;
	void setVisible(bool b);

	CurveStyle type() const;
	void setType(CurveStyle style);

	const QColor& color() const;
	void setColor(const QColor& color);

	qreal width() const;
	void setWidth(qreal width);

	const QVector<QPointF>& points() const;
	void setPoints(const QVector<QPointF>& points);
    void setPoints(qreal* pXData, qreal* pYData, size_t n);

	QgtPlot* plot();
	const QgtPlot* plot() const;
	
	void replot();
	
	void attach(QgtPlot* pPlot);
	void detach();
	void setSignalReplot(bool);
private:
	bool visible_, signal_replot_;
	CurveStyle style_;
	QColor color_;
	qreal width_;
	QVector<QPointF> points_;
	
	QgtPlot* pPlot_;

signals:
	/*
		redisplay is emmited when need update QgtCurve
	*/
	void redisplay();
};

#endif // QGTCURVE_H
