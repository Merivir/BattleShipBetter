#ifndef ENDGAMEWINDOW_H
#define ENDGAMEWINDOW_H

#include "gameclient.h"
#include <QMessageBox>

class EndGameWindow : public QMessageBox {
    Q_OBJECT

public:
    explicit EndGameWindow(bool won, GameClient* gameClient, QWidget *parent = nullptr);

private:
    GameClient* gameClient; // Pointer to the game client to send messages

private slots:
    void closeGame();
};

#endif // ENDGAMEWINDOW_H
