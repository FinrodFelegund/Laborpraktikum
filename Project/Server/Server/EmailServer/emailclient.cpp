#include "emailclient.h"
#include "../../header.h"


EMailClient::EMailClient()
{

}

EMailClient::~EMailClient()
{

}

bool EMailClient::sendEmail(User user, QString eMailEncrypted)
{

    user.print();
    MimeMessage message;
    EmailAddress adresseSender("patientenakteoth@gmail.com", "Daniel Pietsch");
    message.setSender(adresseSender);
    EmailAddress to(user.getEmail(), user.getEmail());
    message.addRecipient(to);

    message.setSubject("Your Password");

    // Now add some text to the email.
    // First we create a MimeText object.

    MimeText text;

    text.setText("Hi,\n Use this to reset your password:  http://localhost:8888/index.php?email="+ eMailEncrypted);

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

    smtp.quit();

    return true;
}



bool EMailClient::sendResetEMail()
{
        MimeMessage message;

        EmailAddress sender("patientenakteoth@gmail.com", "Daniel Pietsch");
        message.setSender(sender);

        EmailAddress to("daniel.pietsch@st.oth-regensburg.de", "RECIPIENT_NAME");
        message.addRecipient(to);

        message.setSubject("SmtpClient for Qt - Example 3 - Html email with images");

        // Now we need to create a MimeHtml object for HTML content
        MimeHtml html;

        html.setHtml("<html>"
                                    "<form>"
                                    "Password :"
                                    "<input type='password' name='password'>"
                                    "<br>"
                                    "repeat Password :"
                                    "<input type='password' name='passwordRepeat>"
                                     "</form>"
                                    "function othername() {"
                                    "var input = document.getElementById('password').value;"
                                    "alert(input);}"

                                    "</html>");

        message.addPart(&html);

        SmtpClient smtp("smtp.gmail.com", 465, SmtpClient::SslConnection);

            smtp.connectToHost();
            if (!smtp.waitForReadyConnected()) {
                qDebug() << "Failed to connect to host!";
                return -1;
            }

            smtp.login("patientenakteoth@gmail.com", "fgxtrxbtvoifbivp");
            if (!smtp.waitForAuthenticated()) {
                qDebug() << "Failed to login!";
                return -2;
            }

            smtp.sendMail(message);
            if (!smtp.waitForMailSent()) {
                qDebug() << "Failed to send mail!";
                return -3;
            }
            smtp.quit();


        return false;

}
