#ifndef WEAPON_H
#define WEAPON_H

#include "item.h"
#include <QTime>

class Projectile;

class Weapon : public Item
{
public:
    enum WeaponType { Fist, Knife, SolidBall, Rifle, SniperRifle };

    Weapon(WeaponType type, GameScene* scene, QGraphicsItem *parent = nullptr);

    void attack();
    QString getName() const;

private:
    void createProjectile();

    WeaponType m_type;
    QString m_name;
    qreal m_damage;
    int m_ammo;
    int m_cooldown; // in ms
    QTime m_lastAttackTime;
};

#endif // WEAPON_H
