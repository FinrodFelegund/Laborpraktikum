#include "client.h"
#include "../../header.h"

Client::Client(): my_user_id(-1)
{

    m_clientSocket = new QTcpSocket();
    connect(m_clientSocket, &QTcpSocket::readyRead, this, &Client::readSocket);
    connect(m_clientSocket, &QTcpSocket::disconnected, this, &Client::discardSocket);
    connect(m_clientSocket, &QAbstractSocket::errorOccurred, this, &Client::displayError);

    m_clientSocket->connectToHost(QHostAddress::LocalHost,8080);

    if(m_clientSocket->waitForConnected())
         qDebug() << "Connected to Server";
    else{
         qDebug() << "The following error occurred: " << m_clientSocket->errorString();
         exit(EXIT_FAILURE);
        }

    krypter = new Krypter;

    my_user_id = 1; //has to be set after login
}

Client::~Client()
{
    if(m_clientSocket->isOpen())
        m_clientSocket->close();

    if(krypter)
        delete krypter;

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

void Client::sendMessage(QByteArray header, QString message)
{

    message += QString::number(my_user_id)+";"; //Add User ID here for safty so all messages have the same ID

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
    //qDebug() << "reached processNewMessage";
    //qDebug() << header;
    QStringList headerSplit = header.split(",");

    int messageType = headerSplit[0].toInt();
    int entityType = headerSplit[1].toInt();
    int cipherLength = headerSplit[2].toInt();

    //qDebug() << messageType << " " << entityType << " " << cipherLength;

    buffer = buffer.mid(128);

    switch(messageType)
    {
        case MessageHeader::loginRequest:
        {
            QString buf = krypter->decrypt(buffer, cipherLength);
            emit pendingLoginRequest(buf, messageType);

        }


    }
}





