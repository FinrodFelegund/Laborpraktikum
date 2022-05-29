QT       += core gui sql
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

LIBS +=  -L/usr/local/Cellar/openssl@3/3.0.3/lib -lssl -lcrypto
INCLUDEPATH += /usr/local/Cellar/openssl@3/3.0.3/include

SOURCES += \
    appointment.cpp \
    client.cpp \
    doctors.cpp \
    OpeningScreen/login.cpp \
    main.cpp \
    mainwindow.cpp \
    OpeningScreen/openingscreen.cpp \
    OpeningScreen/openingmodel.cpp \
    reportscreen.cpp \
    ../../header.cpp \
    OpeningScreen/signup.cpp

HEADERS += \
    appointment.h \
    client.h \
    doctors.h \
    OpeningScreen/login.h \
    mainwindow.h \
    OpeningScreen/openingscreen.h \
    OpeningScreen/openingmodel.h \
    reportscreen.h \
    ../../header.h \
    OpeningScreen/signup.h

FORMS += \
    appointment.ui \
    doctors.ui \
    OpeningScreen/login.ui \
    mainwindow.ui \
    OpeningScreen/openingscreen.ui \
    reportscreen.ui \
    OpeningScreen/signup.ui



# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


