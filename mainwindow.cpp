#include "mainwindow.h"
#include "playerboard.h"
#include "enemyboard.h"
#include "resetwindow.h"
#include "setupwindow.h"

#include <QLabel>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    QHBoxLayout *boardLayout = new QHBoxLayout(centralWidget);

    // Create the game boards
    player2Board = new EnemyBoard(this);
    player1Board = new PlayerBoard(this);

    player1Board->setupBoard();
    player2Board->setupBoard();

    player1Board->drawGrid();
    player2Board->drawGrid();

    boardLayout->addWidget(player1Board);
    boardLayout->addWidget(player2Board);

    mainLayout->addLayout(boardLayout);
    setCentralWidget(centralWidget);

    SetupWindow* setupWindow = new SetupWindow(this);
    connect(setupWindow, &SetupWindow::setupCompleted, this, &MainWindow::handleSetupCompleted);

}

void MainWindow::handleSetupCompleted(PlayerBoard* boardOb) {
    qDebug() << "Before copying ships:";
    boardOb->renderBoard(); // Print the board state of SetupWindow's PlayerBoard

    player1Board->copyBoardAndSetupShips(boardOb->board); // Copy the state to MainWindow's PlayerBoard

    qDebug() << "After copying ships:";
    player1Board->renderBoard(); // Print the board state to verify the copy

    show();
}

MainWindow::~MainWindow() {
    delete player1Board;
    delete player2Board;
}
