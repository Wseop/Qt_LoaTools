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
    explicit AbilityStoneWidget(QWidget *parent = nullptr, const AbilityStone* stone = nullptr, QString iconUrl = "");
    ~AbilityStoneWidget();

private:
    void requestIcon(QString iconUrl);
    void setText();
    void setStyleSheet();

private:
    Ui::AbilityStoneWidget *ui;
    const AbilityStone* mStone;
    QNetworkAccessManager* mNetworkManager = nullptr;

private slots:
    void slotUpdateIcon(QNetworkReply* reply);
};

#endif // ABILITYSTONE_WIDGET_H