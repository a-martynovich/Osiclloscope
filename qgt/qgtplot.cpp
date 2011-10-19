#include "qgtplot.h"

const GLdouble QgtPlot::LEFT_CP = -1;
const GLdouble QgtPlot::RIGHT_CP = 1;

const GLdouble QgtPlot::BOTTOM_CP = -1;
const GLdouble QgtPlot::TOP_CP = 1;

const float QgtPlot::DEFAULT_HAXIS_MINVALUE = -10.0f;
const float QgtPlot::DEFAULT_HAXIS_MAXVALUE = 10.0f;
const int   QgtPlot::DEFAULT_HAXIS_PRECISION = 2;

const float QgtPlot::DEFAULT_VAXIS_MINVALUE = -10.0f;
const float QgtPlot::DEFAULT_VAXIS_MAXVALUE = 10.0f;
const int   QgtPlot::DEFAULT_VAXIS_PRECISION = 2;

QgtPlot::QgtPlot(QWidget* parent) 
	: QGLWidget(parent), 
	  cPCoordinates_(LEFT_CP, RIGHT_CP, BOTTOM_CP, TOP_CP),
	  clColor_(Qt::white), 
	  antiAliasing_(false), autoReplot_(false)
{
	pCanvas_ = new QgtCanvas(this, cPCoordinates_);
	pHorizontalAxis_ = new QgtHorizontalAxis(this, cPCoordinates_);
	pVerticalAxis_ = new QgtVerticalAxis(this, cPCoordinates_);
	
	connect(pHorizontalAxis_, SIGNAL(changedMinValue(qreal)), pCanvas_, SLOT(setMinXValue(qreal)));
    connect(pHorizontalAxis_, SIGNAL(changedMaxValue(qreal)), pCanvas_, SLOT(setMaxXValue(qreal)));
    connect(pVerticalAxis_, SIGNAL(changedMinValue(qreal)), pCanvas_, SLOT(setMinYValue(qreal)));
    connect(pVerticalAxis_, SIGNAL(changedMaxValue(qreal)), pCanvas_, SLOT(setMaxYValue(qreal)));

	pHorizontalAxis_->setMinValue(DEFAULT_HAXIS_MINVALUE);
    pHorizontalAxis_->setMaxValue(DEFAULT_HAXIS_MAXVALUE);
	pHorizontalAxis_->setPrecision(DEFAULT_HAXIS_PRECISION);
	
	pVerticalAxis_->setMinValue(DEFAULT_VAXIS_MINVALUE);
    pVerticalAxis_->setMaxValue(DEFAULT_VAXIS_MAXVALUE);
	pVerticalAxis_->setPrecision(DEFAULT_VAXIS_PRECISION);

	pLayout_ = new QgtLayout(this, cPCoordinates_);
	pLayout_->setMargin(0.04);

	pLayout_->addGraphicObject(pCanvas_, 0, 1, 16, 16);
	pLayout_->addGraphicObject(pHorizontalAxis_, 16, 1, 1, 16);
	pLayout_->addGraphicObject(pVerticalAxis_, 0, 0, 16, 1);

	renderObjectList_.append(pCanvas_);
	renderObjectList_.append(pHorizontalAxis_);
	renderObjectList_.append(pVerticalAxis_);

	replot();
}

QgtPlot::~QgtPlot()
{
	
}

GLdouble QgtPlot::leftCP() const
{
	return cPCoordinates_.left;
}

GLdouble QgtPlot::rightCP() const
{
	return cPCoordinates_.right;
}

GLdouble QgtPlot::leftRightCPInterval() const
{
	return cPCoordinates_.leftRightInterval();
}

GLdouble QgtPlot::bottomCP() const
{
	return cPCoordinates_.bottom;
}

GLdouble QgtPlot::topCP() const
{
	return cPCoordinates_.top;
}

GLdouble QgtPlot::bottomTopCPInterval() const
{
	return cPCoordinates_.bottomTopInterval();
}

qreal QgtPlot::pixelWidthToOGL(int v)
{
	return (v * cPCoordinates_.leftRightInterval()) / size().width();
}

qreal QgtPlot::pixelHeightToOGL(int v)
{
	return (v * cPCoordinates_.bottomTopInterval()) / size().height();
}

const QColor& QgtPlot::bgColor() const
{
	return clColor_;
}

void QgtPlot::setBgColor(const QColor& color)
{
	clColor_ = color;
	glInit();
}

void QgtPlot::attach(QgtCurve* pCurve)
{
	pCanvas_->addItem(pCurve);
}

void QgtPlot::detach(QgtCurve* pCurve)
{
	pCanvas_->removeItem(pCurve);
}

