#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../../header.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_server = new QTcpServer(this);

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

    db =QSqlDatabase::addDatabase("QMYSQL","demoapp");
        db.setHostName("127.0.0.1");
        db.setDatabaseName("demoapp");
        db.setUserName("LPRApp");
        db.setPassword("DaCiHa22");
        if(!db.open()){
            std::cerr<<db.lastError().text().toStdString()<<std::endl;
        } else{
            std::cout<<"db loaded successfully!"<<std::endl;
        }

    krypter = new Krypter;
}

MainWindow::~MainWindow()
{
    for(QTcpSocket *s : m_connection_set){
        s->close();
        s->deleteLater();
    }

    m_server->close();
    m_server->deleteLater();

    delete krypter;
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
    qDebug() << str;
}

void MainWindow::readSocket(){

    qDebug() << "new Message received";
    QTcpSocket *socket = reinterpret_cast<QTcpSocket*>(sender());
    QByteArray buffer;


    QDataStream socketStream(socket);
    socketStream.setVersion(QDataStream::Qt_5_15);

    socketStream.startTransaction();
    socketStream >> buffer;

    if(!socketStream.commitTransaction())
    {
            QString message = QString("%1 :: Waiting for more data to come..").arg(socket->socketDescriptor());

            qDebug() << message;
            return;

    }


    //here we cut the buffer in header and message

    QString header = buffer.mid(0, 128);


    emit newMessage(header, buffer);



}

void MainWindow::discardSocket(){
    QTcpSocket *socket = reinterpret_cast<QTcpSocket*>(sender());
    QSet<QTcpSocket*>::iterator it = m_connection_set.find(socket);
    if(it != m_connection_set.end())
    {
        QString message = QString("INFO :: A client has just left the room").arg(socket->socketDescriptor());
        qDebug() << message;
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

void MainWindow::sendMessage(QTcpSocket *socket)
{

}

void MainWindow::saveDoctorInDb(QString message)
{
    QStringList docInfo = message.split(";");

    QString name = docInfo[0];
    QString street = docInfo[1];
    QString housenumber = docInfo[2];
    QString city = docInfo[3];
    QString plz = docInfo[4];
    QString phonenumber = docInfo[5];
    int user_id = docInfo[6].toInt();

    if(user_id > -1){
        if(db.open()){

            QSqlQuery queryInsert(db);
                        queryInsert.prepare("insert into doctors(doctorname, street, streetnumber, city, plz, phone, uid) VALUES(?,?,?,?,?,?,?)");
                        queryInsert.bindValue(0,name);
                        queryInsert.bindValue(1,street);
                        queryInsert.bindValue(2,housenumber);
                        queryInsert.bindValue(3,city);
                        queryInsert.bindValue(4,plz);
                        queryInsert.bindValue(5,phonenumber);
                        queryInsert.bindValue(6,user_id);

                        qDebug()<<queryInsert.exec();
                        QSqlError error= queryInsert.lastError();
                        std::cout<<error.databaseText().toUtf8().constData();
        }

    }
    else{
        qDebug() << "User is not logged in";
    }



}

void MainWindow::saveAppointmentInDb(QString message)
{
    QStringList appointmentInfo = message.split(";");

    QDate date = QDate::fromString(appointmentInfo[0]);
    QTime time = QTime::fromString(appointmentInfo[1]);
    QString title = appointmentInfo[2];
    QString notes = appointmentInfo[3];
    int doc_id = appointmentInfo[4].toInt();
    int user_id = appointmentInfo[5].toInt();

    if(user_id > -1){
        if(db.open()){

            QSqlQuery queryInsert(db);
                        queryInsert.prepare("insert into appointments(appdate,apptime,title,notes,did,uid) values(?,?,?,?,?,?)");
                        queryInsert.bindValue(0,date);
                        queryInsert.bindValue(1,time);
                        queryInsert.bindValue(2,title);
                        queryInsert.bindValue(3,notes);
                        queryInsert.bindValue(4,doc_id);
                        queryInsert.bindValue(5,user_id);

                        qDebug()<<queryInsert.exec();
                        QSqlError error= queryInsert.lastError();
                        std::cout<<error.databaseText().toUtf8().constData();
        }

    }
    else{
        qDebug() << "User is not logged in";
    }
}

void MainWindow::displayMessage(QString header, QByteArray buffer)
{

    //this function must be the center piece of the server
    //its task has to be to determine what entity has to be created or if it only has to get data back to the client
    //for now header consists of 2 integers: message type & entity type which are stored as the first two entrances in the header string
    //this function might get big lol


    QStringList headerSplit = header.split(",");

    int messageType = headerSplit[0].toInt();
    int entityType = headerSplit[1].toInt();
    int cipherLength = headerSplit[2].toInt();

    buffer = buffer.mid(128);

    QString message = decrypt(buffer, cipherLength);

    //std::cout<<message.toStdString()<<std::endl;

    //here we determine what the instruction from the client was
    switch(messageType)
    {
        case MessageHeader::saveMessage:
        //saveToDataBase(entityType, buffer)
        safeEntityToDatabase(entityType, cipherLength, buffer);
        break;

        case MessageHeader::returnMessage:
        //get only a certain Entity
        break;

        case MessageHeader::returnMessageArray:
        //get all the database entries for an entity
        break;

        default:
        break;

    }

}


void MainWindow::safeEntityToDatabase(int entityType, int cipherLength, QByteArray buffer)
{
    QString message = decrypt(buffer, cipherLength);

    switch (entityType) {
    case MessageHeader::AppointmentEnt:
        saveAppointmentInDb(message);
        break;
    case MessageHeader::DoctorEnt:
        saveDoctorInDb(message);
        break;
    default:
        qDebug()<<"No entity type";
        break;
    }
}

QString MainWindow::decrypt(QByteArray buffer, int cipherLength)
{
    unsigned char buf[300];
    unsigned char plainText[300];
    qDebug() << "Server Utf8 encoded: " << buffer;
    QString cipher = QString::fromUtf8(buffer);

    for(int i = 0; i < cipherLength; i++)
    {
        buf[i] = cipher[i].toLatin1();
        printf("%d ", cipher[i]);
    }

    qDebug();


    krypter->decrypt(buf, cipherLength, plainText);


    QString message = QString::fromUtf8((char*)plainText);

    qDebug() << message;

    return message;


}


