#include "openingmodel.h"
#include "../../header.h"

OpeningModel::OpeningModel()
{

    m_user = new User();
}

OpeningModel::~OpeningModel()
{

}


void OpeningModel::loginGui2Model()
{
    auto login = screen->getLogin();
    m_user->setProperties("", login->getEmail(), login->getPassword());
}

void OpeningModel::setGui(OpeningScreen *screen)
{
    this->screen = screen;

}

void OpeningModel::connectGui()
{
    auto login = screen->getLogin();
    connect(login, &Login::loginUser, this, &OpeningModel::sendLoginRequest);
    connect(login, &Login::passwordRequest, this, &OpeningModel::sendPasswordRequest);
    auto signUp = screen->getSignUp();
    connect(signUp, &Signup::signUpUser, this, &OpeningModel::sendSignUpRequest);

}


void OpeningModel::receiveMessage(QString buffer, int messageType)
{

    QStringList list = buffer.split(",");
    m_user->setPropertiesAsEntity(list);
    m_user->print();
    qDebug() << messageType;

    switch(messageType)
    {

        case MessageHeader::loginRequest:
        {
            auto login = screen->getLogin();
            login->clearGui();
            if(m_user->getUID().toInt() > 0)
            {

                login->clearGui();
                emit sendLoginProgress("Login successful");
                emit currentUser(m_user);
                emit showMainWindows();
                //emit leaveLoginAndSetUserID(m_user->getUID());
            }
            else if (m_user->getUID().toInt() <= 0)
            {

                emit sendLoginProgress("Login failed. User either logged in on other device or doesn't exist");
            }
            break;
        }

        case MessageHeader::signUpRequest:
        {
            //auto signup = screen->getSignUp();
            if(m_user->getUID().toInt() > 0)
            {

                emit sendLoginProgress("Sign up successful!");
            }
            else
            {
                emit sendLoginProgress("Sign up not successful. User already exists or Sign Up failed");
            }

            break;
        }

        case MessageHeader::passwortRequest:
        {
            if(m_user->getUID().toInt() == 1)
            {
                emit sendLoginProgress("A message with your password was sent to your EMail Adresse.");
            }
            else
            {
                emit sendLoginProgress("EMail could not be sent. Either your EMail was wrong or it does not exist");
            }
            break;
        }
    }
}

void OpeningModel::sendLoginRequest()
{
    //got the login credentials
    loginGui2Model();

    QByteArray header;
    int messageType = MessageHeader::loginRequest;
    int entityType = MessageHeader::UserEnt;
    header.prepend(QString::number(entityType).toUtf8() + ",");
    header.prepend(QString::number(messageType).toUtf8() + ",");

    QString buffer = m_user->getPropertiesAsString();

    emit messageCreated(header, buffer);


}

void OpeningModel::signupGui2Model()
{
    auto signup = screen->getSignUp();
    m_user->setProperties("", signup->getEmail(), signup->getPassword());
}

void OpeningModel::sendSignUpRequest()
{
    signupGui2Model();

    QString password = m_user->getPassword();
    bool ok = checkForStrongPassword(password);

    if(ok)
    {
        QByteArray header;
        int messageType = MessageHeader::signUpRequest;
        int entityType = MessageHeader::UserEnt;
        header.prepend(QString::number(entityType).toUtf8() + ",");
        header.prepend(QString::number(messageType).toUtf8() + ",");

        QString buffer = m_user->getPropertiesAsString();

        emit messageCreated(header, buffer);
    } else
    {
        sendLoginProgress("Password not strong enough. Has to containe one lower case letter, one upper case letter, one digit, a special character and at least 8 characters in size");
    }

}

void OpeningModel::sendPasswordRequest()
{

    loginGui2Model();
    auto login = screen->getLogin();
    bool exec = login->createPasswordRequestDialog(m_user);

    if(exec)
    {
        QByteArray header;
        int messageType = MessageHeader::passwortRequest;
        int entityType = MessageHeader::UserEnt;
        header.prepend(QString::number(entityType).toUtf8() + ",");
        header.prepend(QString::number(messageType).toUtf8() + ",");

        QString buffer = m_user->getPropertiesAsString();

        emit messageCreated(header, buffer);
    }
    else
    {
        emit sendLoginProgress("Please enter a valid EMail Adresse");
    }

}

bool OpeningModel::checkForStrongPassword(QString password)
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

