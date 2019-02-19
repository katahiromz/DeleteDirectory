// DeleteDirectory.h --- delete directory in C/C++
// Copyright (C) 2019 Katayama Hirofumi MZ <katayama.hirofumi.mz@gmail.com>.
// This file is public domain software.
#ifndef DELETE_DIRECTORY_H_
#define DELETE_DIRECTORY_H_     10  /* Version 10 */

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

#ifdef __cplusplus
extern "C" {
#endif

/* deletes a directory tree */
BOOL WINAPI DeleteDirectoryA(LPCSTR dir);
BOOL WINAPI DeleteDirectoryW(LPCWSTR dir);

#ifdef __cplusplus
} // extern "C"
#endif

#ifdef __cplusplus
    #include <string>
    #include <vector>

    #ifdef UNICODE
        typedef std::wstring tstring;
    #else
        typedef std::string tstring;
    #endif

    BOOL DirItemListA(std::vector<std::string>& items, LPCSTR dir);
    BOOL DirItemListW(std::vector<std::wstring>& items, LPCWSTR dir);

    BOOL DirListA(std::vector<std::string>& paths, LPCSTR item);
    BOOL DirListW(std::vector<std::wstring>& paths, LPCWSTR item);
#endif

#endif  /* ndef DELETE_DIRECTORY_H_ */
