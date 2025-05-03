#ifndef BROWSER_H
#define BROWSER_H

#include <stdint.h>
#include <string.h>
#include "net.h"

#define MAX_URL_LEN 256
#define MAX_HISTORY 10
#define MAX_BOOKMARKS 100

typedef struct {
    char url[MAX_URL_LEN];
    uint32_t content_hash;
    uint8_t *framebuffer;
} WebPage;

typedef struct BrowserWindow {
    int x, y;
    uint32_t width, height;
    WebPage current_page;
    char history[MAX_HISTORY][MAX_URL_LEN];
    int history_index;
    int history_size;
    char bookmarks[MAX_BOOKMARKS][MAX_URL_LEN];
} BrowserWindow;

// 核心功能接口
void browser_init(BrowserWindow *win, int x, int y, uint32_t w, uint32_t h);
int load_url(BrowserWindow *win, const char *url);
void render_webpage(volatile uint32_t *fb, BrowserWindow *win);

// 历史管理
void navigate_back(BrowserWindow *win);
void navigate_forward(BrowserWindow *win);

// 书签功能
void add_bookmark(BrowserWindow *win, const char *name);
void remove_bookmark(BrowserWindow *win, int index);

// 输入处理
void handle_keyboard_input(char key);
void handle_mouse_click(int x, int y);

// 网络接口
typedef void (*DownloadCallback)(uint8_t *data, size_t len);
void async_download(const char *url, DownloadCallback callback);

#endif // BROWSER_H