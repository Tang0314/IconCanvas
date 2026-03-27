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
    QDockWidget *palDock = new QDockWidget(T("Colors"), this);
    palDock->setWidget(m_palette);
    palDock->setAllowedAreas(Qt::LeftDockWidgetArea);
    palDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    addDockWidget(Qt::LeftDockWidgetArea, palDock);

    m_statusLabel = new QLabel("X: -  Y: -", this);
    m_hintLabel = new QLabel(
        "  |  Ctrl+N: New  Ctrl+O: Open  Ctrl+S: Save  "
        "Ctrl+Z: Undo  Ctrl+Y: Redo  X: Swap  F: Custom Color", this);
    m_hintLabel->setStyleSheet("color: gray; font-size: 11px;");
    statusBar()->addWidget(m_statusLabel);
    statusBar()->addWidget(m_hintLabel);

    connect(m_palette, &ColorPalette::foreColorChanged,
            m_canvas,  &CanvasWidget::setForeColor);
    connect(m_palette, &ColorPalette::backColorChanged,
            m_canvas,  &CanvasWidget::setBackColor);
    connect(m_canvas, &CanvasWidget::pixelHovered,
            this, [this](int x, int y){
                m_statusLabel->setText(QString("X: %1  Y: %2").arg(x).arg(y));
            });
}

void MainWindow::setupMenuBar()
{
    // 清除旧菜单
    if (m_menuBar) {
        m_menuBar->clear();
    } else {
        m_menuBar = menuBar();
    }

    // File
    QMenu *fileMenu = m_menuBar->addMenu(T("File"));
    fileMenu->addAction(T("New 16x16"), this, [this](){ newCanvas(16,16); });
    fileMenu->addAction(T("New 32x32"), this, [this](){ newCanvas(32,32); });
    fileMenu->addAction(T("New 48x48"), this, [this](){ newCanvas(48,48); });
    fileMenu->addSeparator();
    QAction *openAct = fileMenu->addAction(T("Open..."), this, &MainWindow::openFile);
    openAct->setShortcut(QKeySequence::Open);
    QAction *saveAct = fileMenu->addAction(T("Save..."), this, &MainWindow::saveFile);
    saveAct->setShortcut(QKeySequence::Save);
    fileMenu->addSeparator();
    fileMenu->addAction(T("Exit"), this, &QWidget::close);

    // Edit
    QMenu *editMenu = m_menuBar->addMenu(T("Edit"));
    QAction *undoAct = m_undoStack->createUndoAction(this, T("Undo"));
    QAction *redoAct = m_undoStack->createRedoAction(this, T("Redo"));
    undoAct->setShortcut(QKeySequence::Undo);
    redoAct->setShortcut(QKeySequence::Redo);
    editMenu->addAction(undoAct);
    editMenu->addAction(redoAct);

    // View
    QMenu *viewMenu = m_menuBar->addMenu(T("View"));
    viewMenu->addAction(T("Zoom 4x"),  this, [this](){ m_canvas->setZoom(4);  });
    viewMenu->addAction(T("Zoom 8x"),  this, [this](){ m_canvas->setZoom(8);  });
    viewMenu->addAction(T("Zoom 12x"), this, [this](){ m_canvas->setZoom(12); });
    viewMenu->addAction(T("Zoom 16x"), this, [this](){ m_canvas->setZoom(16); });

    // Language
    QMenu *langMenu = m_menuBar->addMenu(T("Language"));
    QAction *enAct = langMenu->addAction("English", this, [this](){
        Translator::instance().setLanguage(Translator::English);
        setupMenuBar();
        retranslateUI();
    });
    QAction *zhAct = langMenu->addAction("中文", this, [this](){
        Translator::instance().setLanguage(Translator::Chinese);
        setupMenuBar();
        retranslateUI();
        m_palette->update();
    });
    // 当前语言打勾
    enAct->setCheckable(true);
    zhAct->setCheckable(true);
    if (Translator::instance().language() == Translator::English)
        enAct->setChecked(true);
    else
        zhAct->setChecked(true);
}

void MainWindow::retranslateUI()
{
    // 更新状态栏提示
    if (Translator::instance().language() == Translator::Chinese) {
        m_hintLabel->setText(
            "  |  Ctrl+N: 新建  Ctrl+O: 打开  Ctrl+S: 保存  "
            "Ctrl+Z: 撤销  Ctrl+Y: 重做  X: 互换颜色  F: 自定义颜色");
    } else {
        m_hintLabel->setText(
            "  |  Ctrl+N: New  Ctrl+O: Open  Ctrl+S: Save  "
            "Ctrl+Z: Undo  Ctrl+Y: Redo  X: Swap  F: Custom Color");
    }
}

void MainWindow::setupShortcuts()
{
    // Ctrl+N 新建（菜单里没有绑定）
    new QShortcut(QKeySequence::New, this, [this](){ newCanvas(32,32); });

    // X 键互换颜色
    new QShortcut(Qt::Key_X, this, [this](){ m_palette->swapColors(); });

    // F 键自定义颜色
    new QShortcut(Qt::Key_F, this, [this](){ m_palette->openColorDialog(); });
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
        this, T("Open Image"), "",
        T("Icon Files (*.ico);;PNG Files (*.png);;All Files (*)"));
    if (path.isEmpty()) return;
    if (m_canvas->loadImage(path)) {
        m_undoStack->clear();
        setWindowTitle("IconCanvas - " + QFileInfo(path).fileName());
        m_currentFile = path;
    } else {
        QMessageBox::warning(this, T("Error"), T("Failed to open file."));
    }
}

void MainWindow::saveFile()
{
    QString path = m_currentFile.isEmpty()
                       ? QFileDialog::getSaveFileName(
                           this, T("Save Image"), "icon.ico",
                           T("Icon Files (*.ico);;PNG Files (*.png);;All Files (*)"))
                       : m_currentFile;
    if (path.isEmpty()) return;
    if (m_canvas->saveImage(path)) {
        m_currentFile = path;
        setWindowTitle("IconCanvas - " + QFileInfo(path).fileName());
    } else {
        QMessageBox::warning(this, T("Error"), T("Failed to save file."));
    }
}
