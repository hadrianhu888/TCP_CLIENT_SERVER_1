/**
 * @file client.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-23
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "client.h"
#include "listener.h"

Client::Client(QObject* parent) : QObject(parent), socket(new QTcpSocket(this)), listener(nullptr) {
    connect(socket, &QTcpSocket::connected, this, &Client::onSocketConnected);
    connect(socket, &QTcpSocket::disconnected, this, &Client::onSocketDisconnected);
    connect(socket, reinterpret_cast<void(QTcpSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error),
            this, &Client::onSocketError);
}

void Client::connectToHost(const QString& hostAddress, quint16 port) {
    socket->connectToHost(QHostAddress(hostAddress), port);
}

void Client::disconnectFromHost() {
    socket->disconnectFromHost();
}

bool Client::isConnected() const {
    return socket->state() == QAbstractSocket::ConnectedState;
}

QString Client::getClientId() const {
    return clientId;
}

void Client::setClientId(const QString& id) {
    clientId = id;
}

void Client::sendMessage(const QString& message) {
    if (!isConnected()) {
        emit errorOccurred("Error: Not connected to server");
        return;
    }

    QString messageToSend = message.trimmed();
    if (messageToSend.isEmpty()) {
        emit errorOccurred("Error: Message cannot be empty");
        return;
    }

    QString formattedMessage = QString("%1:%2\n").arg(clientId).arg(messageToSend);
    socket->write(formattedMessage.toUtf8());
}

void Client::onSocketConnected() {
    listener = new Listener(socket);
    connect(listener, &Listener::messageReceived, this, &Client::onMessageReceived);
    emit connected();
}

void Client::onSocketDisconnected() {
    delete listener;
    listener = nullptr;
    emit disconnected();
}

void Client::onSocketError(QAbstractSocket::SocketError error) {
    emit errorOccurred(socket->errorString());
}

void Client::onMessageReceived(const QString& message) {
    emit messageReceived(message);
}
