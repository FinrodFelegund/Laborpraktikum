#ifndef REPORTSCREEN_H
#define REPORTSCREEN_H

#include <QWidget>


namespace Ui {
class ReportScreen;
}

class ReportScreen : public QWidget
{
    Q_OBJECT

public:
    explicit ReportScreen(QWidget *parent = nullptr, QString userID = "1234");
    ~ReportScreen();

signals:
    void messageCreated(QByteArray message);

private:
    Ui::ReportScreen *ui;
    QString userID;
};

#endif // REPORTSCREEN_H
