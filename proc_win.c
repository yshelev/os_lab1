#include "proc.h"

#ifdef _WIN32

#include <process.h>
#include <stdlib.h>

struct wt_proc {
    intptr_t h_pid;
};

int wt_start(const char *const argv[], wt_proc **out) {
    if (!argv || !argv[0] || !out) {
        return WT_BAD_PARAMETERS;
    }
    *out = NULL;

    wt_proc *p = malloc(sizeof *p);
    if (!p) {
        return WT_NO_MEMORY;
    }

    intptr_t h = _spawnvp(_P_NOWAIT, argv[0], argv);
    
    if (h == -1) {
        free(p);
        return WT_PROCESS_FAILED; 
    }

    p->h_pid = h;
    *out = p;
    return 0;
}

int wt_wait(wt_proc *p, int *exit_code) {
    if (!p || !exit_code) {
        return WT_BAD_PARAMETERS;
    }

    if (_cwait(exit_code, p->h_pid, 0) == -1){
        return WT_PROCESS_FAILED;
    } 
    return 0;
}

void wt_free(wt_proc *p) {
    if (!p) return;
    free(p);
}

#endif
