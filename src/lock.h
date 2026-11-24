#ifndef LAB_LOCK_H
#define LAB_LOCK_H

int  initLock(void);
void closeLock(void);
void lockShm(void);
void unlockShm(void);

#endif
