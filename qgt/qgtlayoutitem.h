#ifndef QGTLAYOUTITEM_H
#define QGTLAYOUTITEM_H

#include <QSharedPointer>

#include "qgtgraphicobject.h"

struct QgtLayoutItem
{
	typedef QSharedPointer<QgtLayoutItem> SPtr;

	int row;
	int column;

	int rowSpan;
	int columnSpan;

	QgtGraphicObject* pGObject;
};

#endif // QGTLAYOUTITEM_H