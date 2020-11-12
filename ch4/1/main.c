#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

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

/*
 * We are here implementing the tee command. It reads from standard input until end of file, 
 * copies the content to standard output, and to a file named in its command line argument.
 */
int main(int argc, char *argv[]) {
    int opt;
    int append_mode = 0;
    while ((opt = getopt(argc, argv, ":a")) != -1) {
        switch(opt){
            case 'a': 
                append_mode++;
                break;
            case ':':
                errExit("Missing argument!\n");
            case '?':
                errExit("Unexpected argument!\n");
        }
    }

    // 0644 permissions
    int target_perms = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    int target_options = O_CREAT | O_WRONLY;
    if (append_mode) 
        target_options |= O_APPEND;
    else
        target_options |= O_TRUNC;
    int target_fd = open(argv[optind], target_options, target_perms);
    if (target_fd < 0) {
        errExit("Could not create target file.\n");
    }

    char buf[BUF_SIZE];

    int bytes_read;
    while( bytes_read = read(STDIN_FILENO, buf, BUF_SIZE) ) {
        write(STDOUT_FILENO, buf, bytes_read);
        write(target_fd, buf, bytes_read);
    }

    close(target_fd);

    return 0;
}
