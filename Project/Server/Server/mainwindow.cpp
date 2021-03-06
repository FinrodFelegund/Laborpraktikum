#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include "../../header.h"
#include <QtConcurrent>
#include <QFuture>


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

    //-------------------------
    // Einmal auskommentieren und laufen lassen. Erzeugt dann ein .ini File in dem das MySQL Passwort verschlüsselt steht, kann nach Erzeugung wieder auskommentiert werden
    //-------------------------
    /*QSettings settings("Laborpraktikum", "Project");
    settings.beginGroup("Server");
    settings.setValue("buffer", "\u0099\u0096çz-hèóßM<\u0018¹ÀFî");
    settings.setValue("length", 16);
    settings.endGroup();*/

    krypter = new Krypter;
    eMailClient = new EMailClient();
    db.setUpDatabase(fetchDBPassword());

}

QString MainWindow::fetchDBPassword()
{


    QSettings settings("Laborpraktikum", "Project");
    settings.beginGroup("Server");
    qDebug() << settings.fileName();
    QString buffer = settings.value("buffer").toString();
    int length = settings.value("length").toInt();
    settings.endGroup();


    if(!buffer.isEmpty())
       buffer = krypter->decrypt(buffer.toUtf8(), length);
    else return "";

    qDebug() << buffer;


    return buffer;
}

MainWindow::~MainWindow()
{
    for(connections s : m_connectionSet){
        s.m_connection->close();
        s.m_connection->deleteLater();
    }

    m_server->close();
    m_server->deleteLater();

    if(krypter)
        delete krypter;
    if(eMailClient)
        delete eMailClient;

    delete ui;

}

void MainWindow::newConnection(){
    while(m_server->hasPendingConnections())
        appendToSocketList(m_server->nextPendingConnection());
}

void MainWindow::appendToSocketList(QTcpSocket* socket){
    connections con(socket);
    m_connectionSet.push_back(con);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::readSocket);
    connect(socket, &QTcpSocket::disconnected, this, &MainWindow::discardSocket);
    connect(socket, &QAbstractSocket::errorOccurred, this, &MainWindow::displayError);
    QString str = QString("INFO :: Client with socketID:%1 has just entered the room.").arg(socket->socketDescriptor());
    qDebug() << str;

}

void MainWindow::readSocket(){


    QTcpSocket *socket = reinterpret_cast<QTcpSocket*>(sender());
    QByteArray buffer;


    QDataStream socketStream(socket);
    socketStream.setVersion(QDataStream::Qt_6_3);

    socketStream.startTransaction();

    while(socket->bytesAvailable())
    {
           socketStream >> buffer;
    }


    if(!socketStream.commitTransaction())
    {
            QString message = QString("%1 :: Waiting for more data to come..").arg(socket->socketDescriptor());

            qDebug() << message;
            qDebug() << "Test";
            return;

    }


    //here we cut the buffer in header and message and get socket Descriptor in case we have to send data back to client

    QString header = buffer.mid(0, 128);
    long long socketDescriptor = socket->socketDescriptor();

    emit newMessage(header, buffer, socketDescriptor);



}

