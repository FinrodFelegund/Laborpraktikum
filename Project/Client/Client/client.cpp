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
    //here we must interpret the header and create an entity accordingly. Appointment, Report, etc...
}

void Client::sendMessage(QByteArray header, QString message)
{
    qDebug() << "Came this far";
    qDebug();
    message += QString::number(my_user_id)+";"; //Add User ID here for safty so all messages have the same ID

    qDebug() << message;

    int length = 0;
    QByteArray messageToSend = encrypt(message, &length);

    //QString decyphered = decrypt(messageToSend, length);

    //qDebug() << decyphered;


    //last entry of header consists of cipher length
    header.append(QString::number(length).toUtf8() + ",");
    header.resize(128);

    messageToSend.prepend(header);

    if(m_clientSocket)
    {
        if(m_clientSocket->isOpen())
        {
            QDataStream socketStream(m_clientSocket);
            socketStream.setVersion(QDataStream::Qt_5_15);

            socketStream << messageToSend;
        }
    }
}


QString Client::decrypt(QByteArray buffer, int cipherLength)
{
    unsigned char buf[300];
    unsigned char plainText[300];
    //qDebug() << "Server Utf8 encoded: " << buffer;
    //qDebug() << "Cipher Length in decryption: " << cipherLength;
    QString cipher = QString::fromUtf8(buffer);

    for(int i = 0; i < cipherLength; i++)
    {
        buf[i] = cipher[i].toLatin1();
    }


    krypter->decrypt(buf, cipherLength, plainText);

    QString message = QString::fromUtf8((char*)plainText);


    return message;


}

QByteArray Client::encrypt(QString buffer, int* cipherLength)
{
    std::string sequence = buffer.toStdString();
    unsigned char *text = (unsigned char*)sequence.c_str();
    int textLength = strlen((const char*)text);
    unsigned char cipherText[300];

    *cipherLength = krypter->encrypt(text, textLength, cipherText);

    QByteArray returnMessage;

    buffer.clear();
    for(int i = 0; i < *cipherLength; i++){
        buffer.append((const char) cipherText[i]);
    }


    returnMessage = buffer.toUtf8();


    //qDebug() << "Client Utf8 encoded " <<returnMessage;
    //qDebug() << "Cipher Length in encryption: " << *cipherLength;

    return returnMessage;
}


