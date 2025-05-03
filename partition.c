#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "vfs.h"

#define PARTITION_SIZE (1024 * 1024 * 10)  // 每个分区大小为10MB

typedef struct {
    char *name;
    char *mount_point;
    void *start_address;
    size_t size;
} Partition;

Partition partitions[] = {
    {"C:", "/", (void *)0x10000000, PARTITION_SIZE},
    {"D:", "/D:", (void *)0x10A00000, PARTITION_SIZE}
};

int num_partitions = sizeof(partitions) / sizeof(Partition);

int mount_partitions() {
    for (int i = 0; i < num_partitions; i++) {
        printf("Mounting partition %s at %s\n", partitions[i].name, partitions[i].mount_point);
        vfs_mount(partitions[i].start_address, partitions[i].size, partitions[i].mount_point);
    }
    return 0;
}
