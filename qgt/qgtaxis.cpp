#include "qgtaxis.h"
#include "qgtplot.h"

QgtAxis::QgtAxis(QgtPlot* parent)
	: QgtGraphicObject(parent),
	  style_(NORMAL),
	  minValue_(0), maxValue_(0), interval_(0),
	  precision_(2),
	  nStep_(10) 			   
{

}

const QFont& QgtAxis::font() const
{
	return font_;
}

void QgtAxis::setFont(const QFont& font)
{
	font_ = font;
}

QgtAxis::AxisStyle QgtAxis::style() const
{
	return style_;
}

void QgtAxis::setStyle(AxisStyle style)
{
	style_ = style;

	emit changedStyle(style_);
	emit redisplay();
}

qreal QgtAxis::minValue() const
{
    return minValue_;
}

void QgtAxis::setMinValue(qreal v)
{
    minValue_ = v;
	calculateInterval();

	emit changedMinValue(minValue_);
	emit redisplay();
}

qreal QgtAxis::maxValue() const
{
    return maxValue_;
}

void QgtAxis::setMaxValue(qreal v)
{
    maxValue_ = v;
	calculateInterval();

	emit changedMaxValue(maxValue_);
	emit redisplay();
}

qreal QgtAxis::step() const
{
	return interval_ / nStep_;
}

int QgtAxis::nStep() const
{
	return nStep_;
}

void QgtAxis::setNStep(int v)
{
	nStep_ = v;

	emit changedNStep(nStep_);
	emit redisplay();
}

qreal QgtAxis::interval() const
{
    return interval_;
}

int QgtAxis::precision() const
{
	return precision_;
}

void QgtAxis::setPrecision(int v)
{
    precision_ = v;

	emit changedPrecision(precision_);
	emit redisplay();
}

void QgtAxis::calculateInterval()
{
	interval_ = maxValue_ - minValue_;
}
QgtAxis::~QgtAxis() {}



