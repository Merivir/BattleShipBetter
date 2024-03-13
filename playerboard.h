#ifndef PLAYERBOARD_H
#define PLAYERBOARD_H

#include "gameboard.h"

class PlayerBoard : public GameBoard {
    Q_OBJECT
public:
    explicit PlayerBoard(QWidget *parent = nullptr);

    void drawGrid() override;
    void setupBoard() override;

protected:
    void clearBoard() override;
};

#endif // PLAYERBOARD_H
