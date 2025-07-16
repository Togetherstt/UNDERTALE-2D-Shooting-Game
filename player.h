#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include "weapon.h"

class Weapon;
class Armor;
class Item;
class Terrain;
class GameScene;

class Player : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    Player(int playerNumber, GameScene* scene, QGraphicsItem *parent = nullptr);
    ~Player();

    void update(const QList<Terrain*>& terrains);
    void gainExperience(int amount);

    // Actions
    void moveLeft();
    void moveRight();
    void jump();
    void crouch();
    void setCrouching(bool crouch);
    void fire();
    void pickupItem(Item* item);
    int getFacingDirection() const;

    // Getters
    qreal getHealth() const;
    int getPlayerNumber() const;

    // Public methods
    void takeDamage(qreal damage, Weapon::WeaponType weaponType = Weapon::Fist);
    void heal(qreal amount);          // <-- 新增的治疗方法
    qreal getMaxHealth() const;
    bool collidesWithItem(QGraphicsItem* item);
    void applyAdrenaline(int duration, qreal healPerSecond);

private:
    void applyGravity();
    void handleCollisions(const QList<Terrain*>& terrains);
    void updateSprite();
    void updateUI();
    void applyTerrainEffects(const QList<Terrain*>& terrains);
    void updateEffects();
    void levelUp();
    void updateAttachments();

    int m_playerNumber;
    GameScene* m_scene;

    qreal m_health;
    qreal m_velocityX;
    qreal m_velocityY;
    bool m_onGround;
    bool m_isCrouching;
    int m_facingDirection; // 1 for right, -1 for left
    qreal m_maxHealth; // <-- 新增
    int m_level; // <-- 新增
    int m_currentExp; // <-- 新增
    int m_expToNextLevel; // <-- 新增

    Weapon* m_currentWeapon;
    Armor* m_currentArmor;

    // UI Elements
    QGraphicsRectItem* m_healthBar;
    QGraphicsRectItem* m_healthBarBg;
    QGraphicsTextItem* m_weaponText;
    QGraphicsTextItem* m_healthText; // <-- 新增
    QGraphicsTextItem* m_levelText; // <-- 新增
    QGraphicsTextItem* m_armorText;

    // Sprites
    QPixmap m_standSprite;
    QPixmap m_crouchSprite;
    QPixmap m_jumpSprite;

    // Status Effects
    qreal m_speedMultiplier;
    QTimer* m_adrenalineTimer;
    qreal m_adrenalineHealRate;
};

#endif // PLAYER_H
