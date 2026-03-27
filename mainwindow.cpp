#include "mainwindow.h"
#include <QMenuBar>
#include <QStatusBar>
#include <QDockWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("IconCanvas");
    resize(800, 600);
    setupUI();
    setupMenuBar();
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI()
{
    // 中央画布
    m_canvas = new CanvasWidget(32, 32, this);
    QScrollArea *scroll = new QScrollArea(this);
    scroll->setWidget(m_canvas);
    scroll->setAlignment(Qt::AlignCenter);
    scroll->setBackgroundRole(QPalette::Dark);
    setCentralWidget(scroll);

    // 左侧颜色面板
    m_palette = new ColorPalette(this);
    QDockWidget *paletteDock = new QDockWidget("Colors", this);
    paletteDock->setWidget(m_palette);
    paletteDock->setAllowedAreas(Qt::LeftDockWidgetArea);
    paletteDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    addDockWidget(Qt::LeftDockWidgetArea, paletteDock);

    // 状态栏
    m_statusLabel = new QLabel("Ready", this);
    statusBar()->addWidget(m_statusLabel);

    // 画布坐标反馈
    connect(m_canvas, &CanvasWidget::pixelHovered,
            this, [this](int x, int y){
                m_statusLabel->setText(QString("X: %1  Y: %2").arg(x).arg(y));
            });
}

void MainWindow::setupMenuBar()
{
    QMenu *fileMenu = menuBar()->addMenu("File");
    fileMenu->addAction("New",  this, [](){});
    fileMenu->addAction("Open", this, [](){});
    fileMenu->addAction("Save", this, [](){});
    fileMenu->addSeparator();
    fileMenu->addAction("Exit", this, &QWidget::close);

    QMenu *editMenu = menuBar()->addMenu("Edit");
    editMenu->addAction("Undo", this, [](){});
    editMenu->addAction("Redo", this, [](){});
}
