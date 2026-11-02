#ifndef PROC_H
#define PROC_H

enum {
    WT_OK = 0,
    WT_BAD_PARAMETERS = 10,
    WT_NO_MEMORY = 11,
    WT_PROCESS_FAILED = 12
};

typedef struct wt_proc wt_proc;

#ifdef _WIN32
    #include "proc_win.h"
#else
    #include "proc_posix.h"
#endif

#endif