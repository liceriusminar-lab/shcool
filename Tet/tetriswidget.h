#ifndef TETRISWIDGET_H
#define TETRISWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QVector>
#include "tetromino.h"

class TetrisWidget : public QWidget {
    Q_OBJECT

public:
    explicit TetrisWidget(QWidget *parent = nullptr);
    ~TetrisWidget();

    void startGame();

    // ← Константы теперь публичные (чтобы mainwindow.cpp мог их использовать)
    static const int BOARD_WIDTH = 10;
    static const int BOARD_HEIGHT = 20;
    static const int BLOCK_SIZE = 30;

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void timerTick();

private:
    bool isValidPosition(const Tetromino &tet) const;
    void lockTetromino();
    void clearLines();
    void spawnNewTetromino();
    void dropTetromino();

    QVector<QVector<int>> board;
    Tetromino *currentTetromino = nullptr;
    QTimer *gameTimer = nullptr;

    int score = 0;
    bool gameOver = false;
};

#endif
