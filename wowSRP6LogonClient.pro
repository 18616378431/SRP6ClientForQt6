QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

INCLUDEPATH += /opt/homebrew/include
LIBS += -L/opt/homebrew/opt/openssl@3.2/lib -lssl -lcrypto

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    BigNum.cpp \
    libsrp6.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    BigNum.h \
    libsrp6.h \
    mainwindow.h \
    types.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    logon.qrc

macx {
    ICON = app.icns
}

