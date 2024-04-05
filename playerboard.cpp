#include "playerboard.h"
#include <QDebug>

PlayerBoard::PlayerBoard(QWidget *parent)
    : GameBoard(parent)
    , board(10, QVector<int>(10, 0))

{
    scene->setSceneRect(-labelOffset, -labelOffset, gridSize * cellSize + 2 * labelOffset, gridSize * cellSize + 2 * labelOffset);

    setFixedSize(gridSize * cellSize + 2 * labelOffset, gridSize * cellSize + 2 * labelOffset);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

PlayerBoard::PlayerBoard(const PlayerBoard& other)
    : GameBoard(other) // Инициализация с базовым классом
{
    board = QVector<QVector<int>>(10, QVector<int>(10, 0)); // 10x10 board filled with 0

    this->scene = new QGraphicsScene(this);
    this->setScene(scene);

    copyBoardAndSetupShips(other.board); // Копирование и настройка доски и кораблей
}

void PlayerBoard::copyBoardAndSetupShips(const QVector<QVector<int>>& otherBoard) {
    this->board = otherBoard; // Копируем состояние игровой доски из другого экземпляра
    this->scene->clear(); // Очищаем сцену, чтобы начать с чистого листа
    PlayerBoard::drawGrid(); // Перерисовываем сетку

    // Теперь, основываясь на состоянии игровой доски, создаем корабли
    for (int row = 0; row < gridSize; ++row) {
        for (int col = 0; col < gridSize; ++col) {
            int cellState = board[row][col];
            if (cellState == 1) { // Предположим, что 1 обозначает наличие корабля
                QGraphicsRectItem *shipPart = new QGraphicsRectItem(col * cellSize, row * cellSize, cellSize, cellSize);
                shipPart->setBrush(QBrush(Qt::gray)); // Задаем цвет корабля
                scene->addItem(shipPart); // Добавляем элемент корабля на сцену
            }
            // Возможно, потребуются дополнительные проверки для разных типов ячеек
        }
    }
}



bool PlayerBoard::placeShip(int size, Ship::Orientation orientation, int startRow, int startCol) {
    // Проверка, свободно ли пространство вокруг предполагаемого размещения корабля
    if (!isSpaceAroundShipFree(startRow, startCol, size, orientation)) {
        return false; // Если вокруг корабля нет свободного места, возвращаем false
    }

    // Теперь, когда мы проверили пространство вокруг, можно разместить корабль
    // Оставшаяся часть метода не меняется
    if (orientation == Ship::Horizontal) {
        if (startCol + size > gridSize) return false; // Проверка выхода за границы
        for (int i = 0; i < size; ++i) {
            if (board[startRow][startCol + i] != 0) return false; // Место уже занято
        }
        for (int i = 0; i < size; ++i) board[startRow][startCol + i] = 1; // Размещаем части корабля
    } else { // Вертикальное размещение
        if (startRow + size > gridSize) return false; // Проверка выхода за границы
        for (int i = 0; i < size; ++i) {
            if (board[startRow + i][startCol] != 0) return false; // Место уже занято
        }
        for (int i = 0; i < size; ++i) board[startRow + i][startCol] = 1; // Размещаем части корабля
    }
    return true; // Корабль успешно размещен
}



bool PlayerBoard::isSpaceAroundShipFree(int startRow, int startCol, int size, Ship::Orientation orientation) const {
    int fromRow = std::max(0, startRow - 1);
    int toRow = std::min(gridSize - 1, (orientation == Ship::Vertical) ? startRow + size : startRow + 1);
    int fromCol = std::max(0, startCol - 1);
    int toCol = std::min(gridSize - 1, (orientation == Ship::Horizontal) ? startCol + size : startCol + 1);

    for (int row = fromRow; row <= toRow; ++row) {
        for (int col = fromCol; col <= toCol; ++col) {
            if (board[row][col] != 0) return false; // Space not free
        }
    }
    return true; // Space around ship is free
}

void PlayerBoard::setupBoard() {
    clearBoard();
    board = QVector<QVector<int>>(10, QVector<int>(10, 0));
    QList<int> shipSizes = {4, 3, 3, 2, 2, 2, 1, 1, 1, 1};
    QRandomGenerator *rg = QRandomGenerator::global();

    for (int size : shipSizes) {
        bool placed = false;
        int attempts = 0;
        while (!placed && attempts < 100) {
            int row = rg->bounded(10);
            int col = rg->bounded(10);
            Ship::Orientation orientation = rg->bounded(2) == 0 ? Ship::Horizontal : Ship::Vertical;

            if (placeShip(size, orientation, row, col)) {
                // Создание и добавление графического представления корабля
                Ship* newShip = new Ship(size, orientation); // Предполагается, что у тебя есть конструктор, принимающий размер и ориентацию
                newShip->setPos(col * cellSize, row * cellSize); // Установка начальной позиции корабля с учетом смещения
                scene->addItem(newShip); // Добавление корабля на сцену

                placed = true;
            } else {
                attempts++;
            }
        }
        if (attempts >= 100) {
            qDebug() << "Unable to place ship of size" << size << "after 100 attempts.";
        }
    }
    qDebug() << "Finished setting up the board.";
}


void PlayerBoard::renderBoard() {
    for (int row = 0; row < board.size(); ++row) {
        QString line;
        for (int col = 0; col < board[row].size(); ++col) {
            switch (board[row][col]) {
                case 0: line += "~"; break; // Water
                case 1: line += "S"; break; // Ship (unhit)
                case 2: line += "*"; break; // Hit ship part
                default: line += "?"; break; // Unknown state
            }
            line += " ";
        }
        qDebug() << line; // Assuming you're using Qt's debug output
    }
}

bool PlayerBoard::isHit(int row, int col) {
    // Ensure row and col are within the board's bounds before checking
    if (row >= 0 && row < board.size() && col >= 0 && col < board[row].size()) {
        if (board[row][col] == 1) {
            board[row][col] = 2; // Mark this part as hit
            return true;
        }
    }
    return false;
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
    scene->clear(); // Явно очищаем сцену
    drawGrid();
    ships.clear(); // Очищаем список кораблей
    board.fill(QVector<int>(10, 0)); // Сброс состояния доски
}


void PlayerBoard::resetBoard() {
    clearBoard(); // Clears the graphical scene and ship items.
    board.fill(QVector<int>(10, 0)); // Reset the logical board representation.
    setupBoard(); // Optionally call setupBoard to reinitialize the game state.
}
