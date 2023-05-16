#include "functions.h"
#define SIZE_WORD 16
#define LOW_INDEX -1
#define MAX_INDEX 100
#define AVER_SIZE 4
#define LINE 1024


void go_back(DblLinkedList* list, const char* text, int length, int count)
{
    Node* ptr = (Node*)malloc(1 * sizeof(Node));
    ptr->count = count;
    ptr->length = length;

    if (length > 0)
    {
        ptr->text = (char*)malloc((ptr->length + 1) * sizeof(char));
        strcpy_s(ptr->text, ptr->length + 1, text);
    }
    else
    {
        ptr->text = NULL;
    }

    if (list->head == NULL) {
        ptr->next = NULL;
        ptr->prev = NULL;
        list->head = ptr;
        list->tail = ptr;
        list->size++;
    }
    else
    {
        ptr->next = NULL;
        ptr->prev = list->tail;
        list->tail->next = ptr;
        list->tail = ptr;
        list->size++;
    }
}

int check_benefit(DblLinkedList* list, int pop, int rare)
{
    int lenA = back_index(list, pop)->length;
    int lenB = back_index(list, rare)->length;
    int countA = back_index(list, pop)->count;
    int countB = back_index(list, rare)->count;

    if (((lenA * countA) + lenB + countB) > ((lenA * countB) + lenB + countA))
    {
        return 1;
    } else
    {
        return 0;
    }
}


Node* back_index(DblLinkedList* list, int index)
    {
        if (index < 0 || index >= list->size)
        {
            return NULL;
        }
        if (index == list->size - 1)
        {
            return list->tail;
        }
        Node* current = list->head;

        for (size_t i = 0; i < index; i++)
        {
            current = current->next;
        }
        return current;
    }


void get_word(FILE* file, DblLinkedList* list)
{
    char* word = (char*)calloc(SIZE_WORD, sizeof(char));
    char* temp = (char*)calloc(SIZE_WORD, sizeof(char));


    while (fscanf(file, "%32s", word) == 1) {
        size_t len = strlen(word);
        char* start = word;
        char* end = word + len - 1;
        while (start <= end && ispunct(*start)) {
            start++;
        }
        while (end >= start && ispunct(*end)) {
            end--;
        }
        len = end - start + 1;
        strncpy(temp, start, len);
        temp[len] = '\0';
        strcpy(word, temp);

        int index;
        if ((index = find_str(list, word)) == -1) {
            go_back(list, word, (int)strlen(word), 1);
        } else {
            back_index(list, index)->count++;
        }
    }

    free(word);
    free(temp);
}




void show_list(DblLinkedList* list)
{
    Node* temp;
    temp = list->head;
    while (temp != NULL)
    {
        printf("%d %d %s\n", temp->count, temp->length, temp->text);
        temp = temp->next;
    }
}


int find_str(DblLinkedList* list,const char* cmp_word)
{
    if (list != NULL && list->head != NULL)
    {
        Node* ptr;
        ptr = list->head;
        int i = 0;
        while (ptr != NULL)
        {
            if (strcmp(ptr->text, cmp_word) == 0)
            {
                return i;
            }
            ptr = ptr->next;
            i++;
        }
        return -1;
    }
    else
        return -1;
}

void print_compressed(FILE* file,FILE* compressedFile, char** wordsA, char** wordsB, int numReplacements)
{
    int size = 16;
    char* word = (char*)calloc(size,sizeof(char));

    while (fscanf(file, "%16s", word) == 1)
    {
        unsigned long len = strlen(word);

        if (ispunct(word[len - 1]))
        {
            char lastChar = word[len - 1];
            word[len - 1] = '\0';
            int found = 0;
            for (int i = 0; i < numReplacements; i++)
            {
                if (strcmp(word, wordsA[i]) == 0)
                {
                    fprintf(compressedFile, "%s%c ", wordsB[i], lastChar);
                    found = 1;
                    break;
                }
                else if (strcmp(word, wordsB[i]) == 0) {
                    fprintf(compressedFile, "%s%c ", wordsA[i], lastChar);
                    found = 1;
                    break;
                }
            }
            if (!found) {
                fprintf(compressedFile, "%s%c ", word, lastChar);
            }
        }
        else
        {
            int found = 0;
            for (int i = 0; i < numReplacements; i++)
            {
                if (strcmp(word, wordsA[i]) == 0)
                {
                    fprintf(compressedFile, "%s ", wordsB[i]);
                    found = 1;
                    break;
                }
                else if (strcmp(word, wordsB[i]) == 0) {
                    fprintf(compressedFile, "%s ", wordsA[i]);
                    found = 1;
                    break;
                }
            }
            if (!found) {
                fprintf(compressedFile, "%s ", word);
            }
        }
    }
    free(word);
}




