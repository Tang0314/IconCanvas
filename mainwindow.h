#pragma once
#include <QMainWindow>
#include <QLabel>
#include <QScrollArea>
#include <QUndoStack>
#include <QFileInfo>
#include "canvaswidget.h"
#include "colorpalette.h"
#include "translator.h"
#include "previewwidget.h"

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
    void retranslateUI();

    CanvasWidget  *m_canvas;
    ColorPalette  *m_palette;
    QLabel        *m_statusLabel;
    QLabel        *m_hintLabel;
    QUndoStack    *m_undoStack;
    QString        m_currentFile;
    PreviewWidget *m_preview;

    // 菜单引用，切换语言时重建
    QMenuBar      *m_menuBar = nullptr;
};
