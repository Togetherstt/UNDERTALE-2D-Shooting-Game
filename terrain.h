#ifndef TERRAIN_H
#define TERRAIN_H

#include <QGraphicsRectItem>

class Terrain : public QGraphicsRectItem
{
public:
    enum TerrainType { Land, Grass, Ice };

    Terrain(TerrainType type, qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent = nullptr);

    TerrainType getType() const;

private:
    TerrainType m_type;
};

#endif // TERRAIN_H
