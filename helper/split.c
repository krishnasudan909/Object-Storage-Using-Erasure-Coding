#include "helper.h"

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