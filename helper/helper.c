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
int mergeFile(char *filename, size_t splitSize, unsigned char *buffer[4], unsigned char *parity[3]){
    char splitlocation[100];
    FILE *fIn;
    FILE *fOut;
    int boolflag = 0;
    char resultfilelocation[100];
    char dirname[3];
    sprintf(resultfilelocation, "result/%s", filename);

    fOut = fopen(resultfilelocation, "wb");
    for (int i = 1; i <= 4; i++)
    {
        sprintf(splitlocation, "d%d/%s.00%d", i, filename, i);
        // printf("%s\n", splitlocation);
        if(access(splitlocation, F_OK) == 0){
            fIn = fopen(splitlocation, "rb");
            fread(buffer[i-1], splitSize, 1, fIn);
            // printf("%s\n", buffer);
            fwrite(buffer[i-1], splitSize, 1, fOut);
            fclose(fIn);
            fIn = NULL;
        }
        else
        {
            printf("The split at %s has been deleted\n", splitlocation);
            boolflag++;
        }
        
    }
    for (int i = 1; i <= 3; i++)
    {
        sprintf(dirname, "p%d", i);
        if(access(dirname, F_OK) == 0){
            sprintf(splitlocation, "%s/%s.%03d", dirname, filename, i);
            fIn = fopen(splitlocation, "rb");
            fread(parity[i-1], splitSize, 1, fIn);
            fclose(fIn);
            fIn = NULL;
        }
        else
        {
            printf("The parity folder at %s is deleted.\nAborting..", dirname);
            return -2;
        }
    }
    
    fclose(fOut);
    if (boolflag > 0)
    {
        remove(resultfilelocation);
        return boolflag;
    }

    return 0;
    
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
        if(access(dirname, F_OK) == 0){
        sprintf(filename, "%s/%s.%03d", dirname, fileIn, result);
        fOut = fopen(filename, "wb");

        fread(buffer[bufferIndex], maxSize, 1, fIn);
        
        fwrite(buffer[bufferIndex], maxSize, 1, fOut );
        printf("Successfully put in the %s drectory\n", dirname);
        buffer[bufferIndex][maxSize-1] = '\0';
        bufferIndex += 1;
        
        fclose(fOut);
        
        result++;
        }
        else
        {
            printf("The directory %s does not exist \nCreating...\n", dirname);
            mkdir(dirname,0777);
        }
        
    }

    fclose(fIn);
    return result-1;
}

//Put Parity
void putParity(char *fileIn, size_t maxSize, unsigned char *buffer[3]){
    int index = 1;
    char dirname[3];
    char filename[50];
    FILE *fOut;
    while (index <= 3)
    {
        sprintf(dirname, "p%d", index);
        if(access(dirname, F_OK) == 0){
            sprintf(filename, "%s/%s.%03d", dirname, fileIn, index);
            fOut = fopen(filename, "wb");
            fwrite(buffer[index - 1], maxSize, 1, fOut );
            printf("Successfully put in the %s drectory\n", dirname);
            fclose(fOut);
            index++;
        }
        else
        {
            printf("The directory %s does not exist \nCreating...\n", dirname);
            mkdir(dirname,0777);
        }
    }
    
}