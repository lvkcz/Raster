#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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


int cmd_task(int argc, char *argv[]) { //CLI操作 一共有 add list remove edit done search help(没写)七种模式

    if (argc < 2) return 1;
//——————————————————————————————————————————————————————————————————————————————————
    if (strcmp(argv[1], "add") == 0) { //添加task 最大5个
        if (argc < 3) {
            printf("Error: PLEASE INPUT TASK TITLE\n");
            return 1;
        }
        
        
        FILE *check = fopen(FILENAME_TODO, "r");
                int count = 0;
                if (check != NULL) {
                    char buffer[256];
                    while (fgets(buffer, sizeof(buffer), check) != NULL) {
                        count++;
                    }
                    fclose(check);
                }

                // 2. 判断是否达到 MaxTASKSIZE (5个)
                if (count >= MaxTASKSIZE) {
                    printf("Error: Todo list is full (Max %d tasks).\n", MaxTASKSIZE);
                    return 1;
                }
        
        else {
            FILE *todo5 = fopen(FILENAME_TODO, "a");
            if (todo5 == NULL) {
                printf("Error: Cannot open file.\n");
                return 1;
            }
            // 写入任务标题（argv[2]），而非命令本身（argv[1]）
            fprintf(todo5, "%ld|%s\n", (long)time(NULL), argv[2]);
            fclose(todo5);
            printf("Added: %s\n", argv[2]);
        }
    }
    
    //———————————————————————————————————————————————————————————————————————————
    else if (strcmp(argv[1], "list") == 0) { //列表
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
            char *title = strchr(buffer, '|');
            printf("%d. %s", id++, title ? title + 1 : buffer);
        }

        if (id == 1) {
            printf("(Empty)\n");
        }

        fclose(todo5);
    }
    
    //————————————————————————————————————————————————————————————————————————————————
    else if (strcmp(argv[1], "remove") == 0) { //删除
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

        }
    //————————————————————————————————————————————————————————————————————————————————————————————————————
    
    else if (strcmp(argv[1], "edit") == 0) { //编辑
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

        }
    //————————————————————————————————————————————————————————————————————————————————————————————————————————————————
    else if (strcmp(argv[1], "done") == 0) { //完成
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
//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————
        } else if (strcmp(argv[1], "search") == 0) { //搜索
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

int time_task(void) {
    // 48小时没打勾自动删除
    FILE *fp = fopen(FILENAME_TODO, "r");
    if (fp == NULL) return 0;

    // 1. 读出文件内容，直接喂给你原本的 Create_List
    ElemType raw[MaxTASKSIZE];
    int n = 0;
    while (n < MaxTASKSIZE && fgets(raw[n], sizeof(ElemType), fp) != NULL) {
        raw[n][strcspn(raw[n], "\r\n")] = '\0';
        n++;
    }
    fclose(fp);

    SqList *L = NULL;
    if (!Create_List(&L, raw, n)) return 0;

    // 2. 在顺序表里检查并删掉超过 48 小时（172800 秒）的任务
    time_t now = time(NULL);
    int i = 0, removed = 0;
    while (i < L->length) {
        long task_time = strtol(L->data[i], NULL, 10);
        if (task_time > 0 && (now - task_time) >= 48 * 3600) {
            for (int j = i; j < L->length - 1; j++) {
                strcpy(L->data[j], L->data[j + 1]);
            }
            L->length--;
            removed++;
        } else {
            i++;
        }
    }

    // 3. 如果有过期任务被删，直接覆盖写回 FILENAME_TODO
    if (removed > 0) {
        fp = fopen(FILENAME_TODO, "w");
        if (fp != NULL) {
            for (int k = 0; k < L->length; k++) {
                fprintf(fp, "%s\n", L->data[k]);
            }
            fclose(fp);
        }
    }

    free(L);
    return removed;
}

//——————————————————————————————————————————————————————————————————————————————————————————————————————————————————
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
    time_task();

    // 传入 argc 与 argv
    cmd_task(argc, argv);
    
    if (argc < 2){
        printf("Notice: Please input ./Raster <order> used it.\n" );
        printf("'Enter' to exit.");
        getchar();
    }
    
   

    return 0;
    
 
}

