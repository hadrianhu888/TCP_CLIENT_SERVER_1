#ifndef LISTENER_H
#define LISTENER_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>

class Listener : public QObject
{
    Q_OBJECT
public:
    explicit Listener(QObject *parent = nullptr);
    ~Listener();
    QTcpServer *m_server;

signals:
    void newMessage(QString message);
    void messageReceived(const QString &message,const QString &clientID);

private slots:
    void acceptConnection();
    void startRead();
    void socketDisconnected();

public slots:
    void StartListening();
    void NewConnection();

private:
    QTcpServer *tcpServer;
    QList<QTcpSocket*> clients;
};

#endif // LISTENER_H
