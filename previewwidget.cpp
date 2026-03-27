#include "previewwidget.h"
#include <QPainter>

PreviewWidget::PreviewWidget(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(130, 150);
}

void PreviewWidget::setImage(const QImage &img)
{
    m_image = img;
    update();
}

void PreviewWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    int W = width();

    // 标题
    p.setPen(QColor(80, 80, 80));
    p.setFont(QFont("Arial", 8));
    p.drawText(4, 14, T("Preview:"));

    // 预览区域背景（棋盘格表示透明）
    int previewSize = 100;
    int ox = (W - previewSize) / 2;
    int oy = 20;

    for (int y = 0; y < previewSize; y += 8)
        for (int x = 0; x < previewSize; x += 8) {
            QColor bg = ((x / 8 + y / 8) % 2 == 0)
                            ? QColor(204, 204, 204) : Qt::white;
            p.fillRect(ox + x, oy + y,
                       qMin(8, previewSize - x),
                       qMin(8, previewSize - y), bg);
        }

    // 边框
    p.setPen(QPen(QColor(160, 160, 160), 1));
    p.drawRect(ox, oy, previewSize, previewSize);

    if (m_image.isNull()) return;

    // 1:1 真实大小居中显示
    int imgW = m_image.width();
    int imgH = m_image.height();
    int ix = ox + (previewSize - imgW) / 2;
    int iy = oy + (previewSize - imgH) / 2;
    p.drawImage(ix, iy, m_image);

    // 尺寸标注
    p.setPen(QColor(100, 100, 100));
    p.setFont(QFont("Arial", 8));
    p.drawText(4, oy + previewSize + 16,
               QString("%1 x %2 px").arg(imgW).arg(imgH));
}
