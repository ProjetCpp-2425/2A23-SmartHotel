QT += core gui sql serialport network
QT += printsupport
QT += charts

include($$PWD/Qt-QrCodeGenerator.pri)

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    arduinosms.cpp \
    chambre.cpp \
    commande.cpp \
    connexion.cpp \
    fenetremessage.cpp \
    finance.cpp \
    gs_conge.cpp \
    gs_employe.cpp \
    hotelmanagerui.cpp \
    interfacelogin.cpp \
    main.cpp \
    mainwindow.cpp \
    smtp.cpp \
    stock.cpp

HEADERS += \
    arduinosms.h \
    chambre.h \
    commande.h \
    connexion.h \
    fenetremessage.h \
    finance.h \
    gs_conge.h \
    gs_employe.h \
    hotelmanagerui.h \
    interfacelogin.h \
    mainwindow.h \
    smtp.h \
    stock.h

FORMS += \
    fenetremessage.ui \
    hotelmanagerui.ui \
    interfacelogin.ui \
    mainwindow.ui

# Default rules for deployment.

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    ../../Downloads/PureStay/purestaybackground.png

RESOURCES += \
    purestaybackground.qrc
