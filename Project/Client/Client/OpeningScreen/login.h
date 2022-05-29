#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>

namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();
    QString getEmail() const;
    QString getPassword() const;


signals:
    void showSignUp();
    void loginUser();

private slots:

    void on_pushButtonShowSignUp_clicked();

    void on_pushButton_Login_clicked();

private:
    Ui::Login *ui;
    void clearGui();
    QString eMail;
    QString password;

};

#endif // LOGIN_H
