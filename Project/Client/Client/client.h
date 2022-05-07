#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QMessageBox>

class Client : public QObject
{
    Q_OBJECT
public:
    Client();
    ~Client();

private slots:

    void discardSocket();
    void displayError(QAbstractSocket::SocketError socketError);
public slots:
    void sendMessage(QByteArray message);
    void readSocket();

private:
    QTcpSocket *m_clientSocket;
    std::string IPAdress;
    int portNumber;
};

#endif // CLIENT_H
