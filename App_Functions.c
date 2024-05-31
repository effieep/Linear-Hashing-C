#include "functions.h"

void Run(Bucket* ht,list_pointers* lp,Info * info,int bucketentries){
    char str[80];
    const char separator[2] = " ";
    char* word;
    printf("The commands you can run in the app are: \n\n");
    printf(">> l <pin> : load the information of the voter with pin <pin>\n\n");
    printf(">> i <pin> <lname> <fname> <zip> : insert a new voter with these data\n\n");
    printf(">> m <pin> : change the voting status of the voter with pin <pin>, if he/she has not voted yet\n\n");
    printf(">> bv <fileofkeys> : for all participants, whose keys appear in the file <fileofkeys>, change their voting status, if they have not voted yet\n\n");
    printf(">> v : show the number of voters who have voted\n\n");
    printf(">> perc : show the percentage of the voters who have already voted over the whole number of participants\n\n");
    printf(">> z <zipcode> : show all the participants with zipcode <zipcode>\n\n");
    printf(">> exit : Terminate\n\n");
    bool f = true;
    while(f){
        printf("\nWaiting for input...\n >> ");
        fgets(str,80,stdin);
        word = strtok(str,separator);
        Choose_Function(ht,lp,info,word,bucketentries,separator);
        if(strcmp(word,"exit\n")==0) break;
    }
}

bool IsChar(char* str){
    int i=0;
    char c = str[i];
    bool ch = true;
    while(c != '\0'){                       //until the end of the string
        if(isdigit(c) != 0){                //if a char is not a digit then string is not int
            ch = false;
            break;
        }
        i++;
        c = str[i];
    }
    return ch;
}

bool IsInt(char *str){
    int i=0;
    char c = str[i];
    bool digit = true;
    while(c != '\0'){                       //until the end of the string
        if(isdigit(c) == 0){                //if a char is number then string is not a valid word
            digit = false;
            break;
        }
        i++;
        c = str[i];
    }
    return digit;
}

void l_pin(Bucket* ht,Info* info,char *token,int bucketentries,const char *s){
    participant* p;    
    token = strtok(NULL, s);
    token[strlen(token)-1] = '\0';                              //remove '\n' char from the end of the string
    bool digit = IsInt(token);  
    if(digit == true){                                          //Case: Pin is int
        int pin = atoi(token);
        p = Search_Pin(ht,info,pin,bucketentries);
        if(p == NULL){                                          //if p==NULL,SearchPin did not find the current pin in hash table
            printf("Participant %d not in cohort\n",pin);
        }else{
            printf("%d %s %s %d ",p->pin,p->lname,p->fname,p->zc);
            if(p->vote == false){
                printf("N\n");
            }else{
                printf("Y\n");
            }
        }
    }else{                                                      //Case: Pin is not an int
        printf("Malformed pin\n");
    }
}

void i_pin_name_zip(Bucket* ht,Info* info,char *token,int bucketentries,const char *s){
    int count = 1;
    int pin,zip_code;
    bool valid;
    participant *par;
    char lname[50],fname[50];           
    while( token != NULL ) {
        token = strtok(NULL, s);
        if(token != NULL){
            valid = false;
        }
        count++;                            //count identifies the current argument of the string,1st arg: i
        switch (count)                  
        {
        case 2:                             //2nd arg: pin
            valid = IsInt(token);
            pin = atoi(token);
            break;
        case 3:                             //3rd arg: lname
            valid = IsChar(token);
            strcpy(lname,token);
            break;
        case 4:
            valid = IsChar(token);          //4th arg: fname
            strcpy(fname,token);
            break;  
        case 5:                             
            token[strlen(token)-1] = '\0';  //5th arg: zip_code
            valid = IsInt(token);
            zip_code = atoi(token);
            break;
        default:
            break;
        }
        if(!valid && token!=NULL){                  //if valid == false, this indecates that an input argument was not given correctly
            printf("Malformed Input\n");
            break;
        }
    }
    if(valid){
        par = Search_Pin(ht,info,pin,bucketentries);            //Check if pin already exists in hash table 
        if(par==NULL){
            par = malloc(sizeof(participant));                  //If it does not exist, insert it
            if(par != NULL){
                Init_Participant(par,pin,fname,lname,zip_code);
                Insert_Participant(ht,info,par,bucketentries);
                info->participants++;
            }else{
                printf("Failed to allocate memory\n");
            }
        }else{
            printf("%d already exist",pin);
        }
    }
}

void m_pin(Bucket* ht,Info* info,char *token,int bucketentries,const char *s){
    token = strtok(NULL,s);
    token[strlen(token)-1] = '\0';                          //Remove '\n' from the end of string
    bool valid = IsInt(token);
    if(valid){
        int pin = atoi(token);
        Change_Status(ht,info,pin,bucketentries);
    }else{
        printf("Malformed Input\n");
    }
}

void bv_keys(Bucket* ht,Info* info,char *token,int bucketentries,const char *s){        
    char p[10];
    int pin;
    token = strtok(NULL,s);
    token[strlen(token)-1] = '\0';
    FILE *file=NULL;
    file = fopen(token,"r");                                //if file don't take value, fopen failed
    if(file != NULL){
        while( fscanf(file,"%s",p) == 1 ){                  //Read per one the pins in <fileofkeys>
            bool valid = IsInt(p);                          //Check if it is int 
            if(valid){
                pin = atoi(p);
                Change_Status(ht,info,pin,bucketentries);
            }else{
                printf("Malformed Input\n");
            }
        }
    }else{
        printf("%s could not be opened,if it does not exist\n",token);
    }
}

