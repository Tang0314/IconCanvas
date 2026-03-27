#pragma once
#include <QWidget>
#include <QImage>
#include <QColor>

class CanvasWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CanvasWidget(int w, int h, QWidget *parent = nullptr);

    void setZoom(int zoom);
    void setForeColor(const QColor &c);
    void setBackColor(const QColor &c);
    QImage image() const { return m_image; }

signals:
    void pixelHovered(int x, int y);
    void imageChanged();

protected:
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;

private:
    QPoint toPixel(const QPoint &pos) const;
    void drawPixel(const QPoint &pos, Qt::MouseButton btn);

    QImage  m_image;
    int     m_zoom   = 12;
    bool    m_drawing = false;
    QColor  m_foreColor = Qt::black;
    QColor  m_backColor = Qt::white;
};
