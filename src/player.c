#include "player.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Player *player_create(void) {
    Player *p = (Player *)malloc(sizeof(Player));
    if (p == NULL) { printf("[!] Нет памяти под игрока.\n"); return NULL; }
    printf("Введите имя фермера: ");
    if (scanf("%31s", p->name) != 1) strcpy(p->name, "Безымянный");
    clear_input();
    for (int i = 0; i < INVENTORY_SIZE; i++) p->inventory[i] = SLOT_EMPTY;
    return p;
}

void player_destroy(Player *p) {
    if (p == NULL) return;
    printf("Память структуры по адресу %p освобождена\n", (void *)p);
    free(p);
}

void player_print(const Player *p) {
    if (p == NULL) { printf("Персонаж не создан.\n"); return; }
    printf("\n=== Карточка: %s ===\n", p->name);
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        printf("Слот %d: [%d] - %s\n", i, p->inventory[i], get_item_name(p->inventory[i]));
    }
}

void player_put_item(Player *p) {
    if (p == NULL) { printf("Сначала создайте персонажа.\n"); return; }
    int slot = 0, id = 0;
    if (!read_int("Введите индекс слота (0-9): ", MIN_SLOT, MAX_SLOT, &slot)) return;
    if (!read_int("Введите ID предмета (0-9): ", MIN_ITEM_ID, MAX_ITEM_ID, &id)) return;
    p->inventory[slot] = id;
    printf("Предмет [%d] (%s) положен в слот %d.\n", id, get_item_name(id), slot);
}

void player_drop_item(Player *p) {
    if (p == NULL) { printf("Сначала создайте персонажа.\n"); return; }
    int slot = 0;
    if (!read_int("Введите индекс слота (0-9): ", MIN_SLOT, MAX_SLOT, &slot)) return;
    if (p->inventory[slot] == SLOT_EMPTY) {
        printf("Слот %d уже пуст.\n", slot);
        return;
    }
    printf("Из слота %d выброшен [%d] (%s).\n",
           slot, p->inventory[slot], get_item_name(p->inventory[slot]));
    p->inventory[slot] = SLOT_EMPTY;
}

void player_search_heavy(const Player *p) {
    if (p == NULL) { printf("Сначала создайте персонажа.\n"); return; }
    int a = 0;
    if (!read_int("Введите ID предмета a (0-9): ", MIN_ITEM_ID, MAX_ITEM_ID, &a)) return;
    printf("Слоты с ID строго больше %d:\n", a);
    int found = 0;
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        if (p->inventory[i] > a) {
            printf("  Слот %d: [%d] (%s)\n", i, p->inventory[i], get_item_name(p->inventory[i]));
            found = 1;
        }
    }
    if (!found) printf("  Таких предметов нет.\n");
}

void player_find_by_name(const Player *p) {
    if (p == NULL) { printf("Сначала создайте персонажа.\n"); return; }
    char name[MAX_NAME_LEN];
    printf("Введите название предмета: ");
    if (scanf("%31s", name) != 1) { clear_input(); return; }
    clear_input();

    int target_id = -1;
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (strcmp(get_item_name(i), name) == 0) { target_id = i; break; }
    }
    if (target_id == -1) { printf("Название '%s' не найдено.\n", name); return; }

    printf("Предмет '%s' (ID %d). Слоты: ", name, target_id);
    int found = 0;
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        if (p->inventory[i] == target_id) { printf("%d ", i); found = 1; }
    }
    if (!found) printf("(в рюкзаке нет)");
    printf("\n");
}

void player_write_diary(const Player *p, int day, int hour) {
    if (p == NULL) { printf("Сначала создайте персонажа.\n"); return; }
    FILE *f = fopen(FILE_DIARY, "a");
    if (f == NULL) { printf("[!] Не удалось открыть %s\n", FILE_DIARY); return; }

    fprintf(f, "=== Дневник: %s ===\n", p->name);
    fprintf(f, "Время: День %d, %02d:00\n", day, hour);
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        if (p->inventory[i] != SLOT_EMPTY)
            fprintf(f, "  Слот %d: %s\n", i, get_item_name(p->inventory[i]));
    }
    fprintf(f, "====================\n\n");
    fclose(f);
    printf("Сохранено в %s\n", FILE_DIARY);
}