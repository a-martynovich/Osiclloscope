#ifndef QGTGRAPHICOBJECT_H
#define QGTGRAPHICOBJECT_H

#include <QPointF>
#include <QSizeF>
#include <QFont>
#include <QSharedPointer>
#include <QGLWidget>

#include "qgtrenderobject.h"

/*
	Base class for all graphic object in qgt toolkit
*/
class QgtGraphicObject : public QgtRenderObject
{
	Q_OBJECT

public:
	typedef QSharedPointer<QgtGraphicObject> QgtGraphicObjectPtr;

	QgtGraphicObject(QGLWidget* parent);

	bool isVisible() const;
	
	const QSizeF& size() const;
	virtual void setSize(const QSizeF& size);

	const QPointF& position() const;
	virtual void setPosition(const QPointF& position);

public slots:
	void setVisible(bool b);

private:
	virtual void buildGeometry() = 0;

	virtual const QMatrix4x4& matrix() const = 0;

	virtual const QList<QgtRenderGraphicItem::SPtr>& renderGraphicItemList() const = 0;
	virtual const QList<QgtRenderTextItem::SPtr>& renderTextItemList() const = 0;

	bool visible_;
	QSizeF  size_;
	QPointF position_;

signals:
	/*
		redisplay is emmited when need update QgtGraphicWidget
	*/
	void redisplay();
};

#endif // QGTGRAPHICOBJECT_H