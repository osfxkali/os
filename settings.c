#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "desktop.h"

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

void display_settings() {
    volatile uint32_t *fb = (uint32_t *)FRAMEBUFFER_BASE;

    // 绘制背景
    clear_screen(fb, 0x00FF00); // 绿色背景

    // 绘制按钮
    draw_button(fb, 50, 50, 100, 50, 0xFFFFFF, "Display");
    draw_button(fb, 50, 120, 100, 50, 0xFFFFFF, "System");
    draw_button(fb, 50, 190, 100, 50, 0xFFFFFF, "Boot Screen");
    draw_button(fb, 50, 260, 100, 50, 0xFFFFFF, "Network");
}

void settings_run() {
    volatile uint32_t *fb = (uint32_t *)FRAMEBUFFER_BASE;

    while (1) {
        // 显示设置界面
        display_settings();

        // 模拟鼠标点击事件
        int x, y;
        printf("Click on a setting (x y): ");
        scanf("%d %d", &x, &y);

        if (x >= 50 && x <= 150 && y >= 50 && y <= 100) {
            printf("Display Settings\n");
            // 添加显示器设置逻辑
        } else if (x >= 50 && x <= 150 && y >= 120 && y <= 170) {
            printf("System Settings\n");
            // 添加系统设置逻辑
        } else if (x >= 50 && x <= 150 && y >= 190 && y <= 240) {
            printf("Boot Screen Settings\n");
            // 添加开机界面设置逻辑
        } else if (x >= 50 && x <= 150 && y >= 260 && y <= 310) {
            printf("Network Settings\n");
            // 添加网络设置逻辑
        }
    }
}
