#include "qgtgraphicobject.h"

QgtGraphicObject::QgtGraphicObject(QGLWidget* parent)
	: QgtRenderObject(parent), visible_(true)
{

}

bool QgtGraphicObject::isVisible() const
{
	return visible_;
}

void QgtGraphicObject::setVisible(bool b)
{
	visible_ = b;
	emit redisplay();
}

const QSizeF& QgtGraphicObject::size() const
{
	return size_;
}

void QgtGraphicObject::setSize(const QSizeF& size)
{
	size_ = size;
	emit redisplay();
}

const QPointF& QgtGraphicObject::position() const
{
	return position_;
}

void QgtGraphicObject::setPosition(const QPointF& position)
{
	position_ = position;
	emit redisplay();
}