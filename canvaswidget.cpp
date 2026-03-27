#include "canvaswidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QUndoCommand>

// 撤销命令：记录一次完整笔划前后的图像
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
    m_zoom = zoom;
    setFixedSize(m_image.width()  * m_zoom + 1,
                 m_image.height() * m_zoom + 1);
    update();
}

void CanvasWidget::setForeColor(const QColor &c) { m_foreColor = c; }
void CanvasWidget::setBackColor(const QColor &c) { m_backColor = c; }

void CanvasWidget::newImage(int w, int h)
{
    m_image = QImage(w, h, QImage::Format_ARGB32);
    m_image.fill(Qt::white);
    setFixedSize(w * m_zoom + 1, h * m_zoom + 1);
    update();
    emit imageChanged();
}

bool CanvasWidget::loadImage(const QString &path)
{
    QImage img(path);
    if (img.isNull()) return false;
    m_image = img.convertToFormat(QImage::Format_ARGB32);
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

QPoint CanvasWidget::toPixel(const QPoint &pos) const
{
    return QPoint(pos.x() / m_zoom, pos.y() / m_zoom);
}

void CanvasWidget::applyPixel(const QPoint &px, const QColor &c)
{
    if (!m_image.rect().contains(px)) return;
    m_image.setPixelColor(px, c);
    update();
}

void CanvasWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    int W = m_image.width(), H = m_image.height();

    // 棋盘格背景（表示透明）
    for (int y = 0; y < H; ++y)
        for (int x = 0; x < W; ++x) {
            QColor bg = ((x + y) % 2 == 0)
                            ? QColor(204,204,204) : QColor(255,255,255);
            p.fillRect(x*m_zoom, y*m_zoom, m_zoom, m_zoom, bg);
        }

    // 绘制图像
    for (int y = 0; y < H; ++y)
        for (int x = 0; x < W; ++x) {
            QColor c = m_image.pixelColor(x, y);
            if (c.alpha() > 0)
                p.fillRect(x*m_zoom, y*m_zoom, m_zoom, m_zoom, c);
        }

    // 网格线
    p.setPen(QPen(QColor(160,160,160), 0.5));
    for (int x = 0; x <= W; ++x)
        p.drawLine(x*m_zoom, 0, x*m_zoom, H*m_zoom);
    for (int y = 0; y <= H; ++y)
        p.drawLine(0, y*m_zoom, W*m_zoom, y*m_zoom);
}

void CanvasWidget::mousePressEvent(QMouseEvent *e)
{
    m_drawing = true;
    m_backup  = m_image.copy();  // 保存笔划前的状态
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
    if (m_drawing && m_undoStack) {
        // 笔划结束，推入撤销栈
        m_undoStack->push(new DrawCommand(this, m_backup, m_image));
    }
    m_drawing = false;
    emit imageChanged();
}
