#ifndef DATABASE_H
#define DATABASE_H

#include <QtSql>
#include "../../header.h"


class Database
{
public:
    Database();

    bool saveDoctorInDb(DoctorEntity ent, QString user_id);
    bool saveAppointmentInDb(AppointmentEntity ent, QString user_id);
    bool saveUserInDb(User user);
    void setUpDatabase(QString password);

    int findUserInDb(User user);
    QString getPasswordFromUser(User user);
    bool setLoginStateInDb(QString user_id, bool loginState);
    QString getEMailFromUser(User user);

    std::vector<std::shared_ptr<Entity>> selectAppointmentsFromDatabase(QString user_id);
    std::vector<std::shared_ptr<Entity>> selectDoctorsFromDatabase(QString user_id);

    bool selectDateTest();


private:
    QSqlDatabase db;
};

#endif // DATABASE_H
