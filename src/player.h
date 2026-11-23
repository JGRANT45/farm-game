#ifndef PLAYER_H
#define PLAYER_H

#include "utils.h"

typedef struct Player {
    char name[MAX_NAME_LEN];
    int  inventory[INVENTORY_SIZE];
} Player;

/* Жизненный цикл */
Player *player_create(void);
void    player_destroy(Player *p);
void    player_print(const Player *p);

/* Инвентарь */
void player_put_item(Player *p);
void player_drop_item(Player *p);
void player_search_heavy(const Player *p);
void player_find_by_name(const Player *p);

/* Дневник */
void player_write_diary(const Player *p, int day, int hour);

#endif /* PLAYER_H */