#ifndef MAP_H
#define MAP_H

#include "player.h"

/* ==== Размеры и параметры ==== */
#define MAP_MIN       5
#define MAP_MAX       30
#define OBST_MIN      15
#define OBST_MAX      30
#define JUMPS_NEEDED  3

/* ==== Тайлы ==== */
#define T_EMPTY  '.'
#define T_TREE   'T'
#define T_ROCK   'R'
#define T_WALL   '#'
#define T_HERO   '@'
#define T_PORTAL 'P'

typedef struct Map {
    char **tiles;
    int    width;
    int    height;
    int    depth;
} Map;

/* Жизненный цикл */
Map  *map_create(int depth);
void  map_destroy(Map *m);

/* Печать */
void  map_print(const Map *m);

/* Движение: -1 блок, 0 шаг, 1 портал */
int   map_step(Map *m, Player *p, int dx, int dy, int *jumps);

#endif /* MAP_H */