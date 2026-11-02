#include <stdio.h>
#include "proc.h"

int main(void) {
    const char *const argv[] = {"ping", "-n", "4", "127.0.0.1", NULL};
    wt_proc *p = NULL;
    int code = 0;

    int rc = wt_start(argv, &p);
    if (rc != WT_OK) {
        printf("start failed: %d\n", rc);
        return 1;
    }

    printf("process started, waiting...\n");

    // rc = wt_wait(p, &code);
    // if (rc == WT_OK) {
    //     printf("process exited with code %d\n", code);
    // } else {
    //     printf("wait failed: %d\n", rc);
    // }

    // wt_free(p);
    return 0;
}