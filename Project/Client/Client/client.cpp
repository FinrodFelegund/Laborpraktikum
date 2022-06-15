#include "client.h"
#include "../../header.h"
#include <QMessageBox>

Client::Client()
{

    m_timer = new QTimer(this);
    m_clientSocket = new QTcpSocket();
    connect(m_clientSocket, &QTcpSocket::readyRead, this, &Client::readSocket);
    //connect(m_clientSocket, &QTcpSocket::disconnected, this, &Client::discardSocket);
    connect(m_clientSocket, &QAbstractSocket::errorOccurred, this, &Client::displayError);
    connect(m_clientSocket, &QTcpSocket::disconnected, this, &Client::attemtingToRecconect);

    m_clientSocket->connectToHost(QHostAddress::LocalHost,8080);

    if(m_clientSocket->waitForConnected())
         qDebug() << "Connected to Server";
    else{
         qDebug() << "The following error occurred: " << m_clientSocket->errorString();
         exit(EXIT_FAILURE);
        }

    krypter = new Krypter;

    m_user=new User();
    m_user->setProperties("0","NoEmail","NoPasswort");
}

Client::~Client()
{
    if(m_clientSocket->isOpen())
        m_clientSocket->close();

    if(krypter)
        delete krypter;


}

void Client::attemtingToRecconect()
{

    if(m_timeout)
    {

        connect(m_timer, &QTimer::timeout, this, &Client::reconnectingToServer);
        m_timer->start(1000);
    }
}

void Client::endTimer()
{
    m_timer->stop();
}

void Client::reconnectingToServer()
{
    m_clientSocket->connectToHost(QHostAddress::LocalHost, 8080);
    if(m_clientSocket->waitForConnected())
    {
            qDebug() << "reconnected";
            this->endTimer();
            emit sendProgress("Reconnecting successful");
            m_timeout = true;
            m_Time = 0;
    } else
    {
        qDebug() << "reconnecting";
        m_Time++;
        emit sendProgress("Trying to reconnect to Server");
        if(m_Time  == 20) //Time is over
        {
            m_Time = 0;
            m_timer->stop();
            QMessageBox box;
            box.setText("Connecting was unsuccessful. Please restart the application to a differeent time.");
            box.exec();
            emit closeApplication();
        }
    }
}

void Client::endApplication()
{
    this->discardSocket();
    m_timeout = false;
}

void Client::discardSocket()
{
    m_clientSocket->deleteLater();
    m_clientSocket = nullptr;

    qDebug() << "Disconnected";
}

void Client::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError){
        case QAbstractSocket::RemoteHostClosedError:
        break;

        case QAbstractSocket::HostNotFoundError:
        qDebug() <<  "The host was not found. Please check the host name and port settings.";
        break;

        case QAbstractSocket::ConnectionRefusedError:
        qDebug() << "The connection was refused by the peer. Make sure QTCPServer is running, and check that the host name and port settings are correct.";
        break;

        default:
        qDebug() << "The following error occurred: " << m_clientSocket->errorString();
        break;
    }
}


void Client::readSocket()
{
    //here we must interpret the header and create an entity accordingly. Appointment, Report, etc... or just confirm if an action performed was successful
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
    processNewMessage(header, buffer);
}

void Client::setCurrentUser(User *currUser)
{
    m_user= currUser;
}


void Client::sendMessage(QByteArray header, QString message)
{

    message += m_user->getUID()+","; //Add User ID here for safty so all messages have the same ID

    int length = 0;
    QByteArray messageToSend = krypter->encrypt(message, &length);



    //last entry of header consists of cipher length
    header.append(QString::number(length).toUtf8() + ",");
    header.resize(128);

    messageToSend.prepend(header);

    if(m_clientSocket)
    {
        if(m_clientSocket->isOpen())
        {
            QDataStream socketStream(m_clientSocket);
            socketStream.setVersion(QDataStream::Qt_6_3);

            socketStream << messageToSend;
        }
    }
}

void Client::processNewMessage(QString header, QByteArray buffer)
{
    qDebug() << "reached processNewMessage";
    //qDebug() << header;
    QStringList headerSplit = header.split(",");

    int messageType = headerSplit[0].toInt();
    int entityType = headerSplit[1].toInt();
    int cipherLength = headerSplit[2].toInt();

    qDebug() << messageType << " " << entityType << " " << cipherLength;

    buffer = buffer.mid(128);

    switch(entityType)
    {
    case MessageHeader::UserEnt:
    {
            if(messageType == MessageHeader::loginRequest || messageType == MessageHeader::signUpRequest)
            {
                QString buf = krypter->decrypt(buffer, cipherLength);
                qDebug() << buf;
                emit pendingOpeningRequest(buf, messageType);
                break;
            }

            if(messageType == MessageHeader::logoutRequest)
            {
                QString buf = krypter->decrypt(buffer, cipherLength);
                qDebug() << "0 : error, 1: success; " << buf;
                emit pendingLogoutRequest(buf, messageType);
                break;
            }

        break;
    }

    case MessageHeader::AppointmentEnt:
    {
        QString buf = krypter->decrypt(buffer, cipherLength);
        qDebug() << buf;
        emit returnAppointments(buf);
        break;
    }
    case MessageHeader::DoctorEnt:
    {
        QString buf = krypter->decrypt(buffer, cipherLength);
        qDebug() << buf;
        emit returnDoctors(buf);
        break;
    }
    case MessageHeader::DoctorSaved:
      QMessageBox::information(nullptr, "Arzt wurde gespeichert!","Der Arzt wurde erfolgreich gespeichert.");
        break;
    case MessageHeader::DoctorNotSaved:
        QMessageBox::warning(nullptr,"Arzt wurde nicht gespeichert!","Der Arzt konnte nicht in der Datenbank gespeichert werden.");
        break;
    case MessageHeader::AppointmentSaved:
        QMessageBox::information(nullptr, "Termin wurde gespeichert!","Der Termin wurde erfolgreich gespeichert.");
        break;
    case MessageHeader::AppointmentNotSaved:
        QMessageBox::warning(nullptr,"Termin wurde nicht gespeichert!","Der Termin konnte nicht in der Datenbank gespeichert werden.");
        break;



    }
}

void Client::sendLogoutRequest()
{
    QByteArray header;
    header.append(QString::number(MessageHeader::logoutRequest).toUtf8() + ",");
    header.append(QString::number(MessageHeader::UserEnt).toUtf8() + ",");
    header.append(QString::number(0).toUtf8() + ",");
    QString buffer;

    sendMessage(header, buffer);
}





