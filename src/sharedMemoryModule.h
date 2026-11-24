#ifndef SHAREDMEMORYMODULE_H
#define SHAREDMEMORYMODULE_H

#include <stdint.h>

typedef enum {
    SHM_OK = 0,
    SHM_CREATE_POINTER_ERR = -1,
    SHM_MEM_ALLOCATE_ERR = -2
} SharedMemoryErrors;

typedef struct {
    intptr_t file_descriptor;
    void* mapped_memory_ptr;
} shm_file_type;

int writeValueToMemory(shm_file_type *shm_obj, int value);
SharedMemoryErrors createSharedMem(char *mem_name, int shm_size, shm_file_type *out);
void closeSharedMem(shm_file_type *shm_obj);

#endif
