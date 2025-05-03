#ifndef MENU_H
#define MENU_H

#include <stdint.h>

// 多级菜单项结构
typedef struct MenuItem {
    char* label;               // 显示文本
    void (*action)();          // 回调函数
    struct MenuItem* parent;   // 父级菜单指针
    struct MenuItem* children; // 子菜单链表
    struct MenuItem* next;     // 同级菜单项
    uint8_t is_highlighted;    // 焦点状态标志
} MenuItem;

// 全局菜单状态
extern volatile MenuItem* current_menu;
extern volatile MenuItem* selected_item;

// 初始化根菜单
void menu_init(volatile uint32_t* fb);

// 动态添加菜单项（支持多级嵌套）
MenuItem* menu_add_item(MenuItem* parent, 
                       char* label, 
                       void (*action)(),
                       MenuItem* children);

// 图形界面渲染（兼容现有framebuffer）
void menu_render(volatile uint32_t* fb);

// 输入处理（适配shell模块的按键编码）
void menu_handle_input(uint8_t key_code);

// 焦点导航（支持方向键操作）
void menu_navigate(uint8_t direction);

#endif