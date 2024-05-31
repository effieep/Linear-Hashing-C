#include "linear_hashing.h"
#include "inverted_index.h"

int num_of_keys = 0;

void Init_Participant(participant* new_par,int pin, char *first,char* last,int zip_code){
    new_par->pin = pin;
    strcpy(new_par->lname,last);
    strcpy(new_par->fname,first);
    new_par->zc = zip_code;
    new_par->vote = false;
}

void Initialize_Info(Info* info){
    info->participants = 0;
    info->voters = 0;
    info->buckets = m;
}

void deallocate_overflow_bucket(Bucket *overflow,int bucketentries){
    for(int j=0; j<bucketentries; j++){
        free(overflow->keys[j].par_ptr);
    }
    free(overflow->keys);
    free(overflow);
}

void Init_Pair(Pair* p){
    p->key = 0;
    p->par_ptr = NULL;
}

void Init_Bucket_entries(Bucket* b, int bucketentries){
    b->keys = malloc(bucketentries * sizeof(Pair));
    if(b->keys != NULL){
        for(int j=0; j<bucketentries; j++){
            (b->keys[j]).key = 0;
            (b->keys[j]).par_ptr = NULL;
        }
        b->overflow = NULL;
    }else{
        printf("Failed to allocate memory\n");
    }
}

Bucket* Initialize_Hash_Table(int bucketentries){
    Bucket* ht = malloc(m*sizeof(Bucket));                              //allocate the initial number of buckets
    if(ht != NULL){
        for(int i=0; i<m; i++){
            Init_Bucket_entries(&ht[i], bucketentries);
        }
    }else{
        printf("Failed to allocate memory\n");

    }
    return ht;
}

int hash_function(int key,int i){
    int h = key % ((1 << i)*m);
    return h;
}

void Split_Bucket(Bucket *ht,int *split, int buckets, int i, int bucketentries){
    int key;
    participant* par;
    Bucket* cursor;
    Bucket* prev_node;
    for(int j=0; j<bucketentries; j++){                                 //initialize the new bucket
        ht[buckets].keys[j].key = 0;
        ht[buckets].keys[j].par_ptr = NULL;
    }

    for(int j=0; j<bucketentries; j++){
        key = ht[*split].keys[j].key;                                   //renitialize the keys and their pointers that will be redistributed
        ht[*split].keys[j].key = 0;                             
        par = ht[*split].keys[j].par_ptr;
        ht[*split].keys[j].par_ptr = NULL;
        if(key != 0){   
            Insert_Key(ht, key, par, i+1 ,*split, bucketentries);       //redistribute the key with new h(i+1) hash function 
        }
    }

    if(ht[*split].overflow != NULL){                                    //overflow has occured
        int index = *split;
        cursor = ht[index].overflow;
        prev_node = &ht[index];
        do{
            for(int j=0; j<bucketentries; j++){                         //for each key of the overflown bucket
                key = cursor->keys[j].key;
                cursor->keys[j].key = 0;                                //reinitialize key to 0
                par = cursor->keys[j].par_ptr;              
                cursor->keys[j].par_ptr = NULL;                         
                if(key != 0){
                    Insert_Key(ht, key, par,  i+1 ,*split, bucketentries);
                }
            }
            bool free_ov = true;
            for(int k=0; k<bucketentries; k++){                         //for each key of the overflown bucket
                if(cursor->keys[k].key != 0){
                    free_ov = false;
                }
            }
            if(free_ov){                                                //if the overflown bucket is empty
            
                if(cursor->overflow != NULL){                           //if there are overflow buckets after the current we examine
                    prev_node->overflow = cursor->overflow;             //connect the node before the overflown with the node after
                }else{
                    prev_node->overflow = NULL;                         //if there are not other overflow buckets next, pointer becomes NULL
                }
                deallocate_overflow_bucket(cursor,bucketentries);                     //overflown bucket is freed
                cursor = prev_node->overflow;
            }else{                                                      
                prev_node = cursor; 
                cursor = cursor->overflow;                              //continue the redistribution of the keys
            }
        }while(cursor != NULL);
    }
    (*split)++;
}

