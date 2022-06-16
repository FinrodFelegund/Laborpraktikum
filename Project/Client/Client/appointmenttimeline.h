#ifndef APPOINTMENTTIMELINE_H
#define APPOINTMENTTIMELINE_H

#include <QWidget>
//#include "../../header.h"

class AppointmentEntity;
namespace Ui {
class AppointmentTimeline;
}

class AppointmentTimeline : public QWidget
{
    Q_OBJECT

public:
    explicit AppointmentTimeline(QWidget *parent = nullptr);
    ~AppointmentTimeline();

public slots:
    void setAppointmentVector(QString appointments);
    void reciveDocMap(std::map<int, QString> map);

signals:
    void getAllAppointments();

private:
    void showList();

    Ui::AppointmentTimeline *ui;
    std::vector<std::shared_ptr<AppointmentEntity>> appEntVector;
    std::map<int, QString> docMap;
};

#endif // APPOINTMENTTIMELINE_H
