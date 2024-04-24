#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "gameboard.h"
#include "playerboard.h"
#include "enemyboard.h"
#include "gameclient.h"

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
    void onAttackReceived(int row, int col);
    void handleGameOver();
    void onGameOverReceived();
private:
    PlayerBoard *player1Board;
    EnemyBoard *player2Board;
    GameClient *gameClient;
};

#endif // MAINWINDOW_H
