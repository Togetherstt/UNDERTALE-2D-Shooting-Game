#include "armor.h"
#include "customization.h"

Armor::Armor(ArmorType type, GameScene* scene, QGraphicsItem *parent)
    : Item(scene, parent), m_type(type)
{
    switch(m_type) {
    case Chainmail:
        setPixmap(QPixmap(ITEM_LIGHT_ARMOR_SPRITE));
        // Per requirements, Chainmail doesn't break, so we can set durability to a symbolic high value.
        m_durability = 999999;
        break;
    case BulletproofVest:
        setPixmap(QPixmap(ITEM_VEST_SPRITE));
        m_durability = BULLETPROOF_VEST_DURABILITY; // [cite: 76]
        break;
    }
}
QString Armor::getName() const
{
    switch (m_type) {
    case Chainmail:
        return QString("%1").arg("装备：锁子甲");
    case BulletproofVest:
        return QString("%1 (%2)").arg("装备：防弹衣").arg(m_durability);
    default:
        return "装备：无";
    }
}

qreal Armor::reduceDamage(qreal initialDamage, Weapon::WeaponType weaponType)
{
    if (isBroken()) {
        return initialDamage;
    }

    switch(m_type) {
    case Chainmail:
        if (weaponType == Weapon::Fist) {
            return 0; // Immune to fist damage [cite: 75]
        }
        if (weaponType == Weapon::Knife) {
            return initialDamage * (1.0 - KNIFE_DAMAGE_REDUCTION_LIGHT_ARMOR); // Reduces knife damage [cite: 75]
        }
        // No effect on other weapons
        return initialDamage;

        case BulletproofVest:
        // Only provides defense against bullets (Rifle, Sniper)
        if (weaponType == Weapon::Rifle || weaponType == Weapon::SniperRifle) {
            qreal absorbedDamage = initialDamage * BULLET_DAMAGE_ABSORPTION_VEST; // [cite: 76]
            qreal playerDamage = initialDamage - absorbedDamage; // [cite: 76]

            m_durability -= absorbedDamage; // Vest durability decreases [cite: 76]

            return playerDamage;
        }
        // No effect on other weapons
        return initialDamage;
    }
    return initialDamage;
}

bool Armor::isBroken() const
{
    // When durability is below 0, the armor is broken [cite: 76]
    return m_durability <= 0;
}
