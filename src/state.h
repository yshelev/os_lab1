#ifndef STATE_H
#define STATE_H

#include <sharedMemoryModule.h>

#ifdef _WIN32
    #define SHM_NAME "Local\\LabShm"
#else
    #define SHM_NAME "/LabShm"
#endif
#define SHM_SIZE 1024
#define LOG_NAME "lab.log"

extern shm_file_type g_shm;    
extern int g_is_leader;     

#define COUNTER (((int*)g_shm.mapped_memory_ptr)[0])
#define LEADER (((int*)g_shm.mapped_memory_ptr)[1])
#define COPY1 (((int*)g_shm.mapped_memory_ptr)[2])
#define COPY2 (((int*)g_shm.mapped_memory_ptr)[3])

#endif
