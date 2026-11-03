#include "proc.h"

#ifndef _WIN32

#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

struct wt_proc {
    pid_t pid;
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

    p->pid = fork();
    if (p->pid < 0) {
        free(p);
        return WT_PROCESS_FAILED;
    }
    if (p->pid == 0) {                      
        execvp(argv[0], (char *const *)argv);
        _exit(127);                         
    }

    *out = p;
    return WT_OK;
}

int wt_wait(wt_proc *p, int *exit_code) {
    if (!p || !exit_code) {
        return WT_BAD_PARAMETERS;
    }
    int status;
    if (waitpid(p->pid, &status, 0) == -1) {
        return WT_PROCESS_FAILED;
    }
    *exit_code = WIFEXITED(status) ? WEXITSTATUS(status) : -1;
    return WT_OK;
}

void wt_free(wt_proc *p) {
    free(p);
}

#endif