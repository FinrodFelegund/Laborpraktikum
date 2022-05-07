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
    void newMessage(QString &header, QString &message);

private slots:
    void newConnection();
    void appendToSocketList(QTcpSocket *socket);
    void readSocket();
    void discardSocket();
    void displayError(QAbstractSocket::SocketError socketError);

    void displayMessage(QString &header, QString &str);
    void sendMessage(QTcpSocket *socket);



private:
    Ui::MainWindow *ui;
    QTcpServer *m_server;
    QSet<QTcpSocket*> m_connection_set;
    int port_number = 1234;

};
#endif // MAINWINDOW_H
