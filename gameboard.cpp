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

    this->scene->setSceneRect(other.scene->sceneRect());

    for (const Ship* ship : other.ships) {
        Ship* newShip = new Ship(*ship);
        this->ships.append(newShip);
        this->scene->addItem(newShip);
    }

    this->interactive = other.interactive;
}


GameBoard::~GameBoard() {
}




