#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

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
    int fd = open("./test.txt", O_APPEND | O_WRONLY);

    if (fd < 0) {
        errExit("Could not open file");
    }

    lseek(fd, 0, SEEK_SET);

    write(fd, "new\n", 4);
    close(fd);
    return 0;
}
