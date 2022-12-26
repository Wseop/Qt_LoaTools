#ifndef SPEC_WIDGET_H
#define SPEC_WIDGET_H

#include <QWidget>

class Character;

namespace Ui {
class SpecWidget;
}

class SpecWidget : public QWidget
{
    Q_OBJECT

public:
    SpecWidget();
    ~SpecWidget();

    void setCharacter(Character* pCharacter);

private:
    void setFonts();
    void setAlignments();
    void setConnects();

    void setProfileData();
    void setEngravingData();
    void setGemData();
    void setTripodData();
    void setWeaponData();
    void setAccData();
    void setSetEffectData();
    void setCardData();

private:
    Ui::SpecWidget *ui;

    Character* m_pCharacter;
};

#endif // SPEC_WIDGET_H
