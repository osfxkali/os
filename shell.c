#include <stdio.h>
#include <string.h>
#include "games.h"

#define MAX_CMD_LENGTH 128

char cmd_buffer[MAX_CMD_LENGTH];
int cmd_index = 0;

void shell_init() {
    printf("Simple Shell v1.0\n");
    printf("Type 'help' for a list of commands.\n");
    shell_prompt();
}

void shell_prompt() {
    printf(">>> ");
}

void shell_read_char(char c) {
    if (c == '\n' || c == '\r') {
        cmd_buffer[cmd_index] = '\0';
        shell_execute(cmd_buffer);
        cmd_index = 0;
        shell_prompt();
    } else if (c == '\b' && cmd_index > 0) {
        cmd_index--;
        printf("\b \b"); // 删除一个字符
    } else if (cmd_index < MAX_CMD_LENGTH - 1) {
        cmd_buffer[cmd_index++] = c;
        printf("%c", c);
    }
}

void shell_execute(const char *cmd) {
    if (strncmp(cmd, "help", 4) == 0) {
        printf("Available commands:\n");
        printf("  help - Show this help message\n");
        printf("  exit - Exit the shell\n");
        printf("  ls - List files\n");
        printf("  unzip - Unzip a file\n");
        printf("  guess - Play Guess Number Game\n");
        printf("  snake - Play Snake Game\n");
    } else if (strncmp(cmd, "exit", 4) == 0) {
        printf("Exiting shell...\n");
        exit(0);
    } else if (strncmp(cmd, "ls", 2) == 0) {
        vfs_ls();
    } else if (strncmp(cmd, "unzip", 4) == 0) {
        char *filename = cmd + 5;
        if (*filename == ' ') {
            filename++;
        }
        if (*filename) {
            unzip_file(filename);
        } else {
            printf("Usage: unzip <filename>\n");
        }
    } else if (strncmp(cmd, "guess", 5) == 0) {
        guess_number_game();
    } else if (strncmp(cmd, "snake", 5) == 0) {
        snake_game();
    } else {
        printf("Unknown command: '%s'\n", cmd);
    }
}

void shell_run() {
    char c;
    while (1) {
        c = getchar();
        if (c != EOF) {
            shell_read_char(c);
        }
    }
}