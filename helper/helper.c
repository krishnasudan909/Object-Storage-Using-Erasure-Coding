#include "helper.h"

//Absolute Path
char *absolutePath(const char *filename){
    char *path = realpath(filename, NULL);
    if (path == NULL)
    {
        printf("\nCannot find the file\n");
    }
    else
    {
        return path;
    }
    
    return NULL;
}

//Full Size
off_t fullSize(const char *filename) {
    struct stat st;

    if (stat(filename, &st) == 0)
        return st.st_size;

    fprintf(stderr, "Cannot determine size of %s: %s\n",
            filename, strerror(errno));

    return -1;
}

//Merge file
void mergeFile(char *filename, size_t splitSize){
    char splitlocation[100];
    FILE *fIn;
    FILE *fOut;
    char buffer[splitSize];
    char resultfilelocation[100];
    mkdir("result",0777);
    sprintf(resultfilelocation, "result/%s", filename);

    fOut = fopen(resultfilelocation, "wb");
    for (int i = 1; i <= 4; i++)
    {
        sprintf(splitlocation, "d%d/%s.00%d", i, filename, i);
        // printf("%s\n", splitlocation);
        fIn = fopen(splitlocation, "rb");
        fread(buffer, splitSize, 1, fIn);
        // printf("%s\n", buffer);
        fwrite(buffer, splitSize, 1, fOut);
        fclose(fIn);
        fIn = NULL;
    }
    fclose(fOut);
}

//Name extension
char *fullName(const char *path){
    char* ts1 = strdup(path);
    char* filename = basename(ts1);

    return filename;
}

const char *get_filename_ext(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot + 1;
}

//Split file
int splitFile(char *fileIn, size_t maxSize, unsigned char *buffer[4]){
    int result = 1;
    FILE *fIn;
    FILE *fOut;
    
    char dirname[3];
    char filename[50];


    fIn = fopen(fileIn, "rb");
    fileIn = fullName(fileIn);
    fOut = NULL;
    int bufferIndex = 0;
    while(bufferIndex<4){
        sprintf(dirname, "d%d", result);
        mkdir(dirname,0777);
        sprintf(filename, "%s/%s.%03d", dirname, fileIn, result);
        fOut = fopen(filename, "wb");

        fread(buffer[bufferIndex], maxSize, 1, fIn);
        
        fwrite(buffer[bufferIndex], maxSize, 1, fOut );
        buffer[bufferIndex][maxSize-1] = '\0';
        bufferIndex += 1;
        
        fclose(fOut);
        result++;
    }

    fclose(fIn);
    return result-1;
}