QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    AuctionCalc/AuctionCalc.cpp \
    EngraveSimulator/EngraveSimulator.cpp \
    MeteorTimer/MeteorTimer.cpp \
    Profile/Profile.cpp \
    main.cpp \
    loatools.cpp

HEADERS += \
    AuctionCalc/AuctionCalc.h \
    EngraveSimulator/EngraveSimulator.h \
    MeteorTimer/MeteorTimer.h \
    Profile/Profile.h \
    loatools.h

FORMS += \
    AuctionCalc/auctioncalc.ui \
    EngraveSimulator/engravesimulator.ui \
    MeteorTimer/meteortimer.ui \
    Profile/profile.ui \
    loatools.ui \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
