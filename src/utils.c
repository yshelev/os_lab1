#include "utils.h"
#include "state.h"

#include <stdio.h>
#include <string.h>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <signal.h>
    #include <sys/time.h>
    #include <time.h>
    #include <unistd.h>
#endif

int getPid(void) {
#ifdef _WIN32
    return (int)GetCurrentProcessId();
#else
    return (int)getpid();
#endif
}

int pidAlive(int pid) {
    if (pid <= 0) return 0;
#ifdef _WIN32
    HANDLE h = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, (DWORD)pid);
    if (h == NULL) return 0;
    DWORD code = 0;
    int alive = (GetExitCodeProcess(h, &code) && code == STILL_ACTIVE);
    CloseHandle(h);
    return alive;
#else
    return kill((pid_t)pid, 0) == 0;
#endif
}

void msleep(int ms) {
#ifdef _WIN32
    Sleep(ms);
#else
    struct timespec ts = { ms / 1000, (ms % 1000) * 1000000L };
    nanosleep(&ts, NULL);
#endif
}

void nowStr(char *buf, size_t n) {
#ifdef _WIN32
    SYSTEMTIME st;
    GetLocalTime(&st);
    snprintf(buf, n, "%04u-%02u-%02u %02u:%02u:%02u.%03u",
             st.wYear, st.wMonth, st.wDay,
             st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
#else
    struct timeval tv;
    gettimeofday(&tv, NULL);
    struct tm tm;
    localtime_r(&tv.tv_sec, &tm);
    snprintf(buf, n, "%04d-%02d-%02d %02d:%02d:%02d.%03d",
             tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
             tm.tm_hour, tm.tm_min, tm.tm_sec, (int)(tv.tv_usec / 1000));
#endif
}

void logMsg(const char *line) {
    FILE *f = fopen(LOG_NAME, "a");
    if (!f) return;
    fputs(line, f);
    fputc('\n', f);
    fclose(f);
}
