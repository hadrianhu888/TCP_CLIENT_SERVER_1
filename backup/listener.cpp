/**
 * @file listener.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-23
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "listener.h"

Listener::Listener(QTcpSocket* socket) : QObject(nullptr), socket(socket) {
    connect(socket, &QTcpSocket::readyRead, this, &Listener::onReadyRead);
}

void Listener::onReadyRead() {
    while (socket->canReadLine()) {
        QString message = QString::fromUtf8(socket->readLine()).trimmed();
        emit messageReceived(message);
    }
}

