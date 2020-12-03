#include <stdio.h>
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

int dupclone(int oldfd) {
    int newfd = fcntl(oldfd, F_DUPFD, 1);
    if (newfd < 0) {
        errExit("fcntl dupclone failure\n");
    }
    return newfd;
}

int dup2clone(int oldfd, int newfd) {
    if (oldfd == newfd) {
        if (fcntl(oldfd, F_GETFL) < 0) {
            return oldfd;
        } else {
            return -1;
        }
    }
    if (fcntl(newfd, F_GETFL) > 0) {
        // Have to close the newfd so we can reserve it.
        close(newfd);
    }
    return fcntl(oldfd, F_DUPFD, newfd);
}

int main(int argc, char* argv[]) {
    int open_perms = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    int open_flags = O_CREAT | O_WRONLY | O_APPEND;

    int fd = open("./test.txt", open_flags, open_perms);

    int fdclone = dupclone(fd);

    printf("File descriptors: %d %d\n", fd, fdclone);

    write(fd, "hey 1\n", 6);
    write(fdclone, "hey 2\n", 6);

    close(fd);
    close(fdclone);

    int fd2 = open("./test2.txt", open_flags, open_perms);
    int fdclone2 = dup2clone(fd, 66);

    printf("File descriptors 2: %d %d\n", fd2, fdclone2);

    close(fd2);
    close(fdclone2);
    return 0;
}
