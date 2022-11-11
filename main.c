

#include "helper/helper.h"

struct FileData
{
    char* file_name;
    const char* file_type;
    char* file_path;
    off_t  file_size;
    size_t UID;

    struct FileData* next;

};

int totId = 1;


void ec_encode(){

  char splitlocation[100];
  sprintf(splitlocation, "d1/%s.001", current->file_name);
  FILE *f = fopen(splitlocation, "rb");
  FILE* f2 = fopen(current->file_name, "wb" );
}



void put(struct FileData** head_ref) {
  struct FileData* new_FileData = (struct FileData*)malloc(sizeof(struct FileData));
  struct FileData* last = *head_ref; /* used in step 5*/

  char path[100];
//   printf("Enter file path: \n");
  scanf("%s", &path);
  if (access(path, F_OK) == 0){
    new_FileData->file_name = fullName(path);
    new_FileData->file_path = absolutePath(path);
    new_FileData->file_size = fullSize(path);
    new_FileData->file_type = get_filename_ext(fullName(path));
    new_FileData->UID = totId++;
    new_FileData->next = NULL;
    printf("The file has been split to %d parts with maximum size as %lf\n", splitFile(path, ceil((double)fullSize(path)/4)), ceil((double)fullSize(path)/4));
    printf("The file has been added with ID %jd\n", new_FileData->UID);
    if (*head_ref == NULL) {
    *head_ref = new_FileData;
    return;
    }

    while (last->next != NULL) last = last->next;

    last->next = new_FileData;

    
  }
  else
  {
    printf("The file is not accesible\n");
  }
  
  return;
}


void get(struct FileData** head_ref) {
  struct FileData* current = *head_ref;
  size_t key;
//   printf("Enter the ID: \n");
  scanf("%zd", &key);

  while (current != NULL) {
  if (current->UID == key){
    printf("The name of the file is: %s\n", current->file_name);
    printf("The path of the file is: %s\n", current->file_path);
    printf("The size of the file is: %jd Bytes\n", current->file_size);
    printf("The type of the file is: %s\n", current->file_type);
    /////////////////////
    char splitlocation[100];
    sprintf(splitlocation, "d1/%s.001", current->file_name);
    FILE *f = fopen(splitlocation, "rb");
    FILE* f2 = fopen(current->file_name, "wb" );
    
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);  /* same as rewind(f); */
    char string[fsize];
    fread(string, fsize, 1, f);
    fclose(f);
    fwrite(string, fsize, 1, f2 );
//////////////////////
    sprintf(splitlocation, "d2/%s.002", current->file_name);
    f = fopen(splitlocation, "rb");
    fseek(f, 0, SEEK_END);
    fsize = ftell(f);
    fseek(f, 0, SEEK_SET);
    fread(string, fsize, 1, f);
    fclose(f);  
    fwrite(string, fsize, 1, f2 );
/////////////////////////
    sprintf(splitlocation, "d3/%s.003", current->file_name);
    f = fopen(splitlocation, "rb");
    fseek(f, 0, SEEK_END);
    fsize = ftell(f);
    fseek(f, 0, SEEK_SET);
    fread(string, fsize, 1, f);
    fclose(f);
    fwrite(string, fsize, 1, f2 );
/////////////////
    sprintf(splitlocation, "d4/%s.004", current->file_name);
    f = fopen(splitlocation, "rb");
    fseek(f, 0, SEEK_END);
    fsize = ftell(f);
    fseek(f, 0, SEEK_SET);
    fread(string, fsize, 1, f);
    fclose(f);
    fwrite(string, fsize, 1, f2 );
    fclose(f2);

    //////////////
    
    return;
  }
  current = current->next;
  }
  printf("The ID specified was not found\n");
  return ;
}

void list(struct FileData* FileData) {
  while (FileData != NULL) {
    printf("The name of the file is: %s\n", FileData->file_name);
    printf("The path of the file is: %s\n", FileData->file_path);
    printf("The size of the file is: %jd Bytes\n", FileData->file_size);
    printf("The type of the file is: %s\n", FileData->file_type);
    printf("---------------------------\n");
  FileData = FileData->next;
  }
}


int main(int argc, char const *argv[])
{
    struct FileData* head = NULL;
    // printf("The size is: %jd Bytes\n", fullSize("d1/sample-15s.mp3.001"));
    // printf("The absolute path is: %s \n", absolutePath("d1/sample-15s.mp3.001"));
    // printf("The name of the file is: %s \n", fullName(absolutePath("file.txt")));
    // printf("The extension of the file is: %s\n", get_filename_ext(fullName("file.txt")));
    int input;
    int flag = 1;
    char command[4];
    printf("\t\t Veritas object storage Major Project\n");
    while (flag)
    {
        printf("\n--------------------------\n");
        // printf("Enter 1 for put \nEnter 2 for get \nEnter 3 for list \nEnter 0 for exit\n" );
        scanf("%s", &command);
        if (strcmp(command, "put") == 0)
        {
            put(&head);
        }
        else if (strcmp(command, "get") == 0)
        {
            get(&head);
        }
        else if (strcmp(command, "list") == 0)
        {
            list(head);
        }
        else
        {
            printf("Invalid command\n");
            // getchar();
            // system("clear");
        }

    }
    return 0;
}

