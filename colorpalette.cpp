#include "colorpalette.h"
#include <QPainter>
#include <QMouseEvent>

const QList<QColor> ColorPalette::s_palette = {
    Qt::black,      QColor(128,128,128), Qt::darkRed,    Qt::red,
    QColor(255,128,0), Qt::yellow,       Qt::darkGreen,  Qt::green,
    Qt::darkCyan,   Qt::cyan,            Qt::darkBlue,   Qt::blue,
    QColor(128,0,128), QColor(255,0,255),Qt::white,      QColor(255,255,128)
};

ColorPalette::ColorPalette(QWidget *parent) : QWidget(parent)
{
    setFixedWidth(120);
    setMinimumHeight(200);
}

void ColorPalette::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    const int sz = 20;

    // 前景/背景色预览
    p.fillRect(10, 10, 30, 30, m_back);
    p.setPen(Qt::black);
    p.drawRect(10, 10, 30, 30);
    p.fillRect(4, 4, 30, 30, m_fore);
    p.drawRect(4, 4, 30, 30);

    // 调色板
    int cols = 4;
    for (int i = 0; i < s_palette.size(); ++i) {
        int col = i % cols;
        int row = i / cols;
        int x = 4 + col * (sz + 2);
        int y = 50 + row * (sz + 2);
        p.fillRect(x, y, sz, sz, s_palette[i]);
        p.setPen(QColor(100,100,100));
        p.drawRect(x, y, sz, sz);
    }
}

void ColorPalette::mousePressEvent(QMouseEvent *e)
{
    QColor c = colorAt(e->pos());
    if (!c.isValid()) return;
    if (e->button() == Qt::LeftButton) {
        m_fore = c;
        emit foreColorChanged(c);
    } else {
        m_back = c;
        emit backColorChanged(c);
    }
    update();
}

QColor ColorPalette::colorAt(const QPoint &pos) const
{
    const int sz = 20, cols = 4;
    for (int i = 0; i < s_palette.size(); ++i) {
        int col = i % cols;
        int row = i / cols;
        int x = 4 + col * (sz + 2);
        int y = 50 + row * (sz + 2);
        if (QRect(x, y, sz, sz).contains(pos))
            return s_palette[i];
    }
    return QColor();
}
