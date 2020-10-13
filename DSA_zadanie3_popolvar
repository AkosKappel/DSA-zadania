#include <stdio.h>
#include <stdlib.h>
#define INFINITY 2147083647
#define UNDEFINED -1

typedef struct edge {
    int destinationID;
    int value;
    struct edge* next;
} EDGE;

typedef struct vertex {
    int ID;
    char blockType;
    EDGE* adjacentVertices;
    int previousID;
    int totalTime;
    int visited;
} VERTEX;

typedef struct graph {
    int vertexCount;
    VERTEX** array;
    int princessCount;
    VERTEX** princessArray;
    VERTEX* dragon;
} GRAPH;

typedef struct heap {
    int capacity;
    int actualSize;
    VERTEX** array;
} HEAP;

typedef struct map {
    int height;
    int width;
    int time;
    char** terrain;
} MAP;

typedef struct path {
    int* coordinates;
    int length;
    int time;
} PATH;

//----------------------------------------------------------------------------------------------------

//Funkcie pre pracu s grafom, vrcholmi, hranami a mapou
GRAPH* createGraph(MAP* map);
void freeGraph(GRAPH* graph);
void loadGraph(GRAPH* graph, MAP* map);
EDGE* getNewEdge(int destination, char blockType);
void insertEdge(VERTEX* vertex, EDGE* edge);
MAP* loadMap(char* fileName);
void freeMap(MAP* map);
void printMap(MAP* map);
void printGraph(GRAPH* graph, int mapWidth);
void printVertex(VERTEX* vertex, int mapWidth);
void printEdge(EDGE* edge);

//Funkcie pre priority queue (min heap)
HEAP* createHeap(int size);
void loadHeap(HEAP* heap, GRAPH* graph, VERTEX* start);
void resizeHeap(HEAP* heap);
void freeHeap(HEAP* heap);
void clearHeap(HEAP* heap);
int isEmpty(HEAP* heap);
void swap(int index1, int index2, HEAP* heap);
int getHeapIndex(VERTEX* vertex, HEAP* heap);
void heapify(HEAP* heap, int index);
void heapifyUp(HEAP* heap);
void heapifyDown(HEAP* heap);
void insertToHeap(VERTEX* vertex, HEAP* heap);
VERTEX* removeFromHeap(HEAP* heap);

//Funkcie pracujuce s najkratsou cestou a Djikstra algoritmom
void resetPrincesses(GRAPH* graph, int princessCount);
void printPath(PATH* path);
void copyPath(PATH* from, PATH* into);
void reversePath(int* path, int firstIndex, int lastIndex);
PATH* findDragon(GRAPH* graph, MAP* map);
VERTEX* findShortestPath(GRAPH* graph, VERTEX* start, PATH* path, MAP* map);
VERTEX* Dijkstra(GRAPH* graph, VERTEX* start, VERTEX* end, PATH* path, MAP* map);
int *zachran_princezne(char** mapa, int n, int m, int t, int* dlzka_cesty);

//----------------------------------------------------------------------------------------------------

int main() {
    int testNumber;
    printf("Zadajte cislo testu:\n");
    scanf("%d", &testNumber);

    MAP* map;
    switch(testNumber) {
        case 1:
            map = loadMap("mapa1.txt");     //Mapa z testovacieho prikladu z AIS
            break;
        case 2:
            map = loadMap("mapa2.txt");     //MaLa mapa s tromi princeznami
            break;
        case 3:
            map = loadMap("mapa3.txt");     //Velka mapa s piatimi princeznami
            break;
        case 4:
            map = loadMap("mapa4.txt");     //Nedostatok casu na zneskodnenie draka
            break;
        case 5:
            map = loadMap("mapa5.txt");     //Prilis vela princezien
            break;
        case 6:
            map = loadMap("mapa6.txt");     //Neexistuje cesta k drakovi
            break;
        case 7:
            map = loadMap("mapa7.txt");     //Neexistuje cesta k jednej z princezien
            break;
        default:
            map = loadMap("mapa8.txt");     //Mapa s jednou princeznou
            break;
    }

    //printMap(map);
    int dlzkaCesty;
    int* cesta = zachran_princezne(map->terrain, map->height, map->width, map->time, &dlzkaCesty);
    for (int i = 0; i < dlzkaCesty; i++) {
        printf("%d %d\n", cesta[i * 2], cesta[i * 2 + 1]);
    }

    freeMap(map);
    return 0;
}

