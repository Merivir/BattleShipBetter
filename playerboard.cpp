#include "playerboard.h"

PlayerBoard::PlayerBoard(QWidget *parent)
    : GameBoard(parent) {
    scene->setSceneRect(-labelOffset, -labelOffset, gridSize * cellSize + 2 * labelOffset, gridSize * cellSize + 2 * labelOffset);

    setFixedSize(gridSize * cellSize + 2 * labelOffset, gridSize * cellSize + 2 * labelOffset);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void PlayerBoard::setupBoard() {
    clearBoard();
    QList<int> shipSizes = {4, 3, 3, 2, 2, 2, 1, 1, 1, 1};
    QRandomGenerator *rg = QRandomGenerator::global();

    for (int size : shipSizes) {
        bool placed = false;
        while (!placed) {
            int row = rg->bounded(10);
            int col = rg->bounded(10);
            Ship::Orientation orientation = rg->bounded(2) == 0 ? Ship::Horizontal : Ship::Vertical;

            Ship *newShip = new Ship(size, orientation);
            newShip->setShipPosition(col, row);

            if (canPlaceShip(newShip, col, row) && isSpaceAroundShipFree(newShip)) {
                ships.append(newShip);
                scene->addItem(newShip);
                placed = true;
            } else {
                delete newShip;
            }
        }
    }
}


void PlayerBoard::drawGrid() {
    const int gridSize = 10;
    const int cellSize = 40;
    const int labelOffset = 20;


    for (int i = 0; i < gridSize; ++i) {
        QGraphicsTextItem *columnLabel = scene->addText(columnLabels[i]);
        columnLabel->setPos(i * cellSize + cellSize / 2, -labelOffset);
    }

    for (int i = 0; i < gridSize; ++i) {
        QGraphicsTextItem *rowLabel = scene->addText(rowLabels[i]);
        rowLabel->setPos(-labelOffset, i * cellSize + cellSize / 2 - rowLabel->boundingRect().height() / 2);
    }

    for (int i = 0; i <= gridSize; ++i) {
        int x = i * cellSize;
        scene->addLine(x, 0, x, gridSize * cellSize, QPen(Qt::black));
    }

    for (int i = 0; i <= gridSize; ++i) {
        int y = i * cellSize;
        scene->addLine(0, y, gridSize * cellSize, y, QPen(Qt::black));
    }

}

void PlayerBoard::clearBoard() {
    for (Ship* ship : qAsConst(ships)) {
        scene->removeItem(ship);
        delete ship;
    }

    ships.clear();
}
