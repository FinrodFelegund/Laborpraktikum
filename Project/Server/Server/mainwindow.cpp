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
            qDebug() << db.lastError().text();
        } else{
            qDebug() <<"db loaded successfully!";
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

    if(krypter)
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
    QString str = QString("INFO :: Client with socketID:%1 has just entered the room").arg(socket->socketDescriptor());
    qDebug() << str;
}

void MainWindow::readSocket(){

    qDebug() << "new Message received";
    QTcpSocket *socket = reinterpret_cast<QTcpSocket*>(sender());
    QByteArray buffer;


    QDataStream socketStream(socket);
    socketStream.setVersion(QDataStream::Qt_6_3);

    socketStream.startTransaction();
    socketStream >> buffer;

    if(!socketStream.commitTransaction())
    {
            QString message = QString("%1 :: Waiting for more data to come..").arg(socket->socketDescriptor());

            qDebug() << message;
            return;

    }


    //here we cut the buffer in header and message and get socket Descriptor in case we have to send data back to client

    QString header = buffer.mid(0, 128);
    long long socketDescriptor = socket->socketDescriptor();

    emit newMessage(header, buffer, socketDescriptor);



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




void MainWindow::displayMessage(QString header, QByteArray buffer, long long socketDescriptor)
{

    //this function must be the center piece of the server
    //its task has to be to determine what entity has to be created or if it only has to get data back to the client
    //for now header consists of 2 integers: message type & entity type which are stored as the first two entrances in the header string
    //this function might get big lol


    QStringList headerSplit = header.split(",");

    int messageType = headerSplit[0].toInt();
    int entityType = headerSplit[1].toInt();
    int cipherLength = headerSplit[2].toInt();

    //qDebug() << messageType << " " << entityType << " " << cipherLength;

    buffer = buffer.mid(128);




    //here we determine what the instruction from the client was
    switch(messageType)
    {
        case MessageHeader::saveMessage:
        createEntityAndSafeToDatabase(entityType, cipherLength, buffer);
        break;

        case MessageHeader::returnMessage:
        //get all the entities of a certain type

        //ToDo Hannah
        returnEntityFromDatabaseWithGivenUserID(entityType, cipherLength, buffer, socketDescriptor);
        break;

        case MessageHeader::loginRequest:
        {
            qDebug() << "Login Request received";
            loginUserAndReturnStatus(entityType, cipherLength, buffer, socketDescriptor);
            break;
        }

        default:
        break;

    }

}

void MainWindow::loginUserAndReturnStatus(int entityType, int cipherLength, QByteArray buffer, long long socketDescriptor)
{

    QTcpSocket *receiver = getSocket(socketDescriptor);
    QString message = krypter->decrypt(buffer, cipherLength);
    QStringList list = message.split(",");
    User user;
    user.setPropertiesAsEntity(list);
    //... find the user with given credentails

    int userID = findUserInDatabase(user);

    //User ID = 0 means user not in database, everything else is userID
    list.clear();
    list.append(QString::number(userID));
    list.append(user.getEmail());
    list.append(user.getPassword());
    QByteArray header;
    header.append(QString::number(MessageHeader::loginRequest).toUtf8() + ",");
    header.append(QString::number(MessageHeader::UserEnt).toUtf8() + ",");
    std::vector<std::shared_ptr<Entity>> ent(1);
    ent[0] = std::make_shared<User>();
    ent[0]->setPropertiesAsEntity(list);

    qDebug() << "made it till return login request ";
    ent[0]->print();
    sendMessage(receiver, ent, header);
}


void MainWindow::createEntityAndSafeToDatabase(int entityType, int cipherLength, QByteArray buffer)
{

    QString message = krypter->decrypt(buffer, cipherLength);
    QStringList list = message.split(",");


    switch (entityType)
    {
        case MessageHeader::AppointmentEnt:
        {
           AppointmentEntity ent;
           ent.setProperties(list[0], list[1], list[2], list[3], list[4]);
           saveAppointmentInDb(ent, list[5]);
           break;
        }
        case MessageHeader::DoctorEnt:
        {
            DoctorEntity ent;
            ent.setProperties(list[0], list[1], list[2], list[3], list[4], list[5]);
            saveDoctorInDb(ent, list[6]);
            break;
        }

        default:
            qDebug()<<"No entity type";
            break;
    }
}

void MainWindow::returnEntityFromDatabaseWithGivenUserID(int entityType, int cipherLength, QByteArray buffer, long long socketDescriptor)
{
    QString message = krypter->decrypt(buffer, cipherLength);
    QStringList list = message.split(",");

    QTcpSocket *receiver = getSocket(socketDescriptor);

    switch(entityType)
    {
        case MessageHeader::AppointmentEnt:
        {
            //1. get all the appointments from database to fill them up in a calendar and send them back via socket; let this be a vector of appointments
            QString userID = list[0];
            std::vector<std::shared_ptr<Entity>> appointments = selectAppointmentsFromDatabase(userID);
            //2. send them back via Socket
            QByteArray header;
            header.append(QString::number(MessageHeader::AppointmentEnt).toUtf8());
            sendMessage(receiver, appointments, header);
            break;
        }

        case MessageHeader::DoctorEnt:
        {
            //1. get a specific doctor back from database
            //2. send it back via Socket
        }



    }
}



void MainWindow::sendMessage(QTcpSocket *socket, std::vector<std::shared_ptr<Entity>> entities, QByteArray header)
{
    int size = entities.size();
    QString buffer;

    for(int i = 0; i < size; i++)
    {
        buffer.append(entities[i]->getPropertiesAsString() + ";");
    }

    int cipherLength;
    QByteArray byteBuffer = krypter->encrypt(buffer, &cipherLength);

    //we created the buffer and we have the socket to which we want to send our data

    //QByteArray header;
    header.append(QString::number(cipherLength).toUtf8() + ",");
    header.resize(128);

    QByteArray message;
    message.append(header);
    message.append(byteBuffer);


    if(socket)
    {
        if(socket->isOpen())
        {
            QDataStream socketStream(socket);
            socketStream.setVersion(QDataStream::Qt_6_3);

            socketStream << message;
        }
    }

}


std::vector<std::shared_ptr<Entity>> MainWindow::selectAppointmentsFromDatabase(QString userID)
{


    //To do Hanna
    //create the Select Statements for Appointments with given User ID

    // .... do all the sql specific stuff here leading to a for loop where we get one appointment at a time from the sql query

    std::vector<std::shared_ptr<Entity>> ent(0/*query.size() ? is that a thing?*/);
    for(unsigned long i = 0; i < ent.size(); i++)
    {
        //Fill a QStringList with all the information from the query for each table row
        QStringList list;
        ent[i] = std::make_shared<AppointmentEntity>();
        ent[i]->setPropertiesAsEntity(list);
    }


    return ent;
}


void MainWindow::saveDoctorInDb(DoctorEntity ent, QString user_id)
{


    if(user_id.toInt() > -1){
        if(db.open()){

            ent.print();
            QSqlQuery queryInsert(db);
                        queryInsert.prepare("INSERT INTO Doctors(doctorname, street, streetnumber, city, plz, phone, uid)"
                                            "VALUES(?,?,?,?,?,?,?)");

                        queryInsert.bindValue(0,ent.getName());
                        queryInsert.bindValue(1,ent.getStreet());
                        queryInsert.bindValue(2,ent.getStreetNumber());
                        queryInsert.bindValue(3,ent.getCity());
                        queryInsert.bindValue(4,ent.getPostalCode());
                        queryInsert.bindValue(5,ent.getPhoneNumber());
                        queryInsert.bindValue(6,user_id.toInt());

                        qDebug() << queryInsert.exec();
                        QSqlError error= queryInsert.lastError();
                        qDebug() <<error.databaseText().toUtf8().constData();
        }

    }
    else{
        qDebug() << "User is not logged in";
    }



}

void MainWindow::saveAppointmentInDb(AppointmentEntity ent, QString user_id)
{

    if(user_id.toInt() > -1){
        if(db.open())
        {

            QSqlQuery queryInsert(db);
            queryInsert.prepare("insert into appointments(appdate,apptime,title,notes,did,uid) values(?,?,?,?,?,?)");
            queryInsert.bindValue(0,QDate::fromString(ent.getDate()));
            queryInsert.bindValue(1,QTime::fromString(ent.getTime()));
            queryInsert.bindValue(2,ent.getTitle());
            queryInsert.bindValue(3,ent.getTitle());
            queryInsert.bindValue(4,ent.getDoctorID());
            queryInsert.bindValue(5,user_id);

            qDebug()<<queryInsert.exec();
            QSqlError error= queryInsert.lastError();
            qDebug() <<error.databaseText().toUtf8().constData();
        }

    }
    else{
        qDebug() << "User is not logged in";
    }
}

int MainWindow::findUserInDatabase(User user)
{
    if(db.open())
    {
        QSqlQuery queryFind(db);
        queryFind.prepare("select * from Users where email = '" +user.getEmail()+ "' ");

        qDebug() << queryFind.exec();
        QSqlError error = queryFind.lastError();
        if(error.isValid())
            qDebug() << "In Find User Query: " << error.databaseText().toUtf8().constData();

        int userID = 0;
        queryFind.next();
        {
            userID = queryFind.value(0).toInt();
        }

        return userID;
    }

    return 0;
}

QTcpSocket* MainWindow::getSocket(long long socketDescriptor)
{
    QTcpSocket *receiver;
    for(QTcpSocket *w : m_connection_set)
    {
        if(w->socketDescriptor() == socketDescriptor)
        {
            receiver = w;
            return receiver;
        }

    }

    return nullptr;
}






