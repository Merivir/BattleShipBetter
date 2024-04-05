#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "gameboard.h"
#include "playerboard.h"
#include "enemyboard.h"

#include <QPushButton>
#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void handleSetupCompleted(PlayerBoard* board);

private:
    PlayerBoard *player1Board;
    EnemyBoard *player2Board;

};

#endif // MAINWINDOW_H
