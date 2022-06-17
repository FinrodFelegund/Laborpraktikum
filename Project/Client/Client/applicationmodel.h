#ifndef APPLICATIONMODEL_H
#define APPLICATIONMODEL_H

#include <QObject>
#include "appointment.h"
#include "appointmenttimeline.h"
#include "doctors.h"
#include "doctoroverview.h"

class DoctorEntity;
class AppointmentEntity;

class ApplicationModel : public QObject
{
    Q_OBJECT
public:
    explicit ApplicationModel(QObject *parent = nullptr);



    Appointment *getAppointmentGui() const;
    void setAppointment(Appointment *newAppointment);

    AppointmentTimeline *getAppointment_timelineGui() const;
    void setAppointment_timeline(AppointmentTimeline *newAppointment_timeline);

    Doctors *getDoctors() const;
    void setDoctors(Doctors *newDoctors);

    DoctorOverview *getDoctor_overviewGui() const;
    void setDoctor_overview(DoctorOverview *newDoctor_overview);

    void connectGui();

    void sendDeleteRequest();

    void doctorstToGui();

    void appointmentGuiToModel();

signals:
    void sendApplicationProgress(QString progress);
    void logoutUser();
    void sendMessage(QByteArray header, QString message);
    void getDoctorsFromServer(QByteArray header, QString buffer);


public slots:
    void logoutRequest(QString buffer);
    void deleteRequest(QString buf);
    void getReturnedDoctors(QString doctors);
    void getReturnedAppointments(QString appointments);

    void setDoctorsForAppointmentGui(std::vector<std::pair<int, QString>>doctorMap);



private slots:
    void sendDoctorToServer();
    void sendAppointmentToServer();
    void prepareAppointmentEntMessage();

private:
    int getDocIDFromDocMap(QString name);
    Appointment *m_appointment;
    AppointmentTimeline *m_appointment_timeline;

    Doctors *m_doctors;
    DoctorOverview * m_doctor_overview;

    AppointmentEntity *m_appointmentEntity;
    DoctorEntity *m_doctorEntity;



   std::vector<std::pair<int, QString>>doctorMap;


};


#endif // APPLICATIONMODEL_H
