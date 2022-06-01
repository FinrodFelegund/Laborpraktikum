#include "database.h"

Database::Database()
{
    db =QSqlDatabase::addDatabase("QMYSQL","demoapp");
        db.setHostName("127.0.0.1");
        db.setDatabaseName("demoapp");
        db.setUserName("LPRApp");
        db.setPassword("DaCiHa22");
        if(!db.open()){
            qDebug() << db.lastError().text();
        } else{
            qDebug() <<"db loaded successfully!";
        }

}

bool Database::saveDoctorInDb(DoctorEntity ent, QString user_id)
{
    if(user_id.toInt() > -1){
        if(db.open()){

            QSqlQuery queryInsert(db);
                        queryInsert.prepare("insert into doctors(doctorname, street, streetnumber, city, plz, phone, uid) VALUES(?,?,?,?,?,?,?)");
                        queryInsert.bindValue(0,ent.getName());
                        queryInsert.bindValue(1,ent.getStreet());
                        queryInsert.bindValue(2,ent.getStreetNumber());
                        queryInsert.bindValue(3,ent.getCity());
                        queryInsert.bindValue(4,ent.getPostalCode());
                        queryInsert.bindValue(5,ent.getPhoneNumber());
                        queryInsert.bindValue(6,user_id);

                        bool executed = queryInsert.exec();
                        qDebug()<<executed;
                        QSqlError error= queryInsert.lastError();
                        std::cout<<error.databaseText().toUtf8().constData();
                        return executed;
        }

    }
    else{
        qDebug() << "User is not logged in";
    }


    return false;
}

bool Database::saveAppointmentInDb(AppointmentEntity ent, QString user_id)
{
    if(user_id.toInt() > -1){
        if(db.open()){

            QSqlQuery queryInsert(db);
                        queryInsert.prepare("insert into appointments(appdate,apptime,title,notes,did,uid) values(?,?,?,?,?,?)");
                        queryInsert.bindValue(0,QDate::fromString(ent.getDate()));
                        queryInsert.bindValue(1,QTime::fromString(ent.getTime()));
                        queryInsert.bindValue(2,ent.getTitle());
                        queryInsert.bindValue(3,ent.getText());
                        queryInsert.bindValue(4,ent.getDoctorID());
                        queryInsert.bindValue(5,user_id);

                        bool executed = queryInsert.exec();
                        qDebug()<<executed;
                        QSqlError error= queryInsert.lastError();
                        std::cout<<error.databaseText().toUtf8().constData();
                        return executed;
        }

    }
    else{
        qDebug() << "User is not logged in";
    }

    return false;
}

bool Database::saveUserInDb(User user)
{
    bool executed = false;
    int logedIn = 0;
    if(db.open())
    {
        QSqlQuery queryInsert(db);
        queryInsert.prepare("insert into Users(email, passwort, loginState) values(?, ?, ?)");
        //queryInsert.bindValue(0, logedIn);
        queryInsert.bindValue(0, user.getEmail());
        queryInsert.bindValue(1, user.getPassword());
        queryInsert.bindValue(2, logedIn);

        executed = queryInsert.exec();
        qDebug() << "In Save User In db: " << executed;
        QSqlError error = queryInsert.lastError();
        qDebug() << error;
        return executed;
    }

    return executed;
}

int Database::findUserInDatabase(User user)
{
    if(db.open())
    {
        QSqlQuery queryFind(db);
        queryFind.prepare("select uid, loginState from Users where email = '" +user.getEmail()+ "' "); // +password

        qDebug() << queryFind.exec();
        QSqlError error = queryFind.lastError();
        if(error.isValid())
            qDebug() << "In Find User Query: " << error.databaseText().toUtf8().constData();

        int userID = 0;
        int loginState = 0;
        queryFind.next();

        if(queryFind.isValid())
        {
            userID = queryFind.value(0).toInt();
            loginState = queryFind.value(1).toInt();
            return (loginState == 1 ? -1 : userID);

        } else
        {
            return 0;
        }


    }

    return 0;
}

QString Database::getPasswordFromUser(User user)
{
    QString password = "";
    if(db.open())
    {
        QSqlQuery queryFind(db);
        queryFind.prepare("select passwort from Users where email = '"+user.getEmail()+"' ");
        qDebug() << "Inf function getPasswordFromUser: " << queryFind.exec();

        QSqlError error = queryFind.lastError();
        qDebug() << error;

        queryFind.next();

        if(queryFind.isValid())
        {
            password = queryFind.value(0).toString();
        }
    }
    return password;
}

void Database::setLoginStateInDB(QString userID, bool loginState)
{
    if(db.open())
    {
        QSqlQuery queryAlter(db);
        queryAlter.prepare("update Users set loginState = '"+QString::number(loginState)+"' where uid = '"+userID+"'");
        qDebug() << "In Function SetLoginState in DB: " << queryAlter.exec();
        QSqlError error = queryAlter.lastError();
        qDebug() << error;
    }
}
