#pragma once

#define DWORD_MAX 4294967295

#define Clamp(value, min, max) ((value < min) ? min : (value > max) ? max : value)

/* VIEWPORT */
#define MAX_MAIN_VIEWPORT 400
#define REAL_WIDTH(width) ((width) * WindowWidth / 640)
#define REAL_HEIGHT(height) ((height) * WindowHeight / 480)

/* TERRAIN */
#define TERRAIN_SCALE 100.0f
#define TERRAIN_SIZE 256
#define TERRAIN_SIZE_MASK 255

/* OPENGL */
#define Color3b(r, b, g)	((g << 16) + (b << 8) + r)
#define Color4b(r, b, g, o)	((o << 24) + (g << 16) + (b << 8) + r)

/* ITEM */
#define MAX_ITEM_TYPE 32
#define MAX_ITEM_SECTION 16
#define MAX_ITEM (MAX_ITEM_SECTION * MAX_ITEM_TYPE)
#define ITEM_BASE_MODEL 400
#define GET_ITEM(x,y) (((x) * MAX_ITEM_TYPE) + (y))
#define GET_ITEM_MODEL(x,y) ((((x) * MAX_ITEM_TYPE) + (y)) + ITEM_BASE_MODEL)

#define MAX_SPECIAL_OPTION 9

#define MAX_ITEM_PRICE 2000000000

#define INVENTORY_WEAR_SIZE 12
#define INVENTORY_COLUMN_SIZE 8
#define INVENTORY_ROW_SIZE 8
#define INVENTORY_ITEM_SIZE (INVENTORY_COLUMN_SIZE * INVENTORY_ROW_SIZE)
#define INVENTORY_MAX_SIZE (INVENTORY_WEAR_SIZE + INVENTORY_ITEM_SIZE)

#define INVENTORY_WEAR_RANGE(x) (((x) < 0) ? 0 : ((x) >= INVENTORY_WEAR_SIZE) ? 0 : 1)
#define INVENTORY_MAX_RANGE(x) (((x) < 0) ? 0 : ((x) >= INVENTORY_MAX_SIZE) ? 0 : 1)

#define MAX_ITEM_INFO 4

#define GET_ITEM_OPT_LEVEL(x)	((x >> 3) & 0xF) // GET_ITEM_OPT_LEVEL(TargetItem->Level)
#define GET_ITEM_OPT_DUR(x)	((x < 0) ? (x + 256) : x) // GET_ITEM_OPT_DUR(TargetItem->Durability)
#define GET_ITEM_OPT_SKILL(x)	((x >> 7) & 1) // GET_ITEM_OPT_SKILL(TargetItem->Level)
#define GET_ITEM_OPT_LUCK(x)	((x >> 2) & 1) // GET_ITEM_OPT_LUCK(TargetItem->Level)
#define GET_ITEM_OPT_OPT(x,y)	((x & 3) + ((y & 64) >> 4)) // GET_ITEM_OPT_OPT(TargetItem->Level, TargetItem->Option1)
#define GET_ITEM_OPT_EXC(x)	(x & 0x3F) // GET_ITEM_OPT_EXC(TargetItem->Option1)

/* WNDPROC */
#define WM_AUTOCLICKTIMER 0x01
#define WM_NPROTECT_EXIT_TWO (WM_USER + 0x2711)
#define WINDOWMINIMIZED_TIMER 1001
#define WM_TRAY_MODE_ICON (WM_USER+100)
#define WM_TRAY_MODE_MESSAGE (WM_USER+101)
#define HACK_TIMER 1000

/* CHARACTER */
#define MAX_CLASS 4
#define MAX_RESISTANCE 4
#define MAX_CHARACTER_LEVEL 1000
#define MAX_CHARACTER_ZEN 2000000000

/* MONSTER */
#define MONSTER_BASE_MODEL 270
#define MAX_MONSTER 256

/* BUFFS */
#define FindEffect(x,y) ((((x) & (y)) == (y)) ? 1 : 0)
#define AddEffect(x,y) (x |= y)
#define DelEffect(x,y) (x &= ~y)

/* SOUND */
#define MAX_SOUND_LEVEL 9
#define MAX_MUSIC_LEVEL 9

/* MODELS */
#define MAX_TEXTURE 5500
#define PLUS_TEXTURE 5500
#define TOTAL_TEXTURE (MAX_TEXTURE + PLUS_TEXTURE)

#define MAX_MODELS 963
#define PLUS_MODELS 256
#define TOTAL_MODELS (MAX_MODELS + PLUS_MODELS)

#define MAX_BITMAP_FILE_NAME 32

/* MAP */
#define MAX_MAPS 32
#define MAP_RANGE(x) ((x < 0 || x >= MAX_MAPS) ? false : true)
#define MAX_GATES 100

/* Interfaces */
#define CheckRightInterfaces() (InventoryOpened || CharacterOpened || GuildOpened || PartyOpened || GoldenArcherOpenType || GuildCreatorOpened || EventWindowOpened || QuestNPCOpened)
#define CheckInputInterfaces() (InputEnable || TabInputEnable || GoldInputEnable || GuildInputEnable)

/* ServerList */
#define MAX_SERVER_HI 25
#define MAX_SERVER_LO 20