#include <stdio.h>
#include "proc.h"

int main(void) {
    #ifdef _WIN32
        const char *const argv[] = {"ping", "-n", "4", "127.0.0.1", NULL};
    #else
        const char *const argv[] = {"sleep", "3", NULL};
    #endif

    proc *p = NULL;
    int code = 0;

    int rc = start(argv, &p);
    if (rc != OK) {
        printf("start failed: %d\n", rc);
        return 1;
    }

    printf("process started, waiting...\n");

    rc = wait(p, &code);
    if (rc == OK) {
        printf("process exited with code %d\n", code);
    } else {
        printf("wait failed: %d\n", rc);
    }

    free(p);
    return 0;
}