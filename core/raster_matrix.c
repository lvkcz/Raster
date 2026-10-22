#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "color_hal.h"
#include "raster_matrix.h"

#define MATRIX_WEEKS 53
#define MATRIX_DAYS (MATRIX_WEEKS * 7)

static void day_key(time_t value, char out[11]) {
    struct tm *parts = localtime(&value);
    if (parts == NULL) {
        out[0] = '\0';
        return;
    }
    strftime(out, 11, "%Y-%m-%d", parts);
}

static AppColor activity_color(int count) {
    if (count >= 3) return COLOR_GREEN_DARK;
    if (count == 2) return COLOR_GREEN_MEDIUM;
    if (count == 1) return COLOR_GREEN_LIGHT;
    return COLOR_DIM;
}

static void print_cell(int count) {
    set_color(activity_color(count));
    printf("■ ");
    reset_color();
}

int raster_matrix_view(const char *done_filename) {
    int counts[MATRIX_DAYS] = {0};
    char keys[MATRIX_DAYS][11];
    int month_at_week[MATRIX_WEEKS];
    time_t now = time(NULL);
    struct tm start_tm;
    struct tm *now_tm = localtime(&now);
    FILE *fp;
    char line[512];
    int i, week, row;

    if (now_tm == NULL) {
        fprintf(stderr, "Error: Cannot read local time.\n");
        return 1;
    }

    /* Sunday of the first displayed week, with the current week at the right. */
    start_tm = *now_tm;
    start_tm.tm_hour = 12;
    start_tm.tm_min = 0;
    start_tm.tm_sec = 0;
    start_tm.tm_mday -= start_tm.tm_wday + (MATRIX_WEEKS - 1) * 7;
    start_tm.tm_isdst = -1;

    for (i = 0; i < MATRIX_DAYS; ++i) {
        struct tm date_tm = start_tm;
        time_t date_value;
        date_tm.tm_mday += i;
        date_tm.tm_isdst = -1;
        date_value = mktime(&date_tm);
        day_key(date_value, keys[i]);
    }

    for (week = 0; week < MATRIX_WEEKS; ++week) {
        struct tm date_tm = start_tm;
        struct tm *date_parts;
        time_t date_value;
        date_tm.tm_mday += week * 7;
        date_tm.tm_isdst = -1;
        date_value = mktime(&date_tm);
        date_parts = localtime(&date_value);
        month_at_week[week] = date_parts ? date_parts->tm_mon : -1;
    }

    fp = fopen(done_filename, "r");
    if (fp != NULL) {
        while (fgets(line, sizeof(line), fp) != NULL) {
            char record_key[11];
            char *end = NULL;
            long stamp = strtol(line, &end, 10);
            if (stamp <= 0 || end == line) continue;
            day_key((time_t)stamp, record_key);
            for (i = 0; i < MATRIX_DAYS; ++i) {
                if (strcmp(record_key, keys[i]) == 0) {
                    counts[i]++;
                    break;
                }
            }
        }
        fclose(fp);
    }

    printf("     ");
    for (week = 0; week < MATRIX_WEEKS; ++week) {
        int show = (week == 0 || month_at_week[week] != month_at_week[week - 1]);
        if (show) {
            static const char *months[] = {
                "Jan", "Feb", "Mar", "Apr", "May", "Jun",
                "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
            };
            printf("%-4s", months[month_at_week[week]]);
            ++week; /* A month label occupies two two-character cells. */
        } else {
            printf("  ");
        }
    }
    printf("\n");

    for (row = 0; row < 7; ++row) {
        if (row == 1)
            printf("Mon  ");
        else if (row == 3)
            printf("Wed  ");
        else if (row == 5)
            printf("Fri  ");
        else
            printf("     ");

        for (week = 0; week < MATRIX_WEEKS; ++week) {
            int index = week * 7 + row;
            time_t today_limit;
            struct tm limit_tm = start_tm;
            limit_tm.tm_mday += index;
            limit_tm.tm_isdst = -1;
            today_limit = mktime(&limit_tm);

            if (today_limit > now + 24 * 60 * 60)
                printf("  ");
            else print_cell(counts[index]);
        }
        printf("\n");
    }

    printf("\nLess ");
    print_cell(0);
    printf(" ");
    print_cell(1);
    printf(" ");
    print_cell(2);
    printf(" ");
    print_cell(3);
    printf(" More\n");
    return 0;
}

