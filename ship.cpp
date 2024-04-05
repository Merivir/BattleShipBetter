#include "ship.h"

Ship::Ship(int length, Orientation orientation, QGraphicsItem *parent)
    : QGraphicsRectItem(parent), length(length), orientation(orientation) {
    if (orientation == Horizontal) {
        setRect(0, 0, length * 40, 40);
    } else {
        setRect(0, 0, 40, length * 40);
    }
    setBrush(QBrush(Qt::gray));
}

Ship::Ship(const Ship& other) {
        this->length = other.length;
        this->orientation = other.orientation;
}

void Ship::setShipPosition(int x, int y) {
    setPos(x * 40, y * 40);
}

QList<QPoint> Ship::getOccupiedCells() const {
    QList<QPoint> cells;
    QPoint topLeft = this->pos().toPoint();
    for (int i = 0; i < length; ++i) {
        if (orientation == Horizontal) {
            cells.append(QPoint(topLeft.x() / 40 + i, topLeft.y() / 40));
        } else {
            cells.append(QPoint(topLeft.x() / 40, topLeft.y() / 40 + i));
        }
    }
    return cells;
}

int Ship::getLength() const {
    return length;
}

Ship::Orientation Ship::getOrientation() const {
    return orientation;
}
