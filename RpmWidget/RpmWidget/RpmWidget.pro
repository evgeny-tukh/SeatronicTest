QT += quick
QT += sql
QT += widgets
QT += opengl

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        ../../RpmServer/RpmServer/Data.cpp \
        ../../RpmServer/RpmServer/DbHost.cpp \
        ../../RpmServer/RpmServer/Splitter.cpp \
        main.cpp

RESOURCES += qml.qrc

win32:CONFIG(debug, debug|release): LIBS += -lQt5Sqld
else:win32:CONFIG(release, debug|release): LIBS += -lQt5Sql

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    ../../RpmServer/RpmServer/Data.h \
    ../../RpmServer/RpmServer/DbHost.h \
    ../../RpmServer/RpmServer/Defs.h \
    ../../RpmServer/RpmServer/Splitter.h \
    gauge.h
