#ifndef ACCESSORY_WIDGET_H
#define ACCESSORY_WIDGET_H

#include <QWidget>
#include <QLabel>

class Accessory;

namespace Ui {
class AccessoryWidget;
}

class AccessoryWidget : public QWidget
{
    Q_OBJECT

public:
    AccessoryWidget(QWidget* parent, const Accessory* accessory);
    ~AccessoryWidget();

private:
    void setFonts();
    void setAlignment();
    void requestIcon();
    void setTexts();
    void setQuality();
    void addAbilities();
    void addEngraves();

private:
    Ui::AccessoryWidget *ui;
    QWidget* m_pParent;
    const Accessory* m_pAccessory;
    class QNetworkAccessManager* m_pNetworkManager;
    QList<QLabel*> m_abilityLabels;
    QList<QLabel*> m_engraveLabels;
};

#endif // ACCESSORY_WIDGET_H
