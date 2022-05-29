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



