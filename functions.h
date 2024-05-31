#include "data.h"
#include "linear_hashing.h"
#include "inverted_index.h"

void Run(Bucket*,list_pointers*,Info*,int);
void Choose_Function(Bucket*,list_pointers*,Info* ,char *,int ,const char *);
void Percentage_of_Voters(Info* info);
void Change_Status(Bucket *,Info*, int ,int);
void Count_participants_by_zipcode(list_pointers* , int);
void Insert_Participant(Bucket*,Info*,participant *,int );
participant* Search_Pin(Bucket*,Info*, int, int);
void Destroy_All_Data_Staructures(Bucket*, list_pointers*,Info *,int);