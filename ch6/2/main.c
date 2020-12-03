#include <setjmp.h>
#include <stdio.h>

static jmp_buf env;

int printer() {
    int status = setjmp(env);
    printf("Welcome to the printer function!");
    return status;
}

int main() {
    int status = printer();
    if (status == 0) {
        longjmp(env, 1);
    }
    return 0;
}
