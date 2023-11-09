QT       += core gui
QT += svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ModManager
TEMPLATE = app

SOURCES += main.cpp\
    PackageManager/Manager/GameManager.cpp \
    PackageManager/Manager/GamePackage.cpp \
    PackageManager/Manager/GameProfile.cpp \
    PackageManager/Manager/ManagerController.cpp \
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
    PackageManager/Manager/GameManager.h \
    PackageManager/Manager/GamePackage.h \
    PackageManager/Manager/GameProfile.h \
    PackageManager/Manager/ManagerController.h \
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

win32:RC_ICONS += WindowHandler/icons/logo_main_icon.ico

CONFIG+=c++2a
DEFINES -= UNICODE
DEFINES += _MBCS

