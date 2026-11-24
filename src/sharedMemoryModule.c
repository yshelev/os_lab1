#include <sharedMemoryModule.h>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <fcntl.h>
    #include <sys/mman.h>
    #include <sys/stat.h>
    #include <unistd.h>
#endif

intptr_t __crt_shm_win(char *shm_name, int shm_size);
int __crt_shm_posix(char *shm_name, int shm_size);
void* __map_shm_win(intptr_t fd, int shm_size);
void* __map_shm_posix(intptr_t fd, int shm_size);
void __cls_shm_win(shm_file_type *shm_obj);
void __cls_shm_posix(shm_file_type *shm_obj);

SharedMemoryErrors createSharedMem(char *shm_name, int shm_size, shm_file_type *out)
{
    intptr_t _fd;
    #ifdef _WIN32
        _fd = __crt_shm_win(shm_name, shm_size);
    #else
        _fd = (intptr_t) __crt_shm_posix(shm_name, shm_size);
    #endif
    if (_fd < 0) {
        return (SharedMemoryErrors)_fd;
    }
    out->file_descriptor = _fd;

    void* _mmap;
    #ifdef _WIN32
        _mmap = __map_shm_win(_fd, shm_size);
    #else
        _mmap = __map_shm_posix(_fd, shm_size);
    #endif

    if (_mmap == NULL || _mmap == (void*)(intptr_t)SHM_CREATE_POINTER_ERR) {
        return SHM_CREATE_POINTER_ERR;
    }

    out->mapped_memory_ptr = _mmap;
    out->shm_size = shm_size;
    return SHM_OK;
}

void closeSharedMem(shm_file_type *shm_obj) {
    #ifdef _WIN32
        __cls_shm_win(shm_obj);
    #else
        __cls_shm_posix(shm_obj);
    #endif
}

int writeValueToMemory(shm_file_type *shm_obj, int value) {
    int *ptr = (int*)shm_obj->mapped_memory_ptr;
    if (ptr == NULL) {
        return SHM_CREATE_POINTER_ERR;
    }
    *ptr = value;
    return SHM_OK;
}

#ifdef _WIN32
intptr_t __crt_shm_win(char* shm_name, int shm_size) {
    HANDLE file_descriptor = CreateFileMappingA(
        INVALID_HANDLE_VALUE,
        NULL,
        PAGE_READWRITE,
        0,
        shm_size,
        shm_name
    );

    if (file_descriptor == NULL) {
        return (intptr_t)SHM_CREATE_POINTER_ERR;
    }

    return (intptr_t)file_descriptor;
}
#else
int __crt_shm_posix(char* shm_name, int shm_size) {
    int file_descriptor = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
    if (file_descriptor == -1) {
        return SHM_CREATE_POINTER_ERR;
    }

    if (ftruncate(file_descriptor, shm_size) == -1) {
        close(file_descriptor);
        return SHM_MEM_ALLOCATE_ERR;
    }

    return file_descriptor;
}
#endif

#ifdef _WIN32
void* __map_shm_win(intptr_t fd, int shm_size) {
    void *ptr = MapViewOfFile((HANDLE)fd, FILE_MAP_ALL_ACCESS, 0, 0, shm_size);
    if (ptr == NULL) {
        return (void*)SHM_CREATE_POINTER_ERR;
    }
    return ptr;
}
#else
void* __map_shm_posix(intptr_t fd, int shm_size) {
    void *ptr = mmap(NULL, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, (int)fd, 0);
    if (ptr == MAP_FAILED) {
        close((int)fd);
        return (void*)(intptr_t)SHM_CREATE_POINTER_ERR;
    }
    return ptr;
}
#endif

#ifdef _WIN32
void __cls_shm_win(shm_file_type *shm_obj) {
    UnmapViewOfFile(shm_obj->mapped_memory_ptr);
    CloseHandle((HANDLE)shm_obj->file_descriptor);
}
#else
void __cls_shm_posix(shm_file_type *shm_obj) {
    munmap(shm_obj->mapped_memory_ptr, (size_t)shm_obj->shm_size);
    close((int)shm_obj->file_descriptor);
}
#endif
