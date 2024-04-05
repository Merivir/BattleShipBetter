#include "setupwindow.h"
#include <QPushButton>
#include <QVBoxLayout>

SetupWindow::SetupWindow(QWidget *parent)
    : QMainWindow(parent), playerBoard(new PlayerBoard(this)) {
    setupUi();
}

SetupWindow::~SetupWindow() {
    delete playerBoard; // Ensure proper cleanup
}

void SetupWindow::setupUi() {
    // Setup UI components, buttons, and connect signals
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    playerBoard->setupBoard(); // Initialize player board with default or empty setup
    playerBoard->drawGrid();

    randomizeButton = new QPushButton("Randomize", this);
    connect(randomizeButton, &QPushButton::clicked, this, &SetupWindow::onRandomizeClicked);

    playButton = new QPushButton("Play", this);
    connect(playButton, &QPushButton::clicked, this, &SetupWindow::onPlayClicked);

    layout->addWidget(playerBoard);
    layout->addWidget(randomizeButton);
    layout->addWidget(playButton);

    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);
}

void SetupWindow::onRandomizeClicked() {
    playerBoard->clearBoard();
    playerBoard->setupBoard(); // Assuming setupBoard() randomizes the ship placement
}

void SetupWindow::onPlayClicked() {
    emit setupCompleted(playerBoard);
    this->close(); // Optionally close the setup window
}

PlayerBoard* SetupWindow::getPlayerBoard() const {
    return playerBoard;
}


