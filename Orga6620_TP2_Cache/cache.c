//
// Created by agustin on 6/11/19.
//

#include "cache.h"

void init(){

    int i,j,k;
    /*inicializa memorias en 0*/
    Cache cache;
    MainMemory mainMemory;

    for(i=0; i<WAYS; i++){
        Way *aWayPointer = &cache.ways[i];
        for(j=0; j<SETS; j++){
            CacheBlock *aCacheBlockPointer = &aWayPointer->cacheBlocks[j];
            aCacheBlockPointer->counter=0;
            aCacheBlockPointer->dirtyBit=NO_DIRTY;
            aCacheBlockPointer->validateBit=NO_VALID;
            for(k=0; k<OFFSET ; k++){
                aCacheBlockPointer->blockByte[k]=0 /*value en 0*/;
            }
        }
    }

    for (i=0; i<MAIN_MEMORY_SIZE; i++) {
        mainMemory.blockByte[i]=0 /*value en 0*/;
    }


}