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
    void sendLoginRequest();

signals:
    void messageCreated(QByteArray header, QString message);

public slots:
    void receiveMessage();


private:
    User *m_user = nullptr;
    OpeningScreen *screen = nullptr;


};

#endif // OPENINGMODEL_H
