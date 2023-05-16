#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


typedef struct Node {
    char* text;
    int count;
    int length;
    struct Node* next;
    struct Node* prev;
} Node;

typedef struct DblLinkedList {
    size_t size;
    Node* head;
    Node* tail;
} DblLinkedList;

void show_list(DblLinkedList*);
int find_str(DblLinkedList*,const char*);
void get_word(FILE*, DblLinkedList *);
void print_compressed(FILE*,FILE*, char**, char**,int);
int find_popular(DblLinkedList*, int, int);
int find_rare(DblLinkedList*,int, int );
void add_symb(FILE*, char**, char**,int);
Node* back_index(DblLinkedList*,int);
void go_back(DblLinkedList*,const char*,int,int);
void remove_node(DblLinkedList*,const char *);
int check_benefit(DblLinkedList*,int,int);
void swap_word(DblLinkedList*, char**, char**, int*);
void check(FILE **compressedFile, FILE** file);