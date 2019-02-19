#include "DeleteDirectory.h"
#include <stdio.h>
#include <assert.h>

int main(void)
{
    CreateDirectoryA("a1", NULL);
    CreateDirectoryA("a1/b1", NULL);
    CreateDirectoryA("a1/b1/c1", NULL);
    CreateDirectoryA("a1/b1/c2", NULL);
    CreateDirectoryA("a1/b2", NULL);
    CreateDirectoryA("a1/b2/c1", NULL);
    CreateDirectoryA("a1/b2/c2", NULL);
    CreateDirectoryA("a1/b3", NULL);

    fclose(fopen("a1/a1.txt", "wb"));
    fclose(fopen("a1/b1/b1.txt", "wb"));
    fclose(fopen("a1/b2/b2.txt", "wb"));
    fclose(fopen("a1/b3/b3.txt", "wb"));
    fclose(fopen("a1/b1/c1/c1.txt", "wb"));
    fclose(fopen("a1/b1/c2/c2.txt", "wb"));
    fclose(fopen("a1/b2/c1/c1.txt", "wb"));
    fclose(fopen("a1/b2/c2/c2.txt", "wb"));

    if (DeleteDirectory(TEXT("a1")))
    {
        puts("success");
    }
    else
    {
        puts("failure");
        assert(0);
    }

    return 0;
}
