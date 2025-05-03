#ifndef ANTIVIRUS_H
#define ANTIVIRUS_H

#include <time.h>

#include "file_manager.h"
#include "net.h"

#define MAX_SIGNATURE_LENGTH 256
#define QUARANTINE_DIR "/quarantine"

typedef enum {
    AV_CLEAN,
    AV_INFECTED,
    AV_ERROR
} AVScanResult;

typedef struct {
    char pattern[MAX_SIGNATURE_LENGTH];
    char name[64];
    int severity_level;
} VirusSignature;

typedef struct {
    int scan_depth;
    int heuristic_analysis;
    unsigned scan_timeout;
} AVConfig;

typedef struct {
    char original_path[256];
    char quarantine_path[256];
    time_t detection_time;
    char virus_name[64];
} QuarantinedFile;

// 核心扫描接口
AVScanResult scan_file(const char *path, const VirusSignature *signatures, size_t count);
AVScanResult scan_memory(const void *buffer, size_t size, const VirusSignature *signatures, size_t count);

// 实时监控
void start_realtime_monitoring(AVConfig config);
void stop_realtime_monitoring(void);

// 病毒库管理
int load_virus_signatures(const char *path);
int update_virus_signatures(const char *update_url);

// 隔离操作
int quarantine_file(const char *path, const char *virus_name);
int restore_file(const QuarantinedFile *record);

// 系统集成
typedef void (*FileOperationCallback)(const char *path);
void register_file_scan_callback(FileOperationCallback callback);
void update_taskbar_icon(int status);

// 配置管理
void set_av_config(AVConfig config);
AVConfig get_current_config(void);

#endif // ANTIVIRUS_H