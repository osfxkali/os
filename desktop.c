#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "file_manager.h"
#include "browser.h"
#include "wps.h"
#include "games.h"
#include "settings.h"
#include "antivirus.h"
#include "net.h"

#define FRAMEBUFFER_BASE 0xC0000000
#define WIDTH 640
#define HEIGHT 480
#define BYTES_PER_PIXEL 4

void draw_pixel(volatile uint32_t *fb, int x, int y, uint32_t color) {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        fb[y * WIDTH + x] = color;
    }
}

void clear_screen(volatile uint32_t *fb, uint32_t color) {
    memset((void*)fb, color, WIDTH * HEIGHT * BYTES_PER_PIXEL);
}

void draw_button(volatile uint32_t *fb, int x, int y, int width, int height, uint32_t color, const char *label) {
    volatile uint32_t *framebuffer = fb;
    for (int i = y; i < y + height; i++) {
        for (int j = x; j < x + width; j++) {
            draw_pixel(fb, j, i, color);
        }
    }
    int label_x = x + (width - strlen(label) * 8) / 2;
    int label_y = y + (height - 8) / 2;
    for (int i = 0; i < strlen(label); i++) {
        for (int j = 0; j < 8; j++) {
            for (int k = 0; k < 8; k++) {
                if ((label[i] << k) & 0x80) {
                    draw_pixel(fb, label_x + i * 8 + k, label_y + j, 0x000000); // 黑色文字
                }
            }
        }
    }
}

void draw_taskbar(volatile uint32_t *fb) {
    // 绘制任务栏背景
    for (int y = HEIGHT - 50; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            draw_pixel(fb, x, y, 0x0000FF); // 蓝色背景
        }
    }

    // 绘制任务栏图标
    const char *taskbar_icons[] = {"Start", "FM", "Br", "WP", "AV", "Vol", "Net"};
    int taskbar_icon_width = 40;
    for (int i = 0; i < sizeof(taskbar_icons) / sizeof(taskbar_icons[0]); i++) {
        draw_button(fb, 10 + i * taskbar_icon_width, HEIGHT - 45, taskbar_icon_width, 40, 0xFFFFFF, taskbar_icons[i]);
    }
}

void desktop_init() {
    volatile uint32_t *fb = (uint32_t *)FRAMEBUFFER_BASE;

    // 绘制桌面背景
    clear_screen(fb, 0x00FF00); // 绿色背景

    // 绘制按钮
    const char *app_labels[] = {"File Manager", "Browser", "WPS Office", "Guess Game", "Snake Game", "Settings", "Antivirus"};
    int app_width = 100;
    for (int i = 0; i < sizeof(app_labels) / sizeof(app_labels[0]); i++) {
        draw_button(fb, 50, 50 + i * 70, app_width, 50, 0xFFFFFF, app_labels[i]);
    }

    // 绘制任务栏
    draw_taskbar(fb);
}

void desktop_run() {
    volatile uint32_t *fb = (uint32_t *)FRAMEBUFFER_BASE;

    while (1) {
        // 模拟鼠标点击事件
        int x, y;
        printf("Click on a button (x y): ");
        if (scanf("%d %d", &x, &y) == 2) {
            while (getchar() != '\n'); // 清空输入缓冲区
            // 检查点击的按钮并执行相应的操作
            // ...（省略按钮点击检查和执行代码）
        }
    }
}