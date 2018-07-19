#ifndef SWS_FILEIO_H
#define SWS_FILEIO_H
#define MAX_FILE_BUFFER 2097152
#include <stdio.h>

void read_file(char *filepath, char *dest, int max_len);

int write_file(char *filepath, char *src);
int is_absolute_path(char *filepath);

void get_absolute_path(char *relative_path, char *absolute_path);

#endif //SWS_FILEIO_H
