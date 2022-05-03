#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_clientSocket = new QTcpSocket(this);
    IPAdress = "127.0.0.1";
    portNumber = 8080;

    connect(this, &MainWindow::newMessage, this, &MainWindow::displayMessage);
    connect(m_clientSocket, &QTcpSocket::readyRead, this, &MainWindow::readSocket);
    connect(m_clientSocket, &QTcpSocket::disconnected, this, &MainWindow::discardSocket);
    connect(m_clientSocket, &QAbstractSocket::errorOccurred, this, &MainWindow::displayError);

    m_clientSocket->connectToHost(QHostAddress::Any, portNumber);

    if(m_clientSocket->waitForConnected()){
        ui->statusBar->showMessage("Connected to server");
    } else {
        QMessageBox::critical(this, "QTcpClient", QString("The following error occurred: %1.").arg(m_clientSocket->errorString()));
        exit(EXIT_FAILURE);
    }
}

MainWindow::~MainWindow()
{
    if(m_clientSocket->isOpen())
        m_clientSocket->close();
    delete ui;

}

void MainWindow::discardSocket(){
    m_clientSocket->deleteLater();
    m_clientSocket = nullptr;

    ui->statusBar->showMessage("Disconnected");
}

void MainWindow::displayError(QAbstractSocket::SocketError socketError){

    switch (socketError){
        case QAbstractSocket::RemoteHostClosedError:
        break;

        case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(this, "TcpClient", "The host was not found. Please check the host name and port settings.");
        break;

        case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(this, "QTCPClient", "The connection was refused by the peer. Make sure QTCPServer is running, and check that the host name and port settings are correct.");
        break;

        default:
        QMessageBox::information(this, "QTCPClient", QString("The following error occurred: %1.").arg(m_clientSocket->errorString()));
        break;


    }
}

void MainWindow::readSocket(){

    QByteArray byteArray = m_clientSocket->readAll();
    qDebug() << byteArray;

    QString message = QString::fromStdString(byteArray.toStdString());
    emit newMessage(message);
}

void MainWindow::displayMessage(QString &str){

    ui->textBrowser_receivedMessages->append(str);
}





void MainWindow::on_pushButton_sendMessage_clicked()
{
    if(m_clientSocket){
        if(m_clientSocket->isOpen()){
            QString str = ui->lineEdit_message->text();

            QDataStream socketStream(m_clientSocket);
            socketStream.setVersion(QDataStream::Qt_5_15);

            QByteArray byteArray = str.toUtf8();
            socketStream << byteArray;
            ui->lineEdit_message->clear();
        } else
            QMessageBox::critical(this,"QTCPClient","Socket doesn't seem to be opened");
    } else
        QMessageBox::critical(this,"QTCPClient","Not connected");
}

