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

__inline BOOL WINAPI DeleteDirectory(LPCTSTR dir)
{
    TCHAR dir_old[MAX_PATH];
    HANDLE hFind;
    WIN32_FIND_DATA find;
    LPTSTR pch;

    GetCurrentDirectory(MAX_PATH, dir_old);
    if (!SetCurrentDirectory(dir))
        return FALSE;

    hFind = FindFirstFile(TEXT("*"), &find);
    if (hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            pch = find.cFileName;
            if (pch[0] == TEXT('.') && 
                (pch[1] == 0 || (pch[1] == TEXT('.') && pch[2] == 0)))
            {
                // "." or ".."
            }
            else
            {
                if (find.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                {
                    DeleteDirectory(find.cFileName);
                }
                else
                {
                    SetFileAttributes(find.cFileName, FILE_ATTRIBUTE_NORMAL);
                    DeleteFile(find.cFileName);
                }
            }
        } while(FindNextFile(hFind, &find));
        FindClose(hFind);
    }
    SetCurrentDirectory(dir_old);

    SetFileAttributes(dir, FILE_ATTRIBUTE_DIRECTORY);
    return RemoveDirectory(dir);
}

#endif  /* ndef DeleteDirectory */
