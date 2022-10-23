#include "bracelet.h"

Bracelet::Bracelet() : Item(Part::BRACELET)
{

}

QString Bracelet::getEffect()
{
    return mEffect;
}

void Bracelet::setEffect(QString effect)
{
    mEffect = effect;
}
