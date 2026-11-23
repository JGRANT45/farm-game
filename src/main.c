#include "utils.h"
#include "player.h"
#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static void print_menu(void) {
    printf("\n===== Меню =====\n");
    printf("[0] Выход\n");
    printf("[1] Создать/пересоздать персонажа\n");
    printf("[2] Карточка персонажа\n");
    printf("[3] Часы\n");
    printf("[4] Промотать время\n");
    printf("[5] Положить предмет\n");
    printf("[6] Выбросить предмет\n");
    printf("[7] Поиск тяжестей\n");
    printf("[8] Поиск по названию\n");
    printf("[9] Записать в дневник\n");
    printf("[10] Логи\n");
    printf("[11] Отправиться в приключение\n");
    printf("Выберите пункт: ");
}

static void adventure(Player *player, int *day, int *hour) {
    if (player == NULL) { printf("Сначала создайте персонажа.\n"); return; }

    srand((unsigned)time(NULL));

    int depth = 1;
    int jumps = 0, dist = 0, need_portal = 0;
    Map *map = map_create(depth);
    if (!map) return;

    player->x = map->width / 2;
    player->y = map->height / 2;
    map->tiles[player->y][player->x] = T_HERO;

    printf("\n=== Приключение: лес-кольцо ===\n");
    printf("W/A/S/D — шаг, Q — домой. Край телепортирует.\n");
    printf("После %d переходов через край появится портал P.\n\n", JUMPS_NEEDED);

    while (1) {
        printf("\nДальность: %d | Переходов: %d | День %d, %02d:00\n",
               dist, jumps, *day, *hour);
        map_print(map);

        printf("Ход (W/A/S/D/Q): ");
        char c;
        if (scanf(" %c", &c) != 1) { clear_input(); continue; }
        clear_input();

        int dx = 0, dy = 0;
        if      (c == 'w' || c == 'W') dy = -1;
        else if (c == 's' || c == 'S') dy =  1;
        else if (c == 'a' || c == 'A') dx = -1;
        else if (c == 'd' || c == 'D') dx =  1;
        else if (c == 'q' || c == 'Q') { printf("Домой.\n"); break; }
        else { printf("Только W/A/S/D/Q.\n"); continue; }

        int r = map_step(map, player, dx, dy, &jumps);
        if (r == -1) { printf("Препятствие.\n"); continue; }

        if (jumps >= JUMPS_NEEDED && !need_portal) {
            if (put_random_public(map, T_PORTAL)) {
                need_portal = 1;
                printf("\n>>> Открылся портал P!\n");
            }
        }

        if (r == 1) {
            printf("\n>>> Портал! Новая локация...\n");
            map_destroy(map);
            dist++; jumps = 0; need_portal = 0;
            depth++;
            map = map_create(depth);
            if (!map) { printf("[!] Нет памяти.\n"); return; }
            player->x = map->width / 2;
            player->y = map->height / 2;
            map->tiles[player->y][player->x] = T_HERO;
            printf("=== Новый участок (глубина %d) ===\n", depth);
        }
    }

    map_destroy(map);
    printf("Карта освобождена.\n");
}

int main(void) {
    Player *player = NULL;
    int day = START_DAY, hour = START_HOUR;
    int choice = -1;

    load_item_names();

    do {
        print_menu();
        if (scanf("%d", &choice) != 1) { clear_input(); choice = -1; continue; }
        clear_input();

        switch (choice) {
            case 0:
                printf("Выход.\n");
                break;
            case 1: {
                if (player != NULL) {
                    int confirm = 0;
                    if (!read_int("Удалить старого персонажа? (1 — да, 0 — нет): ",
                                  0, 1, &confirm) || !confirm) {
                        printf("Отмена.\n");
                        break;
                    }
                    player_destroy(player);
                }
                player = player_create();
                break;
            }
            case 2: player_print(player); break;
            case 3: show_clock(day, hour); break;
            case 4: pass_time(&day, &hour); break;
            case 5: player_put_item(player); break;
            case 6: player_drop_item(player); break;
            case 7: player_search_heavy(player); break;
            case 8: player_find_by_name(player); break;
            case 9: player_write_diary(player, day, hour); break;
            case 10: variant_process_logs(); break;
            case 11: adventure(player, &day, &hour); break;
            default: printf("Неверный пункт.\n"); break;
        }
    } while (choice != 0);

    if (player != NULL) player_destroy(player);
    return 0;
}