/*
	Redraw the plot

	If the autoReplot option is not set (which is the default)
	or if any curves are attached to raw data, the plot has to
	be refreshed explicitly in order to make changes visible.
*/
void QgtPlot::replot()
{
	for(QList<QgtRenderObject*>::ConstIterator it = renderObjectList_.constBegin();
		it != renderObjectList_.constEnd();
		++it) {
	
		(*it)->buildGeometry();
	}

	glDraw();
}

QgtVerticalAxis* QgtPlot::verticalAxis()
{
	return pVerticalAxis_;
}

const QgtVerticalAxis* QgtPlot::verticalAxis() const
{
	return pVerticalAxis_;
}

QgtHorizontalAxis* QgtPlot::horizontalAxis()
{
	return pHorizontalAxis_;
}

const QgtHorizontalAxis* QgtPlot::horizontalAxis() const
{
	return pHorizontalAxis_;
}

bool QgtPlot::antiAliasing() const
{
	return antiAliasing_;
}

void QgtPlot::setAntiAliasing(bool b)
{
	antiAliasing_ = b;
	glInit();
}

bool QgtPlot::autoReplot() const
{
	return autoReplot_;
}

void QgtPlot::setAutoReplot(bool b)
{
	autoReplot_ = b;

	/*
		if true == autoReplot_ connect redisplay signal of all QgtGraphicObject to replot slot 
		or disconnect in another
	*/

	if(true == autoReplot_) {
		connect(pCanvas_, SIGNAL(redisplay()), SLOT(replot()));
		connect(pHorizontalAxis_, SIGNAL(redisplay()), SLOT(replot()));
		connect(pVerticalAxis_, SIGNAL(redisplay()), SLOT(replot()));
	}
	else {
		disconnect(pCanvas_, SIGNAL(redisplay()));
		disconnect(pHorizontalAxis_, SIGNAL(redisplay()));
		disconnect(pVerticalAxis_, SIGNAL(redisplay()));
	}
}

void QgtPlot::renderGraphicItems(const QList<QgtRenderGraphicItem::SPtr>& items)
{
	glEnableClientState(GL_VERTEX_ARRAY);

	for(QList<QgtRenderGraphicItem::SPtr>::ConstIterator it = items.constBegin();
		it != items.constEnd();
		++it) {

		glVertexPointer(2, GL_FLOAT, 0, (*it)->vertices.constData());

		switch((*it)->type)
		{
		case GL_LINE_STRIP: glLineWidth((*it)->width);
			break;

		case GL_POINTS: glPointSize((*it)->width);
			break;
		}
		
		qglColor((*it)->color);
		glDrawArrays((*it)->type, 0, (*it)->vertices.size());
	}

	glDisableClientState(GL_VERTEX_ARRAY);	
}

void QgtPlot::renderTextItems(const QList<QgtRenderTextItem::SPtr>& items)
{
	for(QList<QgtRenderTextItem::SPtr>::ConstIterator it = items.constBegin();
		it != items.constEnd();
		++it) {
			
		qglColor((*it)->color);
		
		/*
			native implementation of drawing text... 
		*/
	}
}

void QgtPlot::initializeGL()
{
	qglClearColor(clColor_);
	
	if(true == antiAliasing_) {
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	}
}

void QgtPlot::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(cPCoordinates_.left, cPCoordinates_.right, cPCoordinates_.bottom, cPCoordinates_.top, -1, 1);
}

void QgtPlot::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);

	for(QList<QgtRenderObject*>::ConstIterator it = renderObjectList_.constBegin();
		it != renderObjectList_.constEnd();
		++it) {
		
			if(sizeof(qreal) == sizeof(GLdouble))
				glLoadMatrixd((*it)->matrix().constData());
			else 
				glLoadMatrixf(reinterpret_cast<const GLfloat*>((*it)->matrix().constData()));
		
			renderGraphicItems((*it)->renderGraphicItemList());
			//renderTextItems((*it)->renderTextItemList());

			/*
				FIXME:
					renderText it is not optimal method for drawing text
			*/
			glLoadIdentity();

			const QMatrix4x4& matrix = (*it)->matrix();
			const QList<QgtRenderTextItem::SPtr>& items = (*it)->renderTextItemList();

			for(QList<QgtRenderTextItem::SPtr>::ConstIterator it = items.constBegin();
				it != items.constEnd();
				++it) {
			
					QFontMetrics fontMetrics((*it)->font);
					qreal xCorrection = pixelWidthToOGL(fontMetrics.width((*it)->text) / 2);
					qreal yCorrection = pixelHeightToOGL((fontMetrics.height() - 4) / 2);

					qglColor((*it)->color);
					renderText(matrix.map(QVector3D((*it)->vertex)).x() - xCorrection, 
							   matrix.map(QVector3D((*it)->vertex)).y() - yCorrection, 0.0, (*it)->text, (*it)->font);
			} 
	}
	
	glFlush();
}