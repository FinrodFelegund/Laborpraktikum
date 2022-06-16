#ifndef APPLICATIONMODEL_H
#define APPLICATIONMODEL_H

#include <QObject>
#include "appointment.h"
#include "appointmenttimeline.h"
#include "doctors.h"
#include "doctoroverview.h"

class ApplicationModel : public QObject
{
    Q_OBJECT
public:
    explicit ApplicationModel(QObject *parent = nullptr);

    Appointment *appointment() const;
    void setAppointment(Appointment *newAppointment);

    AppointmentTimeline *appointment_timeline() const;
    void setAppointment_timeline(AppointmentTimeline *newAppointment_timeline);

    Doctors *doctors() const;
    void setDoctors(Doctors *newDoctors);

    DoctorOverview *doctor_overview() const;
    void setDoctor_overview(DoctorOverview *newDoctor_overview);

    void connectGui();

signals:
    void sendApplicationProgress(QString progress);
    void logoutUser();
    void sendMessage(QByteArray header, QString message);

public slots:
    void logoutRequest(QString buffer);
    void getReturnedDoctors(QString doctors);
    void getReturnedAppointments(QString appointments);


private:
    Appointment *m_appointment;
    AppointmentTimeline *m_appointment_timeline;

    Doctors *m_doctors;
    DoctorOverview * m_doctor_overview;

};


#endif // APPLICATIONMODEL_H
