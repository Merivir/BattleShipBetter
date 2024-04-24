#include "mainwindow.h"
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

    gameClient = new GameClient(this);

    // Connect to the server
    QString serverIP = "127.0.0.1"; // Example IP for localhost
    quint16 serverPort = 8088; // Example port number
    gameClient->connectToServer(serverIP, serverPort);

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
    connect(player2Board, &EnemyBoard::cellAttackRequested, gameClient, &GameClient::sendCoordinatesToServer);
    connect(gameClient, &GameClient::serverResponseReceived, player2Board, &EnemyBoard::updateCellBasedOnServerResponse);
    connect(gameClient, &GameClient::attackReceived, this, &MainWindow::onAttackReceived);
}

void MainWindow::handleSetupCompleted(PlayerBoard* boardOb) {
    qDebug() << "Before copying ships:";
    boardOb->renderBoard(); // Print the board state of SetupWindow's PlayerBoard

    player1Board->copyBoardAndSetupShips(boardOb->board); // Copy the state to MainWindow's PlayerBoard

    qDebug() << "After copying ships:";
    player1Board->renderBoard(); // Print the board state to verify the copy

    show();
}

void MainWindow::onAttackReceived(int row, int col) {
    QString result = player1Board->checkAttack(row, col); // Check if it's a hit or miss
    // Assuming column 'A' is represented by 0, adjust col to character
    char colChar = 'A' + col;
    QString coordinates = QString("%1,%2").arg(colChar).arg(row + 1); // Rows are 0-indexed

    QString message = coordinates + "," + result; // Format: "A2,hit" or "A2,miss"

    // Now send the formatted string back through GameClient
    gameClient->sendData(message.toUtf8()); // Send the message
}


MainWindow::~MainWindow() {
    delete player1Board;
    delete player2Board;
}
