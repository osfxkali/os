#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "games.h"
#include "vfs.h" // 包含 vfs_ls 函数
#include "shell.h" // 包含 shell_run 函数
#include "net.h" // 包含 net_init 函数
#include "unzip.h" // 包含 unzip_file 函数

void menu_prompt(void);

void menu_init(void) {
    printf("System Menu:\n");
    printf("1. Open Shell\n");
    printf("2. List Files\n");
    printf("3. Unzip File\n");
    printf("4. Network Settings\n");
    printf("5. Play Guess Number Game\n");
    printf("6. Play Snake Game\n");
    printf("7. Exit\n");
    menu_prompt();
}

void menu_prompt() {
    printf("Enter your choice: ");
}

void menu_execute(int choice) {
    switch (choice) {
        case 1:
            shell_run();
            break;
        case 2:
            vfs_ls();
            break;
        case 3: {
            char filename[256];
            printf("Enter filename to unzip: ");
            if (fgets(filename, sizeof(filename), stdin)) {
                filename[strcspn(filename, "\n")] = 0; // 移除换行符
                unzip_file(filename);
            }
            break;
        }
        case 4:
            net_init();
            break;
        case 5:
            guess_number_game();
            break;
        case 6:
            snake_game();
            break;
        case 7: {
            printf("Exiting system...\n");
            // 调用系统退出函数，而不是无限循环
            exit(0);
        }
        default:
            printf("Invalid choice. Please try again.\n");
            break;
    }
}

void menu_run() {
    int choice;
    while (1) {
        menu_prompt();
        if (scanf("%d", &choice) != 1) {
            // 清空输入缓冲区以处理非数字输入
            while (getchar() != '\n');
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        while (getchar() != '\n'); // 清空输入缓冲区
        menu_execute(choice);
    }
}