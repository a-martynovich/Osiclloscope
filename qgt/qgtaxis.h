#ifndef QGTAXIS_H
#define QGTAXIS_H

#include <qmath.h>
#include <QObject>

#include "qgtcanvas.h"
#include "qgtgraphicobject.h"

class QgtPlot;

/*
	A QgtAxis is a base class for all axis
*/
class QgtAxis : public QgtGraphicObject
{
	Q_OBJECT

public:
	enum AxisStyle 
	{ 
		/*
			right scale
			
			-|
			 |
			-|
			 |
			-|
		*/
		NORMAL,

		/*
			left scale

			 |-
			 |
			 |-
			 |
			 |-
		*/
		INVERTED 
	};

	QgtAxis(QgtPlot* parent);
	virtual ~QgtAxis() =0;

	const QFont& font() const;
	
	AxisStyle style() const;
	
	qreal minValue() const;
    qreal maxValue() const;
   
	qreal step() const;
	int nStep() const;
   
    qreal interval() const;

    int precision() const;
   
public slots:
	virtual void setFont(const QFont& font);
	
	void setStyle(AxisStyle s);
	
	void setMinValue(qreal v);
	void setMaxValue(qreal v);
	
	virtual void setNStep(int v);
	
	virtual void setPrecision(int v);

private:
	void calculateInterval();
	
	QFont   font_;
	AxisStyle  style_;
	
	qreal	   minValue_;
	qreal      maxValue_;
	qreal      interval_;
	
	qreal      precision_;
	int        nStep_;
    
signals:
	void changedStyle(AxisStyle style);
	
	void changedMinValue(qreal v);
	void changedMaxValue(qreal v);
	
	void changedNStep(int v);
	
	void changedPrecision(int v);
};

#endif // QGTAXIS_H
