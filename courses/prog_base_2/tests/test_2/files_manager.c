#include "files_manger.h"

#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <windows.h>

/**
*   @return the existence of directory
*/
int dir_exists(const char * dirname) {
    struct stat buffer;
    return (stat (dirname, &buffer) == 0) && S_ISDIR(buffer.st_mode);
}

/**
*   Prints out list of files in directory
*
*   @return number of files
*/
int dir_printFiles(const char * dirname) {
    DIR *dp;
    struct dirent *ep;
    int file_count = 0;
    dp = opendir (dirname);
    if (dp != NULL) {
        while ((ep = readdir (dp))) {
            if(32 == dp->dd_dta.attrib) {
                puts(dp->dd_dir.d_name);
                file_count++;
            }
        }
        (void) closedir (dp);
        return file_count;
    }
    else {
        return -1;
    }
}

/**
*   @return the existence of file
*/
int file_exists(const char * filename)
{
    struct stat buffer;
    return (stat (filename, &buffer) == 0);
}

/**
*   @return -1 if file not found
*/
long long file_getSize(const char * filename) {
    struct stat st;
    if (0 != stat(filename, &st)) {
        return -1;
    }
    long long size = st.st_size;
    return size;
}

time_t file_getCreateTime(const char * filename) {
    struct stat st;
    if (0 != stat(filename, &st)) {
        return (time_t)0;
    }
    return st.st_ctime;
}

/**
*   @return success of the action
*/
int file_create(const char * filename) {
    FILE * fp;
    int success = NULL != (fp = fopen(filename, "ab+"));
    if (!success) {
        return 0;
    }
    fclose(fp);
    return 1;
}

/**
*   @return success of the action
*/
int file_remove(const char * filename) {
    return 0 == remove(filename);
}
