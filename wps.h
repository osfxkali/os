#ifndef WPS_H
#define WPS_H

#include "file_manager.h"

#define MAX_CONTENT_LEN 4096
#define MAX_PARAGRAPHS 50

typedef struct {
    char content[MAX_CONTENT_LEN];
    int cursor_pos;
    int paragraph_count;
    int font_size;
    uint32_t text_color;
} Document;

// 文档操作接口
typedef struct {
    int (*open)(Document *doc, const char *path);
    int (*save)(const Document *doc, const char *path);
    void (*insert_text)(Document *doc, const char *text);
    void (*delete_text)(Document *doc, int length);
} WPSOperations;

// 文件格式支持
typedef enum {
    FORMAT_TXT,
    FORMAT_DOC,
    FORMAT_RTF
} FileFormat;

// 初始化文档对象
void document_init(Document *doc);

// 设置段落格式
void set_paragraph_format(Document *doc, int spacing, int alignment);

// 注册文件操作接口
void register_wps_operations(WPSOperations *ops);

#endif // WPS_H