#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#define BUFFER_SIZE 4096

void handle_fsize(int signum) {
    fprintf(stderr, "Error: file size limit exceeded (signal %d)\n", signum);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Program need two arguments\n");
        return 1;
    }

    FILE *src = fopen(argv[1], "rb");
    if (src == NULL) {
        fprintf(stderr, "Cannot open file %s for reading: %s\n", argv[1], strerror(errno));
        return 1;
    }

    FILE *dest = fopen(argv[2], "wb");
    if (dest == NULL) {
        fprintf(stderr, "Cannot open file %s for writing: %s\n", argv[2], strerror(errno));
        fclose(src);
        return 1;
    }

    signal(SIGXFSZ, handle_fsize);

    char buffer[BUFFER_SIZE];
    size_t bytes;

    while ((bytes = fread(buffer, 1, BUFFER_SIZE, src)) > 0) {
        if (fwrite(buffer, 1, bytes, dest) < bytes) {
            perror("Error writing to file (possibly file size limit reached)");
            break;
        }
    }

    fclose(src);
    fclose(dest);
    return 0;
}

