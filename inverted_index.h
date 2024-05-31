#include "data.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  
#include <string.h>

void Initialize_List(list_pointers* );
void Show_List(list_pointers* );
listnode* Search_List(list_pointers* ,int);
void Insert_pin(list_pointers* , int, int);
void Insert_code(list_pointers *,int );