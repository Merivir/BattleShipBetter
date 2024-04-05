#include "setupwindow.h"
#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow* mainWindow = new MainWindow(); // Создаём главное окно
    SetupWindow* setupWindow = new SetupWindow(); // Создаём окно настроек

    QObject::connect(setupWindow, &SetupWindow::setupCompleted, mainWindow, &MainWindow::handleSetupCompleted);

    setupWindow->show(); // Показываем окно настроек

    return a.exec(); // Начинаем цикл обработки событий
}

