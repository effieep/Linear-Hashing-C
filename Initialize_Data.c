#include "inverted_index.h"
#include "linear_hashing.h"

bool flags_handling(int argc,char **argv,int *entries,FILE **file){
    int opt = 0; 
    if (argc<5) {
        printf("Correct syntax is: %s -f registeredvoters -b bucketentries\n", argv[0]);
        return false;
    }
    while((opt = getopt(argc, argv, ":f:b")) != -1)  
    {  
        switch(opt)  
        {  
            case 'f':  
                *file=fopen(argv[2],"r");
                break;
            case 'b':  
                *entries = atoi(argv[4]);
                break;   
        }  
    }  
    return true;
}

Bucket* read_file(FILE *file,Bucket* hash_table,list_pointers* lp,Info* info, int bucketentries){
    int pin,zip_code;
    char lname[99],fname[99];

    while ( fscanf(file,"%d %99s %99s %d",&pin, fname, lname, &zip_code) == 4 ) {
            info->participants++;
            participant* ptr = malloc(sizeof(participant));
            if(ptr != NULL){
                Init_Participant(ptr, pin, fname, lname, zip_code);

                hash_table = Insertion(hash_table,info,ptr,bucketentries);
                listnode* found = Search_List(lp,ptr->zc);
                if(found == NULL){
                    Insert_code(lp,ptr->zc);
                }
                Insert_pin(lp,ptr->pin, ptr->zc);
                
                //fprintf(stdout,"%d %s %s %d\n",pin,lname, fname, zip_code);

            }else{
                printf("Failed to allocate memory\n");
            }
            
        }  
    fclose(file);
    return hash_table;
}