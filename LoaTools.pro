QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    db/db.cpp \
    db/db_request.cpp \
    font/font_manager.cpp \
    game_data/item/abilitystone.cpp \
    game_data/item/accessory.cpp \
    game_data/item/bracelet.cpp \
    game_data/item/equip.cpp \
    game_data/item/gem.cpp \
    game_data/item/item.cpp \
    game_data/profile/engrave.cpp \
    game_data/skill/rune.cpp \
    game_data/skill/skill.cpp \
    http_client/http_client.cpp \
    http_client/json_builder.cpp \
    main.cpp \
    loatools.cpp \
    tools/auction_calc/auction_calc.cpp \
    tools/engrave_simulator/engrave_simulator.cpp \
    tools/meteor_timer/meteor_timer.cpp \
    tools/profile/card_set.cpp \
    tools/profile/character.cpp \
    tools/profile/character_list.cpp \
    tools/profile/profile.cpp \
    tools/profile/skill.cpp \
    tools/profile/ui/abilitystone_widget.cpp \
    tools/profile/ui/acc_widget.cpp \
    tools/profile/ui/bracelet_widget.cpp \
    tools/profile/ui/card_label.cpp \
    tools/profile/ui/engrave_widget.cpp \
    tools/profile/ui/equip_widget.cpp \
    tools/profile/ui/gem_widget.cpp \
    tools/profile/ui/skill_widget.cpp \
    tools/ranking_board/ranking_board.cpp \
    tools/setting_adviser/class_selector.cpp \
    tools/setting_adviser/setting_adviser.cpp \
    tools/setting_adviser/setting_code.cpp \
    tools/setting_adviser/ui/setting_widget.cpp

HEADERS += \
    db/db.h \
    db/db_request.h \
    db/enum/collection.h \
    font/font_manager.h \
    game_data/item/abilitystone.h \
    game_data/item/accessory.h \
    game_data/item/bracelet.h \
    game_data/item/enum/item_grade.h \
    game_data/item/enum/item_type.h \
    game_data/item/enum/set_effect.h \
    game_data/item/equip.h \
    game_data/item/gem.h \
    game_data/item/item.h \
    game_data/profile/engrave.h \
    game_data/profile/enum/ability.h \
    game_data/profile/enum/class.h \
    game_data/skill/rune.h \
    game_data/skill/skill.h \
    http_client/http_client.h \
    http_client/json_builder.h \
    loatools.h \
    tools/auction_calc/auction_calc.h \
    tools/engrave_simulator/engrave_simulator.h \
    tools/meteor_timer/meteor_timer.h \
    tools/profile/card_set.h \
    tools/profile/character.h \
    tools/profile/character_list.h \
    tools/profile/profile.h \
    tools/profile/skill.h \
    tools/profile/ui/abilitystone_widget.h \
    tools/profile/ui/acc_widget.h \
    tools/profile/ui/bracelet_widget.h \
    tools/profile/ui/card_label.h \
    tools/profile/ui/engrave_widget.h \
    tools/profile/ui/equip_widget.h \
    tools/profile/ui/gem_widget.h \
    tools/profile/ui/skill_widget.h \
    tools/ranking_board/ranking_board.h \
    tools/setting_adviser/class_selector.h \
    tools/setting_adviser/setting_adviser.h \
    tools/setting_adviser/setting_code.h \
    tools/setting_adviser/ui/setting_widget.h

FORMS += \
    loatools.ui \
    tools/auction_calc/auctioncalc.ui \
    tools/engrave_simulator/engravesimulator.ui \
    tools/meteor_timer/meteortimer.ui \
    tools/profile/character_list.ui \
    tools/profile/profile.ui \
    tools/profile/ui/abilitystone_widget.ui \
    tools/profile/ui/acc_widget.ui \
    tools/profile/ui/bracelet_widget.ui \
    tools/profile/ui/card_label.ui \
    tools/profile/ui/engrave_widget.ui \
    tools/profile/ui/equip_widget.ui \
    tools/profile/ui/gem_widget.ui \
    tools/profile/ui/skill_widget.ui \
    tools/ranking_board/ranking_board.ui \
    tools/setting_adviser/class_selector.ui \
    tools/setting_adviser/setting_adviser.ui \
    tools/setting_adviser/ui/setting_widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc

win32: LIBS += -L$$PWD/../../mongo-cxx-driver/lib/ -llibbsoncxx.dll

INCLUDEPATH += $$PWD/../../mongo-cxx-driver/include
DEPENDPATH += $$PWD/../../mongo-cxx-driver/include

win32: LIBS += -L$$PWD/../../mongo-cxx-driver/lib/ -llibmongocxx.dll

INCLUDEPATH += $$PWD/../../mongo-cxx-driver/include
DEPENDPATH += $$PWD/../../mongo-cxx-driver/include
