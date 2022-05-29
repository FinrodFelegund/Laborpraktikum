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

signals:
    void showLogin();

private slots:
    void on_pushButton_back2Login_clicked();

private:
    Ui::Signup *ui;
};

#endif // SIGNUP_H
