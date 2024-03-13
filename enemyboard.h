#ifndef ENEMYBOARD_H
#define ENEMYBOARD_H

#include "gameboard.h"

class EnemyBoard : public GameBoard {
    Q_OBJECT
public:
    explicit EnemyBoard(QWidget *parent = nullptr);

    void setupBoard() override;
    void drawGrid() override;

private slots:
    void cellClicked(int row, int col);

protected:
    void clearBoard() override;

private:
    QVector<QVector<QPushButton*>> gridButtons;
};

#endif // ENEMYBOARD_H
