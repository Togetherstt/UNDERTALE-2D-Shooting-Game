#include "player.h"
#include "customization.h"
#include "weapon.h"
#include "equipment.h"
#include "armor.h"
#include "terrain.h"
#include "gamescene.h"
#include <QDebug>
#include <QTimer>
#include <QGraphicsScene>
#include <QTransform>

Player::Player(int playerNumber, GameScene* scene, QGraphicsItem *parent)
    : QObject(), QGraphicsPixmapItem(parent), m_playerNumber(playerNumber), m_scene(scene),
    m_health(PLAYER_MAX_HEALTH), m_maxHealth(PLAYER_MAX_HEALTH), // 初始化maxHealth
    m_level(1), m_currentExp(0), m_expToNextLevel(1), m_velocityX(0), m_velocityY(0), m_onGround(false),
    m_isCrouching(false), m_facingDirection(1), m_currentWeapon(nullptr),
    m_currentArmor(nullptr), m_speedMultiplier(1.0), m_adrenalineTimer(nullptr), m_adrenalineHealRate(0)
{
    // Load sprites based on player number
    if (m_playerNumber == 1) {
        m_standSprite.load(PLAYER1_STAND_SPRITE);
        m_crouchSprite.load(PLAYER1_CROUCH_SPRITE);
        m_jumpSprite.load(PLAYER1_JUMP_SPRITE);
    } else {
        m_standSprite.load(PLAYER2_STAND_SPRITE);
        m_crouchSprite.load(PLAYER2_CROUCH_SPRITE);
        m_jumpSprite.load(PLAYER2_JUMP_SPRITE);
    }
    setPixmap(m_standSprite);

    // Set up default weapon (Fist)
    m_currentWeapon = new Weapon(Weapon::Fist, m_scene);
    m_currentWeapon->setOwner(this);

    // Set Z-value to be on top of other elements
    setZValue(2);

    // UI Setup
    m_healthBarBg = new QGraphicsRectItem(this);
    m_healthBar = new QGraphicsRectItem(this);
    m_weaponText = new QGraphicsTextItem(this);

    m_healthBarBg->setBrush(Qt::darkGray);
    m_healthBar->setBrush(m_playerNumber == 1 ? Qt::cyan : Qt::magenta);
    m_weaponText->setDefaultTextColor(Qt::white);
    m_weaponText->setFont(QFont("Arial", 10));
    m_healthText = new QGraphicsTextItem(this);
    m_levelText = new QGraphicsTextItem(this);
    m_healthText->setDefaultTextColor(Qt::white);
    m_levelText->setDefaultTextColor(Qt::yellow);
    m_healthText->setFont(QFont("Arial", 10));
    m_levelText->setFont(QFont("Arial", 10, QFont::Bold));
    m_armorText = new QGraphicsTextItem(this);
    m_armorText->setDefaultTextColor(Qt::white);
    m_armorText->setFont(QFont("Arial", 10));

    updateUI();
}

Player::~Player() {
    delete m_currentWeapon;
    delete m_currentArmor;
    if (m_adrenalineTimer) delete m_adrenalineTimer;
}

int Player::getFacingDirection() const {
    return m_facingDirection;
}

void Player::update(const QList<Terrain*>& terrains)
{
    applyGravity();
    setPos(x() + m_velocityX, y() + m_velocityY);
    handleCollisions(terrains);

    // Horizontal deceleration
    m_velocityX *= 0.85;

    applyTerrainEffects(terrains);
    updateEffects();
    updateSprite();
    updateUI();
    updateAttachments();
}

void Player::applyGravity() {
    if (!m_onGround) {
        m_velocityY += GRAVITY;
    }
}

void Player::handleCollisions(const QList<Terrain*>& terrains)
{
    // 使用一个临时变量来记录本帧的碰撞状态
    bool isCollidingWithGround = false;

    for (Terrain* terrain : terrains) {
        // 使用 >= 0 是为了处理玩家速度为0但仍站在平台上的情况
        if (m_velocityY >= 0 && collidesWithItem(terrain)) {
            setY(terrain->y() - 149);
            m_velocityY = 0;
            isCollidingWithGround = true; // 记录下发生了地面碰撞
            break;
        }
    }

    // 根据本帧的最终碰撞结果，来设定玩家的onGround状态
    m_onGround = isCollidingWithGround;

    // 处理掉出世界底部的情况 (保持之前的逻辑不变)
    if (!m_onGround && y() > SCREEN_HEIGHT) {
        setPos(x(), 0);
        m_velocityY = 0;
    }

    // 水平边界检查 (保持不变)
    if (x() < 0) setX(0);
    if (x() + boundingRect().width() > scene()->width()) setX(scene()->width() - boundingRect().width());
}


