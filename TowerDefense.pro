#-------------------------------------------------
#
# Project created by QtCreator 2024-05-23T16:47:31
#
#-------------------------------------------------

QT       += core gui
QT += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TowerDefense
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    levelselector.cpp \
    gamescene.cpp \
    waypoint.cpp \
    towerposition.cpp \
    meleetower.cpp \
    rangedtower.cpp \
    tower.cpp \
    enemy.cpp \
    modifier.cpp \
    mapdata.cpp \
    enemyplus.cpp \
    item.cpp

HEADERS += \
        mainwindow.h \
    levelselector.h \
    gamescene.h \
    waypoint.h \
    towerposition.h \
    tower.h \
    meleetower.h \
    rangedtower.h \
    modifier.h \
    enemy.h \
    mapdata.h \
    enemyplus.h \
    item.h

FORMS += \
        mainwindow.ui \
    levelselector.ui \
    gamescene.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    img.qrc
