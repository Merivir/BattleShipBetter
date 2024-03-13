#include "gameboard.h"


GameBoard::GameBoard(QWidget *parent)
    : QGraphicsView(parent) {
    scene = new QGraphicsScene(this);
    setScene(scene);
    setFixedSize(400, 400); // Assuming a 10x10 grid
    scene->setSceneRect(0, 0, 400, 400);

}

GameBoard::~GameBoard() {
}


bool GameBoard::canPlaceShip(Ship* ship, int col, int row) {
    // First, update the ship's proposed position
    ship->setShipPosition(col, row);

    // Check board boundaries
    if (ship->getOrientation() == Ship::Horizontal) {
        if (col + ship->getLength() > 10) return false; // Out of bounds horizontally
    } else { // Vertical
        if (row + ship->getLength() > 10) return false; // Out of bounds vertically
    }

    QList<QPoint> newShipCells = ship->getOccupiedCells();

    // Check for overlap with existing ships
    for (const Ship* existingShip : qAsConst(ships)) {
        QList<QPoint> existingShipCells = existingShip->getOccupiedCells();
        for (const QPoint& cell : qAsConst(newShipCells)) {
            if (existingShipCells.contains(cell) ) {
                return false; // Overlap found
            }
        }
    }

    // If no overlaps and within bounds, placement is okay
    return true;
}


bool GameBoard::isSpaceAroundShipFree(const Ship* ship) const {
    QList<QPoint> occupiedCells = ship->getOccupiedCells();
    int bufferZone = 1; // One-cell buffer on all sides

    for (const QPoint &cell : qAsConst(occupiedCells)) {
        for (int dx = -bufferZone; dx <= bufferZone; ++dx) {
            for (int dy = -bufferZone; dy <= bufferZone; ++dy) {
                QPoint checkCell(cell.x() + dx, cell.y() + dy);

                // Skip if checking the cell that the ship occupies
                if (dx == 0 && dy == 0) continue;

                // Check if the surrounding cell is within board boundaries
                if (checkCell.x() < 0 || checkCell.x() >= 10 ||
                    checkCell.y() < 0 || checkCell.y() >= 10) {
                    continue; // Skip out-of-bounds cells
                }

                // Check if the surrounding cell is occupied by any ship
                for (const Ship* existingShip : ships) {
                    QList<QPoint> existingShipCells = existingShip->getOccupiedCells();
                    if (existingShipCells.contains(checkCell)) {
                        return false; // A surrounding cell is occupied, space is not free
                    }
                }
            }
        }
    }
    return true; // All surrounding cells are free
}





