#include <stdio.h>

#define INVENTORY_SIZE 10
#define SLOT_EMPTY     0

#define ITEM_WOOD      1
#define ITEM_STONE     2
#define ITEM_SEEDS     3
#define ITEM_IRON      4
#define ITEM_GOLD      5
#define ITEM_APPLE     6
#define ITEM_BREAD     7
#define ITEM_WATER     8
#define ITEM_TOOL      9

#define START_DAY      1
#define START_HOUR     8
#define HOURS_IN_DAY   24

int main(void)
{
    int current_day  = START_DAY;
    int current_hour = START_HOUR;

    int inventory[INVENTORY_SIZE] = {
        ITEM_WOOD, ITEM_STONE, ITEM_SEEDS, ITEM_IRON, ITEM_GOLD,
        ITEM_APPLE, ITEM_BREAD, ITEM_WATER, ITEM_TOOL, SLOT_EMPTY
    };

    /* TODO: меню и функции */

    return 0;
}