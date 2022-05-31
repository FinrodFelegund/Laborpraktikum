#include "emailclient.h"
#include "../../header.h"

EMailClient::EMailClient()
{

}

EMailClient::~EMailClient()
{

}

bool EMailClient::sendEmail(User user)
{
    MimeMessage message;
    EmailAddress adresseSender("patientenakteoth@gmail.com", "Daniel Pietsch");
    message.setSender(adresseSender);
    EmailAddress to(user.getEmail(), user.getEmail());
    message.addRecipient(to);

    message.setSubject("Your Password");

    // Now add some text to the email.
    // First we create a MimeText object.

    MimeText text;

    text.setText("Hi,\nThis is your password: " + user.getPassword()+".");

    // Now add it to the mail

    message.addPart(&text);

    // Now we can send the mail
    SmtpClient smtp("smtp.gmail.com", 465, SmtpClient::SslConnection);

    smtp.connectToHost();
    if (!smtp.waitForReadyConnected()) {
        qDebug() << "Failed to connect to host!";
        return false;
    }

    smtp.login("patientenakteoth@gmail.com", "fgxtrxbtvoifbivp");
    if (!smtp.waitForAuthenticated()) {
        qDebug() << "Failed to login!";
        return false;
    }

    smtp.sendMail(message);
    if (!smtp.waitForMailSent()) {
        qDebug() << "Failed to send mail!";
        return false;
    }



    return true;
}
