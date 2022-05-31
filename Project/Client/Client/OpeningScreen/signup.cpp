#include "signup.h"
#include "ui_signup.h"

Signup::Signup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Signup)
{
    ui->setupUi(this);
}

Signup::~Signup()
{
    delete ui;
}

void Signup::on_pushButton_back2Login_clicked()
{
    clearGui();
    emit showLogin();
}

void Signup::clearGui()
{
    ui->lineEdit_email->clear();
    ui->lineEdit_password->clear();
}




void Signup::on_pushButton_signUp_clicked()
{
    if((ui->lineEdit_email->text().isEmpty() && ui->lineEdit_password->text().isEmpty()))
        return;

    eMail.clear();
    eMail = ui->lineEdit_email->text();
    password.clear();
    password = ui->lineEdit_password->text();
    emit signUpUser();

}

QString Signup::getEmail() const
{
    return eMail;
}

QString Signup::getPassword() const
{
    return password;
}



