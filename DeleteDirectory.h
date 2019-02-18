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

#ifdef __cplusplus
    #include <cassert>
#else
    #include <assert.h>
#endif

#ifdef UNICODE
    #define DeleteDirectory DeleteDirectoryW
#else
    #define DeleteDirectory DeleteDirectoryA
#endif

/* recursively deletes a directory tree */
__inline BOOL WINAPI DeleteDirectory(LPCTSTR dir)
{
    TCHAR dir_old[MAX_PATH];
    HANDLE hFind;
    WIN32_FIND_DATA find;
    LPTSTR pch;

    /* get the current directory */
    GetCurrentDirectory(MAX_PATH, dir_old);

    /* move to the directory */
    if (!SetCurrentDirectory(dir))
    {
        /* failed to move the current directory */
        assert(0);
        return FALSE;
    }

    /* get the filesystem item list */
    hFind = FindFirstFile(TEXT("*"), &find);
    if (hFind != INVALID_HANDLE_VALUE)
    {
        /* for each item */
        do
        {
            /* check the item name */
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
                    /* a directory */
                    DeleteDirectory(find.cFileName);
                }
                else    /* a file */
                {
                    /* remove the read-only attribute */
                    SetFileAttributes(find.cFileName, FILE_ATTRIBUTE_NORMAL);

                    if (!DeleteFile(find.cFileName))
                    {
                        /* failed to delete a file */
                        assert(0);
                    }
                }
            }
        } while (FindNextFile(hFind, &find));   /* get the next */

        /* close the list */
        FindClose(hFind);
    }
    else
    {
        /* failed to get the list */
        assert(0);
    }

    /* back to the saved directory */
    SetCurrentDirectory(dir_old);

    /* remove the read-only attribute */
    SetFileAttributes(dir, FILE_ATTRIBUTE_DIRECTORY);

    /* remove the directory */
    if (!RemoveDirectory(dir))
    {
        /* failed to remove a directory (maybe not empty) */
        assert(0);
        return FALSE;
    }

    return TRUE;
}

#endif  /* ndef DeleteDirectory */
