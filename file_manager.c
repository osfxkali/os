#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "vfs.h"

void file_manager_run() {
    printf("File Manager\n");
    printf("1. List Files\n");
    printf("2. Create File\n");
    printf("3. Delete File\n");
    printf("4. Back\n");

    while (1) {
        int choice;
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            // 清空输入缓冲区以处理非数字输入
            while (getchar() != '\n');
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        while (getchar() != '\n'); // 清空输入缓冲区

        switch (choice) {
            case 1:
                vfs_ls();
                break;
            case 2: {
                char filename[256];
                printf("Enter filename to create: ");
                if (fgets(filename, sizeof(filename), stdin)) {
                    // 移除换行符
                    filename[strcspn(filename, "\n")] = 0;
                    if (vfs_create(filename) != 0)     {
                        printf("Failed to create file: %s\n", filename);
                    }
                }
                break;
            }
            case 3: {
                char filename[256];
                printf("Enter filename to delete: ");
                if (fgets(filename, sizeof(filename), stdin)) {
                    // 移除换行符
                    filename[strcspn(filename, "\n")] = 0;
                    if (vfs_delete(filename) != 0) {
                        printf("Failed to delete file: %s\n", filename);
                    }
                }
                break;
            }
            case 4:
                return;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    }
}