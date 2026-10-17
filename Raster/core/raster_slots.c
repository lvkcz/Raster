#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME_TODO "raster_todo.dat"
#define FILENAME_TASKS_TEMP "raster_tasks.tmp"
#define FILENAME_DONE "raster_tasks_done.dat"
#define VERSION "0.0.1"
#define MaxTASKSIZE 5 // 最大 task 5 个

// 如果 raster_slots.h 尚未包含以下定义，可作为基础类型支撑
typedef char ElemType[64];

typedef struct {
    ElemType data[MaxTASKSIZE];
    int length;
} SqList;

typedef struct {
    ElemType data[MaxTASKSIZE];
    int top;
} STACK;



// 顺序表创建：使用二级指针确保外部指针能拿到 malloc 分配的内存
int Create_List(SqList **L, ElemType a[], int n) {
    int i = 0, k = 0;
    *L = (SqList *)malloc(sizeof(SqList));
    if (*L == NULL) return 0;

    while (i < n && k < MaxTASKSIZE) {
        strcpy((*L)->data[k], a[i]);
        k++;
        i++;
    }
    (*L)->length = k;
    return 1;
}

int stack_task(void) {
    int task_num = 0;
    STACK s;
    s.top = -1;
    return task_num;
}

int time_task(void) {
    // 48小时没打勾自动删除
    return 0;
}

