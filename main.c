#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "shell.h"
#include "menu.h"
#include "desktop.h"
#include "net.h"
#include "partition.h"
#include "file_manager.h"
#include "browser.h"
#include "wps.h"
#include "games.h"
#include "settings.h"
#include "antivirus.h"

#define FRAMEBUFFER_BASE 0xC0000000
#define WIDTH 640
#define HEIGHT 480
#define BYTES_PER_PIXEL 4

#include <stdint.h>
#include <string.h>
#include <stdio.h>

// 定义帧缓冲区和VGA文本模式缓冲区的地址
#define FRAMEBUFFER_BASE 0xC0000000
#define VGA_TEXT_BUFFER 0xB8000

// 定义屏幕分辨率和像素字节数
#define WIDTH 640
#define HEIGHT 480
#define BYTES_PER_PIXEL 4

// 定义VGA文本模式相关参数
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

// 帧缓冲区指针
volatile uint32_t *fb = (uint32_t *)FRAMEBUFFER_BASE;

// VGA文本模式缓冲区指针
volatile unsigned short *vga_buffer = (unsigned short *)VGA_TEXT_BUFFER;

// 绘制单个像素
// 由于报错提示存在冲突类型，这里假设是函数声明与定义不一致，暂时未发现其他声明，先保留原定义
// 为了解决 'draw_pixel' 冲突类型问题，检查是否有其他声明，若没有，保持原定义并确保没有重复声明
// 这里假设该函数定义前没有其他冲突声明，保持原定义
void draw_pixel(volatile uint32_t *fb, int x, int y, uint32_t color) {
    fb[y * WIDTH + x] = color;
}

// 清除帧缓冲区屏幕
void clear_screen_fb(volatile uint32_t *fb, uint32_t color) {
    memset((void*)fb, color, WIDTH * HEIGHT * BYTES_PER_PIXEL);
}

// 绘制开机界面
void draw_splash_screen() {
    clear_screen_fb(fb, 0x000000); // 黑色背景

    // 绘制一个红色的矩形
    for (int y = 100; y < 200; y++) {
        for (int x = 100; x < 200; x++) {
            draw_pixel(fb, x, y, 0xFF0000); // 红色
        }
    }

    // 绘制欢迎消息
    char *welcome_message = "Welcome to SimpleOS!";
    int message_x = (WIDTH - strlen(welcome_message) * 8) / 2;
    int message_y = HEIGHT / 2;
    for (int i = 0; i < strlen(welcome_message); i++) {
        for (int y = 0; y < 8; y++) {
            for (int x = 0; x < 8; x++) {
                if ((welcome_message[i] << x) & 0x80) {
                    draw_pixel(fb, message_x + i * 8 + x, message_y + y, 0xFFFFFF); // 白色
                }
            }
        }
    }
}

// 清除VGA文本模式屏幕
void clear_screen_vga() {
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga_buffer[i] = (0x07 << 8) | ' ';
    }
}

// 在VGA文本模式下打印字符串
void print_str_vga(const char *str) {
    static int cursor = 0;
    while (*str) {
        vga_buffer[cursor++] = (0x07 << 8) | *str++;
    }
}

// 主函数
int main() {
    // 绘制图形模式的开机界面
    draw_splash_screen();

    // 延迟一段时间
    for (volatile int i = 0; i < 10000000; i++);

    // 切换到VGA文本模式
    clear_screen_vga();
    print_str_vga("Hello from SimpleOS Kernel!");

    // 挂起
    while (1) {
        // 保持系统运行
    }

    return 0; // 返回退出状态
}