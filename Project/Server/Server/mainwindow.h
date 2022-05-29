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
#include <QtSql>
#include <memory>

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



private:

    //Database functions that save an Entity
    void saveDoctorInDb(DoctorEntity ent, QString user_id);
    void saveAppointmentInDb(AppointmentEntity ent, QString user_id);
    int findUserInDatabase(User user);

    //Database functions that return Entities
    std::vector<std::shared_ptr<Entity>> selectAppointmentsFromDatabase(QString userID);


    void returnEntityFromDatabaseWithGivenUserID(int entityType, int cipherLength, QByteArray buffer, long long socketDescriptor);
    void createEntityAndSafeToDatabase(int entityType, int cipherLength, QByteArray buffer);
    void loginUserAndReturnStatus(int entityType, int cipherLength, QByteArray buffer, long long socketDescriptor);

    QTcpSocket * getSocket(long long socketDescriptor);

    Ui::MainWindow *ui;
    QTcpServer *m_server;
    QSet<QTcpSocket*> m_connection_set;
    int port_number = 1234;
    QSqlDatabase db;
    Krypter *krypter;


};
#endif // MAINWINDOW_H
