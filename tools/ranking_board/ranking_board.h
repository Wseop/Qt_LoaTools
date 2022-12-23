#ifndef RANKING_BOARD_H
#define RANKING_BOARD_H

#include "game_data/profile/enum/class.h"
#include "tools/setting_adviser/class_selector.h"

#include <QWidget>
#include <QJsonArray>

namespace Ui {
class RankingBoard;
}

class RankingBoard : public QWidget
{
    Q_OBJECT

private:
    RankingBoard();
    ~RankingBoard();

    void initAlignment();
    void initFont();
    void initConnect();
    void disableInput();
    void enableInput();
    void clear();

    // DB request
    void requestAllCharacters();

    class QLabel* createLabel(QString text);
    class QPushButton* createButton(QString text);
    QString getCharacterName(int index);
    double getCharacterItemLevel(int index);
    Class getCharacterClass(int index);

    void renderCharacters(bool bInitialize);

public:
    void setSelectedClass(Class cls);

public:
    static RankingBoard* getInstance();
    static void destroyInstance();

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
    void slotSetCharacters(QJsonArray characters);

private:
    Ui::RankingBoard *ui;

    const int RENDER_COUNT;

    class DBRequest* m_pDBRequest;
    QJsonArray m_characters;
    int m_rankIndex;
    int m_characterIndex;
    int m_selectedItemLevel;
    Class m_selectedClass;
    ClassSelector* m_pClassSelector = nullptr;
    QList<QLabel*> m_labels;
    QList<QPushButton*> m_btns;

    class QThread* m_pThread;

private:
    static RankingBoard* m_pRankingBoard;
};

#endif // RANKING_BOARD_H
