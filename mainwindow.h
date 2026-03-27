#pragma once
#include <QMainWindow>
#include <QLabel>
#include <QScrollArea>
#include "canvaswidget.h"
#include "colorpalette.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void setupUI();
    void setupMenuBar();

    CanvasWidget   *m_canvas;
    ColorPalette   *m_palette;
    QLabel         *m_statusLabel;
};
