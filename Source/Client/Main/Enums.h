#pragma once

/* MAP NUMBERS */

enum eMapNumber
{
	MAP_LORENCIA = 0,
	MAP_DUNGEON = 1,
	MAP_DEVIAS = 2,
	MAP_NORIA = 3,
	MAP_LOST_TOWER = 4,
	MAP_EXILE = 5,
	MAP_ARENA = 6,
	MAP_ATLANS = 7,
	MAP_TARKAN = 8,
	MAP_DEVIL_SQUARE1 = 9,
	MAP_ICARUS = 10,
	MAP_BLOOD_CASTLE1 = 11,
	MAP_BLOOD_CASTLE2 = 12,
	MAP_BLOOD_CASTLE3 = 13,
	MAP_BLOOD_CASTLE4 = 14,
	MAP_BLOOD_CASTLE5 = 15,
	MAP_BLOOD_CASTLE6 = 16
};

/* BUFFS */

enum eEffectState
{
	STATE_POISON = 0x0001, // Poisoned.
	STATE_FREEZE = 0x0002, // Ice stuck.
	STATE_ATTACK = 0x0004, // Increase attack power.
	STATE_DEFENSE = 0x0008, // Increase defense.
	STATE_ADD_LIFE = 0x0010, // Increase health.
	STATE_HARDEN = 0x0020, // Petrification.
	STATE_REDUCE_DEFENSE = 0x0040, // Decrease defense (magic swordsman) (use monster skills as well)
	STATE_REDUCE_ATTACKDAMAGE = 0x0080, // Attack reduction (Mage)
	STATE_REDUCE_MAGICDEFENSE = 0x0100, // Increase defense (Mage)
	STATE_REDUCE_MAGICPOWER = 0x0200, // Decrease magic ability (monster skill)
};

/* ITEM OPTIONS */

enum eSpecialNumber
{
	SPECIAL_SKILL_OPTION = 0, // SKILL
	SPECIAL_LUCK_OPTION = 1, // LUCK
	SPECIAL_ADDITIONAL_OPTION = 2, // ADDOPTION
	SPECIAL_EXCELLENT1 = 3,
	SPECIAL_EXCELLENT2 = 4,
	SPECIAL_EXCELLENT3 = 5,
	SPECIAL_EXCELLENT4 = 6,
	SPECIAL_EXCELLENT5 = 7,
	SPECIAL_EXCELLENT6 = 8,
};

enum eItemOption
{
	ITEM_OPTION_ADD_PHYSI_DAMAGE = 60,
	ITEM_OPTION_ADD_MAGIC_DAMAGE = 61,
	ITEM_OPTION_ADD_DEFENSE_SUCCESS_RATE = 62,
	ITEM_OPTION_ADD_DEFENSE = 63,
	ITEM_OPTION_ADD_CRITICAL_DAMAGE_RATE = 64,
	ITEM_OPTION_ADD_HP_RECOVERY_RATE = 65,
	ITEM_OPTION_MUL_HP = 66,
	ITEM_OPTION_MUL_MP = 67,
	ITEM_OPTION_ADD_DAMAGE_REDUCTION = 68,
	ITEM_OPTION_ADD_DAMAGE_REFLECT = 69,
	ITEM_OPTION_MUL_DEFENSE_SUCCESS_RATE = 70,
	ITEM_OPTION_ADD_MONEY_AMOUNT_DROP_RATE = 71,
	ITEM_OPTION_ADD_EXCELLENT_DAMAGE_RATE = 72,
	ITEM_OPTION_ADD_PHYSI_DAMAGE_BY_LEVEL = 73,
	ITEM_OPTION_MUL_PHYSI_DAMAGE = 74,
	ITEM_OPTION_ADD_MAGIC_DAMAGE_BY_LEVEL = 75,
	ITEM_OPTION_MUL_MAGIC_DAMAGE = 76,
	ITEM_OPTION_ADD_SPEED = 77,
	ITEM_OPTION_ADD_HUNT_HP = 78,
	ITEM_OPTION_ADD_HUNT_MP = 79,
	ITEM_OPTION_ADD_WING_HP = 80,
	ITEM_OPTION_ADD_WING_MP = 81,
	ITEM_OPTION_ADD_IGNORE_DEFENSE_RATE = 82,
	ITEM_OPTION_ADD_BP = 83,
	ITEM_OPTION_MUL_BP = 84,
};

/* ITEM SLOTS */

