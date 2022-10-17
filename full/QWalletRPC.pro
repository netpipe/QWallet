QT       += core gui multimedia

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
LIBS += -lboost_system -lpython2.7

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    src/cpp_bitcoin_rpc/bitcoin.cpp \
    src/encryption/Qt-AES/qaesencryption.cpp \
    src/encryption/rsa/Rsa.cpp \
    src/encryption/simpleCrypt/simplecrypt.cpp \
    src/loadtheme.cpp \
    src/oglwidget.cpp \
    src/qstylesheetmanager.cpp


HEADERS += \
    mainwindow.h \
    src/cpp_bitcoin_rpc/base64.hpp \
    src/cpp_bitcoin_rpc/bitcoin.hpp \
    src/encryption/Qt-AES/qaesencryption.h \
    src/encryption/encryption.h \
    src/encryption/rsa/Rsa.h \
    src/encryption/simpleCrypt/simplecrypt.h \
    src/loadtheme.h \
    src/oglwidget.h \
    src/qstylesheetmanager.h \
    src/qtpython.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

SUBDIRS += \
    src/encryption/Qt-AES/qaesencryption.pro \
    src/encryption/rsa/Rsa.pro

RESOURCES += \
    src/encryption/Qt-AES/res.qrc

