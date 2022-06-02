#ifndef APPOINTMENTTIMELINE_H
#define APPOINTMENTTIMELINE_H

#include <QWidget>
#include "../../header.h"

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
signals:
    void getAllAppointments();


private:
    void showList();

    Ui::AppointmentTimeline *ui;
    std::vector<std::shared_ptr<AppointmentEntity>> appEntVector;
};

#endif // APPOINTMENTTIMELINE_H
