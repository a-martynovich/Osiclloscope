#ifndef QGTRENDEROBJECT_H
#define QGTRENDEROBJECT_H

#include <QMatrix4x4>
#include <QSharedPointer>
#include <QGLWidget>

#include "qgtrendertextitem.h"
#include "qgtrendergraphicitem.h"

class QgtRenderObject : public QObject
{
	Q_OBJECT

public:
	QgtRenderObject(QGLWidget* parent) : QObject(parent) { };

	typedef QSharedPointer<QgtRenderObject> SPtr;

	virtual const QMatrix4x4& matrix() const = 0;

	virtual const QList<QgtRenderGraphicItem::SPtr>& renderGraphicItemList() const = 0;
	virtual const QList<QgtRenderTextItem::SPtr>& renderTextItemList() const = 0;

public slots:
		virtual void buildGeometry() = 0;
};

#endif // QGTRENDEROBJECT_H