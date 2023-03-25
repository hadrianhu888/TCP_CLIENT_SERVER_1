#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QPushButton>
#include <QtNetwork/QTcpSocket>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow();

private slots:
    void onConnectClicked();
    void onDisconnectClicked();
    void onSendClicked();
    void onSocketConnected();
    void onSocketDisconnected();
    void onSocketError();
    void onSocketReadyRead();

private:
    QTextEdit* hostAddrTextEdit;
    QTextEdit* portTextEdit;
    QTextEdit* clientIdTextEdit;
    QTextEdit* textToSendTextEdit;
    QTextEdit* systemLogsTextEdit;
    QTextEdit* receivedMessagesTextEdit;
    QPushButton* connectButton;
    QPushButton* disconnectButton;
    QPushButton* sendButton;
    QTcpSocket* tcpSocket;
    QString clientId;

    void log(QString message);
    QString generateClientId();
};

#endif // MAINWINDOW_H
