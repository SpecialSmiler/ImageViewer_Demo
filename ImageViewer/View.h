#pragma once
#include <QFrame>
#include <QGraphicsView>
#include <QScrollBar>

class View;

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    GraphicsView(View* v);

public slots:
    void resetZoom();
    void focusToCenter();

protected:
#if QT_CONFIG(wheelevent)
    void wheelEvent(QWheelEvent*evt) override;
#endif
    void mouseMoveEvent(QMouseEvent* evt) override;
    void mousePressEvent(QMouseEvent* evt) override;
    void mouseReleaseEvent(QMouseEvent* evt) override;

private:
    //void updateTrans();
    void updateZoom();

    View* m_view;

    bool m_isMoving = false;
    int m_lastX;
    int m_lastY;
    int m_globalTop;
    int m_globalBottom;
    int m_globalLeft;
    int m_globalRight;

    bool m_isSmoothZooming = false;
    qreal m_zoomValue;
    qreal m_zoomMax;
    qreal m_zoomMin;
    qreal m_zoomFac;

    QScrollBar* m_hBar;
    QScrollBar* m_vBar;

    //qreal m_beginX;
    //qreal m_beginY;
    //qreal m_endX;
    //qreal m_endY;
    //qreal m_dx;
    //qreal m_dy;

};

class View : public QFrame
{
    Q_OBJECT
public:
    explicit View(const QString& name, QWidget* parent = nullptr);

    GraphicsView* view() const;

public slots:
    void zoomIn();
    void zoomOut();
    void zoomInBy(int level);
    void zoomOutBy(int level);
    void resetView();

private:
    void setupMatrix();

private:
    GraphicsView* graphicsView;
};