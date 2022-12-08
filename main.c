#include "helper/helper.h"
#include "erasure_code.h"
#define mapSize 5

//Structure of Filedata
struct FileData
{
    char* file_name;
    const char* file_type;
    char* file_path;
    off_t  file_size;
    size_t UID;

    struct FileData* next;

};



static int gf_gen_decode_matrix_simple(unsigned char * encode_matrix,
				       unsigned char * decode_matrix,
				       unsigned char * invert_matrix,
				       unsigned char * temp_matrix,
				       unsigned char * decode_index, unsigned char * frag_err_list, int nerrs, int k,
				       int m)
{
	int i, j, p, r;
	int nsrcerrs = 0;
	unsigned char s, *b = temp_matrix;
	unsigned char frag_in_err[255];

	memset(frag_in_err, 0, sizeof(frag_in_err));

	// Order the fragments in erasure for easier sorting
	for (i = 0; i < nerrs; i++) {
		if (frag_err_list[i] < k)
			nsrcerrs++;
		frag_in_err[frag_err_list[i]] = 1;
	}

	// Construct b (matrix that encoded remaining frags) by removing erased rows
	for (i = 0, r = 0; i < k; i++, r++) {
		while (frag_in_err[r])
			r++;
		for (j = 0; j < k; j++)
			b[k * i + j] = encode_matrix[k * r + j];
		decode_index[i] = r;
	}

	// Invert matrix to get recovery matrix
	if (gf_invert_matrix(b, invert_matrix, k) < 0)
		return -1;

	// Get decode matrix with only wanted recovery rows
	for (i = 0; i < nerrs; i++) {
		if (frag_err_list[i] < k)	// A src err
			for (j = 0; j < k; j++)
				decode_matrix[k * i + j] =
				    invert_matrix[k * frag_err_list[i] + j];
	}

	// For non-src (parity) erasures need to multiply encode matrix * invert
	for (p = 0; p < nerrs; p++) {
		if (frag_err_list[p] >= k) {	// A parity err
			for (i = 0; i < k; i++) {
				s = 0;
				for (j = 0; j < k; j++)
					s ^= gf_mul(invert_matrix[j * k + i],
						    encode_matrix[k * frag_err_list[p] + j]);
				decode_matrix[k * p + i] = s;
			}
		}
	}
	return 0;
}

///////////
int k = 8;
int p = 3;
int m = 11;

int nerrs = 0;

  unsigned char *frag_ptrs[255];
	unsigned char *recover_srcs[255];
	unsigned char *recover_outp[255];
	unsigned char frag_err_list[255];

	// Coefficient matrices
	unsigned char *encode_matrix, *decode_matrix;
	unsigned char *invert_matrix, *temp_matrix;
	unsigned char *g_tbls;
	unsigned char decode_index[255];


// Global Hashable with it's starting id
struct FileData *hashTable[mapSize];
int totId = 1;

//This function will put the file in hash table and also in different directories after splitting
void put(int hashIndex) {
  struct FileData* new_FileData = (struct FileData*)malloc(sizeof(struct FileData));

  char path[100];
  scanf("%s", &path);

  if (access(path, F_OK) == 0){
    new_FileData->file_name = fullName(path);
    new_FileData->file_path = absolutePath(path);
    new_FileData->file_size = fullSize(path);
    new_FileData->file_type = get_filename_ext(fullName(path));
    new_FileData->UID = totId;
    new_FileData->next = NULL;


    size_t maxSize = ceil((double)fullSize(path)/8);
    
    

    for (int i = 0; i < m; i++) {
		if (NULL == (frag_ptrs[i] = malloc(maxSize))) {
			printf("alloc error: Fail\n");
			return;
		}
	}

    printf("The file has been split to %d parts with maximum size as %zu\n", splitFile(path, maxSize, frag_ptrs), maxSize);

    gf_gen_cauchy1_matrix(encode_matrix, m, k);

	// Initialize g_tbls from encode matrix
	  ec_init_tables(k, p, &encode_matrix[k * k], g_tbls);

	// Generate EC parity blocks from sources
	  ec_encode_data(maxSize, k, p, g_tbls, frag_ptrs, &frag_ptrs[k]);


    putParity(fullName(path), maxSize, &frag_ptrs[k]);
    printf("The parities have been put into respective folders\n");
  

      //Adding new file in the start in O(1) Time complexity
      new_FileData->next = hashTable[hashIndex];
      hashTable[hashIndex] = new_FileData;
  }
  else
  {
    printf("The file is not accesible\n");
  }
  return;
}


