#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include<libgen.h>

char *absolutePath(const char *filename);

off_t fullSize(const char *filename);

void mergeFile(char *filename, size_t splitSize);

char *fullName(const char *path);

const char *get_filename_ext(const char *filename);

int splitFile(char *fileIn, size_t maxSize, unsigned char *buffer[4]);