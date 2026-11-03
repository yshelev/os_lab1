#include "proc.h"

#ifndef _WIN32

#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

struct proc {
    pid_t pid;
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

    p->pid = fork();
    if (p->pid < 0) {
        free(p);
        return PROCESS_FAILED;
    }
    if (p->pid == 0) {                      
        execvp(argv[0], (char *const *)argv);
        _exit(127);                         
    }

    *out = p;
    return OK;
}

int wait(proc *p, int *exit_code) {
    if (!p || !exit_code) {
        return BAD_PARAMETERS;
    }
    int status;
    if (waitpid(p->pid, &status, 0) == -1) {
        return PROCESS_FAILED;
    }
    *exit_code = WIFEXITED(status) ? WEXITSTATUS(status) : -1;
    return OK;
}

void free(proc *p) {
    free(p);
}

#endif