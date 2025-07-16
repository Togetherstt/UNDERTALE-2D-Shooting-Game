#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#include "item.h"

class Equipment : public Item
{
public:
    enum EquipmentType { Bandage, MedKit, Adrenaline, ExpOrb };

    Equipment(EquipmentType type, GameScene* scene, QGraphicsItem* parent = nullptr);
    void applyEffect(Player* player);

private:
    EquipmentType m_type;
};

#endif // EQUIPMENT_H
