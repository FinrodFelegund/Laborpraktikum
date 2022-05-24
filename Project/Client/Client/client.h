#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QMessageBox>


class Krypter;

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
    void sendMessage(QByteArray header, QString message);
    void readSocket();

private:
    QTcpSocket *m_clientSocket;
    std::string IPAdress;
    int portNumber;
    Krypter *krypter;
    int my_user_id;
};

#endif // CLIENT_H
