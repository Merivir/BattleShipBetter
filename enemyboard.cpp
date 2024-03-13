#include "enemyboard.h"

EnemyBoard::EnemyBoard(QWidget *parent)
    : GameBoard(parent) {
    scene->setSceneRect(-labelOffset, -labelOffset, gridSize * cellSize + 2 * labelOffset, gridSize * cellSize + 2 * labelOffset);
    setFixedSize(gridSize * cellSize + 2 * labelOffset, gridSize * cellSize + 2 * labelOffset);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void EnemyBoard::setupBoard() {
    const int gridSize = 10;
    const int buttonSize = 40;
    gridButtons.resize(gridSize);
    setFixedSize(gridSize * cellSize + 2 * labelOffset, gridSize * cellSize + 2 * labelOffset);


    for (int row = 0; row < gridSize; ++row) {
        gridButtons[row].resize(gridSize);
        for (int col = 0; col < gridSize; ++col) {
            QPushButton *button = new QPushButton();
            button->setFixedSize(buttonSize, buttonSize);
            QGraphicsProxyWidget *proxy = scene->addWidget(button);
            proxy->setPos(col * buttonSize, row * buttonSize);
            gridButtons[row][col] = button;
            connect(button, &QPushButton::clicked, [this, row, col]() { cellClicked(row, col); });
        }
    }
}

void EnemyBoard::cellClicked(int row, int col) {
    QPushButton *button = gridButtons[row][col];
    Ship *hitShip = nullptr;

    for (Ship *ship : qAsConst(ships)) {
        if (ship->getOccupiedCells().contains(QPoint(col, row))) {
            hitShip = ship;
            break;
        }
    }

    if (hitShip) {
        button->setText("💣");
        button->setEnabled(false);
    } else {
        button->setText("❌");
        button->setEnabled(false);
    }

    button->setStyleSheet("font-size: 18pt;");
}



void EnemyBoard::drawGrid() {
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

    gridButtons.resize(gridSize);
    for (int row = 0; row < gridSize; ++row) {
        gridButtons[row].resize(gridSize);

        for (int col = 0; col < gridSize; ++col) {
            QPushButton *button = new QPushButton();
            button->setFixedSize(cellSize, cellSize);
            QGraphicsProxyWidget *proxy = scene->addWidget(button);
            proxy->setPos(col * cellSize, row * cellSize);
            gridButtons[row][col] = button;

            connect(button, &QPushButton::clicked, [this, row, col]() {
                cellClicked(row, col); });
        }
    }
}


void EnemyBoard::clearBoard() {
    for (const auto &row : qAsConst(gridButtons)) {
        for (QPushButton *button : row) {
            scene->removeItem(button->graphicsProxyWidget());
            delete button;
        }
    }
    gridButtons.clear();
}
