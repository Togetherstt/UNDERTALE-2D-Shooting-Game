#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <QObject>
#include <QGraphicsPixmapItem>

#include "weapon.h"

class Player;
class GameScene;

class Projectile : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    enum ProjectileType { Bullet, SolidBall, GasterBlaster };

    Projectile(ProjectileType type, Player* owner, Weapon::WeaponType sourceType, GameScene* scene, QGraphicsItem *parent = nullptr);

public slots:
    void move();

private:
    void checkCollision();

    ProjectileType m_type;
    Player* m_owner;
    GameScene* m_scene;
    qreal m_velocityX;
    qreal m_velocityY;
    Weapon::WeaponType m_sourceWeaponType;
    qreal m_damage;
};

#endif // PROJECTILE_H
