/* DeleteDirectory.h --- delete directory in C/C++ */
/* Copyright (C) 2019 Katayama Hirofumi MZ <katayama.hirofumi.mz@gmail.com>. */
/* This file is public domain software. */
#ifndef DELETE_DIRECTORY_H_
#define DELETE_DIRECTORY_H_     13  /* Version 13 */

#ifndef _INC_WINDOWS
    #include <windows.h>
#endif

#ifdef UNICODE
    #define DeleteDirectory DeleteDirectoryW
    #define DirItemList DirItemListW
    #define DirList DirListW
#else
    #define DeleteDirectory DeleteDirectoryA
    #define DirItemList DirItemListA
    #define DirList DirListA
#endif

#ifndef M_NOEXCEPT
    #if defined(__cplusplus) && (__cplusplus >= 201103L)
        #define M_NOEXCEPT noexcept
    #else
        #define M_NOEXCEPT
    #endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* deletes a directory tree */
BOOL WINAPI DeleteDirectoryA(LPCSTR dir) M_NOEXCEPT;
BOOL WINAPI DeleteDirectoryW(LPCWSTR dir) M_NOEXCEPT;

#ifdef __cplusplus
} /* extern "C" */
#endif

#ifdef __cplusplus
    #include <string>
    #include <vector>

    /* get the item list */
    BOOL WINAPI DirItemListA(std::vector<std::string>& items, LPCSTR dir);
    BOOL WINAPI DirItemListW(std::vector<std::wstring>& items, LPCWSTR dir);

    /* get the directory list */
    BOOL WINAPI DirListA(std::vector<std::string>& paths, LPCSTR item);
    BOOL WINAPI DirListW(std::vector<std::wstring>& paths, LPCWSTR item);
#endif  /* C++ */

#endif  /* ndef DELETE_DIRECTORY_H_ */
