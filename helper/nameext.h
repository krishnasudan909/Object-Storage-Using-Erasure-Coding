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