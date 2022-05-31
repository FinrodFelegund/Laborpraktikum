#include "login.h"
#include "ui_login.h"
#include "../../header.h"

Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
}

Login::~Login()
{
    delete ui;
}




void Login::on_pushButtonShowSignUp_clicked()
{
    clearGui();
    emit showSignUp();
}

void Login::clearGui()
{
    ui->lineEdit_email->clear();
    ui->lineEdit_password->clear();
}


void Login::on_pushButton_Login_clicked()
{
    if(ui->lineEdit_password->text().isEmpty() && ui->lineEdit_email->text().isEmpty())
        return;

    password.clear();
    eMail.clear();
    password = ui->lineEdit_password->text();
    eMail = ui->lineEdit_email->text();

    emit loginUser();
}

QString Login::getEmail() const
{
    return eMail;
}

QString Login::getPassword() const
{
    return password;
}

void Login::on_pushButton_ForgotPassword_clicked()
{
    emit passwordRequest();
}

bool Login::createPasswordRequestDialog(User *user)
{
    eMail.clear();

    if(!ui->lineEdit_email->text().isEmpty())
        eMail = ui->lineEdit_email->text();

    QString tmpEmail = QInputDialog::getText(0 , "Input Dialog", "Send Password to this Email:", QLineEdit::Normal, eMail);

    if(!tmpEmail.isEmpty())
    {
        user->setProperties("", tmpEmail, "");
        return true;
    } else
    {
        return false;
    }

}

