#pragma once
#include <QMainWindow>
#include <QLabel>
#include <QScrollArea>
#include <QUndoStack>
#include <QFileInfo>
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
    void setupShortcuts();
    void newCanvas(int w, int h);
    void openFile();
    void saveFile();

    CanvasWidget  *m_canvas;
    ColorPalette  *m_palette;
    QLabel        *m_statusLabel;
    QUndoStack    *m_undoStack;
    QString        m_currentFile;
};
