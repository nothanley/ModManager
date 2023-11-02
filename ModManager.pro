QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ModManager
TEMPLATE = app

SOURCES += main.cpp\
    Widgets/BurgerMenu/burgermenu.cpp \
    Widgets/GameManager/gamemanagerform.cpp \
    WindowHandler/qwinwidget.cpp \
    WindowHandler/winnativewindow.cpp \
    mainwindow.cpp \
    qtgameutils.cpp

HEADERS  += \
    Widgets/BurgerMenu/burgermenu.h \
    Widgets/GameManager/gamemanagerform.h \
    WindowHandler/qwinwidget.h \
    WindowHandler/winnativewindow.h \
    mainwindow.h \
    qtgameutils.hpp

LIBS += -L"C:\Program Files\Microsoft SDKs\Windows\v7.1\Lib" \
        -ldwmapi \
        -lgdi32

FORMS += \
    Widgets/GameManager/gamemanagerform.ui \
    mainwindow.ui

RESOURCES += \
    Widgets/BurgerMenu/burgermenu.qrc \
    WindowHandler/WindowHandler.qrc

CONFIG+=c++2a
DEFINES -= UNICODE
DEFINES += _MBCS
