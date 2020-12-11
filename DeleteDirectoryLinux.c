#include "DeleteDirectoryLinux.h"
#include <stdlib.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

int DeleteDirectoryLinux(const char *dir)
{
    char cmdline[260];
    sprintf(cmdline, "rm -fr \"%s\"", dir);
    return !system(cmdline);
}

#ifdef __cplusplus
} // extern "C"
#endif
