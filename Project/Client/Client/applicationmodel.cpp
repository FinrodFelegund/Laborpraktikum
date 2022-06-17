#include "applicationmodel.h"
#include "../../header.h"

ApplicationModel::ApplicationModel(QObject *parent)
    : QObject{parent}
{

    m_doctorEntity = new DoctorEntity();
    m_appointmentEntity = new AppointmentEntity();
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

void ApplicationModel::deleteRequest(QString buffer)
{
    QStringList list = buffer.split(",");
    int deleteProgress = list[0].toInt();
    if(deleteProgress)
    {
        emit sendApplicationProgress("Deleting Account successful");
        emit logoutUser();
    } else
    {
        emit sendApplicationProgress("Deleting Account not successful");
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



void ApplicationModel::prepareAppointmentEntMessage()
{
    QByteArray header;
    int messageType = MessageHeader::returnMessage;
    int messageEntity = MessageHeader::AppointmentEnt;

    header.prepend(QString::number(messageEntity).toUtf8() + ",");
    header.prepend(QString::number(messageType).toUtf8() + ",");

    emit sendMessage(header,"");
}

DoctorOverview *ApplicationModel::getDoctor_overviewGui() const
{
    return m_doctor_overview;
}

void ApplicationModel::setDoctor_overview(DoctorOverview *newDoctor_overview)
{
    m_doctor_overview = newDoctor_overview;
}



Doctors *ApplicationModel::getDoctors() const
{
    return m_doctors;
}

void ApplicationModel::setDoctors(Doctors *newDoctors)
{
    m_doctors = newDoctors;
}

AppointmentTimeline *ApplicationModel::getAppointment_timelineGui() const
{
    return m_appointment_timeline;
}

void ApplicationModel::setAppointment_timeline(AppointmentTimeline *newAppointment_timeline)
{
    m_appointment_timeline = newAppointment_timeline;
}

Appointment *ApplicationModel::getAppointmentGui() const
{
    return m_appointment;
}

void ApplicationModel::setAppointment(Appointment *newAppointment)
{
    m_appointment = newAppointment;
}

void ApplicationModel::sendDeleteRequest()
{
    QByteArray header;
    int messageType = MessageHeader::deleteUserRequest;
    int entityType = MessageHeader::UserEnt;


    QString buffer;

    header.append(QString::number(messageType).toUtf8() + ",");
    header.append(QString::number(entityType).toUtf8() + ",");

    qDebug() << "sending Delete request";
    emit sendMessage(header, buffer);

}

void ApplicationModel::connectGui()
{
    connect(m_doctors, &Doctors::sendDoctorEntity, this, &ApplicationModel::sendDoctorToServer);
    connect(m_doctor_overview, &DoctorOverview::allDocsLoaded, this, &ApplicationModel::setDoctorsForAppointmentGui);
    connect(m_appointment, &Appointment::sendAppointmentEntity, this, &ApplicationModel::sendAppointmentToServer);

}

void ApplicationModel::sendAppointmentToServer()
{
    appointmentGuiToModel();
    QString docName = m_appointmentEntity->getDoctorID();
    int docID = getDocIDFromDocMap(docName);
    m_appointmentEntity->setDocId(QString::number(docID));
    QString buffer = m_appointmentEntity->getPropertiesAsString();
    int messageType = MessageHeader::saveMessage;
    int entityType = MessageHeader::AppointmentEnt;
    QByteArray header;
    header.append(QString::number(messageType).toUtf8()+ ",");
    header.append(QString::number(entityType).toUtf8()+ ",");

    qDebug() << "sending appointments";
    emit sendMessage(header, buffer);
}

int ApplicationModel::getDocIDFromDocMap(QString name)
{
    for(unsigned long i = 0; i < doctorMap.size(); i++)
    {
        qDebug() << name << " " << doctorMap[i].second;
        if(name == doctorMap[i].second)
            return doctorMap[i].first;
    }

    qDebug() << "Doc ID not found";
    return -1;
}

void ApplicationModel::appointmentGuiToModel()
{
    auto appointment = getAppointmentGui();
    QStringList list = appointment->getGuiInput();
    m_appointmentEntity->setPropertiesAsEntity(list);

}

void ApplicationModel::setDoctorsForAppointmentGui(std::vector<std::pair<int, QString>>doctorMap)
{
    auto appointmentGui = getAppointmentGui();
    appointmentGui->setDocMap(doctorMap);
    this->doctorMap = doctorMap;

}



void ApplicationModel::sendDoctorToServer()
{
    doctorstToGui();
    QString buffer = m_doctorEntity->getPropertiesAsString();
    int messageType = MessageHeader::saveMessage;
    int entityType = MessageHeader::DoctorEnt;
    QByteArray header;
    header.append(QString::number(messageType).toUtf8()+ ",");
    header.append(QString::number(entityType).toUtf8()+ ",");

    qDebug() << "sending doctor";
    emit sendMessage(header, buffer);

}

void ApplicationModel::doctorstToGui()
{
    auto doctor = getDoctors();

    QStringList list = doctor->getGuiInput();
    m_doctorEntity->setPropertiesAsEntity(list);





}