int find_popular(DblLinkedList* list, int wordsCount, int average_size)
{
    int most_popular_index = LOW_INDEX;
    int max_length_count = LOW_INDEX;
    for (int i = 0; i < wordsCount; i++)
    {
        if(back_index(list,i)->length >=average_size) {
            int lengthCount = back_index(list, i)->length * back_index(list, i)->count;
            if (lengthCount > max_length_count) {
                max_length_count = lengthCount;
                most_popular_index = i;
            }
        }
    }
    return most_popular_index;
}

int find_rare(DblLinkedList* list, int wordsCount, int average_size)
{
    int most_rarity_index = LOW_INDEX;
    int min_length_count = MAX_INDEX;
    for (int i = 0; i < wordsCount; i++)
    {
        if(back_index(list,i)->length<average_size)
        {
            int lengthCount = back_index(list, i)->length * back_index(list, i)->count;
            if (lengthCount < min_length_count) {
                min_length_count = lengthCount;
                most_rarity_index = i;
            }
        }
    }
    return most_rarity_index;
}


void remove_node(DblLinkedList* list, const char* wordRemove)
{
    Node* nodeRemove = list->head;

    while (nodeRemove != NULL)
    {
        if (strcmp(nodeRemove->text, wordRemove) == 0)
        {
            list->size--;

            if (nodeRemove == list->head)
            {
                list->head = nodeRemove->next;
                list->head ? list->head->prev = NULL : 0;
            }
            else if (nodeRemove == list->tail)
            {
                list->tail = nodeRemove->prev;
                list->tail ? list->tail->next = NULL : 0;
            }
            else
            {
                nodeRemove->prev->next = nodeRemove->next;
                nodeRemove->next->prev = nodeRemove->prev;
            }

            break;
        }

        nodeRemove = nodeRemove->next;
    }

    free(nodeRemove);
}

void add_symb(FILE* compressFile, char** wordA, char** wordB,int num)
{
    for(int i = 0;i<num;i++) {
        fprintf(compressFile, "@%s@%s", wordA[i], wordB[i]);
    }
    fprintf(compressFile,"\n");
}

void swap_word(DblLinkedList* list, char** wordsA, char** wordsB, int* numReplacements)
{
    while (1) {
        int indPop = find_popular(list, (int) list->size, AVER_SIZE);
        int indRare = find_rare(list, (int) list->size, AVER_SIZE);

        if (check_benefit(list, indPop, indRare) == 1) {
            char *wordA = (char *) malloc(back_index(list, indPop)->length * sizeof(char));
            char *wordB = (char *) malloc(back_index(list, indRare)->length * sizeof(char));
            strcpy(wordA, back_index(list, indPop)->text);
            strcpy(wordB, back_index(list, indRare)->text);
            wordsA[*numReplacements] = wordA;
            wordsB[*numReplacements] = wordB;
            (*numReplacements)++;
            remove_node(list, wordA);
            remove_node(list, wordB);
        } else {
            break;
        }
    }
}

void check(FILE **compressedFile, FILE **file)
{
    if ((*file = fopen("../fileToCmp.txt", "r")) == NULL) {
        printf("Error\n");
        exit(1);
    }
    if ((*compressedFile = fopen("../compFile.txt", "w")) == NULL) {
        printf("Error\n");
        exit(1);
    }
}
