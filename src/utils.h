#ifndef LAB_UTILS_H
#define LAB_UTILS_H

#include <stddef.h>

int getPid(void);
int pidAlive(int pid);
void msleep(int ms);
void nowStr(char *buf, size_t n);
void logMsg(const char *line);

#endif
