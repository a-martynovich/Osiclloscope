#ifndef QGTRENDERTEXTITEM_H
#define QGTRENDERTEXTITEM_H

#include <QFont>
#include <QString>
#include <QColor>
#include <QVector2D>
#include <QSharedPointer>

struct QgtRenderTextItem
{
	typedef QSharedPointer<QgtRenderTextItem> SPtr;

	QFont font;
	QString text;
	QColor color;
	QVector2D vertex;
};

#endif // QGTRENDERTEXTITEM_H