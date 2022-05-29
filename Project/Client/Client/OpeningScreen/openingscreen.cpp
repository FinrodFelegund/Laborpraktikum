#include "openingscreen.h"
#include "ui_openingscreen.h"

OpeningScreen::OpeningScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OpeningScreen)
{
    ui->setupUi(this);
    this->m_login = new Login();
    this->m_signup = new Signup();

    ui->OpeningStackedWidget->insertWidget(0, m_login);
    ui->OpeningStackedWidget->insertWidget(1,m_signup);

    ui->OpeningStackedWidget->setCurrentWidget(m_login);

    connect(m_login, &Login::showSignUp, this, &OpeningScreen::changeToSignUp);
    connect(m_signup, &Signup::showLogin, this, &OpeningScreen::changeToLogin);

}


OpeningScreen::~OpeningScreen()
{
    delete ui;
}

void OpeningScreen::changeToSignUp()
{
    ui->OpeningStackedWidget->setCurrentWidget(m_signup);
}

void OpeningScreen::changeToLogin()
{
    ui->OpeningStackedWidget->setCurrentWidget(m_login);
}

Login* OpeningScreen::getLogin()
{
    return m_login;
}

Signup* OpeningScreen::getSignUp()
{
    return m_signup;
}


