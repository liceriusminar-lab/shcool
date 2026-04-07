#include "tetriswidget.h"
#include <QPainter>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QKeyEvent>

TetrisWidget::TetrisWidget(QWidget *parent) : QWidget(parent)
{
    setFixedSize(BOARD_WIDTH * BLOCK_SIZE + 220, BOARD_HEIGHT * BLOCK_SIZE);
    board = QVector<QVector<int>>(BOARD_HEIGHT, QVector<int>(BOARD_WIDTH, 0));

    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, &TetrisWidget::timerTick);
}

TetrisWidget::~TetrisWidget()
{
    delete currentTetromino;
}

void TetrisWidget::startGame()
{
    score = 0;
    gameOver = false;
    board.fill(QVector<int>(BOARD_WIDTH, 0));

    if (currentTetromino) {
        delete currentTetromino;
        currentTetromino = nullptr;
    }

    spawnNewTetromino();
    gameTimer->start(600);
    update();
}

void TetrisWidget::timerTick()
{
    if (!currentTetromino || gameOver) return;

    currentTetromino->move(0, 1);

    if (!isValidPosition(*currentTetromino)) {
        currentTetromino->move(0, -1);
        lockTetromino();
        spawnNewTetromino();
    }
    update();
}

void TetrisWidget::keyPressEvent(QKeyEvent *event)
{
    if (!currentTetromino || gameOver) return;

    switch (event->key()) {
    case Qt::Key_Left:
        currentTetromino->move(-1, 0);
        if (!isValidPosition(*currentTetromino)) currentTetromino->move(1, 0);
        break;
    case Qt::Key_Right:
        currentTetromino->move(1, 0);
        if (!isValidPosition(*currentTetromino)) currentTetromino->move(-1, 0);
        break;
    case Qt::Key_Down:
        currentTetromino->move(0, 1);
        if (!isValidPosition(*currentTetromino)) {
            currentTetromino->move(0, -1);
            lockTetromino();
            spawnNewTetromino();
        }
        break;
    case Qt::Key_Up:
        currentTetromino->rotate();
        if (!isValidPosition(*currentTetromino))
            currentTetromino->rotate(); // откат
        break;
    case Qt::Key_Space:
        dropTetromino();
        break;
    default:
        QWidget::keyPressEvent(event);
        return;
    }
    update();
}

void TetrisWidget::dropTetromino()
{
    if (!currentTetromino) return;
    while (isValidPosition(*currentTetromino)) {
        currentTetromino->move(0, 1);
    }
    currentTetromino->move(0, -1);
    lockTetromino();
    spawnNewTetromino();
    update();
}

bool TetrisWidget::isValidPosition(const Tetromino &tet) const
{
    for (const QPoint &p : tet.getBlocks()) {
        int x = p.x(), y = p.y();
        if (x < 0 || x >= BOARD_WIDTH || y >= BOARD_HEIGHT) return false;
        if (y < 0) continue;
        if (board[y][x] != 0) return false;
    }
    return true;
}

void TetrisWidget::lockTetromino()
{
    if (!currentTetromino) return;

    int type = static_cast<int>(currentTetromino->getType()) + 1;
    for (const QPoint &p : currentTetromino->getBlocks()) {
        int x = p.x(), y = p.y();
        if (y >= 0 && y < BOARD_HEIGHT && x >= 0 && x < BOARD_WIDTH)
            board[y][x] = type;
    }

    delete currentTetromino;
    currentTetromino = nullptr;

    clearLines();

    spawnNewTetromino(); // следующая фигура
}

void TetrisWidget::clearLines()
{
    int lines = 0;
    for (int y = BOARD_HEIGHT - 1; y >= 0; --y) {
        bool full = true;
        for (int x = 0; x < BOARD_WIDTH; ++x) {
            if (board[y][x] == 0) { full = false; break; }
        }
        if (full) {
            lines++;
            for (int yy = y; yy > 0; --yy)
                board[yy] = board[yy-1];
            board[0].fill(0);
            y++;
        }
    }

    if (lines > 0) {
        const int points[] = {0, 100, 300, 500, 800};
        score += points[lines] * ((score / 2000) + 1);
    }
}

void TetrisWidget::spawnNewTetromino()
{
    if (currentTetromino) {
        delete currentTetromino;
        currentTetromino = nullptr;
    }

    int r = QRandomGenerator::global()->bounded(7);
    currentTetromino = new Tetromino(static_cast<Tetromino::Type>(r));

    if (!isValidPosition(*currentTetromino)) {
        gameOver = true;
        gameTimer->stop();
        QMessageBox::information(this, "Игра окончена", QString("Ваш счёт: %1").arg(score));
    }
}

void TetrisWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.fillRect(rect(), Qt::black);

    // Фиксированные блоки
    for (int y = 0; y < BOARD_HEIGHT; ++y) {
        for (int x = 0; x < BOARD_WIDTH; ++x) {
            if (board[y][x] != 0) {
                QColor c;
                switch(board[y][x]) {
                case 1: c = Qt::cyan; break;
                case 2: c = Qt::yellow; break;
                case 3: c = Qt::magenta; break;
                case 4: c = Qt::green; break;
                case 5: c = Qt::red; break;
                case 6: c = Qt::blue; break;
                case 7: c = Qt::darkYellow; break;
                }
                p.fillRect(x*BLOCK_SIZE, y*BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, c);
                p.drawRect(x*BLOCK_SIZE, y*BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
            }
        }
    }

    // Текущая фигура
    if (currentTetromino) {
        p.setBrush(currentTetromino->getColor());
        p.setPen(Qt::black);
        for (const QPoint &pos : currentTetromino->getBlocks()) {
            if (pos.y() >= 0) {
                p.fillRect(pos.x()*BLOCK_SIZE, pos.y()*BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE,
                           currentTetromino->getColor());
                p.drawRect(pos.x()*BLOCK_SIZE, pos.y()*BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE);
            }
        }
    }

    // Информация
    p.setPen(Qt::white);
    p.setFont(QFont("Arial", 14));
    p.drawText(BOARD_WIDTH * BLOCK_SIZE + 20, 60, "СЧЁТ");
    p.drawText(BOARD_WIDTH * BLOCK_SIZE + 20, 90, QString::number(score));

    if (gameOver) {
        p.setPen(Qt::red);
        p.setFont(QFont("Arial", 24, QFont::Bold));
        p.drawText(rect(), Qt::AlignCenter, "GAME OVER");
    }
}
