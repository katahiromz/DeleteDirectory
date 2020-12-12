#include "DeleteDirectoryLinux.h"
#include <stdlib.h>
#include <stdio.h>

#define _POSIX_SOURCE
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#undef _POSIX_SOURCE

#ifdef __cplusplus
extern "C" {
#endif

bool DeleteDirectoryLinux(const char *pathname)
{
    DIR *dir;
    struct dirent *entry;
    char path[260];
    struct stat info;
    size_t len = strlen(pathname);
    if (len >= sizeof(path))
        return false;

    dir = opendir(pathname);
    if (!dir)
        return false;

    for (entry = readdir(dir); entry; entry = readdir(dir))
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        if (len > 0 && pathname[len - 1] == '/')
            snprintf(path, sizeof(path), "%s%s", pathname, entry->d_name);
        else
            snprintf(path, sizeof(path), "%s/%s", pathname, entry->d_name);

        if (stat(path, &info) == 0)
        {
            if (S_ISDIR(info.st_mode))
                DeleteDirectoryLinux(path);
            else
                unlink(path);
        }
    }

    closedir(dir);

    return rmdir(pathname) == 0;
}

#ifdef __cplusplus
} // extern "C"
#endif
