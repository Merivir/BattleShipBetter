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
    // Set up the central widget and the main layout
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

    // Assuming you have a function in GameBoard to actually draw the grid or ships
    player1Board->drawGrid(); // You need to implement this
    player2Board->drawGrid(); // You need to implement this

    // Add the game boards to the layout
    boardLayout->addWidget(player1Board);
    boardLayout->addWidget(player2Board);


    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(randomizeButton);
    buttonLayout->addWidget(resetButton);
    buttonLayout->addWidget(playButton);


    // If you have an existing layout, add the button layout to it
    // For example, if you have a mainLayout, you might add:
    mainLayout->addLayout(boardLayout);
    mainLayout->addLayout(buttonLayout);

    // Or, if you are creating a new main layout, set it as the central layout
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
    // Code to start playing the game
}

MainWindow::~MainWindow() {}
