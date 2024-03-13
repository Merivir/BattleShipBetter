#include "enemyboard.h"

EnemyBoard::EnemyBoard(QWidget *parent)
    : GameBoard(parent) {
    scene->setSceneRect(-labelOffset, -labelOffset, gridSize * cellSize + 2 * labelOffset, gridSize * cellSize + 2 * labelOffset);
    // Inside your GameBoard constructor or initialization code
    setFixedSize(gridSize * cellSize + 2 * labelOffset, gridSize * cellSize + 2 * labelOffset);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // Player specific initialization, if any
}

void EnemyBoard::setupBoard() {
    const int gridSize = 10;
    const int buttonSize = 40; // Each button is 40x40 pixels
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

    // Check if any ship occupies the clicked cell
    for (Ship *ship : qAsConst(ships)) {
        if (ship->getOccupiedCells().contains(QPoint(col, row))) {
            hitShip = ship;
            break;
        }
    }

    if (hitShip) {
        // If a ship is hit, display bomb emoji
        button->setText("💣");  // Use the bomb emoji
        // Optionally, disable the button to prevent further clicks
        button->setEnabled(false);
        // Additional logic for handling a hit (e.g., checking if the ship is sunk)
    } else {
        // If it's a miss, display an "X"
        button->setText("❌");  // Use the "X" emoji or character
        // Optionally, disable the button to prevent further clicks
        button->setEnabled(false);
    }

    // Ensure the button text is visible
    button->setStyleSheet("font-size: 18pt;");  // Adjust font size as needed
}



void EnemyBoard::drawGrid() {
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

    gridButtons.resize(gridSize);  // Prepare the vector to hold buttons

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
            // Remove the button from the scene
            scene->removeItem(button->graphicsProxyWidget());
            delete button;
        }
    }
    gridButtons.clear();
}
