QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#LIBS +=  -L/usr/local/Cellar/openssl@3/3.0.3/lib -lssl -lcrypto #Dani
#INCLUDEPATH += /usr/local/Cellar/openssl@3/3.0.3/include/ #Dani

LIBS +=  -LC:\Users\User\Documents\LPR\Laborpraktikum\Project\openssl-3\x64\lib -lssl -lcrypto #Hannah
INCLUDEPATH += C:\Users\User\Documents\LPR\Laborpraktikum\Project\openssl-3\x64\include #Hannah


SOURCES += \
    main.cpp \
    mainwindow.cpp \
    ../../header.cpp

HEADERS += \
    mainwindow.h \
    ../../header.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
