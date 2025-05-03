#ifndef PARTITION_H
#define PARTITION_H

#include <stdint.h>

#define MAX_PARTITIONS 4
#define PARTITION_TABLE_SIGNATURE 0xAA55

// 分区类型枚举
enum PartitionType {
    PART_EMPTY = 0x00,
    PART_FAT32 = 0x0B,
    PART_NTFS = 0x07,
    PART_EXT4 = 0x83,
    PART_SWAP = 0x82
};

// 分区状态标志
#define PARTITION_ACTIVE 0x80
#define PARTITION_HIDDEN 0x10

// 分区表项结构体
struct PartitionEntry {
    uint8_t status;         // 状态标志
    uint8_t chs_start[3];   // 传统CHS起始地址
    uint8_t type;           // 分区类型
    uint8_t chs_end[3];     // 传统CHS结束地址
    uint32_t lba_start;     // LBA起始扇区
    uint32_t sector_count;  // 扇区总数
} __attribute__((packed));

// 主分区表结构
struct MasterBootRecord {
    uint8_t bootstrap[446];
    struct PartitionEntry partitions[4];
    uint16_t signature;
} __attribute__((packed));

// 函数声明
int init_partition_table(uint8_t disk_num, struct MasterBootRecord *mbr);
struct PartitionEntry* find_partition_by_type(uint8_t disk_num, enum PartitionType type);
int read_partition_table(uint8_t disk_num, struct MasterBootRecord *mbr);
int write_partition_table(uint8_t disk_num, struct MasterBootRecord *mbr);

#endif // PARTITION_H