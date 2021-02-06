#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
/**
 * Function to check whether a directory exists or not.
 * It returns 1 if given path is directory and  exists 
 * otherwise returns 0.
 */

int isDirectoryExists(const char *path)
{
    struct stat stats;

    stat(path, &stats);

    // Check for file existence
    if (S_ISDIR(stats.st_mode))
        return 1;

    return 0;
}
