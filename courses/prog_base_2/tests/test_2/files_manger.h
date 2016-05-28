#ifndef FILES_MANGER_H_INCLUDED
#define FILES_MANGER_H_INCLUDED

#include <time.h>

/**
*   @return the existence of directory
*/
int dir_exists(const char * dirname);

/**
*   Prints out list of files in directory
*
*   @return number of files
*/
int dir_printFiles(const char * dirname);

/**
*   @return the existence of file
*/
int file_exists(const char * filename);

/**
*   @return -1 if file not found
*/
long long file_getSize(const char * filename);

time_t file_getCreateTime(const char * filename);

/**
*   @return success of the action
*/
int file_create(const char * filename);

/**
*   @return success of the action
*/
int file_remove(const char * filename);

#endif // FILES_MANGER_H_INCLUDED
