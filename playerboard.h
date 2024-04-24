#ifndef PLAYERBOARD_H
#define PLAYERBOARD_H

#include "gameboard.h"

class PlayerBoard : public GameBoard {
    Q_OBJECT
public:
    explicit PlayerBoard(QWidget *parent = nullptr);
    PlayerBoard(const PlayerBoard& other);

    void drawGrid() override;
    void setupBoard() override;
    void clearBoard() override;

    void copyBoardAndSetupShips(const QVector<QVector<int>>& otherBoard);
    bool isSpaceAroundShipFree(int startRow, int startCol, int size, Ship::Orientation orientation) const;
    bool placeShip(int size, Ship::Orientation orientation, int startRow, int startCol);
    QString checkAttack(int row, int col); // Checks the attack and returns "hit", "miss", or "sunk"
    void markCellAsBombed(int row, int col, const QString& result);
    bool isHit(int row, int col);
    void renderBoard();

    QVector<QVector<int>> board;
    int shipCellsRemaining;

signals:
    void allShipsSunk();

public slots:
    void resetBoard();
};

#endif // PLAYERBOARD_H
