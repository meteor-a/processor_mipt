#ifndef BASE_LIB_H__
#define BASE_LIB_H__

/*--------------------------------------------------------------------------*/

#include <stdio.h>
#include <assert.h>

#ifdef __linux__ 
    #include <unistd.h>
    #include <fcntl.h>
    #include <cstddef>
#elif _WIN32
    #include <windows.h>
    #include <sys\stat.h>
#endif

/*--------------------------------------------------------------------------*/

#ifdef __linux__

#define _IsBadReadPtr(pointer) _IsBadReadPtr__(pointer, sizeof(pointer))    

static bool _IsBadReadPtr__(void* pointer, size_t size) {
    int nullfd = open("/dev/random", O_WRONLY);

    if (write(nullfd, pointer, size) < 0) {
        return true;
    }
    close(nullfd);

    return false;
}

#elif _WIN32
#define _IsBadReadPtr(pointer) _IsBadReadPtr__(pointer)

static bool _IsBadReadPtr__(void* p) {
    MEMORY_BASIC_INFORMATION mbi = { 0 };
    if (::VirtualQuery(p, &mbi, sizeof(mbi))) {
        DWORD mask = (PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY);
        bool b = !(mbi.Protect & mask);

        // check the page is not a guard page
        if (mbi.Protect & (PAGE_GUARD | PAGE_NOACCESS)) {
            b = true;
        }

        return b;
    }

    return true;
}

#endif

/*--------------------------------------------------------------------------*/

#ifdef _WIN32

static int file_cmp(const char* file1, const char* file2) {
    struct _stat st1;
    struct _stat st2;

    _stat(file1, &st1);
    _stat(file2, &st2);

    if (st1.st_mtime < st2.st_mtime) {
        return -1;
    }
    else {
        if (st1.st_mtime > st2.st_mtime) {
            return 1;
        }
        else {
            return 0;
        }
    }
}

#endif


/*--------------------------------------------------------------------------*/


static long long getFileSize(FILE* file/*!< - Pointer to opened file*/) {
    assert(file != nullptr);
    if (file == nullptr) {
        return -1;
    }

    fseek(file, 0L, SEEK_END);
    long long sz = ftell(file);
    rewind(file);

    return sz;
}

#endif

#define LOCATION__(code) __FILE__, __LINE__
