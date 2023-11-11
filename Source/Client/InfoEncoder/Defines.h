#pragma once

/* ITEMS */

#define MAX_ITEM_TYPE 32
#define MAX_ITEM_SECTION 16
#define MAX_ITEM (MAX_ITEM_SECTION * MAX_ITEM_TYPE)

#define MAX_ITEM_OPTION_INFO 4000

#define GET_ITEM(x,y) (((x) * MAX_ITEM_TYPE) + (y))

/* MAPS */

#define MAX_MAPS 32