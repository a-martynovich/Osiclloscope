#ifndef VERTICALAXIS_H
#define VERTICALAXIS_H

#include <QMatrix4x4>
#include <QGLWidget>

#include "qgtcoordinates.h"
#include "qgtrendertextitem.h"
#include "qgtrendergraphicitem.h"
#include "qgtaxis.h"

class QgtPlot;

class QgtVerticalAxis : public QgtAxis
{
    Q_OBJECT

public:
    QgtVerticalAxis(QgtPlot* parent, const QgtCoordinates<GLdouble>& _cPCoordinates);

private:
	void buildGeometry();
	void buildGraphicItems();
	void buildTextItems();

	const QMatrix4x4& matrix() const;

	const QList<QgtRenderGraphicItem::SPtr>& renderGraphicItemList() const;
	const QList<QgtRenderTextItem::SPtr>& renderTextItemList() const;

	const QgtCoordinates<GLdouble>& cPCoordinates_; 
	const QgtCoordinates<qreal> coordinates_;

	QMatrix4x4 matrix_;
	QList<QgtRenderGraphicItem::SPtr> renderGraphicItemsList_;
	QList<QgtRenderTextItem::SPtr> renderTextItemsList_;
};

#endif // VERTICALAXIS_H
