#include "playerboard.h"

PlayerBoard::PlayerBoard(QWidget *parent)
    : GameBoard(parent) {
    scene->setSceneRect(-labelOffset, -labelOffset, gridSize * cellSize + 2 * labelOffset, gridSize * cellSize + 2 * labelOffset);

    setFixedSize(gridSize * cellSize + 2 * labelOffset, gridSize * cellSize + 2 * labelOffset);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void PlayerBoard::setupBoard() {
    clearBoard(); // Clear any existing ships first

    QList<int> shipSizes = {4, 3, 3, 2, 2, 2, 1, 1, 1, 1};
    QRandomGenerator *rg = QRandomGenerator::global();

    for (int size : shipSizes) {
        bool placed = false;
        while (!placed) {
            int row = rg->bounded(10);
            int col = rg->bounded(10);
            Ship::Orientation orientation = rg->bounded(2) == 0 ? Ship::Horizontal : Ship::Vertical;

            Ship *newShip = new Ship(size, orientation);
            newShip->setShipPosition(col, row); // Attempt to set the ship's position

            if (canPlaceShip(newShip, col, row) && isSpaceAroundShipFree(newShip)) {
                ships.append(newShip); // Store the ship
                scene->addItem(newShip); // Add the ship to the scene
                placed = true;
            } else {
                delete newShip; // If the ship cannot be placed, delete it and try again
            }
        }
    }
}


void PlayerBoard::drawGrid() {
    const int gridSize = 10;  // 10x10 grid
    const int cellSize = 40;  // Each cell is 40x40 pixels
    const int labelOffset = 20; // Offset for labels from the grid


    for (int i = 0; i < gridSize; ++i) {
        QGraphicsTextItem *columnLabel = scene->addText(columnLabels[i]);
        columnLabel->setPos(i * cellSize + cellSize / 2, -labelOffset);
    }

      // Draw row labels (1, 2, 3, ...)
    for (int i = 0; i < gridSize; ++i) {
        QGraphicsTextItem *rowLabel = scene->addText(rowLabels[i]);
        rowLabel->setPos(-labelOffset, i * cellSize + cellSize / 2 - rowLabel->boundingRect().height() / 2);
    }

    // Draw the vertical lines
    for (int i = 0; i <= gridSize; ++i) {
        int x = i * cellSize;
        scene->addLine(x, 0, x, gridSize * cellSize, QPen(Qt::black));
    }

    // Draw the horizontal lines
    for (int i = 0; i <= gridSize; ++i) {
        int y = i * cellSize;
        scene->addLine(0, y, gridSize * cellSize, y, QPen(Qt::black));
    }

}

void PlayerBoard::clearBoard() {
    // Iterate through the list of ships and delete them
    for (Ship* ship : qAsConst(ships)) {
        // Remove the ship from the scene
        scene->removeItem(ship);
        // Delete the ship to free the memory
        delete ship;
    }

    // Clear the list of pointers
    ships.clear();
}
