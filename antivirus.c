#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <dirent.h>
#include <sys/stat.h>
#include "vfs.h"
#include "net.h"

#define VIRUS_DEFINITIONS_URL "http://example.com/virus_definitions.bin"
#define VIRUS_DEFINITIONS_PATH "/virus_definitions.bin"

typedef struct {
    char signature[32];  // 病毒签名
    char name[64];       // 病毒名称
} VirusDefinition;

VirusDefinition virus_definitions[100];  // 最多支持100个病毒定义
int num_definitions = 0;

int load_virus_definitions() {
    printf("Loading virus definitions from %s\n", VIRUS_DEFINITIONS_PATH);

    FILE *fp = fopen(VIRUS_DEFINITIONS_PATH, "rb");
    if (!fp) {
        printf("Failed to open file for reading: %s\n", VIRUS_DEFINITIONS_PATH);
        return -1;
    }

    num_definitions = fread(virus_definitions, sizeof(VirusDefinition), 100, fp);
    fclose(fp);

    if (num_definitions <= 0) {
        printf("No virus definitions loaded\n");
        return -1;
    }

    printf("Loaded %d virus definitions\n", num_definitions);
    return 0;
}

int download_virus_definitions() {
    printf("Downloading virus definitions from %s\n", VIRUS_DEFINITIONS_URL);

    FILE *fp = fopen(VIRUS_DEFINITIONS_PATH, "wb");
    if (!fp) {
        printf("Failed to open file for writing: %s\n", VIRUS_DEFINITIONS_PATH);
        return -1;
    }

    int result = net_download(VIRUS_DEFINITIONS_URL, fp);
    fclose(fp);
    if (result <= 0) {
        printf("Failed to download virus definitions\n");
        return -1;
    }
    printf("Virus definitions downloaded successfully\n");
    return 0;
}

int scan_file(const char *path) {
    FILE *fp = fopen(path, "rb");
    if (!fp) {
        printf("Failed to open file for reading: %s\n", path);
        return -1;
    }

    char buffer[1024];
    size_t bytes_read;
    int found = 0;

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), fp)) > 0) {
        for (int i = 0; i < num_definitions; i++) {
            if (strstr(buffer, virus_definitions[i].signature)) {
                printf("Virus detected in %s: %s\n", path, virus_definitions[i].name);
                found = 1;
                break;
            }
        }
        if (found) break;
    }

    fclose(fp);
    return found ? -1 : 0;
}

int scan_filesystem() {
    printf("Scanning filesystem...\n");

    DIR *dir = opendir("/");
    if (!dir) {
        printf("Failed to open directory /\n");
        return -1;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        struct stat st;
        char path[1024];

        if (snprintf(path, sizeof(path), "/%s", entry->d_name) < (int)sizeof(path)) {
            if (stat(path, &st) == 0 && S_ISREG(st.st_mode)) {
                if (scan_file(path) == 0) {
                    printf("File %s is clean\n", path);
                }
            }
        } else {
            printf("Path name too long: %s\n", entry->d_name);
        }
    }

    closedir(dir);
    return 0;
}

void antivirus_run() {
    if (download_virus_definitions() == 0) {
        if (load_virus_definitions() == 0) {
            scan_filesystem();
        }
    }
}