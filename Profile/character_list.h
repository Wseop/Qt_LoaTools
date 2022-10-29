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
    explicit CharacterList(QWidget *parent = nullptr, QWidget* ret = nullptr);
    ~CharacterList();

    void initConnect();

    void addCharacter(QString server, QString name, QString cls);

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    Ui::CharacterList *ui;
    QWidget* mParent;
    class QNetworkAccessManager* mNetworkItemLevel;

    QMap<QString, class QGridLayout*> mServerLayout;
    QMap<QString, GridPos> mServerGridPos;
    QMap<QString, CharacterTitle*> mNameTitle;
    QMap<QString, class QPushButton*> mNameButton;

    QList<class QLabel*> mLabelList;

private:
    void movePos(GridPos& pos);

private slots:
    void slotParseItemLevel(class QNetworkReply*);
    void slotSearchCharacter();
};

#endif // CHARACTER_LIST_H
