#include "gameclient.h"
#include <QDebug>

GameClient::GameClient(QObject *parent) : QObject(parent), socket(new QTcpSocket(this)) {
    connect(socket, &QTcpSocket::connected, this, &GameClient::onConnected);
    connect(socket, &QTcpSocket::disconnected, this, &GameClient::onDisconnected);
    connect(socket, &QTcpSocket::readyRead, this, &GameClient::onDataReceived);
}

void GameClient::connectToServer(const QString &host, quint16 port) {
    socket->connectToHost(host, port);
}

void GameClient::disconnectFromServer() {
    socket->disconnectFromHost();
}


void GameClient::sendData(const QString &data) {
    if (socket->state() == QTcpSocket::ConnectedState) {
        socket->write(data.toUtf8()); // Convert QString to QByteArray
    } else {
        qDebug() << "Socket is not connected, cannot send data.";
    }
}


void GameClient::onConnected() {
    qDebug() << "Connected to server.";
    emit connected();
}

void GameClient::onDisconnected() {
    qDebug() << "Disconnected from server.";
    emit disconnected();
}

// Assume socket is a valid QTcpSocket pointer connected to the server
void GameClient::sendAttackCoordinates(const QString& coordinates) {
    if (socket->state() == QTcpSocket::ConnectedState) {
        socket->write(coordinates.toUtf8());
    }
}

void GameClient::sendCoordinatesToServer(int row, int col) {
    char colLetter = 'A' + col;

    QString coordinates = QString("%1,%2,attack").arg(colLetter).arg(row + 1); // Rows and cols are 0-indexed

    if (socket->state() == QTcpSocket::ConnectedState) {
        qDebug() << "Sending coordinates to server:" << coordinates;
        socket->write(coordinates.toUtf8());
    } else {
        qDebug() << "Socket is not connected.";
    }
}

//void GameClient::onDataReceived() {
//    QByteArray data = socket->readAll();
//    QString receivedData = QString::fromUtf8(data);
//    // Assume data is "attack,row,col"
//    qDebug() << "Received message:" << receivedData;

//    QStringList parts = receivedData.split(',');
//    if (parts.length() == 3 && parts[2] == "attack") {
//        int row = parts[0].toInt() - 65;
//        int col = parts[1].toInt();
//        emit attackReceived(row, col); // Emit the signal
//    }
//}

void GameClient::onDataReceived() {
    QByteArray data = socket->readAll();
    QString receivedData = QString::fromUtf8(data).trimmed(); // Trim any whitespace/newline
    qDebug() << "Received message:" << receivedData;

    QStringList parts = receivedData.split(',');
    if (parts.length() == 3 && parts[2] == "attack") {
        // Assuming parts[0] is the column letter
        QChar colLetter = parts[0].at(0).toUpper(); // Ensure uppercase for consistency
        int col = colLetter.unicode() - 'A'; // Convert column letter to zero-based index

        // Assuming parts[1] is the row number
        int row = parts[1].toInt() - 1; // Convert to int and adjust for zero-based indexing if necessary

        emit attackReceived(row, col); // Emit the signal with adjusted row and col values
    } else {
        emit serverResponseReceived(parts);
        // Handle error or unexpected message format
        qDebug() << "Unexpected message format or content.";
    }
}

