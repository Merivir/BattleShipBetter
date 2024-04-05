#include "gameboard.h"


GameBoard::GameBoard(QWidget *parent)
    : QGraphicsView(parent)
{
    scene = new QGraphicsScene(this);
    setScene(scene);
    setFixedSize(400, 400);
    scene->setSceneRect(0, 0, 400, 400);

}

GameBoard::GameBoard(const GameBoard& other) : QGraphicsView(other.parentWidget()) {

    this->scene = new QGraphicsScene(this);
    // Assuming GameBoard has its own scene, copy relevant scene properties
    this->scene->setSceneRect(other.scene->sceneRect());

    // If GameBoard manages ships or similar entities directly
    for (const Ship* ship : other.ships) {
        Ship* newShip = new Ship(*ship); // Deep copy ships if GameBoard owns them
        this->ships.append(newShip);
        this->scene->addItem(newShip);
    }

    // Copy other necessary properties of GameBoard
    this->interactive = other.interactive;
    // Continue for other properties...

    // Reinitialize or re-setup based on the copied properties
    // For example, setup signals/slots connections if any
}


GameBoard::~GameBoard() {
}




