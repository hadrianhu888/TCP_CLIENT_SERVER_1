#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QPushButton>
#include <QtNetwork/QTcpSocket>
#include <QDateTime>

class MainWindow : public QMainWindow {
public:
    MainWindow::MainWindow() {
        setWindowTitle("Echo Client");

        // Host Address TextEdit
        hostAddrTextEdit = new QTextEdit(this);
        hostAddrTextEdit->setGeometry(10, 10, 200, 30);
        hostAddrTextEdit->setPlaceholderText("Host Address");

        // Port Number TextEdit
        portTextEdit = new QTextEdit(this);
        portTextEdit->setGeometry(220, 10, 100, 30);
        portTextEdit->setPlaceholderText("Port Number");

        // Client ID TextEdit
        clientIdTextEdit = new QTextEdit(this);
        clientIdTextEdit->setGeometry(10, 50, 310, 30);
        clientIdTextEdit->setPlaceholderText("Client ID");
        clientIdTextEdit->setReadOnly(true);

        // Text to Send TextEdit
        textToSendTextEdit = new QTextEdit(this);
        textToSendTextEdit->setGeometry(10, 90, 310, 60);
        textToSendTextEdit->setPlaceholderText("Text to Send");

        // System Logs TextEdit
        systemLogsTextEdit = new QTextEdit(this);
        systemLogsTextEdit->setGeometry(10, 160, 310, 70);
        systemLogsTextEdit->setReadOnly(true);
        systemLogsTextEdit->setPlaceholderText("System Logs");

        // Received Messages TextEdit
        receivedMessagesTextEdit = new QTextEdit(this);
        receivedMessagesTextEdit->setGeometry(10, 240, 310, 110);
        receivedMessagesTextEdit->setReadOnly(true);
        receivedMessagesTextEdit->setPlaceholderText("Received Messages");

        // Connect Button
        connectButton = new QPushButton("Connect", this);
        connectButton->setGeometry(10, 360, 100, 30);
        connect(connectButton, &QPushButton::clicked, this, &MainWindow::onConnectClicked);

        // Disconnect Button
        disconnectButton = new QPushButton("Disconnect", this);
        disconnectButton->setGeometry(120, 360, 100, 30);
        disconnectButton->setDisabled(true);
        connect(disconnectButton, &QPushButton::clicked, this, &MainWindow::onDisconnectClicked);

        // Send Button
        sendButton = new QPushButton("Send", this);
        sendButton->setGeometry(230, 360, 100, 30);
        sendButton->setDisabled(true);
        connect(sendButton, &QPushButton::clicked, this, &MainWindow::onSendClicked);

        // TCP Socket
        tcpSocket = new QTcpSocket(this);
        connect(tcpSocket, &QTcpSocket::connected, this, &MainWindow::onSocketConnected);
        connect(tcpSocket, &QTcpSocket::disconnected, this, &MainWindow::onSocketDisconnected);
        connect(tcpSocket, &QTcpSocket::errorOccurred, this, &MainWindow::onSocketError);
        connect(tcpSocket, &QTcpSocket::readyRead, this, &MainWindow::onSocketReadyRead);
    }

private slots:
    void onConnectClicked() {
        tcpSocket->connectToHost(hostAddrTextEdit->toPlainText(), portTextEdit->toPlainText().toInt());
    }

    void onDisconnectClicked() {
        tcpSocket->disconnectFromHost();
    }

    void onSendClicked() {
        QString message = textToSendTextEdit->toPlainText();
        if (message.length() > 0) {
            tcpSocket->write(QString("%1\n").arg(message).toUtf8());
            textToSendTextEdit->clear();
        }
    }

    void onSocketConnected() {
        clientIdTextEdit->setText(tcpSocket->localAddress().toString());
        connectButton->setDisabled(true);
        disconnectButton->setDisabled(false);
        sendButton->setDisabled(false);Z
        systemLogsTextEdit->append(QString("%1: Connected to %2:%3").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(tcpSocket->peerAddress().toString()).arg(tcpSocket->peerPort()));
    }

    void onSocketDisconnected() {
        clientIdTextEdit->clear();
        connectButton->setDisabled(false);
        disconnectButton->setDisabled(true);
        sendButton->setDisabled(true);
        systemLogsTextEdit->append(QString("%1: Disconnected from %2:%3").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(tcpSocket->peerAddress().toString()).arg(tcpSocket->peerPort()));
    }

    void onSocketError(QAbstractSocket::SocketError socketError) {
        systemLogsTextEdit->append(QString("%1: Socket Error: %2").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(tcpSocket->errorString()));
    }

    void onSocketReadyRead() {
        while (tcpSocket->canReadLine()) {
            QString message = QString::fromUtf8(tcpSocket->readLine()).trimmed();
            receivedMessagesTextEdit->append(QString("%1: %2").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(message));
        }
    }

    void MainWindow::log(QString message) {
        QDateTime now = QDateTime::currentDateTime();
        QString timestamp = now.toString(Qt::ISODate);
        QString logMessage = QString("[%1] %2\n").arg(timestamp).arg(message);
        systemLogsTextEdit->append(logMessage);
    }

private:
    QTextEdit *hostAddrTextEdit;
    QTextEdit *portTextEdit;
    QTextEdit *clientIdTextEdit;
    QTextEdit *textToSendTextEdit;
    QTextEdit *systemLogsTextEdit;
    QTextEdit *receivedMessagesTextEdit;
    QPushButton *connectButton;
    QPushButton *disconnectButton;
    QPushButton *sendButton;
    QTcpSocket *tcpSocket;
};

