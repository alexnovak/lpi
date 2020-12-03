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
    if (argc < 3) 
        errExit("Please specify a filename and number of bytes.\n");

    if (argc > 5) 
        errExit("Please specify at most three arguments.\n");
    
    int open_perms = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    int open_flags = O_CREAT | O_WRONLY | O_APPEND;

    if (argc == 4 && ! strcmp(argv[3], "x")) 
        open_flags = O_CREAT | O_WRONLY;

    int fd = open(argv[1], open_flags, open_perms);

    if (fd < 0) {
        errExit("Could not open file");
    }

    int bytes = atoi(argv[2]);
    for (int i = 0; i < bytes; i++) {
        lseek(fd, 0, SEEK_END);
        write(fd, "a", 1);
    }

    close(fd);
    return 0;
}
