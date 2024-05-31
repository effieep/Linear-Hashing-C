#include "inverted_index.h"

void Initialize_List(list_pointers* ptr){
    ptr->head = NULL;
    ptr->last = NULL;
}

listnode* Search_List(list_pointers* lp,int zip_code){                          //Search for the node with the requested zip_code
    bool find = false;
    listnode* found = NULL;
    listnode *cursor = lp->head;
    if(lp->head != NULL){                                                       //if the list has at least one node, search for the node
        while(find ==false && cursor->next!=NULL){                              //Case: List has more than 1 nodes
            if (cursor->zip_code == zip_code){
                find = true;
                found = cursor;
            }else{
                cursor = cursor->next;
                if(cursor->next==NULL && cursor->zip_code==zip_code){
                    find = true;
                    found = cursor;
                }
            }
        }
        if(find == false && cursor->zip_code == zip_code){                      //Case: List has just 1 node 
            found = cursor;
            find = true;
        }
    }
    return found;
}

void Insert_pin(list_pointers* lp, int pin, int zip_code){
    listnode* node = Search_List(lp,zip_code);                          //node that pin must be added to
    extended_node* cursor;
    extended_node* new = malloc(sizeof(extended_node));                 //allocation of the new node
    if(new != NULL){
        new->key = pin;
        new->nxt = NULL;
        if(node->participants == NULL){                                     //initialize the struct extendedlist_pointers
            node->participants = malloc(sizeof(extendedlist_pointers));
            if(node->participants != NULL){
                node->participants->head = new;
                node->participants->last = new;
                node->participants->count = 1;
            }else{
                printf("Failed to allocate memory\n");
            }
            
        }else{                                                              //if the list already exist, add to its end the new node
            cursor = node->participants->last;
            cursor->nxt = new;
            node->participants->last = new;
            node->participants->count++;
        }
    }else{
        printf("Failed to allocate memory\n");
    }
    
}

void Insert_code(list_pointers *lp,int code){

    listnode* new = malloc(sizeof(listnode));
    if(new != NULL){
        new->zip_code = code;
        new->next = NULL;
        new->participants = NULL;

        if(lp->head == NULL){                                           //if list is empty
            lp->head = new;
            lp->last = new;
        }
        else{                                                           //If list is not empty
            lp->last->next = new;                                       //add the new node to the end of the list
            lp->last = new;
        }       
    }else{
        printf("Failed to allocate memory\n");
    }
    
}

void Show_List(list_pointers* p){
    listnode* main_cursor;
    extended_node* sec_cursor;
    main_cursor = p->head;
    printf("The inverted index list is : \n");
    if(main_cursor != NULL){
        do
        {
            printf("%d      ->  ", main_cursor->zip_code);
            if(main_cursor->participants != NULL){
                sec_cursor = main_cursor->participants->head;
                do{
                    printf(" %d,",sec_cursor->key);
                    sec_cursor = sec_cursor->nxt;
                }while(sec_cursor != NULL);
            }
            printf("\n");
            main_cursor = main_cursor->next;
        } while (main_cursor != NULL);
    }
    printf("\n");
}