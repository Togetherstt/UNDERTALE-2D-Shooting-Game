#ifndef CUSTOMIZATION_H
#define CUSTOMIZATION_H

#include <QString>
#include <QColor>
#include <QIcon>

// --- GAMEPLAY PARAMETERS --- //
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int TARGET_FPS = 60;
const qreal GRAVITY = 0.5;
const int GAME_OVER_DELAY_MS = 3000; // 3 seconds before restart option appears

// --- LEVELING PARAMETERS --- //
const int PLAYER_MAX_HEALTH_PER_LEVEL = 20;

// --- PLAYER PARAMETERS --- //
const qreal PLAYER_MAX_HEALTH = 100.0;
const qreal PLAYER_MOVE_SPEED = 5.0;
const qreal PLAYER_JUMP_FORCE = -12.0;
const qreal ADRENALINE_SPEED_BOOST_MULTIPLIER = 1.5;
const qreal ICE_SPEED_BOOST_MULTIPLIER = 2.0;

// --- WEAPON PARAMETERS --- //
// Fist
const qreal FIST_DAMAGE = 8.0;
const int FIST_RANGE = 40;
const int FIST_COOLDOWN_MS = 300;
// Knife
const qreal KNIFE_DAMAGE = 12.0;
const int KNIFE_RANGE = 50;
const int KNIFE_COOLDOWN_MS = 500;
// Solid Ball
const qreal SOLID_BALL_DAMAGE = 20.0;
const int SOLID_BALL_AMMO_PER_PICKUP = 5;
const int SOLID_BALL_COOLDOWN_MS = 1000;
// Rifle
const qreal RIFLE_BULLET_DAMAGE = 6.0;
const qreal RIFLE_BULLET_SPEED = 20.0;
const int RIFLE_AMMO_PER_PICKUP = 30;
const int RIFLE_COOLDOWN_MS = 150;
// Sniper Rifle
const qreal SNIPER_BULLET_DAMAGE = 30.0;
const qreal SNIPER_BULLET_SPEED = 40.0;
const int SNIPER_AMMO_PER_PICKUP = 5;
const int SNIPER_COOLDOWN_MS = 2000;

// --- EQUIPMENT PARAMETERS --- //
const qreal BANDAGE_HEAL_AMOUNT = 20.0;
const qreal ADRENALINE_DURATION_MS = 10000; // 10 seconds
const qreal ADRENALINE_HEAL_PER_SECOND = 5.0;

// --- ARMOR PARAMETERS (OPTIONAL) --- //
const qreal KNIFE_DAMAGE_REDUCTION_LIGHT_ARMOR = 0.5; // 50% reduction
const qreal BULLET_DAMAGE_ABSORPTION_VEST = 0.5; // Vest takes 60% of damage
const qreal BULLETPROOF_VEST_DURABILITY = 100.0;


// --- ASSET FILE PATHS --- //
// NOTE: All paths should be relative to the resource file (.qrc)

// Characters
const QString PLAYER1_STAND_SPRITE = ":/characters/assets/player1_stand.png";
const QString PLAYER1_CROUCH_SPRITE = ":/characters/assets/player1_crouch.png";
const QString PLAYER1_JUMP_SPRITE = ":/characters/assets/player1_jump.png";

const QString PLAYER2_STAND_SPRITE = ":/characters/assets/player2_stand.png";
const QString PLAYER2_CROUCH_SPRITE = ":/characters/assets/player2_crouch.png";
const QString PLAYER2_JUMP_SPRITE = ":/characters/assets/player2_jump.png";

// Weapons
const QString WEAPON_KNIFE_SPRITE = ":/items/assets/weapon_knife.png";
const QString WEAPON_RIFLE_SPRITE = ":/items/assets/weapon_rifle.png";
const QString WEAPON_SNIPER_SPRITE = ":/items/assets/weapon_sniper.png";
const QString WEAPON_SOLIDBALL_SPRITE = ":/items/assets/weapon_solidball.png";

// Projectiles
const QString PROJECTILE_BULLET_SPRITE = ":/projectiles/assets/bullet.png";
const QString PROJECTILE_SOLIDBALL_SPRITE = ":/projectiles/assets/solid_ball.png";
const QString PROJECTILE_GASTER_BLASTER_SPRITE= ":/projectiles/assets/gaster_blaster.png";

// Equipment & Armor
const QString ITEM_BANDAGE_SPRITE = ":/items/assets/equip_bandage.png";
const QString ITEM_MEDKIT_SPRITE = ":/items/assets/equip_medkit.png";
const QString ITEM_ADRENALINE_SPRITE = ":/items/assets/equip_adrenaline.png";
const QString ITEM_LIGHT_ARMOR_SPRITE = ":/items/assets/armor_light.png";
const QString ITEM_VEST_SPRITE = ":/items/assets/armor_vest.png";
const QString ITEM_EXP_ORB_SPRITE = ":/items/assets/exp_orb.png";

// Terrain
const QString TERRAIN_LAND_SPRITE = ":/terrain/assets/land(true).png";
const QString TERRAIN_GRASS_SPRITE = ":/terrain/assets/grass.png";
const QString TERRAIN_ICE_SPRITE = ":/terrain/assets/ice.png";

// Background
const QString BACKGROUND_SPRITE = ":/background/assets/background.png";

// Icon
const QString GAME_ICON = ":/icon/assets/game_icon.ico";

#endif // CUSTOMIZATION_H
