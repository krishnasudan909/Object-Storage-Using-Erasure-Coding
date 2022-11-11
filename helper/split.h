
int splitFile(char *fileIn, size_t maxSize)
{
    int result = 0;
    FILE *fIn;
    FILE *fOut;
    char buffer[1024 * 16];
    char dirname[3];
    size_t size;
    size_t read;
    size_t written;


    if ((fileIn != NULL) && (maxSize > 0))
    {
        fIn = fopen(fileIn, "rb");
        if (fIn != NULL)
        {
            fileIn = fullName(fileIn);
            fOut = NULL;
            result = 1;   /* we have at least one part */

            while (!feof(fIn))
            {
                /* initialize (next) output file if no output file opened */
                if (fOut == NULL)
                {
                    sprintf(dirname, "d%d", result);
                    mkdir(dirname,0777);
                    sprintf(buffer, "%s/%s.%03d", dirname, fileIn, result);
                    fOut = fopen(buffer, "wb");
                    if (fOut == NULL)
                    {
                        result *= -1;
                        break;
                    }

                    size = 0;
                }

                
                read = sizeof(buffer);
                if ((size + read) > maxSize)
                {
                    read = maxSize - size;
                }

                
                read = fread(buffer, 1, read, fIn);
                if (read == 0)
                {
                    result *= -1;
                    break;
                }

                
                written = fwrite(buffer, 1, read, fOut);
                if (written != read)
                {
                    result *= -1;
                    break;
                }

               
                size += written;
                if (size >= maxSize)   
                {
                    fclose(fOut);
                    fOut = NULL;
                    result++;
                }
            }

            /* clean up */
            if (fOut != NULL)
            {
                fclose(fOut);
            }
            fclose(fIn);
        }
    }

    return (result);
}