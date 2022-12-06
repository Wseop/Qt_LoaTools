#ifndef CHARACTER_LIST_H
#define CHARACTER_LIST_H

#include <QWidget>

struct CharacterTitle
{
    QString name;
    QString server;
    QString cls;
    QString itemLevel;
};

struct GridPos
{
    int row;
    int col;
};

const int MAX_COL = 2;

namespace Ui {
class CharacterList;
}

class CharacterList : public QWidget
{
    Q_OBJECT

public:
    CharacterList(QWidget *parent);
    ~CharacterList();

    void initConnect();

    void addCharacter(QString server, QString name, QString cls);

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    Ui::CharacterList *ui;
    QWidget* m_pParent;
    class QNetworkAccessManager* m_pNetworkItemLevel;

    QMap<QString, class QGridLayout*> m_serverLayout;
    QMap<QString, GridPos> m_serverGridPos;
    QMap<QString, CharacterTitle*> m_nameTitle;
    QMap<QString, class QPushButton*> m_nameButton;

    QList<class QLabel*> m_labelList;

private:
    void movePos(GridPos& pos);

private slots:
    void slotParseItemLevel(class QNetworkReply*);
    void slotSearchCharacter();
};

#endif // CHARACTER_LIST_H
