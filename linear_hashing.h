#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  
#include <string.h>
#include <ctype.h>
#include "data.h"
#define m 2                 //initial number of buckets

bool flags_handling(int,char **,int *,FILE **);
Bucket* read_file(FILE*,Bucket*,list_pointers*,Info *,int );

void Init_Participant(participant *,int,char*,char*,int);
void Initialize_Info(Info*);
Bucket* Initialize_Hash_Table(int bucketentries);
void Split_Bucket(Bucket *,int *, int , int , int);
void Insert_Key(Bucket* , int , participant*, int , int , int);
Bucket* Insertion(Bucket*,Info *,participant* ,int );
void Show_Hash_table(Bucket *,int,int);