void MainWindow::discardSocket(){
    QTcpSocket *socket = reinterpret_cast<QTcpSocket*>(sender());
    std::vector<connections>::iterator it = m_connectionSet.begin(); //= m_connectionSet.find(socket);
    int index = 0;
    while(it->m_connection != socket)
    {
        it++;
        index++;
    }

    if(it != m_connectionSet.end())
    {
        QString message = QString("INFO :: A client has just left the room with SocketID: %1").arg(socket->socketDescriptor());
        qDebug() << message;
        if(it->userID > 0) //Dani: sorgt dafür dass die DB auch nur geupdated wird, falls bereit ein Login durchgeführt wurde
            setLoginStateInDB(QString::number(it->userID), false);

        m_connectionSet.erase(m_connectionSet.begin() + index);
        //this will reset the leaving users Login State

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

bool MainWindow::saveDoctorInDb(DoctorEntity ent, QString user_id)
{
    return db.saveDoctorInDb(ent,user_id);
}

bool MainWindow::saveAppointmentInDb(AppointmentEntity ent, QString user_id)
{
    return db.saveAppointmentInDb(ent,user_id);
}

bool MainWindow::saveUserInDb(User user)
{
    return db.saveUserInDb(user);
}

void MainWindow::displayMessage(QString header, QByteArray buffer, long long socketDescriptor)
{   //this function must be the center piece of the server
    //its task has to be to determine what entity has to be created or if it only has to get data back to the client
    //for now header consists of 2 integers: message type & entity type which are stored as the first two entrances in the header string
    //this function might get big lol

    QStringList headerSplit = header.split(",");

    int messageType = headerSplit[0].toInt();
    int entityType = headerSplit[1].toInt();
    int cipherLength = headerSplit[2].toInt();



    qDebug() << messageType << " " << entityType << " " << cipherLength;

    buffer = buffer.mid(128);

    //here we determine what the instruction from the client was
    switch(messageType)
    {
        case MessageHeader::saveMessage:
        {
            qDebug() << "saving Entity";
            entityType=createEntityAndSafeToDatabase(entityType, cipherLength, buffer, socketDescriptor);
            returnMessage(entityType,cipherLength,buffer,socketDescriptor);
            break;
        }


        case MessageHeader::returnMessage:
        {
            qDebug() << "returning Message";
            returnMessage(entityType, cipherLength, buffer, socketDescriptor);
            break;
        }

        case::MessageHeader::returnMessageArray:
        {
            qDebug() << "returning MessageArray";
            returnMessageArray(entityType, cipherLength, buffer, socketDescriptor);
            break;
        }


        case MessageHeader::loginRequest:
        {
            qDebug() << "Login Request received";
            loginUserAndReturnStatus(entityType, cipherLength, buffer, socketDescriptor);
            break;
        }

        case MessageHeader::signUpRequest:
        {
            qDebug() << "SignUp Request received";
            signUpUserAndReturnStatus(entityType, cipherLength, buffer, socketDescriptor);
            break;

        }

        case MessageHeader::passwortRequest:
        {
            qDebug() << "Password Request received";
            sendEmailToUserAndReturnStatus(entityType, cipherLength, buffer, socketDescriptor);
            break;
        }

        case MessageHeader::logoutRequest:
        {
            qDebug() << "Logout Request received";
            logoutUserAndReturnStatus(entityType, cipherLength, buffer, socketDescriptor);
            break;
        }

        case MessageHeader::deleteUserRequest:
        {
            qDebug() << "Delete user request received";
            deleteUserAndReturnStatus(entityType, cipherLength, buffer, socketDescriptor);
            break;
        }

        default:
        break;

    }

}

void MainWindow::returnMessageArray(int entityType, int cipherLength, QByteArray buffer, long long socketDescriptor)
{
    QTcpSocket *receiver = getSocket(socketDescriptor);
    int userID = getUserIDFromSocketList(socketDescriptor);

    switch(entityType)
    {
        case MessageHeader::DoctorEnt:
        {
            std::vector<std::shared_ptr<Entity>> ents = db.selectDoctorsFromDatabase(QString::number(userID));


            QByteArray header;
            header.append(QString::number(MessageHeader::returnMessageArray).toUtf8()+ ",");
            header.append(QString::number(MessageHeader::DoctorEnt).toUtf8()+ ",");

            qDebug() << "sending doctors back";
            for(int i = 0; i < ents.size(); i++){
               ents[i]->print();
            }
            sendMessage(receiver, ents, header);
            break;
        }

        case MessageHeader::AppointmentEnt:
        {
            std::vector<std::shared_ptr<Entity>> ents = db.selectAppointmentsFromDatabase(QString::number(userID));
            QByteArray header;
            header.append(QString::number(MessageHeader::returnMessageArray).toUtf8()+",");
            header.append(QString::number(MessageHeader::AppointmentEnt).toUtf8()+",");

            qDebug() << "sending appointments back";
           /* for(int i = 0; i < ents.size(); i++){
               ents[i]->print();
            }*/
            sendMessage(receiver, ents, header);
            break;
        }

        default:
        break;
    }
}

void MainWindow::deleteUserAndReturnStatus(int entityType, int cipherLength, QByteArray buffer, long long socketDescriptor)
{
    QTcpSocket *receiver = getSocket(socketDescriptor);
    int userID = getUserIDFromSocketList(socketDescriptor);

    bool retVal = db.deleteUser(userID);


    std::vector<std::shared_ptr<Entity>> ents;
    ents.push_back(std::make_shared<User>());
    QStringList list;
    list.append(QString::number(retVal));
    list.append(" ");
    list.append(" ");
    ents[0]->setPropertiesAsEntity(list);

    QByteArray header;

    if(retVal)
    {
        mapUserToSocket(socketDescriptor, 0);
        header.append(QString::number(MessageHeader::deleteUserRequest).toUtf8() + ",");
        header.append(QString::number(MessageHeader::UserEnt).toUtf8() + ",");
    } else
    {
        header.append(QString::number(MessageHeader::deleteUserRequest).toUtf8() + ",");
        header.append(QString::number(MessageHeader::UserEnt).toUtf8() + ",");
    }

    qDebug() << "Delete Request sending back";
    sendMessage(receiver, ents, header);
}

void MainWindow::logoutUserAndReturnStatus(int entityType, int cipherLength, QByteArray buffer, long long socketDescriptor)
{
    QTcpSocket *receiver = getSocket(socketDescriptor);
    int userID = getUserIDFromSocketList(socketDescriptor);
    bool retVal = db.setLoginStateInDb(QString::number(userID), false);

    std::vector<std::shared_ptr<Entity>> ents;
    ents.push_back(std::make_shared<User>());
    QStringList list;
    list.append(QString::number(retVal));
    list.append(" ");
    list.append(" ");
    ents[0]->setPropertiesAsEntity(list);

    QByteArray header;
    if(retVal){
        mapUserToSocket(socketDescriptor, 0);  // 0 means no User logged in under this socket, but socket still connected to server
        header.append(QString::number(MessageHeader::logoutRequest).toUtf8() + ",");
        header.append(QString::number(MessageHeader::UserEnt).toUtf8() + ",");
    } else
    {
        header.append(QString::number(MessageHeader::logoutRequest).toUtf8() + ",");
        header.append(QString::number(MessageHeader::UserEnt).toUtf8() + ",");
    }


    sendMessage(receiver, ents, header);

}

void MainWindow::sendEmailToUserAndReturnStatus(int entityType, int cipherLength, QByteArray buffer, long long socketDescriptor)
{

    //this needs a revamp
    QTcpSocket *receiver = getSocket(socketDescriptor);
    QString message = krypter->decrypt(buffer, cipherLength);
    QStringList list = message.split(",");
    list.removeLast();
    User userUnencrypted;
    qDebug() << list;
    userUnencrypted.setPropertiesAsEntity(list);

    QString eMailEncrypted = db.getEMailFromUser(userUnencrypted);
    qDebug() << eMailEncrypted;

    //if(!userCredential.isEmpty())

    int retVal = eMailClient->sendEmail(userUnencrypted, eMailEncrypted);

    userUnencrypted.setUID(QString::number(retVal));


    list.clear();
    list.append(userUnencrypted.getUID());
    list.append(userUnencrypted.getEmail());
    list.append(userUnencrypted.getPassword());

    QByteArray header;
    header.append(QString::number(MessageHeader::passwortRequest).toUtf8() + ",");
    header.append(QString::number(MessageHeader::UserEnt).toUtf8() + ",");
    std::vector<std::shared_ptr<Entity>> ent(1);

    ent[0] = std::make_shared<User>();
    ent[0]->setPropertiesAsEntity(list);
    //qDebug() << "made it till return login request ";
    //ent[0]->print();

    //header.append(QString::number(MessageHeader::loginRequest).toUtf8() + ",");
    //header.append(QString::number(MessageHeader::UserEnt).toUtf8() + ",");
    sendMessage(receiver, ent, header);

}

void MainWindow::loginUserAndReturnStatus(int entityType, int cipherLength, QByteArray buffer, long long socketDescriptor)
{

    QTcpSocket *receiver = getSocket(socketDescriptor);
    QString message = krypter->decrypt(buffer, cipherLength);
    QStringList list = message.split(",");
    list.removeLast();
    User user;
    user.setPropertiesAsEntity(list);

    //... find the user with given credentails

    int userID = findUserInDatabase(user);
    //User ID = 0 means user not in database -1 already logged in on other device, everything else is userID

    if(userID > 0)
    {
        setLoginStateInDB(QString::number(userID), true);
        mapUserToSocket(socketDescriptor, userID);
    }

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
    //qDebug() << "made it till return login request ";
    //ent[0]->print();

    //header.append(QString::number(MessageHeader::loginRequest).toUtf8() + ",");
    //header.append(QString::number(MessageHeader::UserEnt).toUtf8() + ",");
    sendMessage(receiver, ent, header);
}

void MainWindow::signUpUserAndReturnStatus(int entityType, int cipherLength, QByteArray buffer, long long socketDescriptor)
{
    QTcpSocket *receiver = getSocket(socketDescriptor);
    QString message = krypter->decrypt(buffer, cipherLength);
    QStringList list = message.split(",");
    list.removeLast();
    User user;
    user.setPropertiesAsEntity(list);

    int userID = findUserInDatabase(user);
    //userID = 0 user is not in database so signup

    int retVal;
    if(userID == 0)
    {
        retVal = createEntityAndSafeToDatabase(entityType, cipherLength, buffer, socketDescriptor); //ja es wird nochmal entschlüsselt I know
        if(retVal == MessageHeader::UserNotSaved)
            list[0] = QString::number(0);
        else list[0] = QString::number(1);

    }
    else
    {
        list[0] = QString::number(0); //error
    }

    QByteArray header;
    header.append(QString::number(MessageHeader::signUpRequest).toUtf8() + ",");
    header.append(QString::number(MessageHeader::UserEnt).toUtf8() + ",");
    std::vector<std::shared_ptr<Entity>> ent(1);

    ent[0] = std::make_shared<User>();
    ent[0]->setPropertiesAsEntity(list);

    sendMessage(receiver, ent, header);
}

int MainWindow::createEntityAndSafeToDatabase(int entityType, int cipherLength, QByteArray buffer, long long socketDescriptor)
{
    //qDebug() << "reached 'createEntityAndSafeToDatabase' function";

    int userID = getUserIDFromSocketList(socketDescriptor);

    QString message = krypter->decrypt(buffer, cipherLength);
    //qDebug()<<"Message: " + message;
    QStringList list = message.split(",");

    MessageHeader result=MessageHeader::DoNothing;


    switch (entityType)
    {
        case MessageHeader::AppointmentEnt:
        {
        //qDebug()<< "create AppointmentEntity";
           AppointmentEntity ent;
           ent.setProperties(list[0], list[1], list[2], list[3], list[4]);
           if(saveAppointmentInDb(ent, QString::number(userID))){
               qDebug()<<"Appointment saved";
               result=MessageHeader::AppointmentSaved;
           } else{
               qDebug()<<"Appointment not saved";
               result = MessageHeader::AppointmentNotSaved;
           }
           break;
        }
        case MessageHeader::DoctorEnt:
        {
        //qDebug()<<"create DoctorEntity";
            DoctorEntity ent;
            ent.setProperties(list[0], list[1], list[2], list[3], list[4], list[5]);
            if(saveDoctorInDb(ent, QString::number(userID))){
                qDebug()<<"Doc saved";
                result=MessageHeader::DoctorSaved;
            } else{
                qDebug()<<"Doc not saved";
                result = MessageHeader::DoctorNotSaved;
            }
            break;
        }

        case MessageHeader::UserEnt:
        {
            User user;
            user.setPropertiesAsEntity(list);
            if(saveUserInDb(user)){
                qDebug()<<"New User saved";
                result=MessageHeader::UserSaved;
            } else{
                qDebug()<<"New User not saved";
                result = MessageHeader::UserNotSaved;
            }
        break;
        }

        default:
            qDebug()<<"No entity type";
            break;
    }

    return result;
}

void MainWindow::returnMessage(int entityType, int cipherLength, QByteArray buffer, long long socketDescriptor)
{
    QString message = krypter->decrypt(buffer, cipherLength);
    QStringList list = message.split(",");

    QTcpSocket *receiver = getSocket(socketDescriptor);

    std::vector<std::shared_ptr<Entity>> emptyVec;
    emptyVec.push_back(std::make_shared<DoctorEntity>());

    QByteArray header;

    switch (entityType) {
    case MessageHeader::AppointmentEnt:
    {
        auto message = selectAppointmentsFromDatabase(list[0]);
        header.append(QString::number(MessageHeader::returnMessage).toUtf8()+ ",");
        header.append(QString::number(MessageHeader::AppointmentEnt).toUtf8()+ ",");
        sendMessage(receiver,message,header);
        break;
    }
    case MessageHeader::DoctorEnt:
    {
        auto message = selectDoctorsFromDatabase(list[0]);
        header.append(QString::number(MessageHeader::returnMessage).toUtf8()+ ",");
        header.append(QString::number(MessageHeader::DoctorEnt).toUtf8()+ ",");
        sendMessage(receiver,message,header);
        break;
    }
    case MessageHeader::DoctorSaved:
    case MessageHeader::DoctorNotSaved:
    case MessageHeader::AppointmentSaved:
    case MessageHeader::AppointmentNotSaved:
    case MessageHeader::UserSaved:
    case MessageHeader::UserNotSaved:
        header.append(QString::number(MessageHeader::ReturnInformation).toUtf8() + ",");
        header.append(QString::number(entityType).toUtf8() + ",");
        sendMessage(receiver,emptyVec,header);
        break;
    default:
        break;
    }
}

void MainWindow::sendMessage(QTcpSocket *socket, std::vector<std::shared_ptr<Entity>> entities, QByteArray header)
{
    QString buffer;

    for(const auto &currEntity:entities)
    {
        buffer.append(currEntity->getPropertiesAsString() + ";");
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

    if(socket->open(QIODevice::ReadWrite))
    {

        if(socket->isOpen())
        {
            QDataStream socketStream(socket);
            socketStream.setVersion(QDataStream::Qt_6_3);

            socketStream << message;

        }
    }

    socket->flush();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));



}

std::vector<std::shared_ptr<Entity>> MainWindow::selectAppointmentsFromDatabase(QString userID)
{
    //db.selectDateTest();
    return db.selectAppointmentsFromDatabase(userID);
}

std::vector<std::shared_ptr<Entity> > MainWindow::selectDoctorsFromDatabase(QString user_id)
{
    return db.selectDoctorsFromDatabase(user_id);
}

QString MainWindow::getPasswordFromUser(User user)
{
    return db.getPasswordFromUser(user);
}

int MainWindow::findUserInDatabase(User user)
{
    return db.findUserInDb(user);
}

void MainWindow::setLoginStateInDB(QString userID, bool loginState)
{
    db.setLoginStateInDb(userID,loginState);
}

QTcpSocket* MainWindow::getSocket(long long socketDescriptor)
{
    QTcpSocket *receiver;
    for(connections w : m_connectionSet)
    {
        if(w.m_connection->socketDescriptor() == socketDescriptor)
        {
            receiver = w.m_connection;
            return receiver;
        }

    }

    return nullptr;
}

void MainWindow::mapUserToSocket(long long socketDescriptor, int userID)
{
    for(unsigned long i = 0; i < m_connectionSet.size(); i++)
    {
        if(m_connectionSet[i].m_connection->socketDescriptor() == socketDescriptor)
            m_connectionSet[i].userID = userID;
    }
}

int MainWindow::getUserIDFromSocketList(long long socketDescriptor)
{
    for(unsigned long i = 0; i < m_connectionSet.size(); i++)
    {
        if(socketDescriptor == m_connectionSet[i].m_connection->socketDescriptor())
            return m_connectionSet[i].userID;
    }

    return 0;
}


void MainWindow::on_EMailTest_clicked()
{
    eMailClient->sendResetEMail();
}

