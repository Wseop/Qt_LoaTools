#ifndef RANKING_BOARD_H
#define RANKING_BOARD_H

#include "game_data/class.h"
#include "setting_adviser/class_selector.h"

#include <QWidget>

namespace Ui {
class RankingBoard;
}

class RankingBoard : public QWidget
{
    Q_OBJECT

private:
    RankingBoard();
    ~RankingBoard();

public:
    void setSelectedClass(int cls);

public:
    static RankingBoard* getInstance();
    static void destroyInstance();

private:
    void initAlignment();
    void initFont();
    void initConnect();
    void disableInput();
    void enableInput();
    void clear();

    // DB request
    void requestAllCharacters();

    class QLabel* createLabel(QString text);
    QString getCharacterName(int index);
    double getCharacterItemLevel(int index);
    QString getCharacterClass(int index);

    void renderCharacters(bool bInitialize);

private slots:
    // hSliderSelectItemLevel
    void slotSetSelectItemLevel(int value);
    void slotItemLevelChanged();
    // pbSelectClass
    void slotShowClassSelector();
    // pbRenderMore
    void slotRenderMore();
    // pbSelectAllClass
    void slotRenderAll();
    // handle DB datas
    void slotHandleCharacters(QVariantList characters);

private:
    Ui::RankingBoard *ui;

    const int RENDER_COUNT;

    class DBRequest* m_pDBRequest;
    QVariantList m_characters;
    int m_rankIndex;
    int m_characterIndex;
    int m_selectedItemLevel;
    int m_selectedClass;
    ClassSelector* m_pClassSelector = nullptr;
    QList<QLabel*> m_labels;

private:
    static RankingBoard* m_pRankingBoard;
};

#endif // RANKING_BOARD_H
