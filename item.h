#ifndef ITEM_H
#define ITEM_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include "gamescene.h"

class Player;
class Terrain;

// Abstract base class for all droppable items
class Item : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Item(GameScene* scene, QGraphicsItem *parent = nullptr);

    void update(const QList<Terrain*>& terrains);
    void setOwner(Player* player);

protected:
    void applyGravity();
    void handleCollisions(const QList<Terrain*>& terrains);

    Player* m_owner;
    GameScene* m_scene;
    qreal m_velocityY;
};

#endif // ITEM_H
