#ifndef VFS_H
#define VFS_H

typedef struct {
    char name[32];
    int is_directory;
} VFS_Entry;

void vfs_init(void);
void vfs_ls(void);
void vfs_create_file(const char *name);
void vfs_create_dir(const char *name);
void vfs_close(int fd);
int vfs_open(const char *path, int flags);
int vfs_read(int fd, void *buf, size_t count);

#endif /* VFS_H */