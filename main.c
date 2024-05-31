#include "linear_hashing.h"
#include "inverted_index.h"
#include "functions.h"

int main(int argc, char **argv){
   Bucket* hash_table;
   Info info;
   list_pointers PostalCodes;
   int bucketentries = 0;
   FILE *file = NULL;
   bool valid_input;
   valid_input = flags_handling(argc,argv,&bucketentries,&file);
   if(file !=  NULL && bucketentries>=1){
      if(valid_input){
         hash_table = Initialize_Hash_Table(bucketentries);  
         Initialize_List(&PostalCodes);
         Initialize_Info(&info);
         hash_table = read_file(file,hash_table,&PostalCodes,&info,bucketentries);
         //Show_List(&PostalCodes);
         //Show_Hash_table(hash_table,info.buckets,bucketentries);
         
      }else{
         printf("Malformed input...\n");
      }
      Run(hash_table,&PostalCodes,&info,bucketentries);
   }else{
      printf("The information that was given, are not sufficient to execute the program.\n");
   }
   exit(0);
}