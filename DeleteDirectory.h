// DeleteDirectory.h --- delete directory in C/C++
// Copyright (C) 2019 Katayama Hirofumi MZ <katayama.hirofumi.mz@gmail.com>.
// This file is public domain software.
#ifndef DeleteDirectory

#ifndef _WIN32
    #error DeleteDirectory requires Win32 C/C++ compiler.
#endif

#ifndef _INC_WINDOWS
    #include <windows.h>
#endif

#ifdef UNICODE
    #define DeleteDirectory DeleteDirectoryW
#else
    #define DeleteDirectory DeleteDirectoryA
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* recursively deletes a directory tree */
BOOL WINAPI DeleteDirectory(LPCTSTR dir);

#ifdef __cplusplus
} // extern "C"
#endif

#endif  /* ndef DeleteDirectory */
