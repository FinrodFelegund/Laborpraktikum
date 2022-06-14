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

bool checkForStrongPassword(QString password)
{
    std::string buf = password.toStdString();
    int n = buf.size();

    if(n < 9)
        return false;

    bool hasLower = false, hasUpper = false, hasDigit = false, hasSpecial = false;

    std::string normalChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890 ";

    for(int i = 0; i < n; i++)
    {
        if(islower(buf[i]))
            hasLower = true;
        if(isupper(buf[i]))
            hasUpper = true;
        if(isdigit(buf[i]))
            hasDigit = true;
        const unsigned long special = buf.find_first_not_of(normalChars);
        if(special != std::string::npos)
            hasSpecial = true;

    }


    if(hasLower && hasUpper && hasDigit && hasSpecial)
        return true;

    return false;
}
