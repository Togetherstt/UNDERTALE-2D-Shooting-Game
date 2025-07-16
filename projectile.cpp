#include "projectile.h"
#include "customization.h"
#include "player.h"
#include <QTimer>
#include <QList>
#include <QDebug>
#include <QGraphicsScene>

Projectile::Projectile(ProjectileType type, Player* owner, Weapon::WeaponType sourceType, GameScene* scene, QGraphicsItem* parent)
    : QObject(), QGraphicsPixmapItem(parent), m_type(type), m_owner(owner), m_sourceWeaponType(sourceType), m_scene(scene),
    m_velocityX(0), m_velocityY(0), m_damage(0)
{
    // ================ 逻辑补全区开始 ================

    // 1. 根据类型设置贴图和伤害值
    switch(sourceType) {
    case Weapon::Rifle:
        setPixmap(QPixmap(PROJECTILE_BULLET_SPRITE));
        m_damage = RIFLE_BULLET_DAMAGE;
        m_velocityX = RIFLE_BULLET_SPEED * m_owner->getFacingDirection();
        break;
    case Weapon::SniperRifle:
        setPixmap(QPixmap(PROJECTILE_GASTER_BLASTER_SPRITE));
        m_damage = SNIPER_BULLET_DAMAGE;
        m_velocityX = SNIPER_BULLET_SPEED * m_owner->getFacingDirection();
        break;
    case Weapon::SolidBall:
        setPixmap(QPixmap(PROJECTILE_SOLIDBALL_SPRITE));
        m_damage = SOLID_BALL_DAMAGE;
        // 实心球拥有水平和垂直初速度，形成抛物线
        m_velocityX = 10.0 * m_owner->getFacingDirection();
        m_velocityY = -15.0; // 向上的初速度
        break;
    default:
        break;
    }

    // 2. 设置初始位置 (在玩家的中心发出)
    QPointF spawnPos = m_owner->sceneBoundingRect().center();
    setPos(spawnPos);

    // ================ 逻辑补全区结束 ================

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Projectile::move);
    timer->start(1000 / TARGET_FPS); // 使用与游戏主循环匹配的帧率

    setZValue(3);
}

void Projectile::move()
{
    // Apply gravity for relevant types
    if (m_type == SolidBall) {
        m_velocityY += GRAVITY;
    }

    setPos(x() + m_velocityX, y() + m_velocityY);

    checkCollision();

    // Remove if out of bounds
    if (x() < 0 || x() > scene()->width() || y() > scene()->height()) {
        scene()->removeItem(this);
        delete this;
    }
}

// 在 projectile.cpp 中

void Projectile::checkCollision() {
    QList<QGraphicsItem*> colliding_items = collidingItems();
    for (int i = 0; i < colliding_items.size(); ++i) {
        Player* player = dynamic_cast<Player*>(colliding_items[i]);
        // 如果是玩家且不是发射者自己
        if (player && player != m_owner) {
            player->takeDamage(m_damage, m_sourceWeaponType);

            // ================ 崩溃修复区 ================
            // 旧的、不安全的代码:
            // scene()->removeItem(this);
            // delete this;

            // 新的、安全的代码:
            // 使用deleteLater()来安全地调度对象的删除，防止崩溃
            deleteLater();
            // ================ 修复区结束 ================

            return; // 命中后立即返回，防止穿透
        }
    }
}
