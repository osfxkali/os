#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <stdint.h>
#include <stdbool.h>
#include "partition.h"

/* 文件属性常量 */
#define FILE_READ_ONLY  0x01
#define FILE_HIDDEN     0x02
#define FILE_SYSTEM     0x04
#define FILE_DIRECTORY  0x10

/* 文件描述符结构体 */
typedef struct {
    uint32_t inode;
    uint32_t position;
    uint16_t access_mode;
    char path[256];
    size_t size;
    uint8_t attributes;
} FileDescriptor;

/* 目录条目结构体 */
typedef struct {
    char name[256];
    uint8_t type;
    uint32_t size;
    uint32_t create_time;
    uint32_t modify_time;
} DirEntry;

/* 文件系统操作接口 */
int fm_open(const char *path, int mode);
int fm_close(int fd);
ssize_t fm_read(int fd, void *buf, size_t count);
ssize_t fm_write(int fd, const void *buf, size_t count);
int fm_mkdir(const char *path);
int fm_remove(const char *path);

/* 目录遍历接口 */
void* fm_opendir(const char *path);
DirEntry* fm_readdir(void *dirp);
int fm_closedir(void *dirp);

/* 与分区模块交互接口 */
struct PartitionEntry* get_partition_info(uint8_t disk_num);

/* 图形界面集成 */
typedef struct {
    int x;
    int y;
    int width;
    int height;
    DirEntry *current_dir;
} FileManagerWindow;

void draw_file_manager_window(volatile uint32_t *fb, FileManagerWindow *win);

#endif /* FILE_MANAGER_H */