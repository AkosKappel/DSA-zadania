#include <stdio.h>

void* memory_alloc(unsigned int size);
int memory_free(void* valid_ptr);
int memory_check(void* ptr);
void memory_init(void* ptr, unsigned int size);
void insertAtBeginning(unsigned short* currentHeader);
void removeBlockFromList(unsigned short* pointer);
void mergeWithNextBlock(unsigned short* p);
void mergeWithPreviousBlock(unsigned short* p);
unsigned short isFree(unsigned short* blockHeader);
unsigned short roundUpToEven(unsigned short number);
unsigned short roundDownToEven(unsigned short number);
unsigned short isEven(unsigned short number);

void* heapPointer;

int main() {
    char region[100];
    memory_init(region, 100);

    int* p = (int*)memory_alloc(10 * sizeof(int));
    if(memory_check(p))
        memory_free(p);

    return 0;
}

void* memory_alloc(unsigned int size) {
    unsigned short area;
    area = (unsigned short)size;

    unsigned short* p;
    p = (unsigned short*)heapPointer;
    p += *p;                        // nastavime pointer p na header prveho volneho bloku

    area = roundUpToEven(area);     // zaokruhlime velkost bloku na parne cislo
    if (area < 8) {                 // nastavime velkost nejmensieho alokovatelneho bloku na 8 bajtov
        area = 8;
    }
    area += 2 * sizeof(short);      // pridame miesto pre hlavicku a paticku typu unsigned short

    while (*p < area) {             // hladame firstFit:
        if (*p == 1) {              // ak sme presli cez celu pamat, tak firstFit neexistuje
            return NULL;
        }                           // posuvame pointer p po volnych blokoch
        p = (unsigned short*)heapPointer + *(p + 1);
    }

    unsigned short* bestFit = p;    // hladame bestFit:
    while (*p != 1) {
        if ((*p >= area) && (*bestFit > *p)) {
            bestFit = p;
        }                           // posuvame pointer p po zvysnych volnych blokoch
        p = (unsigned short*)heapPointer + *(p + 1);
    }

    unsigned short allocatedSize = area;            /* velkost alokovaneho bloku */
    unsigned short originalSize = *bestFit & -2;    /* velkost povodneho bloku pred alokovanim */

    if (originalSize - allocatedSize < 8) {
        allocatedSize = originalSize;               // ak zostane prilis maly prazdny blok, alokujeme cely povodny blok
    }

    *bestFit = allocatedSize | 1;                   // zapiseme velkost noveho bloku do oboch hlaviciek
    *(bestFit + allocatedSize / 2 - 1) = allocatedSize | 1;

    unsigned short* heapStart;
    unsigned short* nextHeader = bestFit + 1;
    unsigned short* previousHeader = bestFit + 2;
    heapStart = (unsigned short*)heapPointer;

    if (allocatedSize < originalSize) {     // 1) alokujeme iba cast volneho bloku:
                                                    // vytvorime nove hlavicky pre zvysok bloku
        *(bestFit + allocatedSize / 2) = originalSize - allocatedSize;
        *(bestFit + allocatedSize / 2 + *(bestFit + allocatedSize / 2) / 2 - 1) = originalSize - allocatedSize;
                                                    // posunieme previous a next pointer do volnej casti bloku
        *(previousHeader + allocatedSize / 2) = *previousHeader;
        *(nextHeader + allocatedSize / 2) = *nextHeader;
                                                    // aktualizujeme okolite pointre, aby ukazovali na novy header volneho bloku:
        if (*(heapStart + *nextHeader) != 1)        // presmerujeme previous pointer nasledujuceho bloku na novy header
            *(heapStart + *nextHeader + 2) = (bestFit + allocatedSize / 2) - heapStart;
        if (*previousHeader != 0)                   // presmerujeme next pointer predchadzajuceho bloku na novy header
            *(heapStart + *previousHeader + 1) = (bestFit + allocatedSize / 2) - heapStart;
        else
            *heapStart = (bestFit + allocatedSize / 2) - heapStart;
    }
    else {                                  // 2) alokujeme cely volny blok:
        if (*(heapStart + *nextHeader) != 1)        // presmerujeme previous pointer nasledujuceho bloku na hlavicku predchadzajuceho bloku
            *(heapStart + *nextHeader + 2) = *previousHeader;
        if (*previousHeader != 0)                   // presmerujeme next pointer predchadzajuceho bloku na hlavicku nasledujuceho bloku
            *(heapStart + *previousHeader + 1) = *nextHeader;
        else
            *heapStart = *nextHeader;
    }

    bestFit += 1;                                   // posunieme bestFit pointer za hlavicku alokovaneho bloku
    return bestFit;
}

