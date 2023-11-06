QT       += core gui
QT += svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ModManager
TEMPLATE = app

SOURCES += main.cpp\
    Widgets/BurgerMenu/burgermenu.cpp \
    Widgets/GameManager/gamemanagerform.cpp \
    WindowHandler/qwinwidget.cpp \
    WindowHandler/winnativewindow.cpp \
    gamecard.cpp \
    gamestatstable.cpp \
    mainwindow.cpp \
    previewpanel.cpp \
    qtgameutils.cpp

HEADERS  += \
    Widgets/BurgerMenu/burgermenu.h \
    Widgets/GameManager/gamemanagerform.h \
    WindowHandler/qwinwidget.h \
    WindowHandler/winnativewindow.h \
    gamecard.h \
    gamestatstable.h \
    mainwindow.h \
    pixmaputils.hpp \
    previewpanel.h \
    qtgameutils.hpp

LIBS += -L"C:\Program Files\Microsoft SDKs\Windows\v7.1\Lib" \
        -ldwmapi \
        -lgdi32

FORMS += \
    Widgets/GameManager/gamemanagerform.ui \
    gamecard.ui \
    gamestatstable.ui \
    mainwindow.ui \
    previewpanel.ui

RESOURCES += \
    Widgets/BurgerMenu/burgermenu.qrc \
    WindowHandler/WindowHandler.qrc

CONFIG+=c++2a
DEFINES -= UNICODE
DEFINES += _MBCS

