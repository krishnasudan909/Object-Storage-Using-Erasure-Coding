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

int mergeFile(char *filename, size_t splitSize, unsigned char *buffer[8], unsigned char *parity[3], unsigned char buffererror[3]);

char *fullName(const char *path);

const char *get_filename_ext(const char *filename);

int splitFile(char *fileIn, size_t maxSize, unsigned char *buffer[8]);

void putParity(char *fileIn, size_t maxSize, unsigned char *buffer[3]);