//----------------------------------------------------------------------------------------------------

GRAPH* createGraph(MAP* map) {
    GRAPH* graph = (GRAPH*)calloc(1, sizeof(GRAPH));
    graph->vertexCount = map->height * map->width;
    graph->array = (VERTEX**)calloc(graph->vertexCount, sizeof(VERTEX*));
    graph->princessCount = 0;
    graph->princessArray = (VERTEX**)calloc(5, sizeof(VERTEX*));
    graph->dragon = NULL;

    int i;
    for (i = 0; i < 5; i++) {
        graph->princessArray[i] = NULL;     //Pointre na jednotlive princezne
    }
    for (i = 0; i < graph->vertexCount; i++) {
        graph->array[i] = NULL;             //Pointer na draka
    }

    loadGraph(graph, map);                  //Naplnenie grafu vrcholmi a hranami
    return graph;
}

void freeGraph(GRAPH* graph) {              //Funkcia na uvolnenie grafu z pamate
    EDGE* edge = NULL;
    EDGE* next = NULL;
    for (int i = 0; i < graph->vertexCount; i++) {
        if (graph->array[i]) {
            edge = graph->array[i]->adjacentVertices;
            while (edge) {
                next = edge->next;
                free(edge);
                edge = next;
            }
            free(graph->array[i]);
        }
    }
    free(graph->princessArray);
    free(graph->array);
    free(graph);
}

void loadGraph(GRAPH* graph, MAP* map) {    //Nacitanie mapy do grafu
    int dragonCount = 0;
    VERTEX* vertex = NULL;
    EDGE* edge = NULL;

    for (int i = 0; i < map->height; i++) {
        for (int j = 0; j < map->width; j++) {
            //Nedostupne vrcholy vynecham z grafu
            if (map->terrain[i][j] == 'N') {
                continue;
            }

            //Inicializujem vsetky ostatne vrcholy v grafe
            vertex = (VERTEX*)calloc(1, sizeof(VERTEX));
            vertex->ID = i * map->width + j;
            vertex->blockType = map->terrain[i][j];
            vertex->adjacentVertices = NULL;
            vertex->previousID = UNDEFINED;
            vertex->totalTime = INFINITY;
            vertex->visited = 0;
            graph->array[vertex->ID] = vertex;

            //Oznacim a spocitam vsetkych drakov a princezne
            if (map->terrain[i][j] == 'P') {
                if (graph->princessCount >= 5) {
                    printf("Prilis vela princezien na mape. Maximalny pocet princezien je 5.\n");
                    exit(0);
                }
                graph->princessArray[graph->princessCount] = vertex;
                graph->princessCount++;
            } else if (map->terrain[i][j] == 'D') {
                graph->dragon = vertex;
                dragonCount++;
            }

            //Pridam hrany pre kazdy jeden vrchol, okrem hran ktore vedu k nedostupnym vrcholom
            if (i + 1 < map->height && map->terrain[i + 1][j] != 'N') {
                edge = getNewEdge((i + 1) * map->width + j, map->terrain[i + 1][j]);
                insertEdge(graph->array[vertex->ID], edge);
            }
            if (j - 1 >= 0 && map->terrain[i][j - 1] != 'N') {
                edge = getNewEdge(i * map->width + (j - 1), map->terrain[i][j - 1]);
                insertEdge(graph->array[vertex->ID], edge);
            }
            if (i - 1 >= 0 && map->terrain[i - 1][j] != 'N') {
                edge = getNewEdge((i - 1) * map->width + j, map->terrain[i - 1][j]);
                insertEdge(graph->array[vertex->ID], edge);
            }
            if (j + 1 < map->width && map->terrain[i][j + 1] != 'N') {
                edge = getNewEdge(i * map->width + (j + 1), map->terrain[i][j + 1]);
                insertEdge(graph->array[vertex->ID], edge);
            }
        }
    }
    if (graph->princessCount < 5) {     //Zmensim pole ukazovatelov, ktore ukazuju na jednotlive princezne
        graph->princessArray = (VERTEX**)realloc(graph->princessArray, graph->princessCount * sizeof(VERTEX*));
    }
    if (dragonCount != 1) {
        printf("Nespravny pocet drakov. Na mape musi byt prave jeden drak.\n");
        exit(0);
    }
    if (graph->princessCount == 0) {
        printf("Na mape sa musi nachadzat minimalne jedna princezna.\n");
        exit(0);
    }
}

