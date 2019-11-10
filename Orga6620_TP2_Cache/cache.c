//
// Created by agustin on 6/11/19.
//

#include "cache.h"


Cache cache;

MainMemory mainMemory;

int main(){

    init();

}

void init(){

    int i,j,k;
    /*inicializa memorias en 0*/

    cache.misses = 0;
    cache.access = 0;

    for(i=0; i<WAYS; i++){
        Way *aWayPointer = &cache.ways[i];
        for(j=0; j<SETS; j++){
            CacheBlock *aCacheBlockPointer = &aWayPointer->cacheBlocks[j];
            aCacheBlockPointer->counter = 99; /**los bloques vacio son los primeros candidatos a ser reemplazados
 * esto hace que select oldest seleccione aquellos que estan vacios*/
            aCacheBlockPointer->dirtyBit = NO_DIRTY;
            aCacheBlockPointer->validateBit = NO_VALID;
            aCacheBlockPointer->Tag = -1;
            for(k=0; k<OFFSET ; k++){
                aCacheBlockPointer->blockByte[k]=0 /*value en 0*/;
            }
        }
    }

    for (i=0; i<MAIN_MEMORY_SIZE; i++) {
        mainMemory.blockByte[i]=0 /*value en 0*/;
    }


}

/*e.g. 2048 da el offset 0 y 2049 da el offset 1*/
unsigned int get_offset(unsigned int address){
    /*return = [2049 - ((2049/64=32 * 64) = 2048)] = 1*/
    return (address - ((address/OFFSET)*OFFSET));
}

/*conjunto = set*/
unsigned int find_set(unsigned int address){
    return (address/OFFSET) % (SETS);
}


unsigned int select_oldest(unsigned int setnum){

    int max = 0,way=0;

    for (int i; i < WAYS; i++) {
        int aCacheBlockCounter = cache.ways[i].cacheBlocks[setnum].counter;

        if(aCacheBlockCounter > max){
            max = aCacheBlockCounter;
            way = i;
        }
    }

    return way;

}

void read_tocache(unsigned int blocknum, unsigned int way, unsigned int set){

    char aByteCopyFromMainMemory;
    int memoryBlockBaseAddress = blocknum * OFFSET;

    CacheBlock *aCacheBlockPointer = &cache.ways[way].cacheBlocks[set];

    for (int i=0; i < OFFSET; i++){
        /*e.g. block num 32 = 2048 address -> me llevo de 2048 a 2111*/

        aByteCopyFromMainMemory = mainMemory.blockByte[(memoryBlockBaseAddress)+i];
        if(aCacheBlockPointer->dirtyBit == DIRTY){
            write_tomem(memoryBlockBaseAddress+i,aCacheBlockPointer->blockByte[i]);
        }/*Ya se que con preguntar una basta pero lo hace las 64 veces*/

        aCacheBlockPointer->blockByte[i]=aByteCopyFromMainMemory;
    }

    aCacheBlockPointer->Tag = memoryBlockBaseAddress;
    aCacheBlockPointer->dirtyBit = NO_DIRTY;
    aCacheBlockPointer->validateBit = VALID;
    aCacheBlockPointer->counter = 0;

}


unsigned char read_byte(unsigned int address){

    cache.access=+1;

    char value;
    unsigned int setnum,actualWay=0,offset,oldestWay;
    bool found = false;
    unsigned int blocknum = address / OFFSET; 
    unsigned int memoryBlockBaseAddress = (blocknum) * OFFSET;

    setnum = find_set(address);
    offset = get_offset(address);
    
    while(!found){
        CacheBlock *aCacheBlockPointer = &cache.ways[actualWay].cacheBlocks[setnum];
        if( aCacheBlockPointer->Tag == memoryBlockBaseAddress){
            printf("Read Hit");
            found = true;
            value = aCacheBlockPointer->blockByte[offset];
        }
        actualWay++;
    }

    if (!found){
        cache.misses=+1;
        printf("Read Miss");
        oldestWay = select_oldest(setnum);
        read_tocache(blocknum,oldestWay,setnum);
    }
    
    return value;
}

void write_byte (unsigned int address, unsigned char value){

    cache.access=+1;

    unsigned int setnum, offset, actualWay=0;
    bool found = false;
    unsigned int blocknum = address / OFFSET;
    unsigned int memoryBlockBaseAddress = (blocknum) * OFFSET;

    setnum = find_set(address);
    offset = get_offset(address);

    while(!found){

        CacheBlock *aCacheBlockPointer = &cache.ways[actualWay].cacheBlocks[setnum];
        if(aCacheBlockPointer->Tag == memoryBlockBaseAddress && aCacheBlockPointer->validateBit == VALID){
            printf("Write Hit");
            found = true;
            aCacheBlockPointer->blockByte[offset] = value;        
        }
        actualWay++;
    }
    
    /**Es identico que al read byte dado que al ser WA el write miss se comporta como un read miss*/
    if (!found){
        cache.misses=+1;
        printf("Write miss");
        unsigned int oldestWay = select_oldest(setnum);
        read_tocache(blocknum,oldestWay,setnum);
    }


}


void write_tomem(unsigned int address, unsigned char value){

    unsigned int memoryBlockBaseAddress = (address/OFFSET)*OFFSET;

    mainMemory.blockByte[memoryBlockBaseAddress]=value;

}

float get_miss_rate(){

    return cache.access/cache.misses;

}


/*TODO
 * Agregar el contador que suma 1 a cada bloque cuando termina el comando*/


