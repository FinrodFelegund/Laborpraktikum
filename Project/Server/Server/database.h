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

    int findUserInDatabase(User user);
    QString getPasswordFromUser(User user);
    void setLoginStateInDB(QString userID, bool loginState);

private:
    QSqlDatabase db;
};

#endif // DATABASE_H
