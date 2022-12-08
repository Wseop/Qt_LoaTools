#include "character_search.h"
#include "ui_character_search.h"

CharacterSearch* CharacterSearch::m_pCharacterSearch = nullptr;

CharacterSearch::CharacterSearch() :
    ui(new Ui::CharacterSearch)
{
    ui->setupUi(this);
}

CharacterSearch::~CharacterSearch()
{
    delete ui;
    destroyInstance();
}

CharacterSearch* CharacterSearch::getInstance()
{
    if (m_pCharacterSearch == nullptr)
        m_pCharacterSearch = new CharacterSearch();

    return m_pCharacterSearch;
}

void CharacterSearch::destroyInstance()
{
    if (m_pCharacterSearch == nullptr)
        return;

    delete m_pCharacterSearch;
    m_pCharacterSearch = nullptr;
}
