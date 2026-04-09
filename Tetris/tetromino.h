#ifndef TETROMINO_H
#define TETROMINO_H

#include <QVector>
#include <QPoint>
#include <QColor>

class Tetromino {
public:
    enum class Type { I, O, T, S, Z, J, L, COUNT };

    explicit Tetromino(Type type);

    void rotate();
    void move(int dx, int dy);
    void setPosition(int x, int y);

    QVector<QPoint> getBlocks() const;
    QVector<QPoint> getBlocksAt(int ox, int oy) const;
    QVector<QPoint> getRotatedBlocks() const;

    QColor getColor() const;
    Type getType() const;
    QPoint getPosition() const;
    QVector<QVector<int>> getShape() const;

    static QColor colorForType(Type t);

private:
    Type type;
    int rotation;
    QPoint position;

    QVector<QVector<int>> rotateMatrix(const QVector<QVector<int>> &m) const;
    static const QVector<QVector<QVector<int>>> baseShapes;
};

#endif // TETROMINO_H
