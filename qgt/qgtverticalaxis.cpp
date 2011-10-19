#include "qgtverticalaxis.h"
#include "qgtplot.h"

QgtVerticalAxis::QgtVerticalAxis(QgtPlot* parent, const QgtCoordinates<GLdouble>& _cPCoordinates)
    : QgtAxis(parent), cPCoordinates_(_cPCoordinates) ,coordinates_(-1.0, 1.0, -1.0, 1.0)

{
	
}

void QgtVerticalAxis::buildGeometry()
{
	if(false == isVisible()) return;

	matrix_.setToIdentity();

	/*
		correction matrix by size and position
	*/
	matrix_.translate(position().x() + size().width() / 2, 
					  position().y() - size().height() / 2);


	qreal cPLRInterval = cPCoordinates_.leftRightInterval();
	qreal cPBTInterval = cPCoordinates_.bottomTopInterval();

	qreal sx = (cPLRInterval < size().width())? cPLRInterval / size().width() : 
												size().width() / cPLRInterval;

	qreal sy = (cPBTInterval < size().height())? cPBTInterval / size().height() : 
											     size().height() / cPBTInterval;
	matrix_.scale(sx, sy);


	/*
		correction matrix by internal coordinate system
	*/
	qreal cLRInterval = coordinates_.leftRightInterval();
	qreal cBTInterval = coordinates_.bottomTopInterval();

	sx = (cPLRInterval < cLRInterval)? cPLRInterval / cLRInterval : 
									   cLRInterval / cPLRInterval;

	sy = (cPBTInterval < cBTInterval)? cPLRInterval / cBTInterval : 
									   cBTInterval / cPLRInterval;
	matrix_.scale(sx, sy);

	qreal dx = coordinates_.left + cLRInterval / 2;
	qreal dy = coordinates_.bottom + cBTInterval / 2;

	matrix_.translate(-dx, -dy);

	if(QgtAxis::INVERTED == style()) {
	
		matrix_.rotate(180.0, 1, 0, 0);
	}

	renderGraphicItemsList_.clear();
	renderTextItemsList_.clear();

	
	buildGraphicItems();
	buildTextItems();
}

void QgtVerticalAxis::buildGraphicItems()
{
	QgtRenderGraphicItem::SPtr spRGItem(new QgtRenderGraphicItem);
	
	spRGItem->type = GL_LINES;
	spRGItem->color = Qt::black;
	spRGItem->width = 1.0f;

	spRGItem->vertices.append(QVector2D(coordinates_.right,  coordinates_.bottom));
	spRGItem->vertices.append(QVector2D(coordinates_.right, coordinates_.top));

	qreal cPLRInterval = cPCoordinates_.leftRightInterval();
	qreal cPBTInterval = cPCoordinates_.bottomTopInterval();

	int n = nStep();
	qreal stepInCPCoordinates = cPBTInterval / n;

	for(int i = 0; i <= n; ++i) {
		qreal y = coordinates_.left + stepInCPCoordinates * i;

		spRGItem->vertices.append(QVector2D(coordinates_.right - cPLRInterval / 4.0, y));
		spRGItem->vertices.append(QVector2D(coordinates_.right, y));
	}

	renderGraphicItemsList_.append(spRGItem);
}

void QgtVerticalAxis::buildTextItems()
{
	qreal cPLRInterval = cPCoordinates_.leftRightInterval();
	qreal cPBTInterval = cPCoordinates_.bottomTopInterval();

	int n = nStep();
	qreal stepInCPCoordinates = cPBTInterval / n;

	qreal mV = minValue();
	qreal stepInAxisCoordinates = step();
	int pr = precision();

	const QFont& f = font();
	
	for(int i = 0; i <= n; ++i) {
		QString text = QString::number(mV + stepInAxisCoordinates * i, 'f', pr);

		QgtRenderTextItem::SPtr spRTItem(new QgtRenderTextItem);
		
		spRTItem->font = f;
		spRTItem->text = text;
		spRTItem->color = Qt::black;

		QVector2D v(coordinates_.right - cPLRInterval / 1.6, 
					coordinates_.left + stepInCPCoordinates* i);

		spRTItem->vertex = v;
		
		renderTextItemsList_.append(spRTItem);
	}
}

const QMatrix4x4& QgtVerticalAxis::matrix() const
{
	return matrix_;
}

const QList<QgtRenderGraphicItem::SPtr>& QgtVerticalAxis::renderGraphicItemList() const
{
	return renderGraphicItemsList_;
}

const QList<QgtRenderTextItem::SPtr>& QgtVerticalAxis::renderTextItemList() const
{
	return renderTextItemsList_;
}
