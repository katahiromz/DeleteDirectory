// DeleteDirectory.c --- delete directory in C/C++
// Copyright (C) 2019 Katayama Hirofumi MZ <katayama.hirofumi.mz@gmail.com>.
// This file is public domain software.

#include "DeleteDirectory.h"

#include <cassert>
#include <algorithm>
#include <shlwapi.h>
#ifndef NO_STRSAFE
    #include <strsafe.h>
#endif

#ifdef UNICODE
    typedef std::wstring tstring;
#else
    typedef std::string tstring;
#endif

#ifndef ARRAYSIZE
    #define ARRAYSIZE(array) (sizeof(array) / sizeof((array)[0]))
#endif

BOOL DirItemList(std::vector<tstring>& items, LPCTSTR dir)
{
    TCHAR szPath[MAX_PATH];
    StringCbCopy(szPath, sizeof(szPath), dir);
    PathAppend(szPath, TEXT("*"));

    WIN32_FIND_DATA find;
    HANDLE hFind = FindFirstFile(szPath, &find);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        assert(0);
        return FALSE;
    }

    do
    {
        LPTSTR pch = find.cFileName;
        if (pch[0] == TEXT('.') && 
            (pch[1] == 0 || (pch[1] == TEXT('.') && pch[2] == 0)))
        {
            // "." or ".."
        }
        else
        {
            items.push_back(find.cFileName);
        }
    } while (FindNextFile(hFind, &find));

    FindClose(hFind);

    return TRUE;
}

BOOL DirList(std::vector<tstring>& paths, LPCTSTR item)
{
    if (!PathFileExists(item))
    {
        assert(0);
        return FALSE;
    }

    size_t i = paths.size(), k;
    paths.push_back(item);

    tstring path;
    TCHAR szPath[MAX_PATH];
    for (; i < paths.size(); ++i)
    {
        path = paths[i];
        if (!PathIsDirectory(path.c_str()))
            continue;

        std::vector<tstring> items;
        DirItemList(items, path.c_str());

        for (k = 0; k < items.size(); ++k)
        {
            StringCbCopy(szPath, sizeof(szPath), path.c_str());
            PathAppend(szPath, items[k].c_str());
            paths.push_back(szPath);
        }
    }

    return TRUE;
}

#ifdef __cplusplus
extern "C" {
#endif

/* deletes a directory tree */
BOOL WINAPI DeleteDirectory(LPCTSTR dir)
{
    if (!PathIsDirectory(dir))
    {
        assert(0);
        return FALSE;
    }

    std::vector<tstring> paths;
    if (!DirList(paths, dir))
    {
        assert(0);
        return FALSE;
    }

    for (size_t i = paths.size(); i--; )
    {
        tstring& path = paths[i];
        const TCHAR *psz = path.c_str();
        if (PathIsDirectory(psz))
        {
            if (!RemoveDirectory(psz))
            {
                assert(0);
            }
        }
        else
        {
            if (!DeleteFile(psz))
            {
                printf("%s\n", psz);
                assert(0);
            }
        }
    }

    return !PathFileExists(dir);
}

#ifdef __cplusplus
} // extern "C"
#endif
