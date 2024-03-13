#include "gameboard.h"


GameBoard::GameBoard(QWidget *parent)
    : QGraphicsView(parent) {
    scene = new QGraphicsScene(this);
    setScene(scene);
    setFixedSize(400, 400);
    scene->setSceneRect(0, 0, 400, 400);

}

GameBoard::~GameBoard() {
}


bool GameBoard::canPlaceShip(Ship* ship, int col, int row) {
    ship->setShipPosition(col, row);

    if (ship->getOrientation() == Ship::Horizontal) {
        if (col + ship->getLength() > 10) return false;
    } else {
        if (row + ship->getLength() > 10) return false;
    }

    QList<QPoint> newShipCells = ship->getOccupiedCells();

    for (const Ship* existingShip : qAsConst(ships)) {
        QList<QPoint> existingShipCells = existingShip->getOccupiedCells();
        for (const QPoint& cell : qAsConst(newShipCells)) {
            if (existingShipCells.contains(cell) ) {
                return false;
            }
        }
    }

    return true;
}


bool GameBoard::isSpaceAroundShipFree(const Ship* ship) const {
    QList<QPoint> occupiedCells = ship->getOccupiedCells();
    int bufferZone = 1;

    for (const QPoint &cell : qAsConst(occupiedCells)) {
        for (int dx = -bufferZone; dx <= bufferZone; ++dx) {
            for (int dy = -bufferZone; dy <= bufferZone; ++dy) {
                QPoint checkCell(cell.x() + dx, cell.y() + dy);

                if (dx == 0 && dy == 0) continue;

                if (checkCell.x() < 0 || checkCell.x() >= 10 ||
                    checkCell.y() < 0 || checkCell.y() >= 10) {
                    continue;
                }

                for (const Ship* existingShip : ships) {
                    QList<QPoint> existingShipCells = existingShip->getOccupiedCells();
                    if (existingShipCells.contains(checkCell)) {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}




