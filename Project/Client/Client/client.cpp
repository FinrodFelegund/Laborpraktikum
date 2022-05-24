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

    message += QString::number(my_user_id)+";"; //Add User ID here for safty so all messages have the same ID

    //lets try the krypto class
    std::string test = message.toStdString();
    unsigned char *mes = (unsigned char*) test.c_str();
    int text_len = strlen((const char*) mes);
    unsigned char cipher[300];



    int length = krypter->encrypt(mes, text_len, cipher);

    for(int i = 0; i < length; i++)
        printf("%d ", cipher[i]);

    //last entry of header consists of cipher length
    header.append(QString::number(length).toUtf8() + ",");
    header.resize(128);

    message.clear();
    for(int i = 0; i < length; i++)
    {
        message.append((const char)cipher[i]);
    }

    qDebug() << "Encrypted Message " << message << " Encryption Length in Client: " << length;



    QByteArray messageToSend;
    messageToSend = message.toUtf8();
    qDebug() << "Utf8 encoded: " << messageToSend;
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