void get() {
  size_t key;
  scanf("%zd", &key);
  int hashIndex = key%5;
  struct FileData* current = hashTable[hashIndex];
  while (current != NULL) {
  if (current->UID == key){
    printf("The file is present at %d index of the hash table\n",hashIndex);
    printf("The name of the file is: %s\n", current->file_name);
    printf("The path of the file is: %s\n", current->file_path);
    printf("The size of the file is: %jd Bytes\n", current->file_size);
    printf("The type of the file is: %s\n", current->file_type);


  size_t maxSize = ceil((double)current->file_size/8);
     

    unsigned char* databuffs[8];

    for (int i = 0; i < 8; i++)
    {
      databuffs[i] = (char *) malloc(maxSize);
    }
    

    nerrs = mergeFile(current->file_name, maxSize, databuffs, frag_err_list);
    printf("--DATA BEFORE--\n");
    for (int i = 0; i < 8; i++)
    {
      printf("%s\n", databuffs[i]);
    }
    printf("----\n");
    


    // Allocate buffers for recovered data
	for (int i = 0; i < p; i++) {
		if (NULL == (recover_outp[i] = malloc(maxSize))) {
			printf("alloc error: Fail\n");
			return;
		}
	}

    if (nerrs <= 0)
		  return;

    gf_gen_decode_matrix_simple(encode_matrix, decode_matrix,
					  invert_matrix, temp_matrix, decode_index,
					  frag_err_list, nerrs, k, m);

    for (int i = 0; i < k; i++)
		  recover_srcs[i] = frag_ptrs[decode_index[i]];

    ec_init_tables(k, nerrs, decode_matrix, g_tbls);
	  ec_encode_data(maxSize, k, nerrs, g_tbls, recover_srcs, recover_outp);

    printf("--DATA RECOVERED--\n");
    for (int i = 0; i < nerrs; i++)
    {
      printf("%s\n", recover_outp[i]);
    }
    printf("------\n");
    
    
    
    return;
  }
  current = current->next;
  }
  printf("The ID specified was not found\n");
  return ;
}

void list() {
  int hashIndex = 0;
  while (hashIndex<5) {
    if(hashTable[hashIndex]==NULL) printf("No files at hashIndex %d\n", hashIndex);
    else{
    printf("Files at HashIndex %d are --> \n", hashIndex);
    struct FileData* head = hashTable[hashIndex];
    while(head!=NULL){
    printf("The name of the file is: %s\n", head->file_name);
    printf("The path of the file is: %s\n", head->file_path);
    printf("The size of the file is: %jd Bytes\n", head->file_size);
    printf("The type of the file is: %s\n", head->file_type);
    printf("---------------------------\n");
    head = head->next;
    }
    }
    hashIndex++;
  }
}


int main(int argc, char const *argv[])
{

  char dirname[3];
  for (int i = 1; i <= 4; i++)
  {
    sprintf(dirname, "d%d", i);
    mkdir(dirname, 0777);
  }

  for (int i = 1; i <= 3; i++)
  {
    sprintf(dirname, "p%d", i);
    mkdir(dirname, 0777);
  }
  mkdir("result",0777);

  encode_matrix = malloc(m * k);
	decode_matrix = malloc(m * k);
	invert_matrix = malloc(m * k);
	temp_matrix = malloc(m * k);
	g_tbls = malloc(k * p * 32);

  

	

    //INITIALIZING MAP ARRAY TO NULL
    for(int i=0; i<mapSize; i++) hashTable[i]=NULL;

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
            int key = (totId % mapSize);
            printf("File added in the %d index of the hash table with ID %d\n",key, totId);
            put(key);
            totId++;
        }
        else if (strcmp(command, "get") == 0)
        {
            get();
        }
        else if (strcmp(command, "list") == 0)
        {
            list();
        }
        else if (strcmp(command, "exit") == 0){
          exit(0);
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