EDGE* getNewEdge(int destination, char blockType) {     //Vytvorenie novej hrany
    EDGE* newEdge = (EDGE*)calloc(1, sizeof(EDGE));
    newEdge->destinationID = destination;
    newEdge->next = NULL;
    if (blockType == 'H') {
        newEdge->value = 2;
    } else {
        newEdge->value = 1;
    }
    return newEdge;
}

void insertEdge(VERTEX* vertex, EDGE* edge) {           //Vlozenie hrany k danemu vrcholu
    edge->next = vertex->adjacentVertices;
    vertex->adjacentVertices = edge;
}

MAP* loadMap(char* fileName) {                                        //Funkcia pre nacitanie mapy zo suboru
    FILE* file;
    if ((file = fopen(fileName, "r")) == NULL) {   //Subor s mapou
        printf("Subor sa nepodarilo otvorit.\n");
        exit(-1);
    }

    MAP* map = (MAP*)calloc(1, sizeof(MAP));
    fscanf(file, "%d%d%d", &map->height, &map->width, &map->time);

    char block;                                         //Nacitavanie mapy
    map->terrain = (char**)calloc(map->height, sizeof(char*));
    for (int i = 0; i < map->height; i++) {
        map->terrain[i] = (char*)calloc(map->width, sizeof(char));
        for (int j = 0; j < map->width; j++) {
            block = fgetc(file);
            if(block == '\n')
                block = fgetc(file);
            map->terrain[i][j] = block;
        }
    }

    if (fclose(file) == EOF) {
        printf("Subor sa nepodarilo zatvorit.\n");
    }
    return map;
}

void freeMap(MAP* map) {
    for (int i = 0; i < map->height; i++) {
        free(map->terrain[i]);
    }
    free(map->terrain);
    free(map);
}

