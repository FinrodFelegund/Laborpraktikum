#include "applicationmodel.h"
#include "../../header.h"

ApplicationModel::ApplicationModel(QObject *parent)
    : QObject{parent}
{

}

void ApplicationModel::receiveMessage(QString buffer, int messageType)
{
    switch(messageType)
    {
        case MessageHeader::logoutRequest:
        {

            QStringList list = buffer.split(",");
            int logoutProgess = list[0].toInt();
            if(logoutProgess)
            {
                 //clearGui()
                emit sendApplicationProgress("Logout successful!");
                emit logoutUser();
            } else
            {
                emit sendApplicationProgress("Logout not successful");
            }
        }
    }
}


