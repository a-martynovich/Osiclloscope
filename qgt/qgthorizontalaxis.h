#ifndef HORIZONTALAXIS_H
#define HORIZONTALAXIS_H

#include <QMatrix4x4>
#include <QGLWidget>

#include "qgtcoordinates.h"
#include "qgtrendertextitem.h"
#include "qgtrendergraphicitem.h"
#include "qgtaxis.h"

class QgtPlot;

class QgtHorizontalAxis : public QgtAxis
{
	Q_OBJECT
    
public:
   QgtHorizontalAxis(QgtPlot* parent, const QgtCoordinates<GLdouble>& _cPCoordinates);
	
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

#endif // HORIZONTALAXIS_H
