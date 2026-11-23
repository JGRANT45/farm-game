#include "map.h"
#include "player.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static int rnd(int min, int max) {
    return min + rand() % (max - min + 1);
}

static char **alloc_tiles(int h, int w) {
    char **t = (char **)malloc(h * sizeof(char *));
    if (!t) return NULL;
    for (int i = 0; i < h; i++) {
        t[i] = (char *)malloc(w * sizeof(char));
        if (!t[i]) {
            while (i > 0) free(t[--i]);
            free(t);
            return NULL;
        }
    }
    return t;
}

static void free_tiles(char **t, int h) {
    if (!t) return;
    for (int i = 0; i < h; i++) free(t[i]);
    free(t);
}

static int put_random(char **t, int h, int w, char sym) {
    for (int i = 0; i < 200; i++) {
        int y = rnd(0, h - 1), x = rnd(0, w - 1);
        if (t[y][x] == T_EMPTY) { t[y][x] = sym; return 1; }
    }
    return 0;
}

Map *map_create(int depth) {
    Map *m = (Map *)malloc(sizeof(Map));
    if (!m) { printf("[!] Нет памяти под карту.\n"); return NULL; }

    m->width  = rnd(MAP_MIN, MAP_MAX);
    m->height = rnd(MAP_MIN, MAP_MAX);
    m->depth  = depth;

    m->tiles = alloc_tiles(m->height, m->width);
    if (!m->tiles) {
        printf("[!] Нет памяти под клетки.\n");
        free(m);
        return NULL;
    }

    int pct = rnd(OBST_MIN, OBST_MAX);
    for (int y = 0; y < m->height; y++) {
        for (int x = 0; x < m->width; x++) {
            int r = rnd(1, 100);
            if (r <= pct) {
                int k = rnd(1, 100);
                m->tiles[y][x] = (k <= 60) ? T_TREE : (k <= 90) ? T_ROCK : T_WALL;
            } else {
                m->tiles[y][x] = T_EMPTY;
            }
        }
    }
    return m;
}

void map_destroy(Map *m) {
    if (m == NULL) return;
    free_tiles(m->tiles, m->height);
    printf("Память структуры по адресу %p освобождена\n", (void *)m);
    free(m);
}

void map_print(const Map *m) {
    if (m == NULL) return;
    for (int y = 0; y < m->height; y++) {
        for (int x = 0; x < m->width; x++) putchar(m->tiles[y][x]);
        putchar('\n');
    }
}

int map_step(Map *m, Player *p, int dx, int dy, int *jumps) {
    if (m == NULL || p == NULL) return -1;

    int nx = p->x + dx, ny = p->y + dy;
    int wrapped = 0;

    if (nx < 0)        { nx += m->width;  wrapped = 1; }
    else if (nx >= m->width)  { nx -= m->width;  wrapped = 1; }
    if (ny < 0)        { ny += m->height; wrapped = 1; }
    else if (ny >= m->height) { ny -= m->height; wrapped = 1; }

    char t = m->tiles[ny][nx];
    if (t == T_WALL || t == T_TREE || t == T_ROCK) return -1;

    m->tiles[p->y][p->x] = T_EMPTY;
    m->tiles[ny][nx]     = T_HERO;
    p->x = nx;
    p->y = ny;
    if (wrapped) (*jumps)++;

    return (t == T_PORTAL) ? 1 : 0;
}