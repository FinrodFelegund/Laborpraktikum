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

    QString buffer = m_user->getPropertiesAsString().toUtf8();

    emit messageCreated(header, buffer);


}

void OpeningModel::receiveMessage(QString buffer, int messageType)
{
    QStringList list = buffer.split(",");
    m_user->setPropertiesAsEntity(list);

    switch(messageType)
    {
        case MessageHeader::loginRequest:
        {

            if(m_user->getUID().toInt() != 0)
            {
                emit sendLoginProgress(true);
                //emit leaveLoginAndSetUserID(m_user->getUID());
            }
            else if (m_user->getUID().toInt() == 0)
            {
                emit sendLoginProgress(false);
            }





        }
    }
}

