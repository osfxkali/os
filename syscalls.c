#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

extern void _exit(int status);

void _exit(int status) {
    // 实现退出功能 - 使用平台无关的无限循环
    while (1) {
        // 移除 ARM 特定的 wfi 指令
        // 简单的无限循环，适用于所有平台
    }
}

int _write(int file, char *ptr, int len) {
    // 实现基本的写操作，例如通过 UART 发送数据
    for (int i = 0; i < len; i++) {
        // 假设 UART 的基地址为 0x10000000
        // 并且 UART 的发送寄存器位于偏移 0x00
        *((volatile unsigned int *)0x10000000) = (unsigned int)(ptr[i]);
    }
    return len;
}

int _close(int file) {
    return -1;
}

int _fstat(int file, struct stat *st) {
    st->st_mode = S_IFCHR;
    return 0;
}

int _isatty(int file) {
    return 1;
}

int _lseek(int file, int ptr, int dir) {
    return 0;
}

int _read(int file, char *ptr, int len) {
    return 0;
}

int _open(const char *path, int flags, int mode) {
    return -1;
}

int _unlink(const char *name) {
    return -1;
}

int _getpid(void) {
    return 1;
}
