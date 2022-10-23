QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    auction_calc/auction_calc.cpp \
    engrave_simulator//engrave_simulator.cpp \
    item/abilitystone.cpp \
    item/accessory.cpp \
    item/bracelet.cpp \
    item/equip.cpp \
    item/gem.cpp \
    item/item.cpp \
    meteor_timer/meteor_timer.cpp \
    profile/character.cpp \
    profile/profile.cpp \
    engrave.cpp \
    main.cpp \
    loatools.cpp

HEADERS += \
    auction_calc/auction_calc.h \
    engrave_simulator/engrave_simulator.h \
    item/abilitystone.h \
    item/accessory.h \
    item/bracelet.h \
    item/equip.h \
    item/gem.h \
    item/item.h \
    meteor_timer/meteor_timer.h \
    profile/character.h \
    profile/profile.h \
    engrave.h \
    loatools.h

FORMS += \
    auction_calc/auctioncalc.ui \
    engrave_simulator/engravesimulator.ui \
    meteor_timer/meteortimer.ui \
    profile/profile.ui \
    loatools.ui \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
