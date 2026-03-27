#include "canvaswidget.h"
#include <QPainter>
#include <QMouseEvent>

CanvasWidget::CanvasWidget(int w, int h, QWidget *parent)
    : QWidget(parent)
    , m_image(w, h, QImage::Format_ARGB32)
{
    m_image.fill(Qt::white);
    setFixedSize(w * m_zoom + 1, h * m_zoom + 1);
    setMouseTracking(true);
}

void CanvasWidget::setZoom(int zoom)
{
    m_zoom = zoom;
    setFixedSize(m_image.width()  * m_zoom + 1,
                 m_image.height() * m_zoom + 1);
    update();
}

void CanvasWidget::setForeColor(const QColor &c) { m_foreColor = c; }
void CanvasWidget::setBackColor(const QColor &c) { m_backColor = c; }

QPoint CanvasWidget::toPixel(const QPoint &pos) const
{
    return QPoint(pos.x() / m_zoom, pos.y() / m_zoom);
}

void CanvasWidget::drawPixel(const QPoint &pos, Qt::MouseButton btn)
{
    QPoint px = toPixel(pos);
    if (!m_image.rect().contains(px)) return;
    QColor c = (btn == Qt::LeftButton) ? m_foreColor : m_backColor;
    m_image.setPixelColor(px, c);
    update();
    emit imageChanged();
}

void CanvasWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    int W = m_image.width(), H = m_image.height();

    // 画每个像素格
    for (int y = 0; y < H; ++y)
        for (int x = 0; x < W; ++x) {
            QColor c = m_image.pixelColor(x, y);
            p.fillRect(x*m_zoom, y*m_zoom, m_zoom, m_zoom, c);
        }

    // 画网格线
    p.setPen(QPen(QColor(180,180,180), 0.5));
    for (int x = 0; x <= W; ++x)
        p.drawLine(x*m_zoom, 0, x*m_zoom, H*m_zoom);
    for (int y = 0; y <= H; ++y)
        p.drawLine(0, y*m_zoom, W*m_zoom, y*m_zoom);
}

void CanvasWidget::mousePressEvent(QMouseEvent *e)
{
    m_drawing = true;
    drawPixel(e->pos(), e->button());
}

void CanvasWidget::mouseMoveEvent(QMouseEvent *e)
{
    QPoint px = toPixel(e->pos());
    emit pixelHovered(px.x(), px.y());
    if (m_drawing)
        drawPixel(e->pos(), e->buttons() & Qt::LeftButton
                                ? Qt::LeftButton : Qt::RightButton);
}

void CanvasWidget::mouseReleaseEvent(QMouseEvent *)
{
    m_drawing = false;
}
