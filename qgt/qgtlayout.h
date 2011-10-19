#ifndef QGTLAYOUT_H
#define QGTLAYOUT_H

#include <QList>
#include <QObject>

#include "qgtcoordinates.h"
#include "qgtgraphicobject.h"
#include "qgtlayoutitem.h"
#include "qgtgraphicobject.h"

class QgtPlot;

/*
	Layout engine for QgtPlot.
	It is used by the QgtPlot widget to organize QgtGraphicObject.
*/
class QgtLayout : public QObject
{
	
public:
	QgtLayout(QObject* parent, const QgtCoordinates<GLdouble>& _cPCoordinates);
	
	void addGraphicObject(QgtGraphicObject* pGObject, int row, int column, int rowSpan = 1, int columnSpan = 1);
	void removeGraphicObject(QgtGraphicObject* pGObject);

	int margin() const;
	void setMargin(qreal v);

private:
	void buildGeometry();
	void updateRowColumnCount();

	const QgtCoordinates<GLdouble>& cPCoordinates_; 

	qreal margin_;
	
	int rowCount_;
	int columnCount_;

	QList<QgtLayoutItem::SPtr> graphicObjectList_;
};

#endif // QGTLAYOUT_H
