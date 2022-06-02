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

    int findUserInDb(User user);
    QString getPasswordFromUser(User user);
    void setLoginStateInDb(QString user_id, bool loginState);

    std::vector<std::shared_ptr<Entity>> selectAppointmentsFromDatabase(QString user_id);

private:
    QSqlDatabase db;
};

#endif // DATABASE_H
