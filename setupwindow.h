#ifndef SETUPWINDOW_H
#define SETUPWINDOW_H

#include <QMainWindow>
#include "playerboard.h"

class SetupWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit SetupWindow(QWidget *parent = nullptr);
    ~SetupWindow();

    PlayerBoard* getPlayerBoard() const; // For retrieving the player board

public slots:
    void onRandomizeClicked();
    void onPlayClicked();

signals:
    void setupCompleted(PlayerBoard* playerBoard);

private:
    PlayerBoard *playerBoard;
    QPushButton *randomizeButton;
    QPushButton *playButton;

    void setupUi(); // Setup the UI components
};

#endif // SETUPWINDOW_H
