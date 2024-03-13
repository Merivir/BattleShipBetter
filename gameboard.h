#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include "ship.h"

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPushButton>
#include <QVector>
#include <QStringList>
#include <QGraphicsProxyWidget>
#include <QRandomGenerator>

class GameBoard : public QGraphicsView {
    Q_OBJECT

public:
    explicit GameBoard(QWidget *parent = nullptr);
    ~GameBoard();

    virtual void drawGrid() = 0;
    virtual void setupBoard() = 0; // A pure virtual function for setting up the board

protected:
    QGraphicsScene *scene;
    QList<Ship*> ships; // Add this to your private section
    bool interactive; // Whether this board is interactive (has buttons)

    const QStringList columnLabels = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J"};
    const QStringList rowLabels = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"};

    virtual void clearBoard() = 0;


    bool isSpaceAroundShipFree(const Ship* ship) const;
    bool canPlaceShip(Ship* ship, int col, int row);
    void setupNonInteractiveBoard();
    void setupInteractiveBoard();

    const int gridSize = 10;  // 10x10 grid
    const int cellSize = 40;  // Each cell is 40x40 pixels
    const int labelOffset = 20; // Offset for labels from the grid

};

#endif // GAMEBOARD_H
