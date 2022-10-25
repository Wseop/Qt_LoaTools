#ifndef CHARACTER_LIST_H
#define CHARACTER_LIST_H

#include <QWidget>

struct CharacterTitle
{
    QString name;
    QString cls;
    QString itemLevel;
};

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
    void updateUI();

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    Ui::CharacterList *ui;
    QWidget* mParent;
    class QNetworkAccessManager* mNetworkItemLevel;
    QList<QWidget*> mWidgetList;
    QList<QLayout*> mLayoutList;

    bool mIsUIUpdated = false;
    QMap<QString, QList<CharacterTitle*>> mServerCharacterList;
    QMap<QString, CharacterTitle*> mNameTitle;

private slots:
    void slotParseItemLevel(class QNetworkReply*);
};

#endif // CHARACTER_LIST_H
