#ifndef RANKING_BOARD_H
#define RANKING_BOARD_H

#include "enum/class.h"
#include "setting_adviser/class_selector.h"

#include <QWidget>

namespace Ui {
class RankingBoard;
}

class RankingBoard : public QWidget
{
    Q_OBJECT

private:
    explicit RankingBoard(QWidget *parent = nullptr);
    ~RankingBoard();

public:
    void setSelectedClass(Class cls);

public:
    static RankingBoard* getInstance();
    static void destroyInstance();

private:
    void initAlignment();
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

    QVariantList m_characters;
    int m_rankIndex;
    int m_characterIndex;
    int m_selectedItemLevel;
    Class m_selectedClass;
    ClassSelector* m_pClassSelector = nullptr;
    QList<QLabel*> m_labels;

private:
    static RankingBoard* m_pRankingBoard;

private:
    static void emitRenderAfterSort(QVariantList* characters);
};

#endif // RANKING_BOARD_H
