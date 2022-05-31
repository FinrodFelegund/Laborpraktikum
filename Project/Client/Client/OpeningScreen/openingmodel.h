#ifndef OPENINGMODEL_H
#define OPENINGMODEL_H

#include <QObject>
#include "openingscreen.h"
class User;

class OpeningModel : public QObject
{
    Q_OBJECT

public:
    OpeningModel();
    ~OpeningModel();
    void loginGui2Model();
    void signupGui2Model();
    void setGui(OpeningScreen *screen);
    void connectGui();


signals:
    void messageCreated(QByteArray header, QString message);
    void sendLoginProgress(QString progress);
    void currentUser(User* currUser);
    void showMainWindows();

public slots:
    void receiveMessage(QString buffer, int messageType);
    void sendLoginRequest();
    void sendSignUpRequest();
    void sendPasswordRequest();


private:
    User *m_user = nullptr;
    OpeningScreen *screen = nullptr;


};

#endif // OPENINGMODEL_H
