#include "functions.h"

int main()
{
    FILE* file;
    DblLinkedList* list;
    list = (DblLinkedList*)malloc(sizeof(DblLinkedList));
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    if ((file = fopen("../fileToCmp.txt", "r")) == NULL) {
        printf("Error\n");
        exit(1);
    }
    get_word(file, list);
    fclose(file);
    FILE *compressedFile;
    check(&compressedFile,&file);
    show_list(list);
    int numReplacements = 0;
    char **wordsA = (char **) malloc(list->size * sizeof(char *));
    char **wordsB = (char **) malloc(list->size * sizeof(char *));
    swap_word(list, wordsA, wordsB, &numReplacements);
    add_symb(compressedFile,wordsA,wordsB,numReplacements);
    print_compressed(file,compressedFile, wordsA, wordsB, numReplacements);
    fclose(file);
    fclose(compressedFile);
    return 0;
}