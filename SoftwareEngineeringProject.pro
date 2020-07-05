QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    DB.cpp \
    arbeit.cpp \
    mainwindow.cpp \
    main.cpp \
    nutzer.cpp \
    searchview.cpp \
    sha512.cpp \
    sonstigesprojekt.cpp \
    studiengang.cpp \

HEADERS += \
    DB.h \
    arbeit.h \
    mainwindow.h \
    nutzer.h \
    searchview.h \
    sha512.h \
    sonstigesprojekt.h \
    studiengang.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    LICENSE.txt \
    README.md

INCLUDEPATH += C:/boost/boost_1_73_0
INCLUDEPATH += D:/Programmieren/Cpp/libs/boost_1_73_0

TRANSLATIONS = strings_en.ts
