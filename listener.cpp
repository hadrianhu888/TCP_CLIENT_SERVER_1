#include "listener.h"

Listener::Listener(QObject *parent) : QObject(parent)
{
    tcpServer = new QTcpServer(this);
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
    tcpServer->listen(QHostAddress::Any, 1502);
}

Listener::~Listener()
{
    delete tcpServer;
}

void Listener::acceptConnection()
{
    QTcpSocket* client = tcpServer->nextPendingConnection();
    clients.append(client);

    connect(client, SIGNAL(readyRead()), this, SLOT(startRead()));
    connect(client, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));
}

void Listener::startRead()
{
    QTcpSocket* client = qobject_cast<QTcpSocket*>(sender());
    if (!client) return;

    QByteArray data = client->readAll();

    // Append client id to message
    QString message = QString::fromUtf8(data).trimmed() + " [ACK:" + QString::number(client->socketDescriptor()) + "]";

    // Emit signal to notify of new message
    emit newMessage(message);

    // Send message back to client
    client->write(message.toUtf8());
}

void Listener::socketDisconnected()
{
    QTcpSocket* client = qobject_cast<QTcpSocket*>(sender());
    if (!client) return;

    clients.removeOne(client);
    client->deleteLater();
}

void Listener::StartListening()
{
    if (m_server->listen(QHostAddress::LocalHost, 1502)) {
        qDebug() << "Listening for connections...";
    } else {
        qDebug() << "Error: " << m_server->errorString();
    }
}

void Listener::NewConnection()
{
    QTcpSocket *socket = m_server->nextPendingConnection();
    qDebug() << "New connection from: " << socket->peerAddress().toString();

    // connect the readyRead signal to the slot that will read the data
    connect(socket, &QTcpSocket::readyRead, [=]() {
        QByteArray data = socket->readAll();
        QString message(data);
        qDebug() << "Received message: " << message;

        // emit the message received signal, along with the client ID
        emit messageReceived(message, socket->peerAddress().toString());
        
        // send the message back to the client with an ACK
        message.append(QString(" [ACK:%1]").arg(socket->peerAddress().toString()));
        socket->write(message.toUtf8());
        socket->flush();
    });
}
