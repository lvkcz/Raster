
#include <stdint.h>
#include <stdbool.h>

#define RASTER_MAX_SLOTS 3
#define RASTER_CONTENT_MAX_LEN 256

#define RASTER_OK         0
#define RASTER_ERR_FULL  -1
#define RASTER_ERR_DB    -2

typedef struct {
    int64_t id;                         // 数据库内部自增主键
        int slot_index;                     // 0 ~ 4 的逻辑槽位编号
        char content[RASTER_CONTENT_MAX_LEN];
        int64_t created_at;                 // 绝对 Unix 秒级时间戳
        int64_t ttl_seconds;                // 存活时长（秒）
        double remaining_hours;
} RasterSlot;

int raster_slots_init(const char *db_path);

// 若存活槽位已达上限返回 RASTER_ERR_FULL
int raster_slot_add(const char *content, int64_t ttl_hours);

// 主动标记完成/舍弃：物理删除，释放空间
int raster_slot_release(int slot_index);

// 惰性清算并读取当前所有有效槽位
int raster_slot_list(RasterSlot *out_slots, int *out_count);
