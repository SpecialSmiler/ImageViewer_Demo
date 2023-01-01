#include <QtGlobal>
#include <QWheelEvent>
#include <QHBoxLayout>
#include <QCursor>
#include "View.h"

GraphicsView::GraphicsView(View* v)
    : QGraphicsView(), m_view(v),
    m_zoomValue(0.0), m_zoomFac(0.5), m_zoomMin(-5.0), m_zoomMax(3.0),
    m_hBar(horizontalScrollBar()), m_vBar(verticalScrollBar())
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setBackgroundBrush(QBrush(QColor::fromRgb(32, 32, 32)));
}

#if QT_CONFIG(wheelevent)
void GraphicsView::wheelEvent(QWheelEvent* evt)
{
    if (!m_isMoving)
    {
        if (evt->angleDelta().y() < 0)
        {
            m_zoomValue = qMax(m_zoomValue - m_zoomFac, m_zoomMin);
        }
        else
        {
            m_zoomValue = qMin(m_zoomValue + m_zoomFac, m_zoomMax);
        }
        updateZoom();
    }
}
#endif

void GraphicsView::mouseMoveEvent(QMouseEvent* evt)
{
    if (m_isMoving)
    {
        int curX = QCursor::pos().x();
        int curY = QCursor::pos().y();
        int dx = curX - m_lastX;
        int dy = curY - m_lastY;

        if (m_isSmoothZooming)
        {
            m_zoomValue -= qreal(dy) * 0.01;
            m_zoomValue = qMax(m_zoomMin, qMin(m_zoomValue, m_zoomMax));
            updateZoom();
        }
        else
        {
            int hValue = m_hBar->value();
            int vValue = m_vBar->value();
            m_hBar->setValue(hValue - dx);
            m_vBar->setValue(vValue - dy);
        }

        QPoint newPos(curX, curY);
        if (curX < m_globalLeft)
        {
            newPos.setX(m_globalRight + curX - m_globalLeft);
            QCursor::setPos(newPos);
        }
        else if (curX > m_globalRight)
        {
            newPos.setX(m_globalLeft + curX - m_globalRight);
            QCursor::setPos(newPos);
        }
        if (curY < m_globalTop)
        {
            newPos.setY(m_globalBottom + curY - m_globalTop);
            QCursor::setPos(newPos);
        }
        else if (curY > m_globalBottom)
        {
            newPos.setY(m_globalTop + curY - m_globalBottom);
            QCursor::setPos(newPos);
        }
        m_lastX = newPos.x();
        m_lastY = newPos.y();
    }
}

void GraphicsView::mousePressEvent(QMouseEvent* evt)
{
    if (!m_isMoving && evt->button() == Qt::MiddleButton)
    {
        m_isMoving = true;
        if (evt->modifiers() & Qt::ControlModifier)
        {
            m_isSmoothZooming = true;
        }
        m_lastX = QCursor::pos().x();
        m_lastY = QCursor::pos().y();
        QPoint topLeft = mapToGlobal(QPoint(0, 0));
        QPoint bottomRight = mapToGlobal(QPoint(width(), height()));
        m_globalLeft = topLeft.x();
        m_globalTop = topLeft.y();
        m_globalRight = bottomRight.x();
        m_globalBottom = bottomRight.y();
        setCursor(QCursor(Qt::SizeAllCursor));
    }
    else if (evt->button() == Qt::RightButton)
    {
        QTransform trans = this->transform();
        QTransform viewportTrans = this->viewportTransform();

        int hBar = m_hBar->value();
        int vBar = m_vBar->value();
        int a = 1;
    }
    else if (evt->button() == Qt::LeftButton)
    {

    }
}

void GraphicsView::mouseReleaseEvent(QMouseEvent* evt)
{
    if (m_isMoving && evt->button() == Qt::MiddleButton)
    {
        m_isMoving = false;
        m_isSmoothZooming = false;
        setCursor(QCursor(Qt::ArrowCursor));
    }
}

void GraphicsView::updateZoom()
{
    qreal scale = qPow(qreal(2), m_zoomValue);
    QTransform matrix;
    matrix.scale(scale, scale);
    setTransform(matrix);
}

void GraphicsView::resetZoom()
{
    //updateZoom();
    QTransform trans;
    setTransform(trans);
    m_zoomValue = 0.0;
}

void GraphicsView::focusToCenter()
{
    int hMax = m_hBar->maximum();
    int hMin = m_hBar->minimum();
    int vMax = m_vBar->maximum();
    int vMin = m_vBar->minimum();
    m_hBar->setValue((hMax + hMin) / 2);
    m_vBar->setValue((vMax + vMin) / 2);
}

//void GraphicsView::updateTrans()
//{
//    QTransform trans;
//    //trans.translate(m_offsetX, m_offsetY);
//    trans.scale(m_zoomed, m_zoomed);
//    setTransform(trans);
//}

View::View(const QString& name, QWidget* parent)
    : QFrame(parent)
{
    setFrameStyle(QFrame::Box);
    graphicsView = new GraphicsView(this);
    graphicsView->setRenderHint(QPainter::Antialiasing, false);
    //graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
    graphicsView->setOptimizationFlags(QGraphicsView::DontSavePainterState);
    graphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    //graphicsView->setTransformationAnchor(QGraphicsView::NoAnchor);

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(graphicsView);
}

GraphicsView* View::view() const
{
    return graphicsView;
}

void View::zoomIn()
{
}

void View::zoomOut()
{
}

void View::zoomInBy(int level)
{
}

void View::zoomOutBy(int level)
{
}

void View::resetView()
{
}

void View::setupMatrix()
{
}
