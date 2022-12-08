#ifndef CHARACTER_SEARCH_H
#define CHARACTER_SEARCH_H

#include <QWidget>

namespace Ui {
class CharacterSearch;
}

class CharacterSearch : public QWidget
{
    Q_OBJECT

private:
    CharacterSearch();
    ~CharacterSearch();

public:
    static CharacterSearch* getInstance();
    static void destroyInstance();

private:
    Ui::CharacterSearch *ui;

    static CharacterSearch* m_pCharacterSearch;
};

#endif // CHARACTER_SEARCH_H