void Player::moveLeft() {
    if (m_isCrouching) return;
    m_velocityX = -PLAYER_MOVE_SPEED * m_speedMultiplier;
    m_facingDirection = -1;
    setTransform(QTransform().scale(-1, 1).translate(-boundingRect().width(), 0));
}

void Player::moveRight() {
    if (m_isCrouching) return;
    m_velocityX = PLAYER_MOVE_SPEED * m_speedMultiplier;
    m_facingDirection = 1;
    setTransform(QTransform());
}

void Player::jump() {
    if (m_onGround && !m_isCrouching) {
        m_velocityY = PLAYER_JUMP_FORCE;
        m_onGround = false;
    }
}

void Player::setCrouching(bool crouch) {
    if (m_onGround) {
        m_isCrouching = crouch;
    }
}


void Player::fire() {
    if (m_currentWeapon) {
        m_currentWeapon->attack();
    }
}

void Player::crouch() {
    if (m_onGround) {
        setCrouching(true);

        // ================ 新增的拾取逻辑 ================
        // 检查是否有可拾取的物品
        QList<QGraphicsItem*> nearbyItems = collidingItems();
        for(QGraphicsItem* graphicsItem : nearbyItems) {
            Item* item = dynamic_cast<Item*>(graphicsItem);
            if(item && !item->parentItem()) { // 确保它是一个可拾取的Item
                pickupItem(item); // 执行拾取动作

                // 通知GameScene将此物品从世界中移除
                GameScene* gs = qobject_cast<GameScene*>(scene());
                if(gs) {
                    gs->removeItemFromWorld(item);
                }
                break; // 每次按键只拾取一个物品
            }
        }
        // ================ 拾取逻辑结束 ================
    }
}

void Player::pickupItem(Item* item)
{
    // This logic is now safe because the GameScene is responsible
    // for removing the item from the world list. This function
    // just handles what the player does with it.

    Weapon* weapon = dynamic_cast<Weapon*>(item);
    Equipment* equipment = dynamic_cast<Equipment*>(item);
    Armor* armor = dynamic_cast<Armor*>(item);

    if (weapon) {
        delete m_currentWeapon; // Delete the old weapon object
        m_currentWeapon = weapon;
        m_currentWeapon->setOwner(this);
        m_currentWeapon->setParentItem(this);
        //m_currentWeapon->hide(); // Hide the item sprite, as it's now "equipped"
    } else if (equipment) {
        equipment->applyEffect(this);
        // The item is consumed, so we delete its object.
        equipment->deleteLater();
    } else if (armor) {
        delete m_currentArmor;
        m_currentArmor = armor;
        m_currentArmor->setOwner(this);
        m_currentArmor->setParentItem(this);
        m_currentArmor->setZValue(3); // 确保在玩家和武器之上

        // 设置护甲名称
        m_armorText->setPlainText(m_currentArmor->getName()); // <-- 新增
    }
}

void Player::takeDamage(qreal damage, Weapon::WeaponType weaponType) {
    qreal finalDamage = damage;

    if (m_currentArmor) {
        finalDamage = m_currentArmor->reduceDamage(damage, weaponType);

        // If armor breaks after taking damage, remove it
        if (m_currentArmor->isBroken()) {
            delete m_currentArmor;
            m_currentArmor = nullptr;
        }
    }

    m_health -= finalDamage;
    if (m_health < 0) m_health = 0;
    if (m_health > m_maxHealth) m_health=m_maxHealth;
    qDebug() << "Player" << m_playerNumber << "took" << finalDamage << "damage. Health is now" << m_health;
}

bool Player::collidesWithItem(QGraphicsItem* item) {
    return QGraphicsItem::collidesWithItem(item, Qt::IntersectsItemBoundingRect);
}

void Player::heal(qreal amount)
{
    m_health += amount;
    // 关键：检查并确保生命值不会超过最大值
    if (m_health > m_maxHealth) {
        m_health = m_maxHealth;
    }
}

qreal Player::getMaxHealth() const
{
    return m_maxHealth;
}

void Player::updateSprite() {
    if (!m_onGround) {
        setPixmap(m_jumpSprite);
    } else if (m_isCrouching) {
        setPixmap(m_crouchSprite);
    } else {
        setPixmap(m_standSprite);
    }
}

