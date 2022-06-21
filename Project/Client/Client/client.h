#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QMessageBox>
#include <QTimer>



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
    void sendLogoutRequest();
    void endTimer();
    void getDoctorsFromServer();
    void getAppointmentsFromServer();
    void setLoginStatus(bool logedIn);
    bool getLoginStatus();


private slots:

    void discardSocket();
    void displayError(QAbstractSocket::SocketError socketError);
    void attemtingToRecconect();


public slots:
    void sendMessage(QByteArray header, QString message);
    void readSocket();
    void setCurrentUser(User * currUser);
    void endApplication();
    void reconnectingToServer();
    void createSocketReader();

signals:
    void pendingOpeningRequest(QString buf, int messageType);
    void returnAppointments(QString buf);
    void returnDoctors(QString buf);
    void pendingLogoutRequest(QString buf);
    void sendProgress(QString progress);
    void closeApplication();
    void pendingDeleteRequest(QString buf);


private:


    QTcpSocket *m_clientSocket;
    QDataStream in;
    std::string IPAdress;
    int portNumber;
    Krypter *krypter;
    User* m_user;
    bool m_timeout = true;
    QTimer *m_timer;
    int m_Time;

    bool logedIn = false;

};

#endif // CLIENT_H
