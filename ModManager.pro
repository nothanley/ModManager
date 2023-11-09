QT += core gui
QT += svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ModManager
TEMPLATE = app

SOURCES += src/main.cpp\
    src/PackageManager/Manager/GameManager.cpp \
    src/PackageManager/Manager/GamePackage.cpp \
    src/PackageManager/Manager/GameProfile.cpp \
    src/PackageManager/Manager/ManagerController.cpp \
    src/Widgets/additemdialog.cpp \
    src/Widgets/burgermenu.cpp \
    src/Widgets/gamemanagerform.cpp \
    src/Widgets/gamecard.cpp \
    src/Widgets/gamestatstable.cpp \
    src/Widgets/previewpanel.cpp \
    src/WindowHandler/qwinwidget.cpp \
    src/WindowHandler/winnativewindow.cpp \
    src/mainwindow.cpp \
    src/qtgameutils.cpp

HEADERS  += \
    src/PackageManager/Manager/GameManager.h \
    src/PackageManager/Manager/GamePackage.h \
    src/PackageManager/Manager/GameProfile.h \
    src/PackageManager/Manager/ManagerController.h \
    src/Widgets/additemdialog.h \
    src/Widgets/burgermenu.h \
    src/Widgets/gamemanagerform.h \
    src/Widgets/gamecard.h \
    src/Widgets/gamestatstable.h \
    src/Widgets/previewpanel.h \
    src/WindowHandler/qwinwidget.h \
    src/WindowHandler/winnativewindow.h \
    src/mainwindow.h \
    src/pixmaputils.hpp \
    src/qtgameutils.hpp

LIBS += -L"C:\Program Files\Microsoft SDKs\Windows\v7.1\Lib" \
        -ldwmapi \
        -lgdi32

FORMS += \
    src/Widgets/additemdialog.ui \
    src/Widgets/gamemanagerform.ui \
    src/Widgets/gamecard.ui \
    src/Widgets/gamestatstable.ui \
    src/mainwindow.ui \
    src/Widgets/previewpanel.ui

RESOURCES += \
    src/Widgets/burgermenu.qrc \
    src/WindowHandler/WindowHandler.qrc

win32:RC_ICONS += src/WindowHandler/icons/logo_main_icon.ico

CONFIG+=c++2a
DEFINES -= UNICODE
DEFINES += _MBCS

