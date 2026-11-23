#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

/* ==== Общие константы ==== */
#define MAX_NAME_LEN   32
#define MAX_LINE_LEN   256
#define MAX_ITEMS      10
#define INVENTORY_SIZE 10

#define START_DAY      1
#define START_HOUR     8
#define HOURS_IN_DAY   24

#define SLOT_EMPTY     0
#define MIN_ITEM_ID    0
#define MAX_ITEM_ID    9
#define MIN_SLOT       0
#define MAX_SLOT       (INVENTORY_SIZE - 1)

/* ==== Файлы ==== */
#define FILE_ITEMS     "data/items.txt"
#define FILE_INPUT     "data/input.txt"
#define FILE_OUTPUT    "output.txt"
#define FILE_DIARY     "diary.txt"

/* ==== Ввод ==== */
void clear_input(void);
int  read_int(const char *prompt, int min, int max, int *out);

/* ==== Время ==== */
void show_clock(int day, int hour);
void pass_time(int *day, int *hour);

/* ==== Файлы и строки ==== */
void load_item_names(void);
const char *get_item_name(int id);
void variant_process_logs(void);

#endif /* UTILS_H */