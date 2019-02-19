// DeleteDirectory.cpp --- delete directory in C/C++
// Copyright (C) 2019 Katayama Hirofumi MZ <katayama.hirofumi.mz@gmail.com>.
// This file is public domain software.

#include "DeleteDirectory.h"

#include <cassert>
#include <shlwapi.h>
#ifndef NO_STRSAFE
    #include <strsafe.h>
#endif

#ifdef UNICODE
    typedef std::wstring tstring;
#else
    typedef std::string tstring;
#endif
typedef std::vector<tstring> container_type;

#ifndef ARRAYSIZE
    #define ARRAYSIZE(array) (sizeof(array) / sizeof((array)[0]))
#endif

/* get the item list */
BOOL WINAPI DirItemList(container_type& items, LPCTSTR dir)
{
    // create the wildcard specifier from dir
    TCHAR szPath[MAX_PATH];
#ifdef NO_STRSAFE
    lstrcpyn(szPath, dir, ARRAYSIZE(szPath));
#else
    StringCbCopy(szPath, sizeof(szPath), dir);
#endif
    PathAppend(szPath, TEXT("*"));

    // start enumerating
    WIN32_FIND_DATA find;
    HANDLE hFind = FindFirstFile(szPath, &find);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        assert(0);
        return FALSE;
    }

    // for each item
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
            // add it
            items.push_back(find.cFileName);
        }
    } while (FindNextFile(hFind, &find));   // go next

    // end enumerating
    FindClose(hFind);

    return TRUE;
}

/* get the directory list */
BOOL WINAPI DirList(container_type& paths, LPCTSTR item)
{
    if (!PathFileExists(item))
    {
        // must exist
        assert(0);
        return FALSE;
    }

    size_t i = paths.size();
    paths.push_back(item);

    tstring path;
    TCHAR szPath[MAX_PATH];
    for (; i < paths.size(); ++i)
    {
        path = paths[i];

        // not directory?
        if (!PathIsDirectory(path.c_str()))
            continue;

        // get the items of the path
        container_type items;
        DirItemList(items, path.c_str());

        // add the items with fixing as path
        container_type::const_iterator it, end = items.end();
        for (it = items.begin(); it != end; ++it)
        {
#ifdef NO_STRSAFE
            lstrcpyn(szPath, path.c_str(), ARRAYSIZE(szPath));
#else
            StringCbCopy(szPath, sizeof(szPath), path.c_str());
#endif
            PathAppend(szPath, it->c_str());
            paths.push_back(szPath);
        }
    }

    return TRUE;
}

#ifdef __cplusplus
extern "C" {
#endif

/* deletes a directory tree */
BOOL WINAPI DeleteDirectory(LPCTSTR dir) M_NOEXCEPT
{
    try
    {
        // dir must be a directory
        if (!PathIsDirectory(dir))
        {
            // not a directory
            assert(0);
            return FALSE;
        }

        // get the directory list
        container_type paths;
        if (!DirList(paths, dir))
        {
            // failed
            assert(0);
            return FALSE;
        }

        // enumerate paths in reverse order
        container_type::const_reverse_iterator rit, rend = paths.rend();
        for (rit = paths.rbegin(); rit != rend; ++rit)
        {
            const tstring& path = *rit;
            const TCHAR *psz = path.c_str();

            // is it a directory?
            if (PathIsDirectory(psz))
            {
                /* remove the read-only attribute */
                SetFileAttributes(psz, FILE_ATTRIBUTE_DIRECTORY);

                // remove a directory
                if (!RemoveDirectory(psz))
                {
                    // unable to remove a directory
                    assert(0);
                    return FALSE;
                }
            }
            else
            {
                // remove the read-only attribute
                SetFileAttributes(psz, FILE_ATTRIBUTE_NORMAL);

                // delete a file
                if (!DeleteFile(psz))
                {
                    // unable to delete a file
                    assert(0);
                    return FALSE;
                }
            }
        }
    }
    catch (...)
    {
        // maybe std::bad_alloc
        assert(0);
        return FALSE;
    }

    return !PathFileExists(dir);
}

#ifdef __cplusplus
} /* extern "C" */
#endif
