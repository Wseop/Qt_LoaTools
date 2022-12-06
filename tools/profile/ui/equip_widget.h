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
    EquipWidget(const Equip* equip = nullptr, QString iconUrl = "");
    ~EquipWidget();

private:
    void requestIcon(QString iconUrl);
    void setText();
    void setStyleSheet();
    QString getQualColor();

private:
    Ui::EquipWidget *ui;
    const Equip* m_pEquip;
    QNetworkAccessManager* m_pNetworkManager = nullptr;

private slots:
    void slotUpdateIcon(QNetworkReply* reply);
};

#endif // EQUIP_WIDGET_H
