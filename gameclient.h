#ifndef GAMECLIENT_H
#define GAMECLIENT_H

#include <QTcpSocket>
#include <QObject>

class GameClient : public QObject {
    Q_OBJECT

public:
    explicit GameClient(QObject *parent = nullptr);
    void connectToServer(const QString &host, quint16 port);
    void disconnectFromServer();
    void sendData(const QString &data);
    void sendAttackCoordinates(const QString& coordinates);
    void sendCoordinatesToServer(int row, int col); // Adapt parameters as needed
    QString encryptData(const QString &data);
    QString decryptData(const QString &data);


signals:
    void connected();
    void disconnected();
    void dataReceived(const QByteArray &data);
    void serverResponseReceived(const QStringList& result);
    void attackReceived(int row, int col);
    void gameOverReceived();

public slots:
    void onConnected();
    void onDisconnected();
    void onDataReceived();

private:
    QTcpSocket *socket;
    const uint8_t encryptionKey;
};

#endif // GAMECLIENT_H
