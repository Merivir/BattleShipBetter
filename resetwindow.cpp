#include "resetwindow.h"

ResetWindow::ResetWindow(QWidget *parent)
    : QMainWindow(parent), shipHorizontal(true), currentShip(nullptr), isDragging(false){

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    boardView = new QGraphicsView(centralWidget);
    scene = new QGraphicsScene(this);

    const int boardSize = 450;
    scene->setSceneRect(0, 0, boardSize, boardSize);
    boardView->setScene(scene);

    drawGrid();

    boardView->setFixedSize(boardSize, boardSize);
    boardView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    boardView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    rotateButton = new QPushButton("Rotate Ship", centralWidget);
    connect(rotateButton, &QPushButton::clicked, this, &ResetWindow::rotateShip);

    placeButton = new QPushButton("Place Ship", centralWidget);
    connect(placeButton, &QPushButton::clicked, this, &ResetWindow::placeShip);

    doneButton = new QPushButton("Done", centralWidget);
    doneButton->setEnabled(false); // Initially disabled
    connect(doneButton, &QPushButton::clicked, this, &ResetWindow::checkAllShipsPlaced);

    scene->installEventFilter(this);

    layout->addWidget(boardView);
    layout->addWidget(rotateButton);
    layout->addWidget(placeButton);
    layout->addWidget(doneButton);

    centralWidget->setLayout(layout);
}

ResetWindow::~ResetWindow() {
}

void ResetWindow::drawGrid() {
    const int gridSize = 10;
    const int cellSize = 40;
    const int labelOffset = 20;

    const int topOffset = 30;
    const int leftOffset = 30;

    for (int i = 0; i < gridSize; ++i) {
        QGraphicsTextItem *columnLabel = scene->addText(columnLabels[i]);
        columnLabel->setPos(leftOffset + i * cellSize + cellSize / 2 - columnLabel->boundingRect().width() / 2, topOffset - labelOffset);
    }

    for (int i = 0; i < gridSize; ++i) {
        QGraphicsTextItem *rowLabel =  scene->addText(rowLabels[i]);
        rowLabel->setPos(leftOffset - labelOffset, topOffset + i * cellSize + cellSize / 2 - rowLabel->boundingRect().height() / 2);
    }

    for (int i = 0; i <= gridSize; ++i) {
        int x = leftOffset + i * cellSize;
        scene->addLine(x, topOffset, x, topOffset + gridSize * cellSize, QPen(Qt::black));
    }

    for (int i = 0; i <= gridSize; ++i) {
        int y = topOffset + i * cellSize;
        scene->addLine(leftOffset, y, leftOffset + gridSize * cellSize, y, QPen(Qt::black));
    }
}

void ResetWindow::rotateShip() {
    shipHorizontal = !shipHorizontal;
    if (currentShip) {
        qreal rotation = shipHorizontal ? 0 : 90;
        currentShip->setRotation(rotation);
    }
}

void ResetWindow::placeShip() {
    if (shipSizes.isEmpty()) {
        qDebug() << "All ships placed!";
        doneButton->setEnabled(true);

        return;
    }

    int size = shipSizes.takeFirst();
    currentShip = new QGraphicsRectItem(0, 0, shipHorizontal ? size * 40 : 40, shipHorizontal ? 40 : size * 40);
    currentShip->setBrush(Qt::gray);
    scene->addItem(currentShip);
}

void ResetWindow::cellClicked(int row, int col) {
    if (currentShip) {
        int x = col * 40;
        int y = row * 40;
        currentShip->setPos(x, y);
        scene->removeItem(currentShip); // Remove from the scene to re-add after the rotation
        scene->addItem(currentShip); // Re-add to the scene to ensure it's on top
    }
}

bool ResetWindow::eventFilter(QObject *object, QEvent *event) {
    if (object == scene) {
        if (event->type() == QEvent::GraphicsSceneMousePress) {
            QGraphicsSceneMouseEvent *mouseEvent = dynamic_cast<QGraphicsSceneMouseEvent *>(event);
            if (mouseEvent && mouseEvent->button() == Qt::LeftButton) {
                lastMousePos = mouseEvent->scenePos();
                isDragging = true;
                return true;
            }
        } else if (event->type() == QEvent::GraphicsSceneMouseMove) {
            if (isDragging) {
                QGraphicsSceneMouseEvent *mouseEvent = dynamic_cast<QGraphicsSceneMouseEvent *>(event);
                if (mouseEvent) {
                    QPointF newPos = mouseEvent->scenePos();
                    QPointF delta = newPos - lastMousePos;
                    currentShip->moveBy(delta.x(), delta.y());
                    lastMousePos = newPos;
                    return true;
                }
            }
        } else if (event->type() == QEvent::GraphicsSceneMouseRelease) {
            if (isDragging) {
                isDragging = false;
                return true;
            }
        }
    }
    return QMainWindow::eventFilter(object, event);

}

void ResetWindow::checkAllShipsPlaced() {
    // Check if all ships have been placed
    if (!currentShip && shipSizes.isEmpty()) {
        doneButton->setEnabled(true);
    }
}

