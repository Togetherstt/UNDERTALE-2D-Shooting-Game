#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gamescene.h"
#include "customization.h"
#include <QGraphicsView>
#include <QStackedWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 1. 创建 QStackedWidget
    m_stackedWidget = new QStackedWidget(this);

    // 2. 创建开始菜单页面
    QWidget* menuWidget = new QWidget;
    QVBoxLayout* menuLayout = new QVBoxLayout(menuWidget);
    menuLayout->setAlignment(Qt::AlignCenter);

    QLabel* titleLabel = new QLabel("Undertale 射击游戏v1.0.0");
    titleLabel->setFont(QFont("Arial", 40, QFont::Bold));

    QPushButton* startButton = new QPushButton("开始游戏");
    // startButton->setFixedSize(200, 50);
    startButton->move(100,50);
    startButton->setGeometry(100, 50, 100, 30);

    menuLayout->addWidget(titleLabel);
    menuLayout->addSpacing(50);
    menuLayout->addWidget(startButton);

    // Create and set up the game scene and view
    m_gameScene = new GameScene(this);
    QGraphicsView* view = new QGraphicsView(m_gameScene);
    view->setFixedSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    m_gameScene->setSceneRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // View optimizations
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setRenderHint(QPainter::Antialiasing);
    view->setCacheMode(QGraphicsView::CacheBackground);
    view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);

    // 4. 将两个页面添加到 QStackedWidget
    m_stackedWidget->addWidget(menuWidget);  // Index 0: Menu
    m_stackedWidget->addWidget(view);   // Index 1: Game

    // 5. 设置主窗口的中心控件，并连接信号和槽
    setCentralWidget(m_stackedWidget);
    connect(startButton, &QPushButton::clicked, this, &MainWindow::startGame);

    // 默认显示菜单页面
    m_stackedWidget->setCurrentIndex(0);

//    setCentralWidget(view);
    setWindowTitle("Undertale 射击游戏v1.0.0");
    setWindowIcon(QIcon(GAME_ICON));
}
void MainWindow::startGame()
{
    // 调用 GameScene 的方法来初始化/重置游戏
    m_gameScene->startNewGame();
    // 切换到游戏页面
    m_stackedWidget->setCurrentIndex(1);
}

MainWindow::~MainWindow()
{
    delete ui;
}
