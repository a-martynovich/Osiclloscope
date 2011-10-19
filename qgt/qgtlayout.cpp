#include <QSizeF>
#include <algorithm>

#include "qgtlayout.h"

QgtLayout::QgtLayout(QObject* parent, const QgtCoordinates<GLdouble>& _cPCoordinates)
	: QObject(parent), cPCoordinates_(_cPCoordinates),
	  margin_(0),
	  rowCount_(0), columnCount_(0)
{

}

void QgtLayout::addGraphicObject(QgtGraphicObject* pGObject, int row, int column, int rowSpan, int columnSpan)
{
	int rowCount = row + rowSpan;
	if(rowCount > rowCount_) rowCount_ = rowCount;
	
	int columnCount = column + columnSpan;
	if(columnCount > columnCount_) columnCount_ = columnCount;
	
	QgtLayoutItem::SPtr spLItem(new QgtLayoutItem);

	spLItem->pGObject = pGObject;
	spLItem->row = row;
	spLItem->column = column;
	spLItem->rowSpan = rowSpan;
	spLItem->columnSpan = columnSpan;

	graphicObjectList_.append(spLItem);

	buildGeometry();
}

void QgtLayout::removeGraphicObject(QgtGraphicObject* pGObject)
{
	std::remove_if(graphicObjectList_.begin(), graphicObjectList_.end(), 
			       [&](const QgtLayoutItem::SPtr& spLItem) { return spLItem->pGObject == pGObject; });

	updateRowColumnCount();
	buildGeometry();
}

int QgtLayout::margin() const
{
	return margin_;
}

void QgtLayout::setMargin(qreal v)
{
	margin_ = v;
	buildGeometry();
}

void QgtLayout::buildGeometry()
{
	if(true == graphicObjectList_.isEmpty()) return;

	QSizeF cellSize((cPCoordinates_.leftRightInterval() - margin_ * 2) / columnCount_,
					(cPCoordinates_.bottomTopInterval() - margin_ * 2) / rowCount_);

	for(QList<QgtLayoutItem::SPtr>::ConstIterator it = graphicObjectList_.constBegin();
		it != graphicObjectList_.constEnd();
		++it) {
			
			QgtGraphicObject* pGObject = (*it)->pGObject;
			pGObject->setPosition(QPointF(cPCoordinates_.left + margin_ + cellSize.width() * (*it)->column,
										  cPCoordinates_.top - margin_ - cellSize.height() * (*it)->row));

			pGObject->setSize(QSizeF(cellSize.width() * (*it)->columnSpan,
									 cellSize.height() * (*it)->rowSpan));
	}
}

void QgtLayout::updateRowColumnCount()
{
	for(QList<QgtLayoutItem::SPtr>::ConstIterator it = graphicObjectList_.constBegin();
		it != graphicObjectList_.constEnd();
		++it) {

			int rowCount = (*it)->row + (*it)->rowSpan;
			if(rowCount > rowCount_) rowCount_ = rowCount;

			int columnCount = (*it)->column + (*it)->columnSpan;
			if(columnCount > columnCount_) columnCount_ = columnCount;
	}
}