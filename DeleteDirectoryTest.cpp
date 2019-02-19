#include "DeleteDirectory.h"
#include <cstdio>
#include <cassert>
#include <algorithm>    // for std::sort

int main(void)
{
    using namespace std;
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

    bool details = false;

    if (details)
    {
        puts("Type Enter key");
        getchar();
    }

    std::vector<tstring> items;
    BOOL ret1 = DirItemList(items, TEXT("a1"));
    assert(ret1);

    std::sort(items.begin(), items.end());
#ifndef UNICODE
    if (details)
    {
        for (size_t i = 0; i < items.size(); ++i)
        {
            printf("%d: %s\n", (int)i, items[i].c_str());
        }
    }
#endif
    assert(items.size() == 4);
    assert(items[0] == TEXT("a1.txt"));
    assert(items[1] == TEXT("b1"));
    assert(items[2] == TEXT("b2"));
    assert(items[3] == TEXT("b3"));

    std::vector<tstring> paths;
    BOOL ret2 = DirList(paths, TEXT("a1"));
    assert(ret2);

    std::sort(paths.begin(), paths.end());
#ifndef UNICODE
    if (details)
    {
        for (size_t i = 0; i < paths.size(); ++i)
        {
            printf("%d: %s\n", (int)i, paths[i].c_str());
        }
    }
#endif
    assert(paths.size() == 16);
    assert(paths[0] == TEXT("a1"));
    assert(paths[1] == TEXT("a1\\a1.txt"));
    assert(paths[2] == TEXT("a1\\b1"));
    assert(paths[3] == TEXT("a1\\b1\\b1.txt"));
    assert(paths[4] == TEXT("a1\\b1\\c1"));
    assert(paths[5] == TEXT("a1\\b1\\c1\\c1.txt"));
    assert(paths[6] == TEXT("a1\\b1\\c2"));
    assert(paths[7] == TEXT("a1\\b1\\c2\\c2.txt"));
    assert(paths[8] == TEXT("a1\\b2"));
    assert(paths[9] == TEXT("a1\\b2\\b2.txt"));
    assert(paths[10] == TEXT("a1\\b2\\c1"));
    assert(paths[11] == TEXT("a1\\b2\\c1\\c1.txt"));
    assert(paths[12] == TEXT("a1\\b2\\c2"));
    assert(paths[13] == TEXT("a1\\b2\\c2\\c2.txt"));
    assert(paths[14] == TEXT("a1\\b3"));
    assert(paths[15] == TEXT("a1\\b3\\b3.txt"));

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
