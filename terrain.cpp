#include "terrain.h"
#include "customization.h"
#include <QBrush>
#include <QPixmap>
#include <QPen>

Terrain::Terrain(TerrainType type, qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent)
    // 核心修改：总是在局部坐标(0,0)创建矩形
    : QGraphicsRectItem(0, 0, width, height, parent), m_type(type)
{
    // 核心修改：然后使用 setPos 将整个对象移动到目标位置
    setPos(x, y);
    QPixmap texture;
    switch (type) {
        case Land:
            texture.load(TERRAIN_LAND_SPRITE);
            break;
        case Grass:
            texture.load(TERRAIN_GRASS_SPRITE);
            break;
        case Ice:
            texture.load(TERRAIN_ICE_SPRITE);
            break;
        }

    // Set tiled texture brush
    QBrush brush(texture);
    setBrush(brush);
    setPen(Qt::NoPen); // No border
    setZValue(1); // Make sure it's behind the player
}

Terrain::TerrainType Terrain::getType() const
{
    return m_type;
}
