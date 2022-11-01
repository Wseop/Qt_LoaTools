#include "bracelet.h"

Bracelet::Bracelet() : Item(Part::BRACELET)
{

}

QString Bracelet::getEffect() const
{
    return mEffect;
}

void Bracelet::setEffect(QString effect)
{
    mEffect = effect;
}
