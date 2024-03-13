#ifndef RESETWINDOW_H
#define RESETWINDOW_H

#include "ship.h"

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGraphicsTextItem>
#include <QDebug>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>


class ResetWindow : public QMainWindow {
    Q_OBJECT
public:
    ResetWindow(QWidget *parent = nullptr);
    ~ResetWindow();

    void drawGrid();

public slots:
    void checkAllShipsPlaced();

private slots:
    void rotateShip();
    void placeShip();
    void cellClicked(int row, int col);

private:
    QGraphicsView *boardView;
    QGraphicsScene *scene;
    QPushButton *rotateButton;
    QPushButton *placeButton;
    QPushButton *doneButton;

    const QStringList columnLabels = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J"};
    const QStringList rowLabels = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"};
    QList<int> shipSizes = {4, 3, 3, 2, 2, 2, 1, 1, 1, 1};

    bool shipHorizontal;
    QGraphicsRectItem *currentShip;

    bool canPlaceShip(Ship* ship, int col, int row);
    bool isSpaceAroundShipFree(const Ship* ship) const;
    bool eventFilter(QObject *object, QEvent *event);
    void resetShipColor();

    bool isDragging;
    QPointF lastMousePos;
};

#endif // RESETWINDOW_H
