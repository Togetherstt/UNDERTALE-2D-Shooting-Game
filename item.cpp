#include "item.h"
#include "customization.h"
#include "terrain.h"

Item::Item(GameScene* scene, QGraphicsItem *parent)
    : QObject(), QGraphicsPixmapItem(parent), m_owner(nullptr), m_scene(scene), m_velocityY(0)
{
    setZValue(3); // Below players
}

void Item::setOwner(Player* player) {
    m_owner = player;
}

void Item::update(const QList<Terrain*>& terrains) {
    // Items on the ground don't need updates
    if (m_owner) return;

    applyGravity();
    setPos(x(), y() + m_velocityY);
    handleCollisions(terrains);
}

void Item::applyGravity() {
    m_velocityY += GRAVITY;
}

void Item::handleCollisions(const QList<Terrain*>& terrains) {

    bool onPlatform = false;
    for (Terrain* terrain : terrains) {
        if (collidesWithItem(terrain)) {
            if (m_velocityY > 0) {
                setY(terrain->y() - boundingRect().height()-1);
                m_velocityY = 0;
                onPlatform = true;
                break; // 物品落在平台上后就停止
            }
        }
    }/*
    if (!onPlatform && (y() + boundingRect().height() > scene()->height())) {
        setY(scene()->height() - boundingRect().height());
        if (m_velocityY > 0) {
            m_velocityY = 0;
        }
    }*/
}
