QT       += core gui widgets charts multimedia network xml
CONFIG   += c++17

TARGET = LASSIE
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

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/EnvelopeLibraryEntry.cpp \
    src/EnvelopeLibraryWindow.cpp \
    src/EnvLibDrawingArea.cpp

HEADERS += \
    src/mainwindow.hpp \
    src/EnvelopeLibraryEntry.h \
    src/EnvelopeLibraryWindow.h \
    src/EnvLibDrawingArea.h

FORMS += \
    src/mainwindow.ui \
    src/EnvelopeLibraryWindow.ui

RESOURCES += \
    resources/resources.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target 