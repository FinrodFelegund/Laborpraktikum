#include "applicationmodel.h"
#include "../../header.h"

ApplicationModel::ApplicationModel(QObject *parent)
    : QObject{parent}
{


}

void ApplicationModel::logoutRequest(QString buffer)
{
    QStringList list = buffer.split(",");
    int logoutProgess = list[0].toInt();
    if(logoutProgess){
        //clearGui()
        emit sendApplicationProgress("Logout successful!");
        emit logoutUser();
    } else{
        emit sendApplicationProgress("Logout not successful");
    }
}

void ApplicationModel::getReturnedDoctors(QString doctors)
{
    m_doctor_overview->setDoctorText(doctors);
}

void ApplicationModel::getReturnedAppointments(QString appointments)
{
    m_appointment_timeline->setAppointmentVector(appointments);
}

DoctorOverview *ApplicationModel::doctor_overview() const
{
    return m_doctor_overview;
}

void ApplicationModel::setDoctor_overview(DoctorOverview *newDoctor_overview)
{
    m_doctor_overview = newDoctor_overview;
}

void ApplicationModel::connectGui()
{
    connect(m_appointment, &Appointment::messageCreated, this, &ApplicationModel::sendMessage);
    connect(m_doctors, &Doctors::messageCreated, this, &ApplicationModel::sendMessage);
    connect(m_doctor_overview, &DoctorOverview::getAllDoctors,this, &ApplicationModel::sendMessage);

}

Doctors *ApplicationModel::doctors() const
{
    return m_doctors;
}

void ApplicationModel::setDoctors(Doctors *newDoctors)
{
    m_doctors = newDoctors;
}

AppointmentTimeline *ApplicationModel::appointment_timeline() const
{
    return m_appointment_timeline;
}

void ApplicationModel::setAppointment_timeline(AppointmentTimeline *newAppointment_timeline)
{
    m_appointment_timeline = newAppointment_timeline;
}

Appointment *ApplicationModel::appointment() const
{
    return m_appointment;
}

void ApplicationModel::setAppointment(Appointment *newAppointment)
{
    m_appointment = newAppointment;
}

