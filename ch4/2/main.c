#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif

int strilen(char* str) {
    int res = 0;
    while (str[res] != '\0') {
        res += 1;
    }
    return res;
}

void errExit(char* message) {
    write(STDERR_FILENO, message, strilen(message));
    exit(1);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        errExit("Incorrect number of arguments.\n");
    }

    int source_fd = open(argv[1], O_RDONLY);
    if (source_fd < 0) {
        errExit("Could not open source file!\n");
    }

    // Get state of source file for permissions
    struct stat st;
    fstat(source_fd, &st);

    int target_flags = O_CREAT | O_WRONLY | O_TRUNC;
    int target_fd = open(argv[2], target_flags, st.st_mode);
    if (target_fd < 0) {
        errExit("Could not create target file!\n");
    }

    char buf[BUF_SIZE];
    int bytes_read;
    while (bytes_read = read(source_fd, buf, BUF_SIZE)) {
        write(target_fd, buf, bytes_read);
    }

    return 0;
}
