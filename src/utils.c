#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Названия предметов — static, наружу через get_item_name */
static char item_names[MAX_ITEMS][MAX_NAME_LEN];

void clear_input(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

int read_int(const char *prompt, int min, int max, int *out) {
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

void show_clock(int day, int hour) {
    printf("Текущее время: День %d, %02d:00\n", day, hour);
}

void pass_time(int *day, int *hour) {
    int worked = 0;
    if (!read_int("Сколько часов потратить на работу? ", 0, 100000, &worked)) return;
    *hour += worked;
    while (*hour >= HOURS_IN_DAY) {
        *hour -= HOURS_IN_DAY;
        (*day)++;
    }
    printf("Новое время: День %d, %02d:00\n", *day, *hour);
}

void load_item_names(void) {
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
        line[strcspn(line, "\r\n")] = '\0';
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

const char *get_item_name(int id) {
    if (id < 0 || id >= MAX_ITEMS) return "?";
    return item_names[id];
}

void variant_process_logs(void) {
    FILE *in = fopen(FILE_INPUT, "r");
    if (in == NULL) { printf("[!] Нет файла %s\n", FILE_INPUT); return; }

    FILE *out = fopen(FILE_OUTPUT, "w");
    if (out == NULL) { fclose(in); return; }

    char line[MAX_LINE_LEN];
    int line_num = 0, c_info = 0, c_warn = 0, c_err = 0;

    while (fgets(line, sizeof(line), in)) {
        line_num++;
        line[strcspn(line, "\r\n")] = '\0';

        if (strncmp(line, "[INFO]", 6) == 0) {
            c_info++;
        } else if (strncmp(line, "[WARN]", 6) == 0) {
            c_warn++;
            fprintf(out, "Строка %d: %s\n", line_num, line + 6);
            printf("Строка %d: %s\n", line_num, line + 6);
        } else if (strncmp(line, "[ERROR]", 7) == 0) {
            c_err++;
            fprintf(out, "Строка %d: %s\n", line_num, line + 7);
            printf("Строка %d: %s\n", line_num, line + 7);
        }
    }
    fclose(in);
    fclose(out);
    printf("\nINFO: %d | WARN: %d | ERROR: %d\n", c_info, c_warn, c_err);
}