void Insert_Key(Bucket* ht, int key, participant* par, int i, int split, int bucketentries){
    int k, j=0;
    Bucket* cursor;
    Bucket* prev_node;
    k = hash_function(key,i);
    if(k < split){                                                      //if k is before the split (bucket that will be splitted next),use the next hash function(i+1)
        k = hash_function(key,i+1);
    }
    bool f=false;
    cursor = &ht[k];
    do{ 
        for(int j=0; j<bucketentries; j++){                             //for all the bucketentries                               
            if(cursor->keys[j].key == 0){                               //when the first free position is found, place the key there
                f = true;
                cursor->keys[j].key = key;
                cursor->keys[j].par_ptr = par;
                break;
            }
        }
        prev_node = cursor;                                             //store the node right before the one that cursor points to
        cursor = cursor->overflow;                                      //point to the next overflown bucket
    }while(cursor != NULL && f == false);
    if(f == false){                                                     //the key has not been inserted,create an overflow bucket for the last node
        prev_node->overflow = malloc(sizeof(Bucket));             
        if(prev_node->overflow != NULL){
            Init_Bucket_entries(prev_node->overflow, bucketentries);
            (prev_node->overflow)->keys[0].key = key;                       //as the overflow bucket was just created, place the key to the first position
            (prev_node->overflow)->keys[0].par_ptr = par;
        }else{
            printf("Failed to allocate memory\n");
        }
    }
}

Bucket* Insertion(Bucket* ht,Info* info,participant* par,int bucketentries){
    static int i = 0, round=0, num_of_buckets = 2, split_index = 0;                //static, in order to retain their values in all calls of the function
    static float load_factor = 0;
    static int temp_buckets = 2;
    if(load_factor <= 0.75){                                                       //load_factor<0.75 means that the insertion of the key is feasible                                     
        //printf("Round %d \n",round);

        Insert_Key(ht, par->pin, par, i, split_index, bucketentries);
        num_of_keys++;
        load_factor = (float)num_of_keys/(2*temp_buckets);                          //load factor changes after the insertion
        //printf("Load factor value : %f \n", load_factor);

        if(load_factor>0.75){                                                       //if the new load factor exceeds 0.75, a split of the buckets must be done
            ht = realloc(ht, (temp_buckets+1)*sizeof(Bucket)); 
            if(ht != NULL){
                info->buckets++;                                                        //allocate a new bucket in the hash table
                ht[temp_buckets].keys = malloc(bucketentries*sizeof(Pair));             //and initialize it 
                if(ht[temp_buckets].keys != NULL){
                    ht[temp_buckets].overflow = NULL;
                    for(int j=0; j<bucketentries; j++){
                        Init_Pair(&ht[temp_buckets].keys[j]);
                    }        
                    Split_Bucket(ht, &split_index, temp_buckets, i, bucketentries);
                    
                    temp_buckets++;
                    load_factor = (float)num_of_keys/(2*temp_buckets);

                    if(temp_buckets == 2*num_of_buckets){                                   //if the number of buckets becomes twice the initial number of buckets                                               
                        round++;                                                            //then a new round must begin
                        split_index = 0;                                                    //split_index reinitialized to 0
                        i = round;                                                          //hash_function index increased by 1
                        num_of_buckets = temp_buckets;
                    }
                }else{
                    printf("Failed to allocate memory\n");
                } 
            }else{
                printf("Failed to allocate memory\n");
            }
        }
    }

    return ht;
}


void Show_Hash_table(Bucket * ht,int buckets,int bucketentries){
    Bucket* cursor;
    for(int j=0; j<buckets; j++){
        printf("%3d:  |", j);
        //printf("Keys:  \n");
        for(int k=0; k<bucketentries; k++){
            if(ht[j].keys[k].key != 0){
                printf(" %8d|", ht[j].keys[k].key);
            }else{
                printf("         |");
            }
        }
        cursor = ht[j].overflow;
        while(cursor != NULL){
            printf("-> |");
            for(int k=0; k<bucketentries; k++){
                if(cursor->keys[k].key != 0){
                    printf("%8d|   ", cursor->keys[k].key);
                }
                else{
                    printf("        |");
                }
            }
            cursor = cursor->overflow;
        }
        printf("\n");
    }
    printf("\n");
}
