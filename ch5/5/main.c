#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

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
    int open_perms = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    int open_flags = O_CREAT | O_WRONLY | O_APPEND;

    int fd = open("./test.txt", open_flags, open_perms);

    if (fd < 0) {
        errExit("Could not open file");
    }

    write(fd, "test\n", 5);

    int fd2 = dup(fd);


    printf("Offsets: fd: %ld, fd2: %ld\n", lseek(fd, 0, SEEK_CUR), lseek(fd2, 0, SEEK_CUR));


    close(fd);
    return 0;
}