enum eEquipment
{
	EQUIPMENT_WEAPON_RIGHT = 0,
	EQUIPMENT_WEAPON_LEFT = 1,
	EQUIPMENT_HELM = 2,
	EQUIPMENT_ARMOR = 3,
	EQUIPMENT_PANTS = 4,
	EQUIPMENT_GLOVES = 5,
	EQUIPMENT_BOOTS = 6,
	EQUIPMENT_WING = 7,
	EQUIPMENT_HELPER = 8,
	EQUIPMENT_AMULET = 9,
	EQUIPMENT_RING_RIGHT = 10,
	EQUIPMENT_RING_LEFT = 11,
};

/* Text aligns enum */
enum eTextAligns
{
	RT3_SORT_LEFT = 0,		// Left align
	RT3_SORT_CENTER = 1,		// Center align
};

/* Tooltip Text Colors */
enum eTooltipTextColor
{
	TEXT_COLOR_WHITE = 0,
	TEXT_COLOR_BLUE = 1,
	TEXT_COLOR_RED = 2,
	TEXT_COLOR_YELLOW = 3,
	TEXT_COLOR_GREEN = 4,
	TEXT_COLOR_DARKRED = 5,
	TEXT_COLOR_PURPLE = 6,
	TEXT_COLOR_DARKBLUE = 7,
	TEXT_COLOR_DARKYELLOW = 8,
	TEXT_COLOR_GREEN_BLUE = 9,
	TEXT_COLOR_GRAY = 10,
	TEXT_COLOR_REDPURPLE = 11,
};

/* Resolutions */
enum eResolutionValues
{
	// 4:3
	R640x480 = 0,
	R800x600,
	R1024x768,
	R1280x960,
	// 16:9
	R1280x720,
	R1366x768,
	R1600x900,
	R1920x1080,
	MAX_RESOLUTION_VALUE
};

/* Window Mode */
enum eWindowModes
{
	FULL_SCREEN = 0,
	WINDOW_MODE,
	BORDERLESS,
	MAX_WINDOW_MODE
};

/* Scene Flags */
enum eSceneFlags
{
	NON_SCENE = 0,
	WEBZEN_SCENE,
	LOG_IN_SCENE,
	LOADING_SCENE,
	CHARACTER_SCENE, 
	MAIN_SCENE
};

/* Skills */
enum eSkillNumber
{
	SKILL_NONE = 0,
	SKILL_POISON = 1,
	SKILL_METEORITE = 2,
	SKILL_LIGHTNING = 3,
	SKILL_FIRE_BALL = 4,
	SKILL_FLAME = 5,
	SKILL_TELEPORT = 6,
	SKILL_ICE = 7,
	SKILL_TWISTER = 8,
	SKILL_EVIL_SPIRIT = 9,
	SKILL_HELL_FIRE = 10,
	SKILL_POWER_WAVE = 11,
	SKILL_AQUA_BEAM = 12,
	SKILL_BLAST = 13,
	SKILL_INFERNO = 14,
	SKILL_TELEPORT_ALLY = 15,
	SKILL_MANA_SHIELD = 16,
	SKILL_ENERGY_BALL = 17,
	SKILL_DEFENSE = 18,
	SKILL_FALLING_SLASH = 19,
	SKILL_LUNGE = 20,
	SKILL_UPPERCUT = 21,
	SKILL_CYCLONE = 22,
	SKILL_SLASH = 23,
	SKILL_TRIPLE_SHOT = 24,
	SKILL_HEAL = 26,
	SKILL_GREATER_DEFENSE = 27,
	SKILL_GREATER_DAMAGE = 28,
	SKILL_SUMMON1 = 30,
	SKILL_SUMMON2 = 31,
	SKILL_SUMMON3 = 32,
	SKILL_SUMMON4 = 33,
	SKILL_SUMMON5 = 34,
	SKILL_SUMMON6 = 35,
	SKILL_SUMMON7 = 36,
	SKILL_TWISTING_SLASH = 41,
	SKILL_RAGEFUL_BLOW = 42,
	SKILL_DEATH_STAB = 43,
	SKILL_IMPALE = 47,
	SKILL_GREATER_LIFE = 48,
	SKILL_FIRE_BREATH = 49,
	SKILL_MONSTER_AREA_ATTACK = 50,
	SKILL_ICE_ARROW = 51,
	SKILL_PENETRATION = 52,
	SKILL_FIRE_SLASH = 55,
	SKILL_POWER_SLASH = 56,
};