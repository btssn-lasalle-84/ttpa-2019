#-------------------------------------------------
#
# Project created by QtCreator 2019-02-28T10:30:52
#
#-------------------------------------------------

QT += core gui bluetooth

CONFIG += mobility c++11
MOBILITY = connectivity

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ecran-TTPA
TEMPLATE = app

#DEFINES += QT_NO_DEBUG_OUTPUT

SOURCES += main.cpp \
    ttpaihm.cpp \
    communicationbluetooth.cpp \
    statistiquesseance.cpp

HEADERS  += \
    ttpaihm.h \
    communicationbluetooth.h \
    statistiquesseance.h

FORMS    += \
    ttpaihm.ui

RESOURCES += \
    ressources.qrc

target.path = /home/pi
INSTALLS += target

DISTFILES +=
