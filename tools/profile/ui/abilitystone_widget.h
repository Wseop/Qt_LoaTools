#ifndef ABILITYSTONE_WIDGET_H
#define ABILITYSTONE_WIDGET_H

#include <QWidget>

class AbilityStone;
class QNetworkAccessManager;
class QNetworkReply;

namespace Ui {
class AbilityStoneWidget;
}

class AbilityStoneWidget : public QWidget
{
    Q_OBJECT

public:
    AbilityStoneWidget(const AbilityStone* stone = nullptr, QString iconUrl = "");
    ~AbilityStoneWidget();

private:
    void requestIcon(QString iconUrl);
    void setText();
    void setStyleSheet();

private:
    Ui::AbilityStoneWidget *ui;
    const AbilityStone* m_pStone;
    QNetworkAccessManager* m_pNetworkManager = nullptr;

private slots:
    void slotUpdateIcon(QNetworkReply* reply);
};

#endif // ABILITYSTONE_WIDGET_H
