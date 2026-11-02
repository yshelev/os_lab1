#ifndef PROC_H
#define PROC_H

enum {
    WT_OK = 0,
    WT_BAD_PARAMETERS = 10,
    WT_NO_MEMORY = 11,
    WT_PROCESS_FAILED = 12
};

typedef struct wt_proc wt_proc;

int wt_start(const char *const argv[], wt_proc **out);
int wt_wait(wt_proc *p, int *exit_code);
void wt_free(wt_proc *p);

#endif