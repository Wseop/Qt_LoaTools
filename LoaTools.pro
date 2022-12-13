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
    game_data/card/card.cpp \
    game_data/character/character.cpp \
    game_data/engrave/engrave.cpp \
    game_data/engrave/engrave_manager.cpp \
    game_data/item/abilitystone.cpp \
    game_data/item/accessory.cpp \
    game_data/item/bracelet.cpp \
    game_data/item/equip.cpp \
    game_data/item/gem.cpp \
    game_data/item/item.cpp \
    game_data/profile/profile.cpp \
    game_data/skill/skill.cpp \
    game_data/skill/skillrune.cpp \
    game_data/skill/skillrune_manager.cpp \
    http_client/http_client.cpp \
    http_client/json_builder.cpp \
    main.cpp \
    loatools.cpp \
    tools/auction_calc/auction_calc.cpp \
    tools/character_search/character_search.cpp \
    tools/character_search/ui/others.cpp \
    tools/character_search/ui/profile_widget.cpp \
    tools/engrave_simulator/engrave_simulator.cpp \
    tools/meteor_timer/meteor_timer.cpp \
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
    game_data/card/card.h \
    game_data/character/character.h \
    game_data/engrave/engrave.h \
    game_data/engrave/engrave_manager.h \
    game_data/item/abilitystone.h \
    game_data/item/accessory.h \
    game_data/item/bracelet.h \
    game_data/item/enum/item_grade.h \
    game_data/item/enum/item_type.h \
    game_data/item/enum/set_effect.h \
    game_data/item/equip.h \
    game_data/item/gem.h \
    game_data/item/item.h \
    game_data/profile/enum/ability.h \
    game_data/profile/enum/class.h \
    game_data/profile/profile.h \
    game_data/skill/skill.h \
    game_data/skill/skillrune.h \
    game_data/skill/skillrune_manager.h \
    http_client/http_client.h \
    http_client/json_builder.h \
    loatools.h \
    tools/auction_calc/auction_calc.h \
    tools/character_search/character_search.h \
    tools/character_search/ui/others.h \
    tools/character_search/ui/profile_widget.h \
    tools/engrave_simulator/engrave_simulator.h \
    tools/meteor_timer/meteor_timer.h \
    tools/ranking_board/ranking_board.h \
    tools/setting_adviser/class_selector.h \
    tools/setting_adviser/setting_adviser.h \
    tools/setting_adviser/setting_code.h \
    tools/setting_adviser/ui/setting_widget.h

FORMS += \
    loatools.ui \
    tools/auction_calc/auctioncalc.ui \
    tools/character_search/character_search.ui \
    tools/character_search/ui/others.ui \
    tools/character_search/ui/profile_widget.ui \
    tools/engrave_simulator/engravesimulator.ui \
    tools/meteor_timer/meteortimer.ui \
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
