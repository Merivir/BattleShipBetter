#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "gameboard.h"

#include <QPushButton>
#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    GameBoard *player1Board;
    GameBoard *player2Board;
    QPushButton *playButton;
    QPushButton *randomizeButton;
    QPushButton *resetButton;

public slots:
    void onRandomizeClicked();
    void onResetClicked();
    void onPlayClicked();

};

#endif // MAINWINDOW_H
