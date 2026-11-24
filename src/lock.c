#include "lock.h"

#ifdef _WIN32
    #include <windows.h>
    #define LOCK_NAME "Local\\LabShmLock"
    static HANDLE g_lock = NULL;
#else
    #include <errno.h>
    #include <fcntl.h>
    #include <semaphore.h>
    #define LOCK_NAME "/LabShmLock"
    static sem_t *g_lock = 0;
#endif

int initLock(void) {
#ifdef _WIN32
    g_lock = CreateMutexA(NULL, FALSE, LOCK_NAME);
    return g_lock != NULL;
#else
    g_lock = sem_open(LOCK_NAME, O_CREAT, 0666, 1);
    return g_lock != SEM_FAILED;
#endif
}

void closeLock(void) {
#ifdef _WIN32
    if (g_lock) CloseHandle(g_lock);
#else
    if (g_lock != SEM_FAILED) sem_close(g_lock);
#endif
}

void lockShm(void) {
#ifdef _WIN32
    WaitForSingleObject(g_lock, INFINITE);
#else
    while (sem_wait(g_lock) != 0 && errno == EINTR) {}
#endif
}

void unlockShm(void) {
#ifdef _WIN32
    ReleaseMutex(g_lock);
#else
    sem_post(g_lock);
#endif
}
