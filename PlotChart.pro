#-------------------------------------------------
#
# Project created by QtCreator 2016-09-09T23:55:24
#
#-------------------------------------------------

QT       += core gui sql
PRECOMPILED_HEADER = stable.h
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = PlotChart
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp \
    resolutionratiodialog.cpp \
    CustomPlotZoom.cpp \
    customplotdrag.cpp \
    dbmanager.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h \
    resolutionratiodialog.h \
    CustomPlotZoom.h \
    customplotdrag.h \
    stable.h \
    dbmanager.h

FORMS    += mainwindow.ui \
    resolutionratiodialog.ui

RESOURCES += \
    icons.qrc
