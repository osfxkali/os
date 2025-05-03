#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

#define BUFFER_SIZE 1024

void unzip_file(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("Error opening file: %s\n", filename);
        return;
    }

    // 创建输出文件名
    char out_filename[256];
    snprintf(out_filename, sizeof(out_filename), "%s.unzipped", filename);

    FILE *out_file = fopen(out_filename, "wb");
    if (!out_file) {
        fclose(file);
        printf("Error creating output file: %s\n", out_filename);
        return;
    }

    unsigned char buffer[BUFFER_SIZE];
    z_stream stream;
    stream.zalloc = Z_NULL;
    stream.zfree = Z_NULL;
    stream.opaque = Z_NULL;
    stream.avail_in = 0;
    stream.next_in = Z_NULL;

    int ret = inflateInit(&stream);
    if (ret != Z_OK) {
        fclose(file);
        fclose(out_file);
        printf("Error initializing decompression: %d\n", ret);
        return;
    }

    while (1) {
        stream.avail_in = fread(buffer, 1, BUFFER_SIZE, file);
        if (stream.avail_in == 0)
            break;

        stream.next_in = buffer;
        do {
            stream.avail_out = BUFFER_SIZE;
            stream.next_out = buffer;
            ret = inflate(&stream, Z_NO_FLUSH);
            if (ret == Z_STREAM_ERROR) {
                printf("Error decompressing: %d\n", ret);
                break;
            }
            fwrite(buffer, 1, BUFFER_SIZE - stream.avail_out, out_file);
        } while (stream.avail_out == 0);
    }

    inflateEnd(&stream);
    fclose(file);
    fclose(out_file);

    printf("File %s unzipped to %s\n", filename, out_filename);
}
