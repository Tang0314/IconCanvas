#include "canvaswidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QUndoCommand>

class DrawCommand : public QUndoCommand
{
public:
    DrawCommand(CanvasWidget *canvas, const QImage &before, const QImage &after)
        : m_canvas(canvas), m_before(before), m_after(after)
    { setText("Draw"); }

    void undo() override { m_canvas->m_image = m_before; m_canvas->update(); }
    void redo() override { m_canvas->m_image = m_after;  m_canvas->update(); }

private:
    CanvasWidget *m_canvas;
    QImage m_before, m_after;
};

CanvasWidget::CanvasWidget(int w, int h, QUndoStack *undoStack, QWidget *parent)
    : QWidget(parent)
    , m_image(w, h, QImage::Format_ARGB32)
    , m_undoStack(undoStack)
{
    m_image.fill(Qt::white);
    setFixedSize(w * m_zoom + 1, h * m_zoom + 1);
    setMouseTracking(true);
}

void CanvasWidget::setZoom(int zoom)
{
    m_zoom      = zoom;
    m_gridDirty = true;  // 缩放变了，网格缓存失效
    setFixedSize(m_image.width()  * m_zoom + 1,
                 m_image.height() * m_zoom + 1);
    update();
}

void CanvasWidget::setForeColor(const QColor &c) { m_foreColor = c; }
void CanvasWidget::setBackColor(const QColor &c) { m_backColor = c; }

void CanvasWidget::newImage(int w, int h)
{
    m_image     = QImage(w, h, QImage::Format_ARGB32);
    m_image.fill(Qt::white);
    m_gridDirty = true;
    setFixedSize(w * m_zoom + 1, h * m_zoom + 1);
    update();
    emit imageChanged();
}

bool CanvasWidget::loadImage(const QString &path)
{
    QImage img(path);
    if (img.isNull()) return false;
    m_image     = img.convertToFormat(QImage::Format_ARGB32);
    m_gridDirty = true;
    setFixedSize(m_image.width()  * m_zoom + 1,
                 m_image.height() * m_zoom + 1);
    update();
    emit imageChanged();
    return true;
}

bool CanvasWidget::saveImage(const QString &path)
{
    return m_image.save(path);
}

// 预先把网格线画到 QPixmap 缓存里，只在尺寸/缩放变化时重建
void CanvasWidget::rebuildGridCache()
{
    int W = m_image.width(), H = m_image.height();
    m_gridCache = QPixmap(W * m_zoom + 1, H * m_zoom + 1);
    m_gridCache.fill(Qt::transparent);

    QPainter p(&m_gridCache);
    p.setPen(QPen(QColor(160, 160, 160), 0.5));
    for (int x = 0; x <= W; ++x)
        p.drawLine(x * m_zoom, 0, x * m_zoom, H * m_zoom);
    for (int y = 0; y <= H; ++y)
        p.drawLine(0, y * m_zoom, W * m_zoom, y * m_zoom);

    m_gridDirty = false;
}

QPoint CanvasWidget::toPixel(const QPoint &pos) const
{
    return QPoint(pos.x() / m_zoom, pos.y() / m_zoom);
}

void CanvasWidget::applyPixel(const QPoint &px, const QColor &c)
{
    if (!m_image.rect().contains(px)) return;
    m_image.setPixelColor(px, c);

    // 只刷新被修改的那一格，不刷新整个画布
    QRect dirty(px.x() * m_zoom, px.y() * m_zoom, m_zoom, m_zoom);
    update(dirty);
}

void CanvasWidget::paintEvent(QPaintEvent *e)
{
    QPainter p(this);
    int W = m_image.width(), H = m_image.height();

    // 只重绘脏区域内的像素格
    QRect clip = e->rect();

    int x0 = qMax(0, clip.left()   / m_zoom);
    int y0 = qMax(0, clip.top()    / m_zoom);
    int x1 = qMin(W - 1, clip.right()  / m_zoom);
    int y1 = qMin(H - 1, clip.bottom() / m_zoom);

    // 棋盘格背景（透明区域）
    for (int y = y0; y <= y1; ++y)
        for (int x = x0; x <= x1; ++x) {
            QColor bg = ((x + y) % 2 == 0)
                            ? QColor(204, 204, 204) : Qt::white;
            p.fillRect(x * m_zoom, y * m_zoom, m_zoom, m_zoom, bg);
        }

    // 图像像素
    for (int y = y0; y <= y1; ++y)
        for (int x = x0; x <= x1; ++x) {
            QColor c = m_image.pixelColor(x, y);
            if (c.alpha() > 0)
                p.fillRect(x * m_zoom, y * m_zoom, m_zoom, m_zoom, c);
        }

    // 网格线（从缓存直接贴，不重新计算）
    if (m_gridDirty) rebuildGridCache();
    p.drawPixmap(0, 0, m_gridCache);
}

void CanvasWidget::mousePressEvent(QMouseEvent *e)
{
    m_drawing = true;
    m_backup  = m_image.copy();
    QPoint px = toPixel(e->pos());
    QColor c  = (e->button() == Qt::LeftButton) ? m_foreColor : m_backColor;
    applyPixel(px, c);
}

void CanvasWidget::mouseMoveEvent(QMouseEvent *e)
{
    QPoint px = toPixel(e->pos());
    emit pixelHovered(px.x(), px.y());
    if (m_drawing) {
        QColor c = (e->buttons() & Qt::LeftButton) ? m_foreColor : m_backColor;
        applyPixel(px, c);
    }
}

void CanvasWidget::mouseReleaseEvent(QMouseEvent *)
{
    if (m_drawing && m_undoStack)
        m_undoStack->push(new DrawCommand(this, m_backup, m_image));
    m_drawing = false;
    emit imageChanged();
}
