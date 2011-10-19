#include "qgtcanvas.h"
#include "qgtplot.h"

QgtCanvas::QgtCanvas(QgtPlot* parent, const QgtCoordinates<GLdouble>& _cPCoordinates) 
	: QgtGraphicObject(parent), cPCoordinates_(_cPCoordinates) ,coordinates_(0.0, 0.0, 0.0, 0.0)
{

}

QgtCanvas::~QgtCanvas()
{
	for(QList<QgtCurve*>::ConstIterator it = itemList_.constBegin();
		it != itemList_.constEnd();
		++it) {
		
			(*it)->attach(nullptr);
	}
}

void QgtCanvas::addItem(QgtCurve* pCurve)
{
	itemList_.append(pCurve);
	connect(pCurve, SIGNAL(redisplay()), SIGNAL(redisplay()));
	emit redisplay();
}

void QgtCanvas::removeItem(QgtCurve* pCurve)
{
	if(true == itemList_.removeOne(const_cast<QgtCurve*>(pCurve))) {
		disconnect(pCurve, SIGNAL(redisplay()));
		pCurve->attach(nullptr);
		emit redisplay();
	}
}

qreal QgtCanvas::minXValue() const
{
	return coordinates_.left;
}

void QgtCanvas::setMinXValue(qreal v)
{
	coordinates_.left = v;
	//emit redisplay();
}

qreal QgtCanvas::maxXValue() const
{
	return coordinates_.right;
}

void QgtCanvas::setMaxXValue(qreal v)
{
	coordinates_.right = v;
	//emit redisplay();
}

qreal QgtCanvas::minMaxXInterval() const
{
	return coordinates_.leftRightInterval();
}

qreal QgtCanvas::minYValue() const
{
	return coordinates_.bottom;
}

void QgtCanvas::setMinYValue(qreal v)
{
	coordinates_.bottom = v;
	//emit redisplay();
}

qreal QgtCanvas::maxYValue() const
{
	return coordinates_.top;
}

void QgtCanvas::setMaxYValue(qreal v)
{
	coordinates_.top = v;
	//emit redisplay();
}

qreal QgtCanvas::minMaxYInterval() const
{
	return coordinates_.bottomTopInterval();
}

void QgtCanvas::buildGeometry()
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

	renderGraphicItemsList_.clear();
	renderTextItemsList_.clear();
	
	
	buildGraphicItems();
	buildTextItems();
}

void QgtCanvas::buildGraphicItems()
{
	for(QList<QgtCurve*>::ConstIterator it = itemList_.constBegin();
		it != itemList_.constEnd();
		++it) {

		const QgtCurve* item = *it;
		if(item->isVisible()) {

			const QVector<QPointF>& points = item->points();
			
			QVector<QVector2D> vertices;
			for(QVector<QPointF>::ConstIterator it = points.constBegin();
				it != points.constEnd();
				++it) {
				
				const QPointF& point = *it;
				/*if(point.x() >=  coordinates_.left && point.x() <=  coordinates_.right &&
				   point.y() >=  coordinates_.bottom && point.y() <=  coordinates_.top) {*/

					vertices.append(QVector2D(point));
				/*}
				else {
					if(false == vertices.isEmpty()) {
					
						QgtRenderGraphicItem::SPtr pRGItem(new QgtRenderGraphicItem);
						pRGItem->type  = curveTypeToOGL(item->type());
						pRGItem->color = item->color();
						pRGItem->width = item->width();
						pRGItem->vertices = vertices;
						renderGraphicItemsList_.append(pRGItem);

						vertices.clear();
					}
				}*/
			}

			if(false == vertices.isEmpty()) {
			
				QgtRenderGraphicItem::SPtr pRGItem(new QgtRenderGraphicItem);
				pRGItem->type  = curveTypeToOGL(item->type());
				pRGItem->color = item->color();
				pRGItem->width = item->width();
				pRGItem->vertices = vertices;
				renderGraphicItemsList_.append(pRGItem);
			}
		}
	}
}

void QgtCanvas::buildTextItems()
{

}

const QMatrix4x4& QgtCanvas::matrix() const
{
	return matrix_;
}

const QList<QgtRenderGraphicItem::SPtr>& QgtCanvas::renderGraphicItemList() const
{
	return renderGraphicItemsList_;
}

const QList<QgtRenderTextItem::SPtr>& QgtCanvas::renderTextItemList() const
{
	return renderTextItemsList_;
}

GLenum QgtCanvas::curveTypeToOGL(QgtCurve::CurveStyle style)
{
	switch(style)
	{
	case QgtCurve::LINE:   return GL_LINE_STRIP;
	case QgtCurve::POINTS: return GL_POINTS;
	}
}
