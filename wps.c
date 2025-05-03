#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "vfs.h"

void wps_run() {
    printf("WPS Office\n");
    char filename[256];
    printf("Enter filename to open: ");
    scanf("%s", filename);

    int fd = vfs_open(filename, 0); // 使用整型flags参数
    if (fd == -1) {
        printf("File not found: %s\n", filename);
        return;
    }

    char buffer[1024];
    vfs_read(fd, buffer, sizeof(buffer));
    printf("Content of %s:\n%s\n", filename, buffer);

    vfs_close(fd);
}
