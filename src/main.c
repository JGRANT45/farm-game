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

static const char *item_name(int id)
{
    switch (id) {
        case SLOT_EMPTY: return "Пусто";
        case ITEM_WOOD:  return "Дерево";
        case ITEM_STONE: return "Камень";
        case ITEM_SEEDS: return "Семена";
        case ITEM_IRON:  return "Железо";
        case ITEM_GOLD:  return "Золото";
        case ITEM_APPLE: return "Яблоко";
        case ITEM_BREAD: return "Хлеб";
        case ITEM_WATER: return "Вода";
        case ITEM_TOOL:  return "Инструмент";
        default:         return "Неизвестно";
    }
}

static void print_inventory(const int inv[INVENTORY_SIZE])
{
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        printf("Слот %d: [%d] (%s)\n", i, inv[i], item_name(inv[i]));
    }
}

static void show_clock(int day, int hour)
{
    printf("Текущее время: День %d, %02d:00\n", day, hour);
}

static void pass_time(int *day, int *hour)
{
    int worked = 0;
    printf("Сколько часов хотите потратить на работу? ");
    if (scanf("%d", &worked) != 1) {
        printf("Ошибка ввода.\n");
        while (getchar() != '\n');
        return;
    }
    *hour += worked;
    while (*hour >= HOURS_IN_DAY) {
        *hour -= HOURS_IN_DAY;
        (*day)++;
    }
    printf("Новое время: День %d, %02d:00\n", *day, *hour);
}

static void print_menu(void)
{
    printf("\n===== МЕНЮ =====\n");
    printf("[0] Выход\n");
    printf("[1] Посмотреть на часы\n");
    printf("[2] Промотать время (Поработать)\n");
    printf("[3] Посмотреть инвентарь\n");
    printf("Выберите пункт: ");
}

int main(void)
{
    int current_day  = START_DAY;
    int current_hour = START_HOUR;

    int inventory[INVENTORY_SIZE] = {
        ITEM_WOOD, ITEM_STONE, ITEM_SEEDS, ITEM_IRON, ITEM_GOLD,
        ITEM_APPLE, ITEM_BREAD, ITEM_WATER, ITEM_TOOL, SLOT_EMPTY
    };

    int choice = -1;

    do {
        print_menu();
        if (scanf("%d", &choice) != 1) {
            printf("Ошибка: введите число.\n");
            while (getchar() != '\n');
            choice = -1;
            continue;
        }
        while (getchar() != '\n');

        switch (choice) {
            case 0: printf("Выход.\n"); break;
            case 1: show_clock(current_day, current_hour); break;
            case 2: pass_time(&current_day, &current_hour); break;
            case 3: print_inventory(inventory); break;
            default: printf("Неверный пункт.\n"); break;
        }
    } while (choice != 0);

    return 0;
}