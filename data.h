#include <stdbool.h>
#pragma once

typedef struct participant{
    int pin;
    char lname[50];
    char fname[50];
    int zc;                 //postal code
    bool vote;
}participant;

typedef struct pair
{
    participant* par_ptr;
    int key;
}Pair;

typedef struct bucket
{
    Pair* keys;                     /*array, the size of the array is initialized buy the argument bucketentries*/
    struct bucket* overflow;       /*in case of overflow*/
}Bucket;

typedef struct extended_node{
    int key;
    participant* par;
    struct extended_node* nxt;
}extended_node;

typedef struct node{
    int zip_code;
    struct extendedlist_pointers* participants;
    struct node* next;
}listnode;

typedef struct pointers{
    listnode* head;
    listnode* last;
}list_pointers;

typedef struct extendedlist_pointers{
    extended_node* head;
    extended_node* last;
    int count;
}extendedlist_pointers;

typedef struct information{
    int participants;
    int voters;
    int buckets;
}Info;