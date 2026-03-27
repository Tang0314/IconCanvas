#pragma once
#include <QWidget>
#include <QImage>
#include "translator.h"

class PreviewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PreviewWidget(QWidget *parent = nullptr);
    void setImage(const QImage &img);

protected:
    void paintEvent(QPaintEvent *) override;

private:
    QImage m_image;
};
