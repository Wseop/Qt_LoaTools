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
    EquipWidget(QWidget *pParent, const Equip* pEquip);
    ~EquipWidget();

private:
    void loadIcon();
    void setQualityBar();
    void setLabels();
    void setFonts();
    void setLayoutAlignments();

private:
    Ui::EquipWidget *ui;
    QWidget* m_pParent;
    const Equip* m_pEquip;
    class QNetworkAccessManager* m_pNetworkManager;
};

#endif // EQUIP_WIDGET_H
