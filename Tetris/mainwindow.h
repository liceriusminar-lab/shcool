#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class TetrisWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override = default;

private:
    TetrisWidget *m_tetris = nullptr;
};

#endif // MAINWINDOW_H
