#include <stdio.h>
#include <string.h>

#define INVENTORY_SIZE 10
#define MAX_ITEMS      10
#define MAX_NAME_LEN   32
#define MAX_LINE_LEN   256

#define SLOT_EMPTY     0
#define START_DAY      1
#define START_HOUR     8
#define HOURS_IN_DAY   24
#define MIN_SLOT       0
#define MAX_SLOT       (INVENTORY_SIZE - 1)
#define MIN_ITEM_ID    0
#define MAX_ITEM_ID    9

#define FILE_ITEMS     "items.txt"

char item_names[MAX_ITEMS][MAX_NAME_LEN];

static void clear_input(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

static int read_int(const char *prompt, int min, int max, int *out) {
    int value;
    printf("%s", prompt);
    if (scanf("%d", &value) != 1) {
        printf("Ошибка: нужно ввести целое число.\n");
        clear_input();
        return 0;
    }
    clear_input();
    if (value < min || value > max) {
        printf("Ошибка: диапазон [%d; %d].\n", min, max);
        return 0;
    }
    *out = value;
    return 1;
}

static void load_item_names(void) {
    FILE *f = fopen(FILE_ITEMS, "r");
    if (f == NULL) {
        printf("[!] Файл %s не найден. Использую стандартные названия.\n", FILE_ITEMS);
        strcpy(item_names[0], "Пусто");
        strcpy(item_names[1], "Дерево");
        strcpy(item_names[2], "Камень");
        strcpy(item_names[3], "Семена");
        strcpy(item_names[4], "Железо");
        strcpy(item_names[5], "Золото");
        strcpy(item_names[6], "Яблоко");
        strcpy(item_names[7], "Хлеб");
        strcpy(item_names[8], "Вода");
        strcpy(item_names[9], "Инструмент");
        return;
    }
    char line[MAX_LINE_LEN];
    int idx = 0;
    while (idx < MAX_ITEMS && fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\n")] = '\0';
        if (strlen(line) == 0) continue;
        strncpy(item_names[idx], line, MAX_NAME_LEN - 1);
        item_names[idx][MAX_NAME_LEN - 1] = '\0';
        idx++;
    }
    fclose(f);
    for (int i = idx; i < MAX_ITEMS; i++) {
        snprintf(item_names[i], MAX_NAME_LEN, "Предмет %d", i);
    }
}

static void show_clock(int day, int hour) {
    printf("Текущее время: День %d, %02d:00\n", day, hour);
}

static void pass_time(int *day, int *hour) {
    int worked = 0;
    if (!read_int("Сколько часов хотите потратить на работу? ", 0, 100000, &worked)) return;
    *hour += worked;
    while (*hour >= HOURS_IN_DAY) {
        *hour -= HOURS_IN_DAY;
        (*day)++;
    }
    printf("Новое время: День %d, %02d:00\n", *day, *hour);
}

static void print_inventory(const int inv[INVENTORY_SIZE]) {
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        printf("Слот %d: [%d] - %s\n", i, inv[i], item_names[inv[i]]);
    }
}

static void put_item(int inv[INVENTORY_SIZE]) {
    int slot = 0, id = 0;
    if (!read_int("Введите индекс слота (0-9): ", MIN_SLOT, MAX_SLOT, &slot)) return;
    if (!read_int("Введите ID предмета (0-9): ", MIN_ITEM_ID, MAX_ITEM_ID, &id)) return;
    inv[slot] = id;
    printf("Предмет [%d] (%s) положен в слот %d.\n", id, item_names[id], slot);
}

static void drop_item(int inv[INVENTORY_SIZE]) {
    int slot = 0;
    if (!read_int("Введите индекс слота (0-9): ", MIN_SLOT, MAX_SLOT, &slot)) return;
    if (inv[slot] == SLOT_EMPTY) {
        printf("Слот %d уже пуст.\n", slot);
        return;
    }
    printf("Из слота %d выброшен [%d] (%s).\n", slot, inv[slot], item_names[inv[slot]]);
    inv[slot] = SLOT_EMPTY;
}

static void variant_search_heavy(const int inv[INVENTORY_SIZE]) {
    int a = 0;
    if (!read_int("Введите ID предмета a (0-9): ", MIN_ITEM_ID, MAX_ITEM_ID, &a)) return;
    printf("Слоты с предметами, чей ID строго больше %d:\n", a);
    int found = 0;
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        if (inv[i] > a) {
            printf("  Слот %d: [%d] (%s)\n", i, inv[i], item_names[inv[i]]);
            found = 1;
        }
    }
    if (!found) printf("  Таких предметов нет.\n");
}

static void print_menu(const char *farmer_name) {
    printf("\n===== Фермер: %s =====\n", farmer_name);
    printf("[0] Выход\n[1] Часы\n[2] Промотать время\n[3] Инвентарь\n");
    printf("[4] Положить\n[5] Выбросить\n[6] Поиск тяжестей\n");
    printf("Выберите пункт: ");
}

int main(void) {
    char farmer_name[MAX_NAME_LEN];
    printf("Введите имя фермера: ");
    if (scanf("%31s", farmer_name) != 1) strcpy(farmer_name, "Безымянный");
    clear_input();
    
    load_item_names();

    int current_day = START_DAY;
    int current_hour = START_HOUR;
    int inventory[INVENTORY_SIZE] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };
    int choice = -1;

    do {
        print_menu(farmer_name);
        if (scanf("%d", &choice) != 1) { clear_input(); choice = -1; continue; }
        clear_input();

        switch (choice) {
            case 0: printf("Выход.\n"); break;
            case 1: show_clock(current_day, current_hour); break;
            case 2: pass_time(&current_day, &current_hour); break;
            case 3: print_inventory(inventory); break;
            case 4: put_item(inventory); break;
            case 5: drop_item(inventory); break;
            case 6: variant_search_heavy(inventory); break;
            default: printf("Неверный пункт.\n"); break;
        }
    } while (choice != 0);

    return 0;
}