int cmd_task(int argc, char *argv[]) {
    if (argc < 2) return 1;

    if (strcmp(argv[1], "add") == 0) {
        if (argc < 3) {
            printf("Error: PLEASE INPUT TASK TITLE\n");
            return 1;
        } else {
            FILE *todo5 = fopen(FILENAME_TODO, "a");
            if (todo5 == NULL) {
                printf("Error: Cannot open file.\n");
                return 1;
            }
            // 写入任务标题（argv[2]），而非命令本身（argv[1]）
            fprintf(todo5, "%s\n", argv[2]);
            fclose(todo5);
            printf("Added: %s\n", argv[2]);
        }
    } else if (strcmp(argv[1], "list") == 0) {
        FILE *todo5 = fopen(FILENAME_TODO, "r");
        if (todo5 == NULL) {
            printf("NO CONTEXT\n");
            return 1;
        }

        char buffer[256];
        int id = 1;
        printf("----- todo list -----\n");
        // 修复原代码中 fgets 处未闭合的括号
        while (fgets(buffer, sizeof(buffer), todo5) != NULL) {
            printf("%d. %s", id++, buffer);
        }

        if (id == 1) {
            printf("(Empty)\n");
        }

        fclose(todo5);
    } else if (strcmp(argv[1], "remove") == 0) {
            if (argc < 3) {
                printf("Error: PLEASE INPUT TASK ID\n");
                return 1;
            }
            int target_id = atoi(argv[2]);
            FILE *todo5 = fopen(FILENAME_TODO, "r");
            FILE *temp = fopen(FILENAME_TASKS_TEMP, "w");
            if (todo5 == NULL || temp == NULL) {
                if (todo5) fclose(todo5);
                if (temp) fclose(temp);
                printf("Error: Cannot open file.\n");
                return 1;
            }

            char buffer[256];
            int current_id = 1;
            int found = 0;
            while (fgets(buffer, sizeof(buffer), todo5) != NULL) {
                if (current_id == target_id) {
                    found = 1;
                    printf("Removed: %s", buffer);
                } else {
                    fputs(buffer, temp);
                }
                current_id++;
            }

            fclose(todo5);
            fclose(temp);

            if (found) {
                remove(FILENAME_TODO);
                rename(FILENAME_TASKS_TEMP, FILENAME_TODO);
            } else {
                remove(FILENAME_TASKS_TEMP);
                printf("Error: Task ID %d not found.\n", target_id);
            }

        } else if (strcmp(argv[1], "edit") == 0) {
            if (argc < 4) {
                printf("Error: PLEASE INPUT TASK ID AND NEW TITLE\n");
                return 1;
            }
            int target_id = atoi(argv[2]);
            FILE *todo5 = fopen(FILENAME_TODO, "r");
            FILE *temp = fopen(FILENAME_TASKS_TEMP, "w");
            if (todo5 == NULL || temp == NULL) {
                if (todo5) fclose(todo5);
                if (temp) fclose(temp);
                printf("Error: Cannot open file.\n");
                return 1;
            }

            char buffer[256];
            int current_id = 1;
            int found = 0;
            while (fgets(buffer, sizeof(buffer), todo5) != NULL) {
                if (current_id == target_id) {
                    found = 1;
                    fprintf(temp, "%s\n", argv[3]);
                    printf("Updated ID %d to: %s\n", target_id, argv[3]);
                } else {
                    fputs(buffer, temp);
                }
                current_id++;
            }

            fclose(todo5);
            fclose(temp);

            if (found) {
                remove(FILENAME_TODO);
                rename(FILENAME_TASKS_TEMP, FILENAME_TODO);
            } else {
                remove(FILENAME_TASKS_TEMP);
                printf("Error: Task ID %d not found.\n", target_id);
            }

        } else if (strcmp(argv[1], "done") == 0) {
            if (argc < 3) {
                printf("Error: PLEASE INPUT TASK ID\n");
                return 1;
            }
            int target_id = atoi(argv[2]);
            FILE *todo5 = fopen(FILENAME_TODO, "r");
            FILE *temp = fopen(FILENAME_TASKS_TEMP, "w");
            FILE *done = fopen(FILENAME_DONE, "a");
            if (todo5 == NULL || temp == NULL || done == NULL) {
                if (todo5) fclose(todo5);
                if (temp) fclose(temp);
                if (done) fclose(done);
                printf("Error: Cannot open file.\n");
                return 1;
            }

            char buffer[256];
            int current_id = 1;
            int found = 0;
            while (fgets(buffer, sizeof(buffer), todo5) != NULL) {
                if (current_id == target_id) {
                    found = 1;
                    fputs(buffer, done); // 移入已完成文件
                    printf("Done: %s", buffer);
                } else {
                    fputs(buffer, temp);
                }
                current_id++;
            }

            fclose(todo5);
            fclose(temp);
            fclose(done);

            if (found) {
                remove(FILENAME_TODO);
                rename(FILENAME_TASKS_TEMP, FILENAME_TODO);
            } else {
                remove(FILENAME_TASKS_TEMP);
                printf("Error: Task ID %d not found.\n", target_id);
            }

        } else if (strcmp(argv[1], "search") == 0) {
            if (argc < 3) {
                printf("Error: PLEASE INPUT KEYWORD\n");
                return 1;
            }
            FILE *todo5 = fopen(FILENAME_TODO, "r");
            if (todo5 == NULL) {
                printf("NO CONTEXT\n");
                return 1;
            }

            char buffer[256];
            int id = 1;
            int found = 0;
            printf("----- search result -----\n");
            while (fgets(buffer, sizeof(buffer), todo5) != NULL) {
                if (strstr(buffer, argv[2]) != NULL) {
                    printf("%d. %s", id, buffer);
                    found = 1;
                }
                id++;
            }

            if (!found) {
                printf("(No matching tasks)\n");
            }
            fclose(todo5);
        }

    return 0;
}

void init(void) {
    FILE *todo5 = fopen(FILENAME_TODO, "a");
    FILE *done = fopen(FILENAME_DONE, "a");

    // 判断文件指针是否为 NULL，并加花括号避免直接触发 exit
    if (todo5 == NULL || done == NULL) {
        printf("Error : NOT FILES\n");
        exit(EXIT_FAILURE);
    }

    fclose(todo5);
    fclose(done);
}

int main(int argc, char *argv[]) {
    init();

    // 传入 argc 与 argv
    cmd_task(argc, argv);

    return 0;
}

