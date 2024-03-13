#include "mainwindow.h"
#include "playerboard.h"
#include "enemyboard.h"
#include "resetwindow.h"

#include <QLabel>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    QHBoxLayout *boardLayout = new QHBoxLayout(centralWidget);

    randomizeButton = new QPushButton("🔄", this);
    resetButton = new QPushButton("↪️", this);
    playButton = new QPushButton("Play", this);

    connect(randomizeButton, &QPushButton::clicked, this, &MainWindow::onRandomizeClicked);
    connect(resetButton, &QPushButton::clicked, this, &MainWindow::onResetClicked);
    connect(playButton, &QPushButton::clicked, this, &MainWindow::onPlayClicked);

    // Create the game boards
    player2Board = new EnemyBoard(this);
    player1Board = new PlayerBoard(this);

    player1Board->setupBoard();
    player2Board->setupBoard();

    player1Board->drawGrid();
    player2Board->drawGrid();

    boardLayout->addWidget(player1Board);
    boardLayout->addWidget(player2Board);


    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(randomizeButton);
    buttonLayout->addWidget(resetButton);
    buttonLayout->addWidget(playButton);

    mainLayout->addLayout(boardLayout);
    mainLayout->addLayout(buttonLayout);

    centralWidget->setLayout(buttonLayout);
    setCentralWidget(centralWidget);

}

void MainWindow::onRandomizeClicked() {
    player1Board->setupBoard();
}

void MainWindow::onResetClicked() {
    ResetWindow *resetWindow = new ResetWindow(this);

    resetWindow->show();
}


void MainWindow::onPlayClicked() {
}

MainWindow::~MainWindow() {}
