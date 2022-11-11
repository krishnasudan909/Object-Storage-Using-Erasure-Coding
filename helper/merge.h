void mergeFile(char *filename, size_t splitSize){
    char splitlocation[100];
    FILE *fIn;
    FILE *fOut;
    char buffer[splitSize];

    fOut = fopen(filename, "wb");
    for (int i = 1; i <= 4; i++)
    {
        sprintf(splitlocation, "d%d/%s.00%d", i, filename, i);
        printf("%s\n", splitlocation);
        fIn = fopen(splitlocation, "rb");
        fread(buffer, splitSize, 1, fIn);
        // printf("%s\n", buffer);
        fwrite(buffer, splitSize, 1, fOut);
        fclose(fIn);
        fIn = NULL;
    }
    fclose(fOut);
}