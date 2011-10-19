#ifndef QGTRENDERGRAPHICITEM_H
#define QGTRENDERGRAPHICITEM_H

#include <QColor>
#include <QVector2D>
#include <QVector>
#include <QGLWidget>

struct QgtRenderGraphicItem
{
	typedef QSharedPointer<QgtRenderGraphicItem> SPtr;

	GLenum type;
	QColor color;
	GLfloat width;
	QVector<QVector2D> vertices;
};

#endif // QGTRENDERGRAPHICITEM_H
