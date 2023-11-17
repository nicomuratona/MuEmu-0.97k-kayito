#pragma once

/* VIEWPORT */
#define MAX_MAIN_VIEWPORT 400

/* OPENGL */
#define Color3f(r, b, g)	((g << 16) + (b << 8) + r)
#define Color4f(r, b, g, o)	((o << 24) + (g << 16) + (b << 8) + r)

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
#define MAX_VOLUME_LEVEL 9