void Choose_Function(Bucket* ht,list_pointers*lp,Info* info,char *token,int bucketentries,const char *s){
    if (strcmp(token,"l")==0){                                          //l <pin>  
        l_pin(ht,info,token,bucketentries,s);
    }else if(strcmp(token,"i")==0){                                     //i <pin> <lname> <fname> <zip>
        i_pin_name_zip(ht,info,token,bucketentries,s);
    }else if(strcmp(token,"m")==0){                                     //m <pin>
        m_pin(ht,info,token,bucketentries,s);
    }else if(strcmp(token,"bv")==0){                                    //bv <fileofkeys>
        bv_keys(ht,info,token,bucketentries,s);
    }else if(strcmp(token,"v\n")==0){                                   //v 
        printf("Voted So Far %d\n",info->voters);
    }else if(strcmp(token,"perc\n")==0){                                //perc
        Percentage_of_Voters(info);
    }else if(strcmp(token,"z") == 0){                                   //z <zipcode>
        token = strtok(NULL, s);
        int zip = atoi(token);
        Count_participants_by_zipcode(lp,zip);
    }else if(strcmp(token,"exit\n") == 0){                              //exit
        Destroy_All_Data_Staructures(ht,lp,info,bucketentries);
        //deallocate the memory
        return;
    }
}

participant* Search_Pin(Bucket* ht,Info* info, int pin, int bucketentries){
    int index = 0;
    int buc = pin % ((1 << index) * m);
    participant* p;
    Bucket* cursor;
    bool found = false;
    while( !found ){
        for(int j=0; j<bucketentries; j++){                 //Check if pin is on the initial bucket
            if(ht[buc].keys[j].key == pin){
                found = true;
                p = ht[buc].keys[j].par_ptr;
                break;                                      //if it is found the iteration stops
            }
        }
        cursor = ht[buc].overflow;                          //If there are overflown buckets check them too 
        while(cursor != NULL){
            for(int k=0; k<bucketentries; k++){
                if(cursor->keys[k].key == pin){
                    found = true;
                    p = cursor->keys[k].par_ptr;
                    break;
                }
            }
            cursor = cursor->overflow;
        }
        if(found == false){                     //if it was not found with the current index, increase it by 1 and check again
            index++;
            buc = pin % ((1 << index)* m);
        }
        if(info->buckets < buc){                //the element was not found, buc has exceeded the number of buckets that hash table has
            p = NULL;
            break;
        }
    }
    return p;
}

void Insert_Participant(Bucket* ht,Info* info,participant *par,int bucketentries){
    int key = par->pin;
    ht = Insertion(ht,info,par,bucketentries);
    printf("Inserted %d %s %s %d ",par->pin,par->lname,par->fname,par->zc);
    if(par->vote == false){
        printf("N\n");
    }else{
        printf("Y\n");
    }
}

void Count_participants_by_zipcode(list_pointers* lp, int zipcode){
    listnode *node = Search_List(lp,zipcode);
    if(node->participants->head != NULL){                           //until the end of the list
        printf("%d voted in %d\n",node->participants->count,zipcode);
        extended_node* cursor = node->participants->head;
        do{
            printf("%d \n",cursor->key);
            cursor = cursor->nxt;
        }while( cursor != NULL);                                     //until the end of extended nodes/participants
    }
}

void Change_Status(Bucket *ht,Info* info, int pin,int bucketentries){
    participant* p = Search_Pin(ht,info,pin,bucketentries);
    if(p != NULL){                                  //if pin is found, change the current status - has voted
        p->vote = true;
        info->voters++;
        printf("|%d| Marked Voted\n",pin);
    }else{
        printf("%d does not exist\n",pin);          //pin does not exist, p == NULL
    }
}

void Percentage_of_Voters(Info* info){
    float result;
    result = (float)info->voters/info->participants * 100.0;
    printf("%.2f%% \n", result);
}

void Destroy_All_Data_Staructures(Bucket* ht, list_pointers* lp,Info *info,int bucketentries){
    //Deallocation of hash table
    int bytes_released = 0;
    Bucket *temp,*head;
    for(int i=0; i<info->buckets;i++){
        head = ht[i].overflow;
        while(head != NULL){
            temp = head;
            head = head->overflow;
            for(int j = 0; j<bucketentries;j++){
                bytes_released+=sizeof(temp->keys[j].par_ptr);
                free(temp->keys[j].par_ptr);
            }
            bytes_released+=sizeof(temp->keys);
            free(temp->keys);
            bytes_released+=sizeof(temp);
            free(temp);
        }
        for(int j = 0; j<bucketentries;j++){
            bytes_released+=sizeof(ht[i].keys[j].par_ptr);
            free(ht[i].keys[j].par_ptr);
           
        }
        bytes_released+=sizeof(ht[i].keys);
        free(ht[i].keys);
    }
    //Deallocation of inverted index list
    listnode *main_cursor = lp->head;
    listnode* x;
    extended_node* sec_cursor,*y;
    while(main_cursor!=NULL){
        x = main_cursor;
        if(main_cursor->participants != NULL){
            sec_cursor = main_cursor->participants->head;
            while(sec_cursor!=NULL){
                y = sec_cursor;
                sec_cursor = sec_cursor->nxt;
                bytes_released+=sizeof(y);
                free(y);
            }
        }
        main_cursor = main_cursor->next;
        bytes_released+=sizeof(y);
        free(x);
    }
    printf("%d of Bytes Released \n",bytes_released);
}
