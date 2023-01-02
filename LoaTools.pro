QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    db/db.cpp \
    db/db_request.cpp \
    db/document/document_manager.cpp \
    db/document/settingcode_manager.cpp \
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
    main.cpp \
    loatools.cpp \
    tools/accessory_searcher/accessory_searcher.cpp \
    tools/accessory_searcher/auction_options.cpp \
    tools/accessory_searcher/search_filter.cpp \
    tools/accessory_searcher/ui/engrave_selector.cpp \
    tools/accessory_searcher/ui/penalty_selector.cpp \
    tools/accessory_searcher/ui/search_result.cpp \
    tools/auction_calc/auction_calc.cpp \
    tools/character_search/character_search.cpp \
    tools/character_search/ui/abilitystone_widget.cpp \
    tools/character_search/ui/accessory_widget.cpp \
    tools/character_search/ui/bracelet_widget.cpp \
    tools/character_search/ui/card_widget.cpp \
    tools/character_search/ui/engrave_widget.cpp \
    tools/character_search/ui/equip_widget.cpp \
    tools/character_search/ui/gem_widget.cpp \
    tools/character_search/ui/others.cpp \
    tools/character_search/ui/profile_widget.cpp \
    tools/character_search/ui/skill_widget.cpp \
    tools/engrave_simulator/engrave_simulator.cpp \
    tools/meteor_timer/meteor_timer.cpp \
    tools/ranking_board/ranking_board.cpp \
    tools/setting_adviser/class_selector.cpp \
    tools/setting_adviser/setting_adviser.cpp \
    tools/setting_adviser/ui/characters_widget.cpp \
    tools/setting_adviser/ui/setting_widget.cpp \
    tools/spec_checker/spec_checker.cpp \
    tools/spec_checker/ui/spec_widget.cpp

HEADERS += \
    db/db.h \
    db/db_request.h \
    db/document/document_manager.h \
    db/document/settingcode_manager.h \
    db/enum/collection.h \
    font/font_manager.h \
    game_data/card/card.h \
    game_data/character/character.h \
    game_data/engrave/engrave.h \
    game_data/engrave/engrave_manager.h \
    game_data/item/abilitystone.h \
    game_data/item/accessory.h \
    game_data/item/bracelet.h \
    game_data/item/enum/gem_type.h \
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
    loatools.h \
    tools/accessory_searcher/accessory_searcher.h \
    tools/accessory_searcher/auction_options.h \
    tools/accessory_searcher/search_filter.h \
    tools/accessory_searcher/ui/engrave_selector.h \
    tools/accessory_searcher/ui/penalty_selector.h \
    tools/accessory_searcher/ui/search_result.h \
    tools/auction_calc/auction_calc.h \
    tools/character_search/character_search.h \
    tools/character_search/ui/abilitystone_widget.h \
    tools/character_search/ui/accessory_widget.h \
    tools/character_search/ui/bracelet_widget.h \
    tools/character_search/ui/card_widget.h \
    tools/character_search/ui/engrave_widget.h \
    tools/character_search/ui/equip_widget.h \
    tools/character_search/ui/gem_widget.h \
    tools/character_search/ui/others.h \
    tools/character_search/ui/profile_widget.h \
    tools/character_search/ui/quality_color.h \
    tools/character_search/ui/skill_widget.h \
    tools/engrave_simulator/engrave_simulator.h \
    tools/meteor_timer/meteor_timer.h \
    tools/ranking_board/ranking_board.h \
    tools/setting_adviser/class_selector.h \
    tools/setting_adviser/setting_adviser.h \
    tools/setting_adviser/ui/characters_widget.h \
    tools/setting_adviser/ui/setting_widget.h \
    tools/spec_checker/spec_checker.h \
    tools/spec_checker/ui/spec_widget.h

FORMS += \
    loatools.ui \
    tools/accessory_searcher/accessory_searcher.ui \
    tools/accessory_searcher/ui/engrave_selector.ui \
    tools/accessory_searcher/ui/penalty_selector.ui \
    tools/accessory_searcher/ui/search_result.ui \
    tools/auction_calc/auctioncalc.ui \
    tools/character_search/character_search.ui \
    tools/character_search/ui/abilitystone_widget.ui \
    tools/character_search/ui/accessory_widget.ui \
    tools/character_search/ui/bracelet_widget.ui \
    tools/character_search/ui/card_widget.ui \
    tools/character_search/ui/engrave_widget.ui \
    tools/character_search/ui/equip_widget.ui \
    tools/character_search/ui/gem_widget.ui \
    tools/character_search/ui/others.ui \
    tools/character_search/ui/profile_widget.ui \
    tools/character_search/ui/skill_widget.ui \
    tools/engrave_simulator/engravesimulator.ui \
    tools/meteor_timer/meteortimer.ui \
    tools/ranking_board/ranking_board.ui \
    tools/setting_adviser/class_selector.ui \
    tools/setting_adviser/setting_adviser.ui \
    tools/setting_adviser/ui/characters_widget.ui \
    tools/setting_adviser/ui/setting_widget.ui \
    tools/spec_checker/spec_checker.ui \
    tools/spec_checker/ui/spec_widget.ui

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