void printMap(MAP* map) {
    printf("n = %d\n", map->height);
    printf("m = %d\n", map->width);
    printf("t = %d\n", map->time);
    printf("\n");
    for (int i = 0; i < map->height; i++) {
        for (int j = 0; j < map->width; j++) {
            printf("%c ", map->terrain[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void printGraph(GRAPH* graph, int mapWidth) {
    int i;
    for (i = 0; i < graph->vertexCount; i++) {
        if (graph->array[i] == NULL) {
            continue;
        }
        printVertex(graph->array[i], mapWidth);
    }
    printf("Vertex count = %d\n", graph->vertexCount);
    printf("Princess count = %d\n", graph->princessCount);
    printf("Dragon coordinates: (%d, %d)\n", graph->dragon->ID % mapWidth, graph->dragon->ID / mapWidth);
    printf("Princesses: ");
    for (i = 0; i < graph->princessCount; i++) {
        printf("(%d, %d) ", graph->princessArray[i]->ID % mapWidth,
               graph->princessArray[i]->ID / mapWidth);
    }
    printf("\n\n");
}

void printVertex(VERTEX* vertex, int mapWidth) {
    if (vertex == NULL) {
        return;
    }
    printf("Vertex ID/Block (%d/%c)\n", vertex->ID, vertex->blockType);
    printf("Coordinates: (%d, %d), ", vertex->ID % mapWidth, vertex->ID / mapWidth);
    printf("Adjacent vertices:\n");
    EDGE* edge = vertex->adjacentVertices;
    while (edge) {
        printEdge(edge);
        edge = edge->next;
    }
    printf("Total time to vertex: %d\n", vertex->totalTime);
    printf("Previous ID: %d\n", vertex->previousID);
    printf("Visited: %d\n", vertex->visited);
    printf("\n");
}

void printEdge(EDGE* edge) {
    printf("\tID: %d, Value: %d\n", edge->destinationID, edge->value);
}

//----------------------------------------------------------------------------------------------------

HEAP* createHeap(int size) {                //Vytvorim binarnu haldu
    HEAP* heap = (HEAP*)calloc(1, sizeof(HEAP));
    heap->array = (VERTEX**)calloc(size, sizeof(VERTEX*));
    heap->capacity = size;
    heap->actualSize = 0;
    return heap;
}

void loadHeap(HEAP* heap, GRAPH* graph, VERTEX* start) {
    VERTEX* vertex;
    for (int i = 0; i < graph->vertexCount; i++) {
        if (graph->array[i] == NULL) {
            continue;
        }
        vertex = graph->array[i];           //Nastavim vrcholy na zaciatocne podmienky pre algoritmus
        vertex->previousID = UNDEFINED;
        vertex->totalTime = INFINITY;
        vertex->visited = 0;
        if (start == vertex) {
            switch (vertex->blockType) {
                case 'N':
                    printf("Neplatna mapa, nemozno zacat na policku 'N'.\n");
                    exit(0);
                case 'H':
                    vertex->totalTime = 2;
                    break;
                default:
                    vertex->totalTime = 1;
                    break;
            }
        }
        insertToHeap(vertex, heap);         //Vlozim vrchol do binarnej haldy
    }
}

void resizeHeap(HEAP* heap) {               //Zvacsenie velkosti binarnej haldy
    heap->array = (VERTEX**)realloc(heap->array, 2 * heap->capacity * sizeof(VERTEX*));
    heap->capacity *= 2;
}

void freeHeap(HEAP* heap) {
    free(heap->array);
    free(heap);
}

void clearHeap(HEAP* heap) {                //Funkcia na vyprazdnenie binarnej haldy
    while (!isEmpty(heap)) {
        removeFromHeap(heap);
    }
}

int isEmpty(HEAP* heap) {                   //Funkcia na kontrolu, ci je halda prazdna
    return heap->actualSize == 0 ? 1 : 0;
}

void swap(int index1, int index2, HEAP* heap) {
    VERTEX* temp = heap->array[index1];     //Zamena poradia prvkov v halde
    heap->array[index1] = heap->array[index2];
    heap->array[index2] = temp;
}

int getHeapIndex(VERTEX* vertex, HEAP* heap) {
    if (vertex != NULL) {                   //Zistime index daneho prvku v binarnej halde
        for (int i = 0; i < heap->actualSize; i++) {
            if (heap->array[i] == vertex) {
                return i;
            }
        }
    }
    return heap->actualSize;
}

void heapify(HEAP* heap, int index) {       //Tato funkcia posunie kazdy aktualizovani
    int parentIndex = (index - 1) / 2;      //prvok na spravnu poziciu v halde
    while (parentIndex >= 0 && heap->array[parentIndex]->totalTime > heap->array[index]->totalTime) {
        swap(parentIndex, index, heap);
        index = parentIndex;
        parentIndex = (index - 1) / 2;
    }
}

void heapifyUp(HEAP* heap) {                //Funkcia umiestni novy vlozeny prvok
    int index = heap->actualSize - 1;       //na spravnu poziciu v halde
    int parentIndex = (index - 1) / 2;
    while (parentIndex >= 0 && heap->array[parentIndex]->totalTime > heap->array[index]->totalTime) {
        swap(parentIndex, index, heap);
        index = parentIndex;
        parentIndex = (index - 1) / 2;
    }
}

void heapifyDown(HEAP* heap) {              //Funkcia upravi poradie prvkov v binarnej halde
    int index = 0;                          //po kazdom vybrati najmensie prvku z haldy
    int smallerChildIndex = 2 * index + 1;
    while (heap->actualSize > smallerChildIndex) {
        if (2 * index + 2 < heap->actualSize &&
            heap->array[2 * index + 2]->totalTime < heap->array[2 * index + 1]->totalTime) {
            smallerChildIndex = 2 * index + 2;
        }
        if (heap->array[index]->totalTime < heap->array[smallerChildIndex]->totalTime) {
            break;
        }
        swap(index, smallerChildIndex, heap);
        index = smallerChildIndex;
        smallerChildIndex = 2 * index + 1;
    }
}

void insertToHeap(VERTEX* vertex, HEAP* heap) {
    if (vertex == NULL) {
        return;
    }                                       //Vlozime novy prvok do binarnej haldy
    if (heap->capacity == heap->actualSize) {
        resizeHeap(heap);
    }
    heap->array[heap->actualSize] = vertex;
    heap->actualSize++;
    heapifyUp(heap);
}

VERTEX* removeFromHeap(HEAP* heap) {        //Vyberieme najmensi prvok z binarnej haldy
    VERTEX* vertex = heap->array[0];
    heap->array[0] = heap->array[heap->actualSize - 1];
    heap->actualSize--;
    heapifyDown(heap);
    vertex->visited = 1;
    return vertex;
}

//----------------------------------------------------------------------------------------------------

void resetPrincesses(GRAPH* graph, int princessCount) {
    graph->princessCount = princessCount;
    for (int i = 0; i < princessCount; i++) {
        graph->princessArray[i]->blockType = 'P';
    }
}

void printPath(PATH* path) {
    for (int i = 0; i < path->length; i++) {
        printf("(%d, %d) ", path->coordinates[i * 2], path->coordinates[i * 2 + 1]);
    }
    printf("\n");
    printf("Path time: %d\n", path->time);
    printf("Path length: %d\n", path->length);
    printf("\n");
}

void copyPath(PATH* from, PATH* into) {
    into->length = from->length;
    into->time = from->time;
    for (int i = 0; i < 2 * from->length; i++) {
        into->coordinates[i] = from->coordinates[i];
    }
}

void reversePath(int* path, int firstIndex, int lastIndex) {
    int temp;                               //Tato funkcia otoci poradie suradnic na danej ceste
    while (firstIndex < lastIndex) {
        temp = path[firstIndex];
        path[firstIndex] = path[lastIndex];
        path[lastIndex] = temp;
        firstIndex++;
        lastIndex--;
    }
}

//----------------------------------------------------------------------------------------------------

PATH* findDragon(GRAPH* graph, MAP* map) {  //Funkcia zisti najrychlejsiu cestu k drakovi
    HEAP* heap = createHeap(graph->vertexCount);
    loadHeap(heap, graph, graph->array[0]);

    VERTEX* vertex = NULL;
    EDGE* edge = NULL;
    int time;

    while (!isEmpty(heap)) {                //Zistime najkratsiu cestu do kazdeho dostupneho vrcholu v grafe
        vertex = removeFromHeap(heap);
        edge = vertex->adjacentVertices;
        while (edge != NULL) {
            if (graph->array[edge->destinationID]->visited == 0) {
                time = vertex->totalTime + edge->value;
                if (time < graph->array[edge->destinationID]->totalTime) {
                    graph->array[edge->destinationID]->totalTime = time;
                    graph->array[edge->destinationID]->previousID = vertex->ID;
                    heapify(heap, getHeapIndex(graph->array[edge->destinationID], heap));
                }
            }
            edge = edge->next;
        }
    }
    freeHeap(heap);
                                            //Zistime ci existuje cesta za drakom
    if (graph->dragon->previousID == UNDEFINED) {
        printf("Nevedie ziadna cesta k drakovi.\n");
        exit(0);
    }
                                            //Zistime ci existuje cesta ku kazdej jednej princeznej
    for (int i = 0; i < graph->princessCount; i++) {
        if (graph->princessArray[i]->previousID == UNDEFINED) {
            printf("Neda sa dostat ku kazdej jednej princeznej.\n");
            exit(0);
        }
    }
                                            //Skontrolujeme ci v danom case stihame cestu za drakom
    if (graph->dragon->totalTime > map->time) {
        printf("V urcenom case sa nepodarilo zneskodnit draka.\n");
    }

    PATH* path = (PATH*)calloc(1, sizeof(PATH));            //Vytvorime cestu k drakovi
    path->coordinates = (int*)calloc(6000, sizeof(int));
    path->length = 1;
    path->time = graph->dragon->totalTime;
    vertex = graph->dragon;
    int index = 0;

    while (vertex->previousID != UNDEFINED) {
        path->length += 1;                                  //Zapiseme cestu od draka k suradnici(0, 0)
        path->coordinates[index++] = vertex->ID / map->width;      // y-ova suradnica
        path->coordinates[index++] = vertex->ID % map->width;      // x-ova suradnica
        vertex = graph->array[vertex->previousID];
    }

    path->coordinates[index++] = vertex->ID % map->width;
    path->coordinates[index] = vertex->ID / map->width;
    reversePath(path->coordinates, 0, index);       //Suradnice su v opacnom poradi (od draka po start)
    return path;                                            //Preto ich musime otocit
}

VERTEX* findShortestPath(GRAPH* graph, VERTEX* start, PATH* path, MAP* map) {
    HEAP* heap = createHeap(graph->vertexCount);            //Funkcia zisti najkratsiu cestu k najblizsej princeznej
    loadHeap(heap, graph, start);
    start->totalTime = 0;

    VERTEX* princess = NULL;
    VERTEX* vertex = NULL;
    EDGE* edge = NULL;
    int time;

    while (!isEmpty(heap)) {
        vertex = removeFromHeap(heap);
        if (vertex->blockType == 'P') {     //Ak najdeme princeznu oznacime ju za najdenu a ukoncime algoritmus
            vertex->blockType = 'C';
            princess = vertex;
            freeHeap(heap);
            break;
        }
        edge = vertex->adjacentVertices;
        while (edge != NULL) {
            if (graph->array[edge->destinationID]->visited == 0) {
                time = vertex->totalTime + edge->value;
                if (time < graph->array[edge->destinationID]->totalTime) {
                    graph->array[edge->destinationID]->totalTime = time;
                    graph->array[edge->destinationID]->previousID = vertex->ID;
                    heapify(heap, getHeapIndex(graph->array[edge->destinationID], heap));
                }
            }
            edge = edge->next;
        }
    }

    int index = path->length * 2;
    time = index;

    path->time += vertex->totalTime;
    while (vertex->previousID != UNDEFINED) {               //Zapiseme najkratsiu najdenu cestu
        path->length += 1;
        path->coordinates[index++] = vertex->ID / map->width;
        path->coordinates[index++] = vertex->ID % map->width;
        vertex = graph->array[vertex->previousID];
    }
                                                            //Otocime poradie suradnic najdenej cesty
    reversePath(path->coordinates, time, index - 1);
    return princess;
}

VERTEX* Dijkstra(GRAPH* graph, VERTEX* start, VERTEX* end, PATH* path, MAP* map) {
    HEAP* heap = createHeap(graph->vertexCount);            //Funkcia najde najkratsiu cestu medzi dvomi vrcholmi
    loadHeap(heap, graph, start);
    start->totalTime = 0;

    VERTEX* princess = NULL;
    VERTEX* vertex = NULL;
    EDGE* edge = NULL;
    int time;

    while (!isEmpty(heap)) {
        vertex = removeFromHeap(heap);
        if (vertex == end && vertex->blockType == 'P') {    //Oznacime najdenu princeznu a ukoncime algoritmus
            vertex->blockType = 'C';
            princess = vertex;
            freeHeap(heap);
            break;
        }
        edge = vertex->adjacentVertices;
        while (edge != NULL) {
            if (graph->array[edge->destinationID]->visited == 0) {
                time = vertex->totalTime + edge->value;
                if (time < graph->array[edge->destinationID]->totalTime) {
                    graph->array[edge->destinationID]->totalTime = time;
                    graph->array[edge->destinationID]->previousID = vertex->ID;
                    heapify(heap, getHeapIndex(graph->array[edge->destinationID], heap));
                }
            }
            edge = edge->next;
        }
    }

    int index = path->length * 2;
    time = index;

    path->time += vertex->totalTime;
    while (vertex->previousID != UNDEFINED) {               //Zapiseme najkratsiu najdenu cestu
        path->length += 1;
        path->coordinates[index++] = vertex->ID / map->width;
        path->coordinates[index++] = vertex->ID % map->width;
        vertex = graph->array[vertex->previousID];
    }
                                                            //Otocime poradie suradnic najdenej cesty
    reversePath(path->coordinates, time, index - 1);
    return princess;
}

int *zachran_princezne(char** mapa, int n, int m, int t, int* dlzka_cesty) {
    MAP* map = (MAP*)calloc(1, sizeof(MAP));
    map->height = n;
    map->width = m;
    map->time = t;
    map->terrain = mapa;

    GRAPH* graph = createGraph(map);                    //Inicializacia grafu
    int tempPrincessCount = graph->princessCount;
    PATH* path = findDragon(graph, map);                //Cesta od suradnice(0, 0) po draka

    int i, j, k;
    int numberOfPaths = 0;                              //Pocet vsetkych kombinacii najkratsich ciest medzi princeznami
    switch (tempPrincessCount) {
        case 3: numberOfPaths = 3;     break;
        case 4: numberOfPaths = 12;    break;
        case 5: numberOfPaths = 60;    break;
    }

    PATH** allPaths = (PATH**)calloc(numberOfPaths, sizeof(PATH*));
    for (i = 0; i < numberOfPaths; i++) {
        allPaths[i] = (PATH*)calloc(1, sizeof(PATH));
        allPaths[i]->coordinates = (int*)calloc(6000, sizeof(int));
        copyPath(path, allPaths[i]);
    }

    int counter = 0;
    VERTEX* start = graph->dragon;
    switch (tempPrincessCount) {
        case 3:                                         //Riesenie pre 3 princezne
            for (i = 0; i < tempPrincessCount; i++) {
                start = graph->dragon;
                start = Dijkstra(graph, start, graph->princessArray[i], allPaths[i], map);
                graph->princessCount--;
                while (graph->princessCount > 0) {
                    start = findShortestPath(graph, start, allPaths[i], map);
                    graph->princessCount--;
                }
                resetPrincesses(graph, tempPrincessCount);
            }
            break;
        case 4:                                         //Riesenie pre 4 princezne
            for (i = 0; i < tempPrincessCount; i++) {
                for (j = 0; j < tempPrincessCount; j++) {
                    if (i != j) {
                        start = graph->dragon;
                        start = Dijkstra(graph, start, graph->princessArray[i], allPaths[counter], map);
                        graph->princessCount--;
                        start = Dijkstra(graph, start, graph->princessArray[j], allPaths[counter], map);
                        graph->princessCount--;
                        while (graph->princessCount > 0) {
                            start = findShortestPath(graph, start, allPaths[counter], map);
                            graph->princessCount--;
                        }
                        resetPrincesses(graph, tempPrincessCount);
                        counter++;
                    }
                }
            }
            break;
        case 5:                                         //Riesenie pre 5 princezien
            for (i = 0; i < tempPrincessCount; i++) {
                for (j = 0; j < tempPrincessCount; j++) {
                    for (k = 0; k < tempPrincessCount; k++) {
                        if (i != j && j != k && k != i) {
                            start = graph->dragon;
                            start = Dijkstra(graph, start, graph->princessArray[i], allPaths[counter], map);
                            graph->princessCount--;
                            start = Dijkstra(graph, start, graph->princessArray[j], allPaths[counter], map);
                            graph->princessCount--;
                            start = Dijkstra(graph, start, graph->princessArray[k], allPaths[counter], map);
                            graph->princessCount--;
                            while (graph->princessCount > 0) {
                                start = findShortestPath(graph, start, allPaths[counter], map);
                                graph->princessCount--;
                            }
                            resetPrincesses(graph, tempPrincessCount);
                            counter++;
                        }
                    }
                }
            }
            break;
        default:                                        //Riesenie pre najviac 2 princezne
            while (graph->princessCount > 0) {
                start = findShortestPath(graph, start, path, map);
                graph->princessCount--;
            }
            resetPrincesses(graph, tempPrincessCount);
            break;
    }

    int shortest = INFINITY;                            //Vyberie najrychlejsiu cestu spomedzi moznych ciest
    for (i = 0; i < numberOfPaths; i++) {
        if (allPaths[i]->time < shortest) {
            shortest = allPaths[i]->time;
            copyPath(allPaths[i], path);
        }
    }

    *dlzka_cesty = path->length;                        //Zapiseme dlzku finalnej najkratsej cesty
    int* finalPath = (int*)calloc(*dlzka_cesty * 2, sizeof(int));
    for (i = 0; i < 2 * path->length; i++) {
        finalPath[i] = path->coordinates[i];
    }

    free(map);                                          //Uvolnenie pamate
    freeGraph(graph);
    for (i = 0; i < numberOfPaths; i++) {
        free(allPaths[i]->coordinates);
        free(allPaths[i]);
    }
    if (allPaths)
        free(allPaths);
    free(path->coordinates);
    free(path);
    return finalPath;
}
