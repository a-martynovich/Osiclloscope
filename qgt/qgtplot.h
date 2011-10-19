#ifndef QGTPLOT_H
#define QGTPLOT_H

#include <QMatrix>
#include <QList>
#include <QGLWidget>

#include "qgtcoordinates.h"
#include "qgthorizontalaxis.h"
#include "qgtverticalaxis.h"
#include "qgtcanvas.h"
#include "qgtcurve.h"
#include "qgtlayout.h"
#include "qgtrenderobject.h"

/*
	QgtPlot is a widget for plotting two-dimensional graphs.
	An unlimited number of curves can be displayed on its canvas.
*/
class QgtPlot : public QGLWidget
{
	Q_OBJECT

public:
	QgtPlot(QWidget* parent = nullptr);
	~QgtPlot();

	/*
		clipping planes
	*/
	GLdouble leftCP() const;
	GLdouble rightCP() const;
	GLdouble leftRightCPInterval() const;

	GLdouble bottomCP() const;
	GLdouble topCP() const;
	GLdouble bottomTopCPInterval() const;

	qreal pixelWidthToOGL(int v);
	qreal pixelHeightToOGL(int v);

	/*
		objects
	*/
	void attach(QgtCurve* pCurve);
	void detach(QgtCurve* pCurve);

	QgtVerticalAxis* verticalAxis();
	const QgtVerticalAxis* verticalAxis() const;

	QgtHorizontalAxis* horizontalAxis();
	const QgtHorizontalAxis* horizontalAxis() const;

	/*
		configurations
	*/
	const QColor& bgColor() const;
	void setBgColor(const QColor& color);

	bool antiAliasing() const;
	void setAntiAliasing(bool b = true);

	bool autoReplot() const;
	void setAutoReplot(bool b = true);

public slots:
	void replot();

private:
	/*
		default value for initialization  
	*/
	static const GLdouble LEFT_CP    /* = -1.0 */;
	static const GLdouble RIGHT_CP   /* = 1.0  */;
	
	static const GLdouble BOTTOM_CP  /* = -1.0 */;
	static const GLdouble TOP_CP     /* = 1.0  */;
	
	static const float DEFAULT_HAXIS_MINVALUE  /* = -10.0f */;
	static const float DEFAULT_HAXIS_MAXVALUE  /* =  10.0f */;
	static const int   DEFAULT_HAXIS_PRECISION /* = 2      */;

	static const float DEFAULT_VAXIS_MINVALUE  /* = -10.0f */;
	static const float DEFAULT_VAXIS_MAXVALUE  /* =  10.0f */;
	static const int   DEFAULT_VAXIS_PRECISION /* =  2     */;

	void renderGraphicItems(const QList<QgtRenderGraphicItem::SPtr>& items);
	void renderTextItems(const QList<QgtRenderTextItem::SPtr>& items);

	const QgtCoordinates<GLdouble> cPCoordinates_; 

	QColor clColor_;

	QgtLayout* pLayout_;

	QgtCanvas*         pCanvas_;
	QgtHorizontalAxis* pHorizontalAxis_;
	QgtVerticalAxis*   pVerticalAxis_;

	bool antiAliasing_;
	bool autoReplot_;
	
	QList<QgtRenderObject*> renderObjectList_;

protected:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();
};

#endif // QGTPLOT_H
