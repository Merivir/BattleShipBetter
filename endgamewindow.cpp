#include "endgamewindow.h"
#include <QApplication>
#include <QPushButton>
#include <QDebug>

EndGameWindow::EndGameWindow(bool won, GameClient* client, QWidget *parent)
    : QMessageBox(parent), gameClient(client){
    if (gameClient) {
        gameClient->sendData("game_over"); // Send a game over message to the server
    }
    setText(won ? "Congratulations! You won!" : "Game Over! You lost.");
    setStandardButtons(QMessageBox::Ok);
    setWindowTitle(won ? "Victory" : "Defeat");
    connect(this, &QMessageBox::buttonClicked, this, &EndGameWindow::closeGame);
}

void EndGameWindow::closeGame() {
    QApplication::quit(); // Close the application or manage as needed
}
