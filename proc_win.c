#include "proc.h"

#ifdef _WIN32

#include <process.h>
#include <stdlib.h>

struct proc {
    intptr_t pid;
};

int start(const char *const argv[], proc **out) {
    if (!argv || !argv[0] || !out) {
        return BAD_PARAMETERS;
    }
    *out = NULL;

    proc *p = malloc(sizeof *p);
    if (!p) {
        return NO_MEMORY;
    }

    intptr_t h = _spawnvp(_P_NOWAIT, argv[0], argv);
    
    if (h == -1) {
        free(p);
        return PROCESS_FAILED; 
    }

    p->pid = h;
    *out = p;
    return 0;
}

int wait(proc *p, int *exit_code) {
    if (!p || !exit_code) {
        return BAD_PARAMETERS;
    }

    if (_cwait(exit_code, p->pid, 0) == -1){
        return PROCESS_FAILED;
    } 
    return 0;
}

void free(proc *p) {
    if (!p) return;
    free(p);
}

#endif
