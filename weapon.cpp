#include "weapon.h"
#include "customization.h"
#include "player.h"
#include "projectile.h"
#include "gamescene.h"
#include <QDebug>

Weapon::Weapon(WeaponType type, GameScene* scene, QGraphicsItem *parent)
    : Item(scene, parent), m_type(type)
{
    m_lastAttackTime = QTime::currentTime().addMSecs(-10000);

    switch (type) {
    case Fist:
        m_name = "武器：拳头";
        m_damage = FIST_DAMAGE;
        m_ammo = -1; // Infinite
        m_cooldown = FIST_COOLDOWN_MS;
        setPixmap(QPixmap()); // No visual
        break;
    case Knife:
        m_name = "武器：玩具刀";
        m_damage = KNIFE_DAMAGE;
        m_ammo = -1; // Infinite
        m_cooldown = KNIFE_COOLDOWN_MS;
        setPixmap(QPixmap(WEAPON_KNIFE_SPRITE));
        break;
    case SolidBall:
        m_name = "武器：实心球";
        m_damage = SOLID_BALL_DAMAGE;
        m_ammo = SOLID_BALL_AMMO_PER_PICKUP;
        m_cooldown = SOLID_BALL_COOLDOWN_MS;
        setPixmap(QPixmap(WEAPON_SOLIDBALL_SPRITE));
        break;
    case Rifle:
        m_name = "武器：骨头发射";
        m_damage = RIFLE_BULLET_DAMAGE;
        m_ammo = RIFLE_AMMO_PER_PICKUP;
        m_cooldown = RIFLE_COOLDOWN_MS;
        setPixmap(QPixmap(WEAPON_RIFLE_SPRITE));
        break;
    case SniperRifle:
        m_name = "武器：Gaster冲击波";
        m_damage = SNIPER_BULLET_DAMAGE;
        m_ammo = SNIPER_AMMO_PER_PICKUP;
        m_cooldown = SNIPER_COOLDOWN_MS;
        setPixmap(QPixmap(WEAPON_SNIPER_SPRITE));
        break;
    }
}

void Weapon::attack() {
    if (!m_owner) return;
    if (m_lastAttackTime.msecsTo(QTime::currentTime()) < m_cooldown) return;

    if (m_ammo == 0) return;

    // ================ 逻辑补全/修改区开始 ================
    if (m_type == Fist || m_type == Knife) {
        // 近战攻击逻辑：创建一个命中框进行碰撞检测
        QRectF hitbox;
        qreal range = (m_type == Fist) ? FIST_RANGE : KNIFE_RANGE;

        // 根据玩家朝向确定命中框位置
        if (m_owner->getFacingDirection() == 1) { // 朝右
            hitbox.setRect(m_owner->x() + m_owner->boundingRect().width(), m_owner->y(), range, m_owner->boundingRect().height());
        } else { // 朝左
            hitbox.setRect(m_owner->x() - range, m_owner->y(), range, m_owner->boundingRect().height());
        }

        QList<QGraphicsItem*> candidates = m_scene->items(hitbox);
        for (QGraphicsItem* item : candidates) {
            Player* opponent = dynamic_cast<Player*>(item);
            if (opponent && opponent != m_owner) {
                opponent->takeDamage(m_damage, m_type);
                break; // 只攻击一个目标
            }
        }

    } else {
        // 远程攻击逻辑 (保持不变)
        createProjectile();
    }
    // ================ 逻辑补全/修改区结束 ================

    if (m_ammo > 0) m_ammo--;
    m_lastAttackTime = QTime::currentTime();

    // 弹药用尽的逻辑可以在Player类中做的更完善，此处暂不处理
}

void Weapon::createProjectile() {
    Projectile* p = nullptr;
    if (m_type == Rifle) {
        p = new Projectile(Projectile::Bullet, m_owner, m_type, m_scene); // Modified line
    } else if (m_type == SolidBall) {
        p = new Projectile(Projectile::SolidBall, m_owner, m_type, m_scene); // Modified line
    } else if (m_type == SniperRifle) {
        p = new Projectile(Projectile::GasterBlaster,m_owner,m_type,m_scene);
    }

    if (p) {
        m_scene->addItem(p);
    }
}


QString Weapon::getName() const {
    if (m_ammo > 0) {
        return QString("%1 (%2)").arg(m_name).arg(m_ammo);
    }
    if (m_ammo == -1) {
        return m_name;
    }
    return QString("%1 (Empty)").arg(m_name);
}
