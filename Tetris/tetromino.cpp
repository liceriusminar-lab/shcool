#include "tetromino.h"

// Базовые формы всех фигур (без поворота)
const QVector<QVector<QVector<int>>> Tetromino::baseShapes = {
    // I
    {{1,1,1,1}},
    // O
    {{1,1},{1,1}},
    // T
    {{0,1,0},{1,1,1}},
    // S
    {{0,1,1},{1,1,0}},
    // Z
    {{1,1,0},{0,1,1}},
    // J
    {{1,0,0},{1,1,1}},
    // L
    {{0,0,1},{1,1,1}}
};

Tetromino::Tetromino(Type t)
    : type(t), rotation(0), position(3, 0)
{}

void Tetromino::rotate()
{
    rotation = (rotation + 1) % 4;
}

void Tetromino::move(int dx, int dy)
{
    position += QPoint(dx, dy);
}

void Tetromino::setPosition(int x, int y)
{
    position = QPoint(x, y);
}

QPoint Tetromino::getPosition() const
{
    return position;
}

QVector<QVector<int>> Tetromino::getShape() const
{
    QVector<QVector<int>> shape = baseShapes[static_cast<int>(type)];
    for (int i = 0; i < rotation; ++i)
        shape = rotateMatrix(shape);
    return shape;
}

QVector<QVector<int>> Tetromino::rotateMatrix(const QVector<QVector<int>> &m) const
{
    int rows = m.size();
    int cols = m[0].size();
    QVector<QVector<int>> result(cols, QVector<int>(rows, 0));
    for (int y = 0; y < rows; ++y)
        for (int x = 0; x < cols; ++x)
            result[x][rows - 1 - y] = m[y][x];
    return result;
}

QVector<QPoint> Tetromino::getBlocks() const
{
    return getBlocksAt(0, 0);
}

QVector<QPoint> Tetromino::getBlocksAt(int ox, int oy) const
{
    QVector<QPoint> blocks;
    auto shape = getShape();
    for (int y = 0; y < shape.size(); ++y)
        for (int x = 0; x < shape[y].size(); ++x)
            if (shape[y][x])
                blocks.append(QPoint(position.x() + x + ox, position.y() + y + oy));
    return blocks;
}

QVector<QPoint> Tetromino::getRotatedBlocks() const
{
    QVector<QPoint> blocks;
    auto shape = baseShapes[static_cast<int>(type)];
    for (int i = 0; i < (rotation + 1) % 4; ++i)
        shape = rotateMatrix(shape);
    for (int y = 0; y < shape.size(); ++y)
        for (int x = 0; x < shape[y].size(); ++x)
            if (shape[y][x])
                blocks.append(QPoint(position.x() + x, position.y() + y));
    return blocks;
}

QColor Tetromino::getColor() const
{
    return colorForType(type);
}

Tetromino::Type Tetromino::getType() const
{
    return type;
}

QColor Tetromino::colorForType(Type t)
{
    switch (t) {
    case Type::I: return QColor(0,   207, 207);
    case Type::O: return QColor(240, 192,  48);
    case Type::T: return QColor(192,  64, 224);
    case Type::S: return QColor( 64, 200,  64);
    case Type::Z: return QColor(224,  64,  64);
    case Type::J: return QColor( 64, 128, 224);
    case Type::L: return QColor(224, 112,  32);
    default:      return Qt::white;
    }
}
