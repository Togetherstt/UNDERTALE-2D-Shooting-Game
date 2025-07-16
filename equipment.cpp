#include "equipment.h"
#include "customization.h"
#include "player.h"

Equipment::Equipment(EquipmentType type, GameScene* scene, QGraphicsItem *parent)
    : Item(scene, parent), m_type(type)
{
    switch (type) {
    case Bandage:
        setPixmap(QPixmap(ITEM_BANDAGE_SPRITE));
        break;
    case MedKit:
        setPixmap(QPixmap(ITEM_MEDKIT_SPRITE));
        break;
    case Adrenaline:
        setPixmap(QPixmap(ITEM_ADRENALINE_SPRITE));
        break;
    case ExpOrb: // <-- 新增
        setPixmap(QPixmap(ITEM_EXP_ORB_SPRITE));
        break;
    }
}

void Equipment::applyEffect(Player *player) {
    if (!player) return;

    switch(m_type) {
    case Bandage:
        // 旧逻辑: player->takeDamage(-BANDAGE_HEAL_AMOUNT);
        // 新逻辑:
        player->heal(BANDAGE_HEAL_AMOUNT);
        break;
    case MedKit:
        // 旧逻辑: player->takeDamage(-PLAYER_MAX_HEALTH);
        // 新逻辑：直接治疗到满血
        player->heal(player->getMaxHealth());
        break;
    case Adrenaline:
        // This needs to be handled in the Player class via a public method
        player->applyAdrenaline(ADRENALINE_DURATION_MS, ADRENALINE_HEAL_PER_SECOND);
        break;

    case ExpOrb: // <-- 新增
        player->gainExperience(1);
        break;
    }
}
