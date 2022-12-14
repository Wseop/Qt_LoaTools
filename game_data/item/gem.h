#ifndef GEM_H
#define GEM_H

#include "item.h"
#include "game_data/item/enum/gem_type.h"

class Gem : public Item
{
public:
    Gem();

    int getLevel() const;
    QString getAttr() const;
    QString getEffect() const;
    GemType getGemType() const;

    void setLevel(int level);
    void setEffect(const QString& effect);
    void setGemType(GemType type);

private:
    int m_level;
    QString m_effect;
    GemType m_gemType;
};

#endif // GEM_H
