#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QMessageBox>


class Krypter;
class User;

class Client : public QObject
{
    Q_OBJECT
public:
    Client();
    ~Client();
    void processNewMessage(QString header, QByteArray buffer);
    User createUserFromBuffer();

private slots:

    void discardSocket();
    void displayError(QAbstractSocket::SocketError socketError);
public slots:
    void sendMessage(QByteArray header, QString message);
    void readSocket();

signals:
    void pendingLoginRequest(QString buf, int messageType);



private:
    QTcpSocket *m_clientSocket;
    std::string IPAdress;
    int portNumber;
    Krypter *krypter;
    int my_user_id;
};

#endif // CLIENT_H
