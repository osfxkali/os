#ifndef SETTINGS_H
#define SETTINGS_H

#include <stdint.h>

#define SETTINGS_SAVE_PATH "/etc/settings.cfg"

/* 系统配置结构体 */
typedef struct {
    uint32_t bg_color;
    uint16_t screen_timeout;
    uint8_t boot_animation;
    char default_shell[32];
} SystemSettings;

/* 显示配置结构体 */
typedef struct {
    uint16_t resolution_x;
    uint16_t resolution_y;
    uint32_t taskbar_color;
    uint8_t font_size;
} DisplaySettings;

/* 设置操作接口 */
void settings_init(void);
void save_settings(void);
void load_settings(void);

/* 子菜单显示函数 */
void display_settings_menu(void);
void display_system_settings(void);
void display_boot_settings(void);
void display_network_settings(void);

/* 桌面模块兼容接口 */
typedef void (*click_handler)(int, int);
void register_settings_handler(click_handler handler);

#endif /* SETTINGS_H */