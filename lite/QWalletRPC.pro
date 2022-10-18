QT       += core gui multimedia network

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
LIBS += -lboost_system -lssl -lcrypto

QMAKE_CXXFLAGS += -fpermissive

SOURCES += \
    cpp_bitcoin_rpc/bitcoin.cpp \
    crypto/Base58Check.cpp \
    crypto/CurvePoint.cpp \
    crypto/Ecdsa.cpp \
    crypto/ExtendedPrivateKey.cpp \
    crypto/FieldInt.cpp \
    crypto/Keccak256.cpp \
    crypto/Ripemd160.cpp \
    crypto/Sha256.cpp \
    crypto/Sha256Hash.cpp \
    crypto/Sha512.cpp \
    crypto/Uint256.cpp \
    crypto/Utils.cpp \
    main.cpp \
    mainwindow.cpp


HEADERS += \
    address.h \
    cpp_bitcoin_rpc/base64.hpp \
    cpp_bitcoin_rpc/bitcoin.hpp \
    crypto/Base58Check.hpp \
    crypto/CurvePoint.hpp \
    crypto/Ecdsa.hpp \
    crypto/ExtendedPrivateKey.hpp \
    crypto/FieldInt.hpp \
    crypto/Keccak256.hpp \
    crypto/Ripemd160.hpp \
    crypto/Sha256.hpp \
    crypto/Sha256Hash.hpp \
    crypto/Sha512.hpp \
    crypto/TestHelper.hpp \
    crypto/Uint256.hpp \
    crypto/Utils.hpp \
    crypto/x8664/AsmX8664.hpp \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    crypto/Readme.markdown \
    crypto/Readme.txt \
    crypto/ecdsa-op-count.py \
    crypto/x8664/AsmX8664.s
