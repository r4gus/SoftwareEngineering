QT       += core gui sql widgets

QT_MAJOR_VERSION = 5

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
    abschlussarbeit.cpp \
    lecturereditview.cpp \
    mainwindow.cpp \
    main.cpp \
    nutzer.cpp \
    projektarbeit.cpp \
    searchview.cpp \
    sha512.cpp \
    sonstigesprojekt.cpp \
    studiengang.cpp \
    ProjectView.cpp\
    gui_utils.cpp \
    ProjectEditView.cpp \
    LoginView.cpp \
    ChangePasswordView.cpp \
    AdminView.cpp \
    LecturerView.cpp \
    test.cpp

HEADERS += \
    DB.h \
    abschlussarbeit.h \
    lecturereditview.h \
    mainwindow.h \
    nutzer.h \
    projektarbeit.h \
    searchview.h \
    sha512.h \
    sonstigesprojekt.h \
    studiengang.h \
    ProjectView.h \
    ProjectEditView.h \
    gui_utils.h \
    LoginView.h \
    ChangePasswordView.h \
    AdminView.h \
    LecturerView.h \



FORMS += \
    lecturereditview.ui \
    mainwindow.ui \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    LICENSE.txt \
    README.md

INCLUDEPATH += C:/boost/boost_1_73_0 # replace this path with your own path to boost lib

TRANSLATIONS = strings_en.ts

RESOURCES = application.qrc
