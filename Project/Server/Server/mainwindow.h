#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QMetaType>
#include <QSet>
#include <QStandardPaths>
#include <QTcpServer>
#include <QTcpSocket>
#include <iostream>
#include "database.h"
#include <memory>

#include "EmailServer/emailclient.h"

class Krypter;
class AppointmentEntity;
class DoctorEntity;
class Entity;
class User;


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


signals:
    void newMessage(QString header, QByteArray message, long long socketDescriptor);

private slots:
    void newConnection();
    void appendToSocketList(QTcpSocket *socket);
    void readSocket();
    void discardSocket();
    void displayError(QAbstractSocket::SocketError socketError);

    void displayMessage(QString header, QByteArray str, long long socketDescriptor);
    void sendMessage(QTcpSocket *socket, std::vector<std::shared_ptr<Entity>> entities, QByteArray header);

    void on_EMailTest_clicked();

private:



    int createEntityAndSafeToDatabase(int entityType, int cipherLength, QByteArray buffer, long long socketDescriptor);

    //Database functions that save an Entity
    bool saveDoctorInDb(DoctorEntity ent, QString user_id);
    bool saveAppointmentInDb(AppointmentEntity ent, QString user_id);
    bool saveUserInDb(User user);


    int findUserInDatabase(User user);
    QString getPasswordFromUser(User user);
    void setLoginStateInDB(QString userID, bool loginState);

    //Server functions that send Entities
    std::vector<std::shared_ptr<Entity>> selectAppointmentsFromDatabase(QString userID);
    std::vector<std::shared_ptr<Entity>> selectDoctorsFromDatabase(QString user_id);


    void returnMessage(int entityType, int cipherLength, QByteArray buffer, long long socketDescriptor);




    //these functions figure out what the user expects from the server

    void returnEntityFromDatabaseWithGivenUserID(int entityType, int cipherLength, QByteArray buffer, long long socketDescriptor);
    void loginUserAndReturnStatus(int entityType, int cipherLength, QByteArray buffer, long long socketDescriptor);
    void signUpUserAndReturnStatus(int entityType, int cipherLength, QByteArray buffer, long long socketDescriptor);
    void sendEmailToUserAndReturnStatus(int entityType, int cipherLength, QByteArray buffer, long long socketDescriptor);
    void logoutUserAndReturnStatus(int entityType, int cipherLength, QByteArray buffer, long long socketDescriptor);
    void deleteUserAndReturnStatus(int entityType, int cipherLength, QByteArray buffer, long long socketDescriptor);
    void returnMessageArray(int entityType, int cipherLength, QByteArray buffer, long long socketDescriptor);

    QTcpSocket *getSocket(long long socketDescriptor);
    void mapUserToSocket(long long socketDescriptor, int userID);
    int getUserIDFromSocketList(long long socketDescriptor);
    QString fetchDBPassword();



    Ui::MainWindow *ui;
    QTcpServer *m_server;

    struct connections{
        connections( QTcpSocket *connection, int userID = -1)
        {
            this->m_connection = connection;
            this->userID = userID;

        }

        //deleting the TcpSockets is job of mainwindow, when connections destructor is invoked, sockets are already deleted
        ~connections(){}
        QTcpSocket* m_connection;
        int userID;

    };

    std::vector<connections> m_connectionSet;

    int port_number = 1234;
    Database db;
    Krypter *krypter;
    EMailClient *eMailClient;


};
#endif // MAINWINDOW_H
