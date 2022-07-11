#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct item {
    char key[30];
    int data;
    struct item* next;
}ITEM;

typedef struct hashtable{
    ITEM** array;
    unsigned int itemCount;
    unsigned int size;
    double loadFactor;
}HASHTABLE;

HASHTABLE* createHashTable(unsigned int tableSize);
ITEM* createNewItem(char key[], int data);
unsigned int hashFunction(char key[], unsigned int tableSize);
int isPrime(unsigned int number);
unsigned int roundUpToPrime(unsigned int number);
void relocateItem(ITEM* item, HASHTABLE** hashTable);
HASHTABLE* increaseTableSize(HASHTABLE* table);
void insertItem(char key[], int data, HASHTABLE** hashTable);
ITEM* searchItem(char key[], HASHTABLE* hashTable);
void printHashTable(HASHTABLE* hashTable);

//Funkcia na vytvorenie prazdnej hasovacej tabulky
HASHTABLE* createHashTable(unsigned int tableSize) {
    HASHTABLE* table = (HASHTABLE*)malloc(sizeof(HASHTABLE));

    if (tableSize < 5) {
        tableSize = 5;
    }

    table->size = tableSize;
    table->itemCount = 0;
    table->array = (ITEM**)malloc(tableSize * sizeof(ITEM*));
    table->loadFactor = 0;

    for (int i = 0; i < tableSize; i++) {
        *(table->array + i) = NULL;
    }
    return table;
}

//Funkcia na vytvorenie noveho prvku
ITEM* createNewItem(char* key, int data) {
    ITEM* newItem = (ITEM*)malloc(sizeof(ITEM));

    int i = 0;
    while (key[i] != '\0') {
        newItem->key[i] = key[i];
        i++;
    }

    newItem->key[i] = '\0';
    newItem->data = data;
    newItem->next = NULL;
    return newItem;
}

//Hashovanie zadaneho stringu
unsigned int hashFunction(char* key, unsigned int tableSize) {
    unsigned int index = 0;

    int i = 0;
    while (key[i] != '\0') {
        index = 31 * index + (int)key[i];
        index %= tableSize;
        i++;
    }

    return (index % tableSize);
}

//Kontrola, ci je dane cislo prvocislo
//Aby funkcia nemusela skontrolovat, ci kazde mensie cislo je delitelom cisla number,
//jej algoritmus je zrychleny pomocou implementovania funkcie zo zdroja:
//https://www.geeksforgeeks.org/program-to-find-the-next-prime-number/
int isPrime(unsigned int number) {
    if ((number % 2 == 0) || (number % 3 == 0)) {
        return 0;
    }

    for (int i = 5; i * i <= number; i+=6) {
        if ((number % i == 0) || (number % (i + 2) == 0)) {
            return 0;
        }
    }

    return 1;
}

//Generovanie vacsieho prvocisla
unsigned int roundUpToPrime(unsigned int number) {
    int found = 0;
    while (!found) {
        number++;
        if (isPrime(number)) {
            found = 1;
        }
    }
    return number;
}

//Funkcia na premiestnenie prvkov do novej tabulky
void relocateItem(ITEM* item, HASHTABLE** hashTable) {
    unsigned int index = hashFunction(item->key, (*hashTable)->size);

    item->next = *((*hashTable)->array + index);
    *((*hashTable)->array + index) = item;

    (*hashTable)->itemCount++;
    (*hashTable)->loadFactor = (double)(*hashTable)->itemCount / (*hashTable)->size;
}

//Funkcia na zvacsenie tabulky
//Najprv sa vytvori nova vacsia tabulka, prenesu sa prvky z povodnej tabulky
//do novej tabulky, a napokon sa zlikviduje stara tabulka
HASHTABLE* increaseTableSize(HASHTABLE* table) {
    unsigned int newSize = table->size * 2;
    newSize = roundUpToPrime(newSize);
    HASHTABLE* newTable = createHashTable(newSize);

    ITEM* temp = NULL;
    ITEM* next = NULL;
    for (int i = 0; i < table->size; i++) {
        if (*(table->array + i) != NULL) {
            temp = *(table->array + i);
            while (temp != NULL) {
                next = temp->next;
                relocateItem(temp, &newTable);
                temp = next;
            }
        }
    }

    free(table);
    return newTable;
}

//Vlozenie do tabulky
//Index zistime podla kluca a aby sme urychlili cas vlozenia,
//tak vkladame na zaciatok jednosmerneho spajaneho zoznamu O(1)
void insertItem(char key[], int data, HASHTABLE** hashTable) {
    ITEM* newItem = createNewItem(key, data);
    unsigned int index = hashFunction(key, (*hashTable)->size);

    newItem->next = *((*hashTable)->array + index);
    *((*hashTable)->array + index) = newItem;

    (*hashTable)->itemCount++;
    (*hashTable)->loadFactor = (double)(*hashTable)->itemCount / (*hashTable)->size;

    if ((*hashTable)->loadFactor > 0.5)
        *hashTable = increaseTableSize(*hashTable);
}

//Vyhladavanie v tabulke
ITEM* searchItem(char key[], HASHTABLE* hashTable) {
    unsigned int index = hashFunction(key, hashTable->size);

    ITEM* temp = *(hashTable->array + index);
    while (temp != NULL) {
        if (strcmp(temp->key, key) == 0)
            return temp;
        temp = temp->next;
    }
    return NULL;
}

//Vypisovanie tabulky (sluzi na kontrolu)
void printHashTable(HASHTABLE* hashTable) {
    ITEM* temp;
    for (int i = 0; i < hashTable->size; i++) {
        if (*(hashTable->array + i) != NULL) {
            temp = *(hashTable->array + i);
            printf("Index %d:\n", i);
            while (temp != NULL) {
                printf("   Key: %s\tData: %d\n", temp->key, temp->data);
                temp = temp->next;
            }
        }
    }
}

