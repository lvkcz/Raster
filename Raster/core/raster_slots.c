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

void get_colors(uint8_t background) {
    uint8_t original[] = {8, 255, 0, 255, 2, 255, 3, 255, 4};
}

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
        // 待实现
    } else if (strcmp(argv[1], "edit") == 0) {
        // 待实现
    } else if (strcmp(argv[1], "done") == 0) {
        // 待实现
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

