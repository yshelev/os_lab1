#include "app.h"
#include "state.h"
#include "lock.h"
#include "utils.h"

#include <stdio.h>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <sys/wait.h>
    #include <unistd.h>
#endif

#define TICK_MS 100   
#define COUNTER_EVERY 3     
#define LOG_EVERY 10
#define SPAWN_EVERY 30     

int g_is_leader = 0;

static void spawnCopies(void);

void copyWork(int which) {
    int *s = (int*)g_shm.mapped_memory_ptr;
    char t[40];
    char msg[128];
    int pid = getPid();

    nowStr(t, sizeof t);
    lockShm();
    snprintf(msg, sizeof msg, "copy%d pid=%d start=%s", which, pid, t);
    logMsg(msg);
    if (which == 1) {
        s[0] += 10;
    } else {
        s[0] *= 2;
    }
    unlockShm();

    if (which == 2) {
        msleep(2000);
        lockShm();
        s[0] /= 2;
        unlockShm();
    }

    nowStr(t, sizeof t);
    lockShm();
    snprintf(msg, sizeof msg, "copy%d pid=%d exit=%s", which, pid, t);
    logMsg(msg);
    unlockShm();
}

void appRun(void) {
    int tick = 0;
    for (;;) {
        msleep(TICK_MS);
        tick++;
        lockShm();
        if (tick % COUNTER_EVERY == 0) COUNTER++;
        if (tick % LOG_EVERY == 0) {
            if (LEADER == 0 || !pidAlive(LEADER)) {
                LEADER = getPid();
                g_is_leader = 1;
            }
            char t[40];
            char msg[128];
            nowStr(t, sizeof t);
            snprintf(msg, sizeof msg, "main pid=%d time=%s counter=%d", getPid(), t, COUNTER);
            logMsg(msg);
        }
        unlockShm();
        if (g_is_leader && tick % SPAWN_EVERY == 0) spawnCopies();
    }
}

static int launchCopy(int which) {
#ifdef _WIN32
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    char exe[MAX_PATH], cmd[MAX_PATH + 32];

    GetModuleFileNameA(NULL, exe, MAX_PATH);
    snprintf(cmd, sizeof cmd, "\"%s\" type=copy%d", exe, which);
    ZeroMemory(&si, sizeof si); si.cb = sizeof si;
    ZeroMemory(&pi, sizeof pi);
    if (!CreateProcessA(NULL, cmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) return -1;
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    return (int)pi.dwProcessId;
#else
    pid_t pid = fork();
    if (pid == 0) {       
        copyWork(which);
        _exit(0);
    }
    if (pid < 0) return -1;
    return (int)pid;
#endif
}
static void spawnCopies(void) {
    #ifndef _WIN32
        while (waitpid(-1, NULL, WNOHANG) > 0) {}
    #endif
    lockShm();
    int need[2] = { !pidAlive(COPY1), !pidAlive(COPY2) };
    int old[2]  = { COPY1, COPY2 };
    unlockShm();

    for (int which = 1; which <= 2; which++) {
        if (!need[which - 1]) {
            char msg[128];
            lockShm();
            snprintf(msg, sizeof msg, "copy%d pid=%d still running, skip spawn", which, old[which - 1]);
            logMsg(msg);
            unlockShm();
            continue;
        }
        int pid = launchCopy(which);
        lockShm();
        ((int*)g_shm.mapped_memory_ptr)[1 + which] = pid;
        unlockShm();
    }
}
