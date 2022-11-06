#ifndef EQUIP_WIDGET_H
#define EQUIP_WIDGET_H

#include <QWidget>

class Equip;
class QNetworkAccessManager;
class QNetworkReply;

namespace Ui {
class EquipWidget;
}

class EquipWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EquipWidget(QWidget *parent = nullptr, const Equip* equip = nullptr, QString iconUrl = "");
    ~EquipWidget();

private:
    void requestIcon(QString iconUrl);
    void setText();
    void setStyleSheet();
    QString getQualColor();

private:
    Ui::EquipWidget *ui;
    const Equip* mEquip;
    QNetworkAccessManager* mNetworkManager = nullptr;

private slots:
    void slotUpdateIcon(QNetworkReply* reply);
};

#endif // EQUIP_WIDGET_H
