#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Тетрис");

    stack = new QStackedWidget(this);
    setCentralWidget(stack);

    menu = new MenuWidget(this);
    game = new TetrisWidget(this);
    api  = new ApiClient(this);

    stack->addWidget(menu);
    stack->addWidget(game);

    connect(menu, &MenuWidget::startRequested, this, &MainWindow::showGame);
    connect(menu, &MenuWidget::nickEntered,    this, [this](const QString &name) {
        api->checkNick(name);
    });
    connect(game, &TetrisWidget::gameFinished, this, &MainWindow::onGameOver);
    connect(api,  &ApiClient::scoresReceived,  menu, &MenuWidget::refreshScores);
    connect(api,  &ApiClient::nickChecked, this, [this](bool ok, const QString &reason) {
        if (ok) {
            menu->setNickOk();
        } else {
            if (reason == "taken")
                menu->setNickError("❌ Ник занят другим игроком");
            else
                menu->setNickError("❌ Недопустимый ник");
        }
    });

    resize(TetrisWidget::BOARD_WIDTH  * TetrisWidget::BLOCK_SIZE + 160,
           TetrisWidget::BOARD_HEIGHT * TetrisWidget::BLOCK_SIZE);

    api->fetchScores();
    showMenu();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::showMenu()
{
    api->fetchScores();
    stack->setCurrentIndex(0);
}

void MainWindow::showGame(const QString &nickname)
{
    currentPlayer = nickname;
    stack->setCurrentIndex(1);
    game->startGame();
}

void MainWindow::onGameOver(int score)
{
    api->postScore(currentPlayer, score);
    showMenu();
}
