#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>

// 帧缓冲区基地址
#define FRAMEBUFFER_BASE 0x80000000

// 屏幕尺寸
#define WIDTH 800
#define HEIGHT 600

// 绘图函数
void draw_pixel(volatile uint32_t *fb, int x, int y, uint32_t color);
void clear_screen(volatile uint32_t *fb, uint32_t color);

#endif // MAIN_H