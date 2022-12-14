#ifndef EQUIP_WIDGET_H
#define EQUIP_WIDGET_H

#include <QWidget>

class Equip;

namespace Ui {
class EquipWidget;
}

class EquipWidget : public QWidget
{
    Q_OBJECT

public:
    EquipWidget(QWidget *parent, const Equip* equip);
    ~EquipWidget();

private:
    void setFonts();
    void setAlignment();
    void setStyleSheets();
    void requestIcon();
    void setTexts();
    void setQuality();

private:
    Ui::EquipWidget *ui;
    QWidget* m_pParent;
    const Equip* m_pEquip;
    class QNetworkAccessManager* m_pNetworkManager;
};

#endif // EQUIP_WIDGET_H
