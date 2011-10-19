#ifndef QGTCANVAS_H
#define QGTCANVAS_H

#include <QRectF>
#include <QObject>

#include "qgtcurve.h"
#include "qgtcoordinates.h"
#include "qgtrendertextitem.h"
#include "qgtrendergraphicitem.h"
#include "qgtgraphicobject.h"

class QgtPlot;

/*
	Canvas of a QgtPlot.
*/
class QgtCanvas : public QgtGraphicObject
{
	Q_OBJECT

public:
	QgtCanvas(QgtPlot* parent, const QgtCoordinates<GLdouble>& _cPCoordinates);
	~QgtCanvas();

	void addItem(QgtCurve* pCurve);
	void removeItem(QgtCurve* pCurve);

	qreal minXValue() const;
	qreal maxXValue() const;
	qreal minYValue() const;
	qreal maxYValue() const;
   
	qreal minMaxXInterval() const;
	qreal minMaxYInterval() const;

public slots:
	void setMinXValue(qreal v);
	void setMaxXValue(qreal v);
	void setMinYValue(qreal v);
	void setMaxYValue(qreal v);

private:
	void buildGeometry();
	void buildGraphicItems();
	void buildTextItems();

	const QMatrix4x4& matrix() const;

	const QList<QgtRenderGraphicItem::SPtr>& renderGraphicItemList() const;
	const QList<QgtRenderTextItem::SPtr>& renderTextItemList() const;

	GLenum curveTypeToOGL(QgtCurve::CurveStyle style);

	const QgtCoordinates<GLdouble>& cPCoordinates_; 
	QgtCoordinates<qreal> coordinates_;

	QMatrix4x4 matrix_;
	QList<QgtRenderGraphicItem::SPtr> renderGraphicItemsList_;
	QList<QgtRenderTextItem::SPtr> renderTextItemsList_;

	QList<QgtCurve*> itemList_;
};

#endif // QGTCANVAS_H