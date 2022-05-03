#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QMessageBox>
#include <QAbstractSocket>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void readSocket();
    void discardSocket();
    void displayError(QAbstractSocket::SocketError socketError);

    void displayMessage(QString &str);


    void on_pushButton_sendMessage_clicked();

signals:
    void newMessage(QString &str);


private:
    Ui::MainWindow *ui;
    QTcpSocket *m_clientSocket;
    std::string IPAdress;
    int portNumber;
};
#endif // MAINWINDOW_H
