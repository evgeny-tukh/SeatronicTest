QT -= gui
QT += network
QT += sql

CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        Data.cpp \
        DataStorage.cpp \
        DbHost.cpp \
        Processor.cpp \
        Sentence.cpp \
        Splitter.cpp \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    Data.h \
    DataStorage.h \
    DbHost.h \
    Defs.h \
    Processor.h \
    Sentence.h \
    Splitter.h
