#ifndef ABILITYSTONE_WIDGET_H
#define ABILITYSTONE_WIDGET_H

#include <QWidget>
#include <QLabel>

class AbilityStone;

namespace Ui {
class AbilityStoneWidget;
}

class AbilityStoneWidget : public QWidget
{
    Q_OBJECT

public:
    AbilityStoneWidget(QWidget* parent, const AbilityStone* stone);
    ~AbilityStoneWidget();

private:
    void setFonts();
    void setStyleSheets();
    void requestIcon();
    void setTexts();
    void addEngraveLabels();

private:
    Ui::AbilityStoneWidget *ui;
    QWidget* m_pParent;
    const AbilityStone* m_pStone;
    class QNetworkAccessManager* m_pNetworkManager;
    QList<QLabel*> m_engraveLabels;
};

#endif // ABILITYSTONE_WIDGET_H
