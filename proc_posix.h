#ifndef PROC_POSIX_H
#define PROC_POSIX_H
#include "proc.h"

int wt_start(const char *const argv[], wt_proc **out);
int wt_wait(wt_proc *p, int *exit_code);
void wt_free(wt_proc *p);

#endif