#include "mainwindow.h"
#include <QMenuBar>
#include <QStatusBar>
#include <QDockWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QShortcut>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("IconCanvas");
    resize(900, 650);
    setupUI();
    setupMenuBar();
    setupShortcuts();
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI()
{
    m_undoStack = new QUndoStack(this);
    m_undoStack->setUndoLimit(64);

    m_canvas = new CanvasWidget(32, 32, m_undoStack, this);
    QScrollArea *scroll = new QScrollArea(this);
    scroll->setWidget(m_canvas);
    scroll->setAlignment(Qt::AlignCenter);
    scroll->setBackgroundRole(QPalette::Dark);
    setCentralWidget(scroll);

    m_palette = new ColorPalette(this);
    QDockWidget *palDock = new QDockWidget("Colors", this);
    palDock->setWidget(m_palette);
    palDock->setAllowedAreas(Qt::LeftDockWidgetArea);
    palDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    addDockWidget(Qt::LeftDockWidgetArea, palDock);

    // 状态栏：坐标 + 快捷键提示
    m_statusLabel = new QLabel("X: -  Y: -", this);
    QLabel *hintLabel = new QLabel(
        "  |  Ctrl+N: New  Ctrl+O: Open  Ctrl+S: Save  "
        "Ctrl+Z: Undo  Ctrl+Y: Redo  X: Swap  F: Custom Color", this);
    hintLabel->setStyleSheet("color: gray; font-size: 11px;");
    statusBar()->addWidget(m_statusLabel);
    statusBar()->addWidget(hintLabel);

    connect(m_palette, &ColorPalette::foreColorChanged,
            m_canvas,  &CanvasWidget::setForeColor);
    connect(m_palette, &ColorPalette::backColorChanged,
            m_canvas,  &CanvasWidget::setBackColor);
    connect(m_canvas,  &CanvasWidget::pixelHovered,
            this, [this](int x, int y){
                m_statusLabel->setText(QString("X: %1  Y: %2").arg(x).arg(y));
            });
}

void MainWindow::setupMenuBar()
{
    QMenu *fileMenu = menuBar()->addMenu("File");
    fileMenu->addAction("New 16x16",  this, [this](){ newCanvas(16,16); });
    fileMenu->addAction("New 32x32",  this, [this](){ newCanvas(32,32); });
    fileMenu->addAction("New 48x48",  this, [this](){ newCanvas(48,48); });
    fileMenu->addSeparator();
    QAction *openAct = fileMenu->addAction("Open...", this, &MainWindow::openFile);
    openAct->setShortcut(QKeySequence::Open);
    QAction *saveAct = fileMenu->addAction("Save...", this, &MainWindow::saveFile);
    saveAct->setShortcut(QKeySequence::Save);
    QAction *newAct  = fileMenu->addAction("New 32x32 (Ctrl+N)",
                                          this, [this](){ newCanvas(32,32); });
    newAct->setShortcut(QKeySequence::New);
    fileMenu->addSeparator();
    fileMenu->addAction("Exit", this, &QWidget::close);

    QMenu *editMenu = menuBar()->addMenu("Edit");
    QAction *undoAct = m_undoStack->createUndoAction(this, "Undo");
    QAction *redoAct = m_undoStack->createRedoAction(this, "Redo");
    undoAct->setShortcut(QKeySequence::Undo);
    redoAct->setShortcut(QKeySequence::Redo);
    editMenu->addAction(undoAct);
    editMenu->addAction(redoAct);

    QMenu *viewMenu = menuBar()->addMenu("View");
    viewMenu->addAction("Zoom 4x",  this, [this](){ m_canvas->setZoom(4);  });
    viewMenu->addAction("Zoom 8x",  this, [this](){ m_canvas->setZoom(8);  });
    viewMenu->addAction("Zoom 12x", this, [this](){ m_canvas->setZoom(12); });
    viewMenu->addAction("Zoom 16x", this, [this](){ m_canvas->setZoom(16); });
}

void MainWindow::setupShortcuts()
{
    // Ctrl+N 新建
    new QShortcut(QKeySequence::New, this, [this](){ newCanvas(32,32); });

    // Ctrl+S 保存
    new QShortcut(QKeySequence::Save, this, [this](){ saveFile(); });

    // Ctrl+O 打开
    new QShortcut(QKeySequence::Open, this, [this](){ openFile(); });

    // X 键 互换前景/背景色
    new QShortcut(Qt::Key_X, this, [this](){
        m_palette->swapColors();
    });

    // F 键 打开自定义颜色对话框
    new QShortcut(Qt::Key_F, this, [this](){
        m_palette->openColorDialog();
    });
}

void MainWindow::newCanvas(int w, int h)
{
    m_canvas->newImage(w, h);
    m_undoStack->clear();
    setWindowTitle(QString("IconCanvas - New %1x%2").arg(w).arg(h));
}

void MainWindow::openFile()
{
    QString path = QFileDialog::getOpenFileName(
        this, "Open Image", "",
        "Icon Files (*.ico);;PNG Files (*.png);;All Files (*)");
    if (path.isEmpty()) return;
    if (m_canvas->loadImage(path)) {
        m_undoStack->clear();
        setWindowTitle("IconCanvas - " + QFileInfo(path).fileName());
        m_currentFile = path;
    } else {
        QMessageBox::warning(this, "Error", "Failed to open file.");
    }
}

void MainWindow::saveFile()
{
    QString path = m_currentFile.isEmpty()
                       ? QFileDialog::getSaveFileName(
                           this, "Save Image", "icon.ico",
                           "Icon Files (*.ico);;PNG Files (*.png);;All Files (*)")
                       : m_currentFile;
    if (path.isEmpty()) return;
    if (m_canvas->saveImage(path)) {
        m_currentFile = path;
        setWindowTitle("IconCanvas - " + QFileInfo(path).fileName());
    } else {
        QMessageBox::warning(this, "Error", "Failed to save file.");
    }
}
