#include "enemyboard.h"

#include <QDebug>

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
    emit cellAttackRequested(row, col);
}

void EnemyBoard::updateCellBasedOnServerResponse(const QStringList& messageParts) {
    if (messageParts.size() != 3) {
        qDebug() << "Invalid message format received: ";
        return;
    }

    QChar colLetter = messageParts.at(0).at(0).toUpper();
    int col = colLetter.unicode() - 'A';

    bool rowOk;
    int row = messageParts.at(1).toInt(&rowOk) - 1;
    QString result = messageParts.at(2);

    if (!rowOk || row < 0 || col < 0 || row >= gridButtons.size() || col >= gridButtons[row].size()) {
        qDebug() << "Received invalid coordinates or unable to convert: " << messageParts.at(0) << "," << messageParts.at(1);
        return;
    }

    QPushButton* button = gridButtons[row][col];
    if (!button) {
        qDebug() << "Button not found at the specified coordinates.";
        return;
    }

    QFont font = button->font();

    font.setPointSize(24);
    button->setFont(font);

    if (result == "hit") {
        button->setText("💣"); // Mark hit
        button->setEnabled(false);
    } else if (result == "miss") {
        button->setText("❌"); // Mark miss
        button->setEnabled(false);
    }
}



void EnemyBoard::parseCoordinates(const QString& coordinates, int &row, int &col) {
    if (coordinates.length() < 2) return;

    col = coordinates[0].toUpper().unicode() - 'A';

    QString rowPart = coordinates.mid(1);
    row = rowPart.toInt() - 1;
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


