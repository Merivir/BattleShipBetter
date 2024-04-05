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
    GameBoard(const GameBoard& other);
    ~GameBoard();

    virtual void drawGrid() = 0;
    virtual void setupBoard() = 0;

protected:
    QGraphicsScene *scene;
    QList<Ship*> ships;
    bool interactive;

    const QStringList columnLabels = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J"};
    const QStringList rowLabels = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"};

    virtual void clearBoard() = 0;

    void setupNonInteractiveBoard();
    void setupInteractiveBoard();

    const int gridSize = 10;
    const int cellSize = 40;
    const int labelOffset = 20;

};

#endif // GAMEBOARD_H
