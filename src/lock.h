// lock.h — именованный межпроцессный лок (мьютекс/семафор), один на весь проект.
// каждая программа открывает его по имени, поэтому блокировка видна всем процессам.
#ifndef LAB_LOCK_H
#define LAB_LOCK_H

int  initLock(void);
void closeLock(void);
void lockShm(void);
void unlockShm(void);

#endif
