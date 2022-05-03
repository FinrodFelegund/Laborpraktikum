#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_server = new QTcpServer();

    if(m_server->listen(QHostAddress::Any, 8080))
    {
        connect(this, &MainWindow::newMessage, this, &MainWindow::displayMessage);
        connect(m_server, &QTcpServer::newConnection, this, &MainWindow::newConnection);
        ui->statusBar->showMessage("Server is listening...");
    } else
    {
        QMessageBox::critical(this,"QTCPServer",QString("Unable to start the server: %1.").arg(m_server->errorString()));
        exit(EXIT_FAILURE);
    }
}

MainWindow::~MainWindow()
{
    for(QTcpSocket *s : m_connection_set){
        s->close();
        s->deleteLater();
    }

    m_server->close();
    m_server->deleteLater();

    delete ui;

}

void MainWindow::newConnection(){
    while(m_server->hasPendingConnections())
        appendToSocketList(m_server->nextPendingConnection());
}

void MainWindow::appendToSocketList(QTcpSocket* socket){
    m_connection_set.insert(socket);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::readSocket);
    connect(socket, &QTcpSocket::disconnected, this, &MainWindow::discardSocket);
    connect(socket, &QAbstractSocket::errorOccurred, this, &MainWindow::displayError);
    QString str = QString("INFO :: Client with sockd:%1 has just entered the room").arg(socket->socketDescriptor());
    displayMessage(str);
}

void MainWindow::readSocket(){

    QTcpSocket *socket = reinterpret_cast<QTcpSocket*>(sender());
    QByteArray buffer;

    QDataStream socketStream(socket);
    socketStream.setVersion(QDataStream::Qt_5_15);

    socketStream.startTransaction();
    socketStream >> buffer;

    if(!socketStream.commitTransaction())
    {
            QString message = QString("%1 :: Waiting for more data to come..").arg(socket->socketDescriptor());
            emit newMessage(message);
            return;

    }

    QString message = QString::fromStdString(buffer.toStdString());

    emit newMessage(message);
}

void MainWindow::discardSocket(){
    QTcpSocket *socket = reinterpret_cast<QTcpSocket*>(sender());
    QSet<QTcpSocket*>::iterator it = m_connection_set.find(socket);
    if(it != m_connection_set.end())
    {
        QString message = QString("INFO :: A client has just left the room").arg(socket->socketDescriptor());
        displayMessage(message);
        m_connection_set.remove(*it);
    }

    socket->deleteLater();

}

void MainWindow::displayError(QAbstractSocket::SocketError socketError){

    switch(socketError)
    {
        case QAbstractSocket::RemoteHostClosedError:
        break;
        case QAbstractSocket::HostNotFoundError:
             QMessageBox::information(this, "QTCPServer", "The host was not found. Please check the host name and port settings.");
        break;
        case QAbstractSocket::ConnectionRefusedError:
             QMessageBox::information(this, "QTCPServer", "The connection was refused by the peer. Make sure QTCPServer is running, and check that the host name and port settings are correct.");
        break;
        default:
             QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
             QMessageBox::information(this, "QTCPServer", QString("The following error occurred: %1.").arg(socket->errorString()));
        break;
    }
}

void MainWindow::sendMessage(QTcpSocket *socket){

}

void MainWindow::displayMessage(QString &str){

    ui->textBrowser_receivedMessages->append(str);
}