void Player::updateUI()
{
    // Health Bar
    qreal healthWidth = (m_health / m_maxHealth) * boundingRect().width();
    m_healthBarBg->setRect(0, -20, boundingRect().width(), 10);
    m_healthBar->setRect(0, -20, healthWidth, 10);

    // Health Text (新增)
    m_healthText->setPlainText(QString("%1/%2").arg(static_cast<int>(m_health)).arg(static_cast<int>(m_maxHealth)));
    m_healthText->setPos(0, -22);

    // Level Text (新增)
    m_levelText->setPlainText(QString("LV: %1").arg(m_level));
    m_levelText->setPos(boundingRect().width() + 15, -52); // 放在右边一点

    // Weapon Text
    if (m_currentWeapon) {
        m_weaponText->setPlainText(m_currentWeapon->getName());
        m_weaponText->setPos(0, -40);
    }
    if (m_currentArmor) {
        m_armorText->setPlainText(m_currentArmor->getName());
        m_armorText->setPos(0, -52); // 放在武器名称上方一点
    } else {
        m_armorText->setPlainText("装备：无");
        m_armorText->setPos(0, -52); // 放在武器名称上方一点
    }
}

void Player::gainExperience(int amount)
{
    m_currentExp += amount;
    while(m_currentExp >= m_expToNextLevel) {
        levelUp();
    }
}

void Player::levelUp()
{
    m_currentExp -= m_expToNextLevel;
    m_level++;
    m_expToNextLevel = m_level; // 下一级需要 LVL 个经验球

    // 增加最大生命值
    m_maxHealth += PLAYER_MAX_HEALTH_PER_LEVEL;
    qDebug() << "Player" << m_playerNumber << "Leveled Up to" << m_level;
}

void Player::updateAttachments()
{
    // 更新护甲位置
    if (m_currentArmor) {
        m_currentArmor->setPos(0, 0); // 直接覆盖在玩家身上
        m_currentArmor->setZValue(7); // 确保护甲在玩家贴图之上
    }

    // 更新武器位置
    if (m_currentWeapon && m_currentWeapon->pixmap().width() > 0) { // 拳头没有贴图
        QPointF handPos;
        if (m_isCrouching) {
            handPos = QPointF(30, 45); // 下蹲时的手部位置
        } else {
            handPos = QPointF(25, 35); // 站立时的手部位置
        }

        m_currentWeapon->setPos(handPos);
        m_currentWeapon->setZValue(7); // 确保武器在最上层
    }
}

void Player::applyTerrainEffects(const QList<Terrain*>& terrains) {
    m_speedMultiplier = 1.0; // Reset every frame
    bool onGrass = false;

    for (Terrain* terrain : terrains) {
        // AABB check for being "on" the terrain
        QRectF playerFoot = QRectF(x(), y() + boundingRect().height() - 5, boundingRect().width(), 10);
        if (playerFoot.intersects(terrain->boundingRect().translated(terrain->pos()))) {
            if (terrain->getType() == Terrain::Ice) {
                m_speedMultiplier = ICE_SPEED_BOOST_MULTIPLIER;
            }
            if (terrain->getType() == Terrain::Grass) {
                onGrass = true;
            }
        }
    }

    // Invisibility on grass while crouching
    if (onGrass && m_isCrouching) {
        // 旧逻辑: setOpacity(0.3);
        // 新逻辑：完全隐藏玩家及其子项（生命条等）
        setVisible(false);
    } else {
        // 旧逻辑: setOpacity(1.0);
        // 新逻辑：确保玩家是可见的
        setVisible(true);
    }

    // Apply adrenaline boost on top of terrain effects
    if (m_adrenalineTimer && m_adrenalineTimer->isActive()) {
        m_speedMultiplier *= ADRENALINE_SPEED_BOOST_MULTIPLIER;
    }
}

void Player::updateEffects() {
    if (m_adrenalineHealRate > 0) {
        m_health += m_adrenalineHealRate / TARGET_FPS;
        if (m_health > m_maxHealth) m_health = m_maxHealth;
    }
}

void Player::applyAdrenaline(int duration, qreal healPerSecond) {
    if (!m_adrenalineTimer) {
        m_adrenalineTimer = new QTimer(this);
        connect(m_adrenalineTimer, &QTimer::timeout, this, [=](){
            m_adrenalineHealRate = 0;
            qDebug() << "Adrenaline wore off";
            m_adrenalineTimer->stop();
        });
    }
    m_adrenalineHealRate = healPerSecond;
    m_adrenalineTimer->start(duration);
    qDebug() << "Adrenaline activated!";
}


// Getters
qreal Player::getHealth() const { return m_health; }
int Player::getPlayerNumber() const { return m_playerNumber; }
