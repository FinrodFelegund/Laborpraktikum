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

void ApplicationModel::addDoc(int doc_id, QString name)
{
    docMap[doc_id]=name;
}

void ApplicationModel::prepareAppointmentEntMessage()
{
    QByteArray header;
    int messageType = MessageHeader::returnMessage;
    int messageEntity = MessageHeader::AppointmentEnt;

    header.prepend(QString::number(messageEntity).toUtf8() + ",");
    header.prepend(QString::number(messageType).toUtf8() + ",");

    emit sendMessage(header,"");
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
    connect(m_doctor_overview, &DoctorOverview::docInfo,this,&ApplicationModel::addDoc);
    connect(m_doctor_overview, &DoctorOverview::allDocsLoaded,this, [this](){
        m_appointment_timeline->reciveDocMap(docMap);
    });
    connect(m_appointment_timeline, &AppointmentTimeline::getAllAppointments,this, &ApplicationModel::prepareAppointmentEntMessage);
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

