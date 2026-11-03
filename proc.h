#ifndef PROC_H
#define PROC_H

enum {
    OK = 0,
    BAD_PARAMETERS = 10,
    NO_MEMORY = 11,
    PROCESS_FAILED = 12
};

typedef struct proc proc;

int start(const char *const argv[], proc **out);
int wait(proc *p, int *exit_code);
void free(proc *p);

#endif