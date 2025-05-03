// 由于不清楚具体的 #include 错误以及 configurationProvider 信息，无法提供更具体的代码。请提供更多上下文信息，以便我能够帮助你解决问题。
#ifndef SHELL_H
#define SHELL_H



#include <stdint.h>

#define MAX_CMD_LENGTH 128
#define MAX_ARGS 8
#define MAX_HISTORY 10

typedef struct {
    char *name;
    void (*handler)(int argc, char **argv);
} Command;

// Shell初始化函数
void shell_init(void);

// 命令解析函数
int parse_input(char *input, char **argv);

// 命令执行循环
void shell_loop(void);

// 显示命令提示符
void display_prompt(void);

// 历史记录操作
void add_history(const char *cmd);
void show_history(void);

// 内置命令声明
void help_handler(int argc, char **argv);
void clear_handler(int argc, char **argv);
void history_handler(int argc, char **argv);

#endif // SHELL_H