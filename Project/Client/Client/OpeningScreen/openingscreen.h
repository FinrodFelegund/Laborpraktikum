#ifndef OPENINGSCREEN_H
#define OPENINGSCREEN_H

#include <QWidget>
#include "login.h"
#include "signup.h"

namespace Ui {
class OpeningScreen;
}

class OpeningScreen : public QWidget
{
    Q_OBJECT

public:
    explicit OpeningScreen(QWidget *parent = nullptr);
    ~OpeningScreen();

    Login* getLogin();
    Signup* getSignUp();




private slots:
    void changeToLogin();
    void changeToSignUp();

private:
    Ui::OpeningScreen *ui;
    Login *m_login;
    Signup *m_signup;
};

#endif // OPENINGSCREEN_H
