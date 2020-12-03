#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

#define GN_NONNEG 01
#define GN_GT_0 02

#define GN_ANY_BASE 0100
#define GN_BASE_8   0200
#define GN_BASE_16  0400

#define MAX_ALLOCS 1000000


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

static long getNum(const char *fname, const char *arg, int flags, const char *name) {
    long res;
    char *endptr;
    int base;

    if (arg == NULL || *arg == '\0')
        errExit("Null or empty string");

    base = (flags & GN_ANY_BASE) ? 0 : (flags & GN_BASE_8) ? 8 : (flags & GN_BASE_16) ? 16: 10;

    errno = 0;
    res = strtol(arg, &endptr, base);

    if (errno != 0)
        errExit("strtol() failed");

    if (*endptr != '\0')
        errExit("nonnumeric characters");

    if ((flags & GN_NONNEG) && res < 0)
        errExit("negative value not allowed");

    if ((flags & GN_GT_0) && res <= 0)
        errExit("value must be > 0");

    return res;
}

int getInt(const char *arg, int flags, const char *name) {
    long res;
    res = getNum("getInt", arg, flags, name);
    if (res > INT_MAX || res < INT_MIN)
        errExit("Integer out of range");

    return (int) res;
}

int main (int argc, char *argv[]) {
    char *ptr[MAX_ALLOCS];

    int freeStep, freeMin, freeMax, blockSize, numAllocs, j;

    printf("\n");

    if (argc < 3 || strcmp(argv[1], "--help") == 0)
        errExit("num-allocs block-size [step [min [max]]]\n");

    numAllocs = getInt(argv[1], GN_GT_0, "num-allocs");

    if (numAllocs > MAX_ALLOCS)
        errExit("num-allocs > MAX_ALLOCS\n");

    blockSize = getInt(argv[2], GN_GT_0 | GN_ANY_BASE, "block-size");

    freeStep = (argc > 3) ? getInt(argv[3], GN_GT_0, "step") : 1;
    freeMin = (argc > 4) ? getInt(argv[4], GN_GT_0, "min") : 1;
    freeMax = (argc > 5) ? getInt(argv[5], GN_GT_0, "max") : numAllocs;

    if (freeMax > numAllocs)
        errExit("free-max > num-allocs\n");

    printf("Initial program break: %10p\n", sbrk(0));

    printf("Allocating %d*%d bytes\n", numAllocs, blockSize);

    for (j = 0; j < numAllocs; j++) {
        ptr[j] = malloc(blockSize);
        printf("Program break is now %10p\n", sbrk(0));
        if (ptr[j] == NULL)
            errExit("malloc");
    }

    printf("Program break is now %10p\n", sbrk(0));

    printf("Freeing blocks from %d to %d in steps of %d\n", freeMin, freeMax, freeStep);

    for (j = freeMin - 1; j < freeMax; j += freeStep)
        free(ptr[j]);

    printf("After free(), program break is %10p\n", sbrk(0));

    return 0;

}
