#-------------------------------------------------
#
# Project created by QtCreator 2015-04-08T12:51:02
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = try
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    number.cpp

HEADERS  += dialog.h \
    number.h

FORMS    += dialog.ui

OTHER_FILES += \
    embedded.txt
