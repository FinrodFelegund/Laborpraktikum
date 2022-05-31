#ifndef SIGNUP_H
#define SIGNUP_H

#include <QWidget>

namespace Ui {
class Signup;
}

class Signup : public QWidget
{
    Q_OBJECT

public:
    explicit Signup(QWidget *parent = nullptr);
    ~Signup();
    void clearGui();
    QString getEmail() const;
    QString getPassword() const;

signals:
    void showLogin();
    void signUpUser();

private slots:
    void on_pushButton_back2Login_clicked();

    void on_pushButton_signUp_clicked();

private:
    Ui::Signup *ui;
    QString eMail;
    QString password;
};

#endif // SIGNUP_H