int memory_free(void* valid_ptr) {
    if (valid_ptr == NULL) {
        return 1;
    }

    unsigned short* p;
    p = (unsigned short*)valid_ptr;
    p -= 1;                                         // nastavime pointer na hlavicku bloku

    unsigned short* heapStart;
    heapStart = (unsigned short*)heapPointer;

    *p &= -2;                                       // oznacime posledny bit oboch hlaviciek bloku nulou
    *(p + *p / 2 - 1) &= -2;

    unsigned short* nextBlock = (p + *p / 2);       // oznacime hlavicky susediacich blokov
    unsigned short* previousBlock = (p - 1);
    if (previousBlock != heapStart) {
        previousBlock -= *previousBlock / 2;
        previousBlock += 1;
    }                                               // susediace volne bloky spojime do jedneho bloku
                                                    // a vlozime ich na zaciatok zoznamu vsetkych prazdnych blokov
    if (!isFree(previousBlock) && !isFree(nextBlock)) {
        insertAtBeginning(p);
    }
    else if (!isFree(previousBlock) && isFree(nextBlock)) {
        removeBlockFromList(nextBlock);
        mergeWithNextBlock(p);
        insertAtBeginning(p);
    }
    else if (isFree(previousBlock) && !isFree(nextBlock)) {
        if (previousBlock == heapStart) {
            insertAtBeginning(p);
        }
        else {
            removeBlockFromList(previousBlock);
            mergeWithPreviousBlock(p);
            insertAtBeginning(previousBlock);
        }
    }
    else {
        if (previousBlock == heapStart) {
            removeBlockFromList(nextBlock);
            mergeWithNextBlock(p);
            insertAtBeginning(p);
        }
        else {
            removeBlockFromList(nextBlock);
            removeBlockFromList(previousBlock);
            mergeWithNextBlock(p);
            mergeWithPreviousBlock(p);
            insertAtBeginning(previousBlock);
        }
    }

    return 0;
}

int memory_check(void* ptr) {
    unsigned short* blockHeader;
    blockHeader = (unsigned short*)heapPointer;
    blockHeader += 1;               // nastavime pointer blockHeader na hlavicku prveho bloku

    unsigned short* p;
    p = (unsigned short*)ptr;       // nastavime pointer p na hlavicku kontrolovaneho bloku
    p -= 1;

    while (blockHeader != p) {      // porovnavame adresy hlaviciek
        if (*blockHeader == 1)
            return 0;               // ak nenastane zhoda a prejdeme celu pamat, pointer ptr je neplatny
        blockHeader += (*blockHeader & -2) / 2;
    }

    return (*p & 1) ? 1 : 0;        // skontrolujeme ci dany blok je alokovany alebo nie
}

void memory_init(void* ptr, unsigned int size) {
    heapPointer = ptr;

    unsigned short memorySize;
    memorySize = (unsigned short)size;

    if (!isEven(memorySize)) {
        memorySize = roundDownToEven(memorySize);
    }

    *(unsigned short*)ptr = 1;                              // oznacime zaciatok a koniec vyhradenej casti pamate
    *((unsigned short*)ptr + memorySize / 2 - 1) = 1;

    *((unsigned short*)ptr + 1) = memorySize - 4;           // pridame header na zaciatok a koniec bloku
    *((unsigned short*)ptr + memorySize / 2 - 2) = memorySize - 4;

    *((unsigned short*)ptr + 2) = memorySize / 2 - 1;       // vytvorime pointer na next header a previous header
    *((unsigned short*)ptr + 3) = 0;
}

void insertAtBeginning(unsigned short* currentHeader) {
    unsigned short* heapStart;
    heapStart = (unsigned short*)heapPointer;

    *(currentHeader + 2) = 0;                   // pred vlozenim blokom je zaciatok pamate
    if (*(heapStart + *heapStart) != 1)         // presmerujeme previous pointer nasledujuceho bloku na vlozeny blok
        *(heapStart + *heapStart + 2) = currentHeader - heapStart;
    *(currentHeader + 1) = *heapStart;          // nastavime next pointer vlozeneho bloku na dalsi blok v poradi
    *heapStart = currentHeader - heapStart;     // aktualizujeme pointer na prvy volny blok
}

void removeBlockFromList(unsigned short* pointer) {
    unsigned short* heapStart;
    heapStart = (unsigned short*)heapPointer;

    if (*(pointer + 2) != 0)                    // presmerujeme next pointer predchadzajuceho bloku na nasledujuci blok
        *(heapStart + *(pointer + 2) + 1) = *(pointer + 1);
    else
        *heapStart = *(pointer + 1);

    if (*(heapStart + *(pointer + 1)) != 1)     // presmerujeme previous pointer nasledujuceho bloku na predchadzajuci blok
        *(heapStart + *(pointer + 1) + 2) = *(pointer + 2);
}

void mergeWithNextBlock(unsigned short* p) {
    unsigned short newSize = *p + *(p + *p / 2);    // nova velkost spojenych blokov
    *p = newSize;                                   // prepis prednej hlavicky
    p = p + newSize / 2 - 1;                        // posun na zadnu hlavicku
    *p = newSize;                                   // prepis zadnej hlavicky
}

void mergeWithPreviousBlock(unsigned short* p) {
    p -= 1;         // posun na zadnnu hlavicku predchadzajuceho bloku
    p -= *p / 2;    // posun na zadnu hlavicku bloku pred predchadzajucim blokom
    p += 1;         // posun na prednu hlavicku predchadzajuceho bloku
    mergeWithNextBlock(p);
}

unsigned short isFree(unsigned short* blockHeader) {
    return !(*blockHeader & 1);                     // funkcia zisti, ci dany blok je volny
}

unsigned short roundUpToEven(unsigned short number) {
    return isEven(number) ? number : number + 1;    // funkcia na zaokruhlenie cisla na najblizsie horne parne cislo
}

unsigned short roundDownToEven(unsigned short number) {
    if (isEven(number))
        return number;                              // funkcia na zaokruhlenie cisla na najblizsie dolne parne cislo
    else return number - 1;
}

unsigned short isEven(unsigned short number) {
    return number % 2 == 0 ? 1 : 0;                 // funkcia kontroluje, ci zadane cislo je parne alebo neparne
}
