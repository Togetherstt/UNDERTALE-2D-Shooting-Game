#ifndef ARMOR_H
#define ARMOR_H

#include "item.h"
#include "weapon.h" // For WeaponType enum

class Armor : public Item
{
public:
    enum ArmorType { Chainmail, BulletproofVest };

    Armor(ArmorType type, GameScene* scene, QGraphicsItem* parent = nullptr);

    // Reduces incoming damage based on armor type and weapon type
    // Returns the final damage the player should take
    qreal reduceDamage(qreal initialDamage, Weapon::WeaponType weaponType);

    // Checks if the armor is broken (e.g., durability <= 0)
    bool isBroken() const;
    QString getName() const;

private:
    ArmorType m_type;
    qreal m_durability;
};

#endif // ARMOR_H
