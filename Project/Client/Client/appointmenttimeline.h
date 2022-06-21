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
    QString getNameFromDocMap(int id);

public slots:
    void setAppointmentVector(QString appointments);
    void setDocMap(std::vector<std::pair<int, QString>>doctorMap);
    void updatePage();

signals:
    void getAllAppointments();

private:
    void showList();

    Ui::AppointmentTimeline *ui;
    std::vector<std::shared_ptr<AppointmentEntity>> appEntVector;
    std::vector<std::pair<int, QString>>doctorMap;
};

#endif // APPOINTMENTTIMELINE_H
