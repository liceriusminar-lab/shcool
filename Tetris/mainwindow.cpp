#include "mainwindow.h"
#include "tetriswidget.h"
#include <QMenuBar>
#include <QAction>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Тетрис");

    m_tetris = new TetrisWidget(this);
    setCentralWidget(m_tetris);

    // Размер окна по содержимому
    adjustSize();
    setFixedSize(size());   // запретить ресайз

    // Меню
    QMenu    *gameMenu    = menuBar()->addMenu("Игра");
    QAction  *actNew      = gameMenu->addAction("Новая игра");
    actNew->setShortcut(QKeySequence("F2"));
    connect(actNew, &QAction::triggered, m_tetris, &TetrisWidget::startGame);

    QAction *actQuit = gameMenu->addAction("Выйти");
    actQuit->setShortcut(QKeySequence("Alt+F4"));
    connect(actQuit, &QAction::triggered, this, &QWidget::close);
}
