#include "AVL_Tree.h"
#include "RedBlack_Tree.h"
#include "HashTable_Chaining.h"
#include "HashTable_LinearProbing.h"
#include <windows.h>

#define INPUT_SIZE 10000  //velkost vstupu

int main() {
    //Init
    LARGE_INTEGER frequency;
    LARGE_INTEGER start;
    LARGE_INTEGER end;
    double interval;
    FILE* file;
    if ((file = fopen("numbers.txt", "r")) == NULL) {   //textovy subor s vygenerovanymi cislami
        printf("Error: subor nenajdeny\n");
        return -1;
    }

//AVL strom
    Node* root = NULL;
    int data;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);
    for (int i = 0; i < INPUT_SIZE; i++) {
        fscanf(file, "%d", &data);
        insert_AVL(&root, data);
    }
    QueryPerformanceCounter(&end);
    interval = (double) (end.QuadPart - start.QuadPart) / frequency.QuadPart;
    printf("AVL insert = %f sec\n", interval);
    rewind(file);
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);
    for (int i = 0; i < INPUT_SIZE; i++) {
        fscanf(file, "%d", &data);
        search_AVL(data, root);
    }
    QueryPerformanceCounter(&end);
    interval = (double) (end.QuadPart - start.QuadPart) / frequency.QuadPart;
    printf("AVL search = %f sec\n", interval);

//Red-Black strom
    NILL = (struct node*)malloc(sizeof(struct node));
    NILL->color = BLACK;
    ROOT = NILL;
    int key;
    rewind(file);
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);
    for (int i = 0; i < INPUT_SIZE; i++) {
        fscanf(file, "%d", &key);
        red_black_insert(key);
    }
    QueryPerformanceCounter(&end);
    interval = (double) (end.QuadPart - start.QuadPart) / frequency.QuadPart;
    printf("Red-Black insert = %f sec\n", interval);
    rewind(file);
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);
    for (int i = 0; i < INPUT_SIZE; i++) {
        fscanf(file, "%d", &key);
        tree_search(key);
    }
    QueryPerformanceCounter(&end);
    interval = (double) (end.QuadPart - start.QuadPart) / frequency.QuadPart;
    printf("Red-Black search = %f sec\n", interval);

//Hashtable chaining
    HASHTABLE* hashTable = createHashTable(101);
    char keyHash[30];
    rewind(file);
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);
    for (int i = 0; i < INPUT_SIZE; i++) {
        fscanf(file, "%s", keyHash);
        insertItem(keyHash, i + 1, &hashTable);
    }
    QueryPerformanceCounter(&end);
    interval = (double) (end.QuadPart - start.QuadPart) / frequency.QuadPart;
    printf("Chaining insert = %f sec\n", interval);
    rewind(file);
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);
    for (int i = 0; i < INPUT_SIZE; i++) {
        fscanf(file, "%s", keyHash);
        searchItem(keyHash, hashTable);
    }
    QueryPerformanceCounter(&end);
    interval = (double) (end.QuadPart - start.QuadPart) / frequency.QuadPart;
    printf("Chaining search = %f sec\n", interval);

//Hashtable open addressing -> linear probing
    dummyItem = (struct DataItem*) malloc(sizeof(struct DataItem));
    dummyItem->data = -1;
    dummyItem->key = -1;

    int keyHash2;
    rewind(file);
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);
    for (int i = 0; i < INPUT_SIZE; i++) {
        fscanf(file, "%d", &keyHash2);
        if (keyHash2 < 0)
            keyHash2 = -keyHash2;
        insert(keyHash2, i + 1);
    }
    QueryPerformanceCounter(&end);
    interval = (double) (end.QuadPart - start.QuadPart) / frequency.QuadPart;
    printf("Open addressing insert = %f sec\n", interval);
    rewind(file);
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);
    for (int i = 0; i < INPUT_SIZE; i++) {
        fscanf(file, "%d", &keyHash2);
        if (keyHash2 < 0)
            keyHash2 = -keyHash2;
        item = search(keyHash2);
    }
    QueryPerformanceCounter(&end);
    interval = (double) (end.QuadPart - start.QuadPart) / frequency.QuadPart;
    printf("Open addressing search = %f sec\n", interval);

    //End
    fclose(file);
    return 0;
}

