#include "mainwindow.h"
#include "tetriswidget.h"      // ← обязательно должен быть
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    TetrisWidget *game = new TetrisWidget(this);
    setCentralWidget(game);

    // Исправленная строка с префиксом TetrisWidget::
    resize(TetrisWidget::BOARD_WIDTH * TetrisWidget::BLOCK_SIZE + 280,
           TetrisWidget::BOARD_HEIGHT * TetrisWidget::BLOCK_SIZE + 60);

    // Запускаем игру сразу
    game->startGame();
}

MainWindow::~MainWindow()
{
    delete ui;
}
