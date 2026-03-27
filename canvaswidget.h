#pragma once
#include <QWidget>
#include <QImage>
#include <QColor>
#include <QPixmap>
#include <QUndoStack>

class CanvasWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CanvasWidget(int w, int h, QUndoStack *undoStack,
                          QWidget *parent = nullptr);

    void setZoom(int zoom);
    void setForeColor(const QColor &c);
    void setBackColor(const QColor &c);
    void newImage(int w, int h);
    bool loadImage(const QString &path);
    bool saveImage(const QString &path);
    QImage image() const { return m_image; }

signals:
    void pixelHovered(int x, int y);
    void imageChanged();

protected:
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;

    friend class DrawCommand;
private:
    void   rebuildGridCache();
    QPoint toPixel(const QPoint &pos) const;
    void   applyPixel(const QPoint &px, const QColor &c);

    QImage      m_image;
    QImage      m_backup;
    QPixmap     m_gridCache;
    bool        m_gridDirty = true;
    int         m_zoom      = 12;
    bool        m_drawing   = false;
    QColor      m_foreColor = Qt::black;
    QColor      m_backColor = Qt::white;
    QUndoStack *m_undoStack = nullptr;
};
