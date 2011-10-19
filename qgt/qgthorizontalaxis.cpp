#include "qgthorizontalaxis.h"
#include "qgtplot.h"

QgtHorizontalAxis::QgtHorizontalAxis(QgtPlot* parent, const QgtCoordinates<GLdouble>& _cPCoordinates)
    : QgtAxis(parent), cPCoordinates_(_cPCoordinates) ,coordinates_(-1.0, 1.0, -1.0, 1.0)

{
	
}

void QgtHorizontalAxis::buildGeometry()
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

void QgtHorizontalAxis::buildGraphicItems()
{
	QgtRenderGraphicItem::SPtr spRGItem(new QgtRenderGraphicItem);
	
	spRGItem->type = GL_LINES;
	spRGItem->color = Qt::black;
	spRGItem->width = 1.0f;

	spRGItem->vertices.append(QVector2D(coordinates_.left,  coordinates_.top));
	spRGItem->vertices.append(QVector2D(coordinates_.right, coordinates_.top));

	qreal cLRInterval = coordinates_.leftRightInterval();
	qreal cBTInterval = coordinates_.bottomTopInterval();

	int n = nStep();
	qreal stepInCPCoordinates = cLRInterval / n;

	for(int i = 0; i <= n; ++i) {
		qreal x = coordinates_.left + stepInCPCoordinates * i;

		spRGItem->vertices.append(QVector2D(x, coordinates_.top - cBTInterval / 4.0));
		spRGItem->vertices.append(QVector2D(x, coordinates_.top));
	}

	renderGraphicItemsList_.append(spRGItem);
}

void QgtHorizontalAxis::buildTextItems()
{
	qreal cLRInterval = coordinates_.leftRightInterval();
	qreal cBTInterval = coordinates_.bottomTopInterval();

	int n = nStep();
	qreal stepInCPCoordinates = cLRInterval / n;

	qreal mV = minValue();
	qreal stepInAxisCoordinates = step();
	int pr = precision();

	const QFont& f = font();
	
	for(int i = 0; i <= n; ++i) {
		QString text = QString::number(mV + stepInAxisCoordinates * i, 'g', pr);

		QgtRenderTextItem::SPtr spRTItem(new QgtRenderTextItem);
		
		spRTItem->font = f;
		spRTItem->text = text;
		spRTItem->color = Qt::black;
		
		QVector2D v(coordinates_.left + stepInCPCoordinates* i, 
					coordinates_.top - cBTInterval / 1.6);

		spRTItem->vertex = v;

		renderTextItemsList_.append(spRTItem);
	}
}

const QMatrix4x4& QgtHorizontalAxis::matrix() const
{
	return matrix_;
}

const QList<QgtRenderGraphicItem::SPtr>& QgtHorizontalAxis::renderGraphicItemList() const
{
	return renderGraphicItemsList_;
}

const QList<QgtRenderTextItem::SPtr>& QgtHorizontalAxis::renderTextItemList() const
{
	return renderTextItemsList_;
}
