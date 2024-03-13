QT       += core gui widgets network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    enemyboard.cpp \
    gameboard.cpp \
    main.cpp \
    mainwindow.cpp \
    playerboard.cpp \
    resetwindow.cpp \
    ship.cpp

HEADERS += \
    enemyboard.h \
    gameboard.h \
    mainwindow.h \
    playerboard.h \
    resetwindow.h \
    ship.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
