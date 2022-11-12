# Object-Storage-Using-Erasure-Coding-

## To compile use the following command:
## gcc main.c -o main -l isal -I "path to the isal include directory"

### Helper description:
1. absolutepath.h - It returns the absolute path of a valid relative path to any file. The parameter is const char* and the return type is char*
2. fullSize.h - It return the full size of any file in bytes. The parameter is const char* and the return type is off_t.
3. nameext.h - It has two functions fullName and get_filename_ext which return the name(with extension) and only extension respectively. The input parameter is const char and the return type is char* and const char*.
4. split.h - It splits the file into 4 almost equal parts (the last one might be little less) and puts them into 4 different directories namely d1, d2, d3 and d4.
The parameters are file location and the split size.
5. merge.h - It is a reverse operation of split.h, It stiches the split file back with the same name. It accepts the same parameters.
6. helper.h - It includes all those previous ".h" files so that in main.c we have to only include the helper.h file.
