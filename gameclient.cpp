#include "gameclient.h"
#include <QDebug>

GameClient::GameClient(QObject *parent) : QObject(parent), socket(new QTcpSocket(this)), encryptionKey{214} {
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
        QString encryptedData = encryptData(data);
        socket->write(data.toUtf8()); // Convert QString to QByteArray
    } else {
        qDebug() << "Socket is not connected, cannot send data.";
    }
}

QString GameClient::encryptData(const QString &data) {
    QString encryptedData;
    for (int i = 0; i < data.length(); ++i) {
        encryptedData.append(QChar(data.at(i).unicode() ^ encryptionKey)); // XOR encryption
    }
    return encryptedData;
}

void GameClient::onConnected() {
    qDebug() << "Connected to server.";
    emit connected();
}

void GameClient::onDisconnected() {
    qDebug() << "Disconnected from server.";
    emit disconnected();
}

void GameClient::sendAttackCoordinates(const QString& coordinates) {
    if (socket->state() == QTcpSocket::ConnectedState) {
        socket->write(coordinates.toUtf8());
    }
}

void GameClient::sendCoordinatesToServer(int row, int col) {
    char colLetter = 'A' + col;

    QString coordinates = QString("%1,%2,attack").arg(colLetter).arg(row + 1);

    if (socket->state() == QTcpSocket::ConnectedState) {
        qDebug() << "Sending coordinates to server:" << coordinates;
        socket->write(coordinates.toUtf8());
    } else {
        qDebug() << "Socket is not connected.";
    }
}


QString GameClient::decryptData(const QString &data) {
    QString decryptedData;
    for (int i = 0; i < data.length(); ++i) {
        decryptedData.append(QChar(data.at(i).unicode() ^ encryptionKey)); // XOR decryption
    }
    return decryptedData;
}

void GameClient::onDataReceived() {
    QByteArray data = socket->readAll();
    QString receivedData = QString::fromUtf8(data).trimmed();
    qDebug() << "Received message:" << receivedData;

    QString decryptedData = decryptData(receivedData);

    QStringList parts = receivedData.split(',');
    if (parts.length() == 3 && parts[2] == "attack") {
        QChar colLetter = parts[0].at(0).toUpper();
        int col = colLetter.unicode() - 'A';

        int row = parts[1].toInt() - 1;

        emit attackReceived(row, col);
    } else {
        emit serverResponseReceived(parts);
        qDebug() << "Unexpected message format or content.";
    }

    if (receivedData == "game_over") {
        qDebug("game_over recieved");
        emit gameOverReceived();
        return;
    }
}

