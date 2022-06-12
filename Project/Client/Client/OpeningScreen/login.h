#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QInputDialog>

class User;

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
    bool createPasswordRequestDialog(User *user);
    void clearGui();

signals:
    void showSignUp();
    void loginUser();
    void passwordRequest();

private slots:

    void on_pushButtonShowSignUp_clicked();

    void on_pushButton_Login_clicked();

    void on_pushButton_ForgotPassword_clicked();

private:
    Ui::Login *ui;

    QString eMail;
    QString password;

};

#endif // LOGIN_H
