#-------------------------------------------------
#
# Project created by QtCreator 2016-10-25T19:47:35
#
#-------------------------------------------------

QT       += core gui
QT       += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtXMLEditor
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    explorer.cpp

HEADERS  += dialog.h \
    explorer.h

FORMS    += dialog.ui \
    explorer.ui

RESOURCES += \
    myresources.qrc
