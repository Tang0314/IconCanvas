#include "colorpalette.h"
#include <QPainter>
#include <QMouseEvent>
#include <QColorDialog>

const QList<QColor> ColorPalette::s_palette = {
    Qt::black,         QColor(128,128,128), QColor(192,192,192), Qt::white,
    QColor(128,0,0),   Qt::red,             QColor(255,128,0),   Qt::yellow,
    QColor(0,128,0),   Qt::green,           QColor(0,128,128),   Qt::cyan,
    QColor(0,0,128),   Qt::blue,            QColor(128,0,128),   QColor(255,0,255)
};

ColorPalette::ColorPalette(QWidget *parent) : QWidget(parent)
{
    setFixedWidth(130);
    setMinimumHeight(260);
}

void ColorPalette::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    const int sz = 22;

    // 背景色块（在后面）
    p.fillRect(16, 16, 36, 36, m_back);
    p.setPen(QPen(Qt::black, 1));
    p.drawRect(16, 16, 36, 36);

    // 前景色块（在前面）
    p.fillRect(4, 4, 36, 36, m_fore);
    p.setPen(QPen(Qt::black, 1));
    p.drawRect(4, 4, 36, 36);

    // 互换箭头提示
    p.setPen(QColor(80,80,80));
    p.setFont(QFont("Arial", 8));
    p.drawText(55, 30, "⇄");

    // 分割线
    p.setPen(QColor(200,200,200));
    p.drawLine(4, 62, 126, 62);

    // 调色板标题
    p.setPen(QColor(80,80,80));
    p.setFont(QFont("Arial", 8));
    p.drawText(4, 76, "Colors (L=fore, R=back):");

    // 调色板格子
    const int cols = 4;
    for (int i = 0; i < s_palette.size(); ++i) {
        int col = i % cols;
        int row = i / cols;
        int x = 4 + col * (sz + 3);
        int y = 82 + row * (sz + 3);
        p.fillRect(x, y, sz, sz, s_palette[i]);
        p.setPen(QColor(120,120,120));
        p.drawRect(x, y, sz, sz);
    }

    // 双击自定义颜色提示
    p.setPen(QColor(120,120,120));
    p.setFont(QFont("Arial", 7));
    p.drawText(4, 200, "Double-click: custom color");
}

void ColorPalette::mousePressEvent(QMouseEvent *e)
{
    // 点击互换区域
    if (QRect(50, 15, 30, 25).contains(e->pos())) {
        std::swap(m_fore, m_back);
        emit foreColorChanged(m_fore);
        emit backColorChanged(m_back);
        update();
        return;
    }

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

void ColorPalette::mouseDoubleClickEvent(QMouseEvent *e)
{
    QColor initial = (e->button() == Qt::RightButton) ? m_back : m_fore;
    QColor c = QColorDialog::getColor(initial, this, "Select Color",
                                      QColorDialog::ShowAlphaChannel);
    if (!c.isValid()) return;
    if (e->button() == Qt::RightButton) {
        m_back = c;
        emit backColorChanged(c);
    } else {
        m_fore = c;
        emit foreColorChanged(c);
    }
    update();
}

QColor ColorPalette::colorAt(const QPoint &pos) const
{
    const int sz = 22, cols = 4;
    for (int i = 0; i < s_palette.size(); ++i) {
        int col = i % cols;
        int row = i / cols;
        int x = 4 + col * (sz + 3);
        int y = 82 + row * (sz + 3);
        if (QRect(x, y, sz, sz).contains(pos))
            return s_palette[i];
    }
    return QColor();
}

void ColorPalette::openColorDialog()
{
    QColor c = QColorDialog::getColor(m_fore, this, "Select Foreground Color",
                                      QColorDialog::ShowAlphaChannel);
    if (!c.isValid()) return;
    m_fore = c;
    emit foreColorChanged(c);
    update();
}
