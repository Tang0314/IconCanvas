#pragma once
#include <QWidget>
#include <QColor>

class ColorPalette : public QWidget
{
    Q_OBJECT
public:
    explicit ColorPalette(QWidget *parent = nullptr);

    QColor foreColor() const { return m_fore; }
    QColor backColor() const { return m_back; }

signals:
    void foreColorChanged(const QColor &c);
    void backColorChanged(const QColor &c);

protected:
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void mouseDoubleClickEvent(QMouseEvent *) override;

private:
    QColor colorAt(const QPoint &pos) const;

    QColor m_fore = Qt::black;
    QColor m_back = Qt::white;

    static const QList<QColor> s_palette;
};
