// main.c — точка входа: выбор режима type=main|cli|copy1|copy2.
#include <sharedMemoryModule.h>
#include "state.h"
#include "lock.h"
#include "utils.h"
#include "app.h"

#include <stdio.h>
#include <string.h>

shm_file_type g_shm;

int main(int argc, char **argv) {
    const char *type = "main";
    if (argc > 1 && strncmp(argv[1], "type=", 5) == 0) type = argv[1] + 5;
    if (strcmp(type, "main") && strcmp(type, "cli") &&
        strcmp(type, "copy1") && strcmp(type, "copy2")) {
        fprintf(stderr, "usage: type=main|cli\n");
        return 1;
    }

    if (strcmp(type, "cli") == 0) {
        if (!initLock()) return 1;
        if (createSharedMem(SHM_NAME, SHM_SIZE, &g_shm) != SHM_OK) { closeLock(); return 1; }
        printf("cli: enter counter value\n");
        char line[128];
        while (fgets(line, sizeof line, stdin)) {
            int v;
            if (sscanf(line, "%d", &v) == 1) {
                lockShm();
                writeValueToMemory(&g_shm, v);
                unlockShm();
                printf("counter = %d\n", v);
            } else {
                printf("enter a number\n");
            }
        }
        closeSharedMem(&g_shm);
        closeLock();
        return 0;
    }

    if (strcmp(type, "copy1") == 0 || strcmp(type, "copy2") == 0) {
        if (!initLock()) return 1;
        if (createSharedMem(SHM_NAME, SHM_SIZE, &g_shm) != SHM_OK) { closeLock(); return 1; }
        copyWork(strcmp(type, "copy1") == 0 ? 1 : 2);
        closeSharedMem(&g_shm);
        closeLock();
        return 0;
    }

    if (!initLock()) { fprintf(stderr, "lock init failed\n"); return 1; }
    SharedMemoryErrors err = createSharedMem(SHM_NAME, SHM_SIZE, &g_shm);
    if (err != SHM_OK) {
        fprintf(stderr, "shared memory error %d\n", (int)err);
        closeLock();
        return 1;
    }

    char t[40];
    nowStr(t, sizeof t);
    lockShm();
    if (LEADER == 0 || !pidAlive(LEADER)) {
        LEADER = getPid();
        g_is_leader = 1;
    }
    char msg[128];
    snprintf(msg, sizeof msg, "main pid=%d start=%s leader=%s", getPid(), t, g_is_leader ? "yes" : "no");
    logMsg(msg);
    unlockShm();

    appRun();
}
