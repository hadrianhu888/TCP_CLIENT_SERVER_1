#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>

class Listener;

class Client : public QObject {
    Q_OBJECT

public:
    explicit Client(QObject* parent = nullptr);
    void connectToHost(const QString& hostAddress, quint16 port);
    void disconnectFromHost();
    bool isConnected() const;
    QString getClientId() const;
    void setClientId(const QString& id);
    void sendMessage(const QString& message);

signals:
    void connected();
    void disconnected();
    void messageReceived(const QString& message);
    void errorOccurred(const QString& error);

private slots:
    void onSocketConnected();
    void onSocketDisconnected();
    void onSocketError(QAbstractSocket::SocketError error);
    void onMessageReceived(const QString& message);

private:
    QTcpSocket* socket;
    Listener* listener;
    QString clientId;
};

#endif // CLIENT_H
