#include <stdio.h>
#include <stdlib.h>
#include <string.h>



struct lines                                                                    //Here is the definition for the struct our linked list will use.
{
	char * string;
	struct lines * prev;
	struct lines * next;
};

int read_lines(struct lines ** line, FILE * stream){                             //Stream can take both stdin and an opened file. This is based on one of the homework assignments, but since 
                                                                                //we can't assume anything about the size of the lines, we can't use the solution as-is. Instead of fgets, 
                                                                                //we'll use readline, which allocates the necessary memory automatically, so it should handle very long strings.                                                                     
    size_t n = 0;                                                               //This corresponds to the second argument needed by getline.
    ssize_t numberOfChars = 0;                                                  //This corresponds to the return value of getline.
	struct lines * lp;
    int i = 0;                                                                  //This is used to detect if a user closes stdin before giving any text for getline to read.
    fseek(stream, 0, SEEK_END);
    if(ftell(stream) == 0){
        fprintf(stderr,"Input file is empty!\n");
        exit(1);
    }
    rewind(stream);
    while(numberOfChars != -1){
        char *lineptr = NULL;                                                  //if getline's pointer is NULL, it allocates required amount of memory.
        if((numberOfChars = getline(&lineptr, &n, stream)) != -1){             //The line gets copied to input. Getline returns -1 when stream is closed. Otherwise it gives you the number of characters in the line.
            lineptr[strlen(lineptr)-1] = 0;                                    //strip newline
            if (*line == NULL) {                                                        //This part handles the linked list
                        if ((*line = lp = malloc(sizeof(struct lines))) == NULL) {
                            fprintf(stderr, "malloc failed\n");
                            return -1;
                        }
                        lp->prev = lp->next = NULL;
                    } else {
                        if ( (lp->next = malloc(sizeof(struct lines))) == NULL) {
                            fprintf(stderr, "malloc failed\n");
                            return -1;
                        }
                        lp->next->prev = lp;
                        lp = lp->next;

                    }
                    
                    if ( (lp->string = malloc(strlen(lineptr) + 1)) == NULL) {
                        fprintf(stderr, "malloc failed\n");
                        return -1;
                    }
                    
                    strcpy(lp->string, lineptr);
            free(lineptr);                                                            //Auto-allocated memory needs to be freed manually before we use getline again.
            i++;                                                                      //i gets iterated here.
            }
            if(i == 0){                                                               //Here is the check for the case where stdin gets closed without any inputs.
                fprintf(stderr,"Stdin was closed, nothing to reverse!\n");
                exit(1);
            }
        }
	return 0;
}

void print_lines(struct lines * line, FILE * stream)                        //This simply iterates to the end of the linked list, and then uses the previous-links to print the values in reverse order.
{
	struct lines * lp;
    lp = line;
    while(lp->next != NULL){
        lp = lp->next;
    }

    while(lp->prev != NULL){
        fprintf(stream,"%s\n",lp->string);
        lp = lp->prev;
    }
    fprintf(stream,"%s\n",lp->string);                                     //Print one more to finish the set.
}

void delete_lines(struct lines * line)                                     //This iterates the linked list and frees the allocated memory.
{
	struct lines * lp;
	
	lp = line;
	while (lp != NULL) {
		line = lp->next;
		free(lp->string);
		free(lp);
		lp = line;
	}
}

int main(int argc, char* argv[])
{

	struct lines * line = NULL;
	FILE *iFile;
    FILE *oFile;
    switch(argc){
        case 1:
            printf("Type something (Ctrl-D to stop):\n");
            if (read_lines(&line, stdin) == -1)
                exit(1);
            print_lines(line, stdout);
            delete_lines(line);
            return 0;
        case 2:
            if((iFile = fopen(argv[1],"r"))== NULL){
                fprintf(stderr,"error: cannot open file '%s'\n",argv[1]);
                exit(1);
            }
            if (read_lines(&line, iFile) == -1)
                exit(1);
            print_lines(line, stdout);
            delete_lines(line);
            fclose(iFile);
            return 0;
        case 3:
            if(strcmp(argv[1],argv[2]) == 0){
                fprintf(stderr,"error: input and output files cannot be the same!\n");
                exit(1);
            }
            else{
                if((iFile = fopen(argv[1],"r"))== NULL){
                    fprintf(stderr,"error: cannot open file '%s'\n",argv[1]);
                    exit(1);
                }
                if((oFile = fopen(argv[2],"w"))== NULL){
                    fprintf(stderr,"error: cannot open file '%s'\n",argv[2]);
                    exit(1);
                }
                if (read_lines(&line, iFile) == -1)
                    exit(1);
                print_lines(line, oFile);
                delete_lines(line);
                fclose(iFile);
                fclose(oFile);
                return 0;
            }
        default:
            printf("usage: reverse <input> <output>\n");
            exit(1);
        }
}
