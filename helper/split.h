int splitFile(char *fileIn, size_t maxSize){
    int result = 1;
    FILE *fIn;
    FILE *fOut;
    char buffer[maxSize];
    char dirname[3];
    char filename[50];
    size_t size;
    size_t read;
    size_t written;


    fIn = fopen(fileIn, "rb");
    fOut = NULL;
    while (!feof(fIn)){
        sprintf(dirname, "d%d", result);
        mkdir(dirname,0777);
        sprintf(filename, "%s/%s.%03d", dirname, fileIn, result);
        fOut = fopen(filename, "wb");

        fread(buffer, maxSize, 1, fIn);
        fwrite(buffer, maxSize, 1, fOut );
        fclose(fOut);
        result++;
    }
    return result;
}