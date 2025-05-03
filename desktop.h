#ifndef DESKTOP_H
#define DESKTOP_H

#include <stdint.h>

#define WIDTH 640
#define HEIGHT 480

/* 颜色常量 */
#define COLOR_BACKGROUND 0x00FF00
#define COLOR_TASKBAR 0x0000FF
#define COLOR_BUTTON 0xFFFFFF
#define COLOR_TEXT 0x000000

/* 窗口组件结构体 */
typedef struct {
    int x;
    int y;
    int width;
    int height;
    char title[32];
} Window;

/* 核心功能声明 */
void desktop_init(void);
void draw_pixel(volatile uint32_t *fb, int x, int y, uint32_t color);
void clear_screen(volatile uint32_t *fb, uint32_t color);
void draw_button(volatile uint32_t *fb, int x, int y, int width, int height, uint32_t color, const char *label);
void draw_taskbar(volatile uint32_t *fb);

/* 用户输入处理 */
void handle_mouse_click(int x, int y);
void handle_keyboard_input(char key);

/* 窗口管理 */
void create_window(Window *win, int x, int y, int w, int h, const char *title);
void draw_window(volatile uint32_t *fb, Window *win);

#endif /* DESKTOP_H */