#ifndef DOCTOROVERVIEW_H
#define DOCTOROVERVIEW_H

#include <QWidget>

namespace Ui {
class DoctorOverview;
}

class DoctorOverview : public QWidget
{
    Q_OBJECT

public:
    explicit DoctorOverview(QWidget *parent = nullptr);
    ~DoctorOverview();

public slots:
    void setDoctorText(QString doctors);
signals:
    void getAllDoctors(QByteArray header, QString message);

private slots:
    void on_loadDocsButton_clicked();

private:
    Ui::DoctorOverview *ui;
};

#endif // DOCTOROVERVIEW_H
