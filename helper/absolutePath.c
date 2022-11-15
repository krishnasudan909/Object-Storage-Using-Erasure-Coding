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