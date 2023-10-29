QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ModManager
TEMPLATE = app

SOURCES += main.cpp\
    Widgets/BurgerMenu/burgermenu.cpp \
    WindowHandler/qwinwidget.cpp \
    WindowHandler/winnativewindow.cpp \
    mainwindow.cpp

HEADERS  += \
    Widgets/BurgerMenu/burgermenu.h \
    WindowHandler/qwinwidget.h \
    WindowHandler/winnativewindow.h \
    mainwindow.h

LIBS += -L"C:\Program Files\Microsoft SDKs\Windows\v7.1\Lib" \
        -ldwmapi \
        -lgdi32

FORMS += \
    mainwindow.ui

RESOURCES += \
    burgermenu.qrc
