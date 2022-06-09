#ifndef EMAILCLIENT_H
#define EMAILCLIENT_H

#include <QObject>

#include "smtpclient.h"
#include "mimetext.h"
#include "mimehtml.h"


class User;

class EMailClient : public QObject
{
    Q_OBJECT
public:
    EMailClient();
    virtual ~EMailClient();
    bool sendEmail(User user);

    bool sendResetEMail();


private:
    QString m_Email = "pietsch-daniel1@gmx.de";
    QString m_password = "R25$NCq7p&";




};

#endif // EMAILCLIENT_H
