#pragma once

/* VIEWPORT */
#define MAX_MAIN_VIEWPORT 400
#define REAL_WIDTH(width) ((width) * WindowWidth / 640)
#define REAL_HEIGHT(height) ((height) * WindowHeight / 480)

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

#define MAX_SPECIAL_OPTION 8

#define MAX_ITEM_PRICE 2000000000

#define INVENTORY_WEAR_SIZE 12

#define INVENTORY_WEAR_RANGE(x) (((x) < 0) ? 0 : ((x) >= INVENTORY_WEAR_SIZE) ? 0 : 1)

/* WNDPROC */
#define WM_AUTOCLICKTIMER 0x01

/* CHARACTER */
#define MAX_CLASS 4
#define MAX_RESISTANCE 4
#define MAX_CHARACTER_LEVEL 400

/* BUFFS */
#define FindEffect(x,y) ((((x) & (y)) == (y)) ? 1 : 0)
#define AddBuff(x,y) (x |= y)
#define DelBuff(x,y) (x &= ~y)

/* SOUND */
#define MAX_SOUND_LEVEL 9
#define MAX_MUSIC_LEVEL 9

/* MODELS */
#define MAX_TEXTURE 1450
#define PLUS_TEXTURE 512
#define TOTAL_TEXTURE (MAX_TEXTURE + PLUS_TEXTURE)

/* WNDProc */
#define WM_NPROTECT_EXIT_TWO (WM_USER + 0x2711)
#define WINDOWMINIMIZED_TIMER 1001