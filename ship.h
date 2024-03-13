#ifndef SHIP_H
#define SHIP_H

#include <QGraphicsRectItem>
#include <QList>
#include <QPoint>
#include <QBrush>

class Ship : public QGraphicsRectItem {
public:
    enum Orientation { Horizontal, Vertical };

    Ship(int length, Orientation orientation, QGraphicsItem *parent = nullptr);

    void setShipPosition(int x, int y);
    QList<QPoint> getOccupiedCells() const;
    int getLength() const;
    Orientation getOrientation() const;

private:
    int length;
    Orientation orientation;
};

#endif // SHIP_H
