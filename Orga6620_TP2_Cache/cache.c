//
// Created by agustin on 6/11/19.
//

#include "cache.h"
#include "fileReader.h"

Cache cache;

MainMemory mainMemory;

int main() {

    fileReader fileReader;

    init();

    struct fileReader *fileReaderPointer = &fileReader;

    /*Cambio master*/

    /*TODO
     * Pregunta: Hay que iterar las 5 pruebas ?
     * e.g ver a continuacion"
     * */

    char baseFileName[6] = "prueba";
    strcat(baseFileName, "1");
    printf("%s \n",baseFileName);

    /**/

    fileReaderInit(fileReaderPointer, "/home/agustin/Documents/Orga6620/tp2/prueba1.mem" );

    char command[5];

    /*TODO
     * Hardcodeado: Arreglar para que Itere hasta el final del file
     * literal cuento las cantidad de comandos y cambio fileLines*/

    /*prueba1: 19
     *prueba2: 7
     *prueba3: 18
     *prueba4: 17
     * prueba5: 8
     * */
    int fileLines = 0;
    while(fileLines < 19){

        fileReaderGetNextCommand(fileReaderPointer,command);
        printf("%c \t", command[0]);
        switch (command[0]) {
            case 'W':
                writeCommand(fileReaderPointer);
                break;

            case 'R':
                readCommand(fileReaderPointer);
                break;
        }

        if(command[0] == 'M' && command[1] == 'R'){
            printf("\n Miss rate:%f \n", get_miss_rate());
        }


        /*TODO
         * dudoso: Agregar el contador que suma 1 a cada bloque cuando termina el comando*/

        for(int k=0; k<WAYS; k++){
            Way *aWayPointer = &cache.ways[k];
            for(int l=0; l<SETS; l++){
                CacheBlock *aCacheBlockPointer = &aWayPointer->cacheBlocks[l];
                aCacheBlockPointer->counter +=1;
            }
        }

        printf("*********************** \n \n");
        fileLines++;
    }

    fileReaderRelease(fileReaderPointer);

    return 0;
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

    for (int i=0; i < WAYS; i++) {
        int aCacheBlockCounter = cache.ways[i].cacheBlocks[setnum].counter;

        if(aCacheBlockCounter > max){
            max = aCacheBlockCounter;
            way = i;
        }
    }

    return way;

}

void read_tocache(unsigned int blocknum, unsigned int way, unsigned int set){

    unsigned char aByteCopyFromMainMemory;
    int memoryBlockBaseAddress = blocknum * OFFSET;

    CacheBlock *aCacheBlockPointer = &cache.ways[way].cacheBlocks[set];
    bool isDirty = (aCacheBlockPointer->dirtyBit == DIRTY);

    for (int i=0; i < OFFSET; i++){
        /*e.g. block num 32 = 2048 address -> me llevo de 2048 a 2111*/

        aByteCopyFromMainMemory = mainMemory.blockByte[(memoryBlockBaseAddress)+i];
        if(isDirty){
            write_tomem(aCacheBlockPointer->Tag+i,aCacheBlockPointer->blockByte[i]);
        }/*Ya se que con preguntar una basta pero lo hace las 64 veces para evitar iterar de nuevo*/

        aCacheBlockPointer->blockByte[i]=aByteCopyFromMainMemory;
    }

    aCacheBlockPointer->Tag = memoryBlockBaseAddress;
    aCacheBlockPointer->dirtyBit = NO_DIRTY;
    aCacheBlockPointer->validateBit = VALID;
    aCacheBlockPointer->counter = 0;

}


unsigned char read_byte(unsigned int address){

    cache.access+=1;

    unsigned char value=0;
    unsigned int setnum,actualWay=0,offset,oldestWay;
    bool found = false;
    unsigned int blocknum = address / OFFSET; 
    unsigned int memoryBlockBaseAddress = (blocknum) * OFFSET;

    setnum = find_set(address);
    offset = get_offset(address);
    
    while(!found && actualWay < WAYS){
        CacheBlock *aCacheBlockPointer = &cache.ways[actualWay].cacheBlocks[setnum];
        if( aCacheBlockPointer->Tag == memoryBlockBaseAddress){
            printf("Read Hit in way:%d, set:%d, offset:%d \n",actualWay, setnum, offset);
            found = true;
            value = aCacheBlockPointer->blockByte[offset];
        }
        actualWay++;
    }

    if (!found){
        cache.misses+=1;
        printf("Read Miss \n");
        oldestWay = select_oldest(setnum);
        read_tocache(blocknum,oldestWay,setnum);
        value = mainMemory.blockByte[address];
        printf("Writing in oldest or empty Way:%d, Set:%d, offset:%d \n",oldestWay, setnum, offset);
    }
    
    return value;
}

void write_byte (unsigned int address, unsigned char value){

    cache.access += 1;

    unsigned int setnum, offset, actualWay=0;
    bool found = false;
    unsigned int blocknum = address / OFFSET;
    unsigned int memoryBlockBaseAddress = (blocknum) * OFFSET;

    setnum = find_set(address);
    offset = get_offset(address);

    while(!found && actualWay < WAYS ){

        CacheBlock *aCacheBlockPointer = &cache.ways[actualWay].cacheBlocks[setnum];
        if(aCacheBlockPointer->Tag == memoryBlockBaseAddress && aCacheBlockPointer->validateBit == VALID){
            printf("Write Hit in way:%d and set:%d \n",actualWay, setnum);
            printf("Writing value: %u in offset: %d \n",value ,offset);
            found = true;
            aCacheBlockPointer->blockByte[offset] = value;
            aCacheBlockPointer->dirtyBit=DIRTY;
        }
        actualWay++;
    }
    
    /**Es identico que al read byte dado que al ser WA el write miss se comporta como un read miss*/
    if (!found){
        cache.misses += 1;
        printf("Write Miss \n");
        unsigned int oldestWay = select_oldest(setnum);
        read_tocache(blocknum,oldestWay,setnum); //Trae bloque mem a cache
        cache.ways[oldestWay].cacheBlocks[setnum].blockByte[offset] = value; //Escribe en el bloque
        printf("Writing value:%u in oldest or empty Way:%d, Set:%d, offset:%d \n",value, oldestWay, setnum, offset);
    }


}


void write_tomem(unsigned int address, unsigned char value){

    mainMemory.blockByte[address] = value;

}

float get_miss_rate(){

    return cache.misses/cache.access;

}



void writeCommand(struct fileReader *self){

    char address[5], value[5], addressWOComma[5];
    int i=0;

    unsigned int memAddress;
    unsigned int byteValue;

    fileReaderGetAddress(self, address);
    fileReaderGetValue(self, value);

    /*La coma jode al fscanf asi que itero el array hasta la coma*/
    while (address[i] != ',') {
        addressWOComma[i] = address[i];
        i++;
    }

    memAddress = atoi(addressWOComma);
    printf("%d \t", memAddress);
    byteValue = atoi(value);
    if(memAddress > MAIN_MEMORY_SIZE || memAddress < 0){
        printf("Error, la direccion: %d supera el tamaño de la memoria principal  \n", memAddress);
    }
    else if (byteValue > MAX_CHAR){
        printf("Error el valor: %d supera el tamaño de un byte  \n", byteValue);
    }
    else{
        unsigned char byteValue_c = byteValue;
        printf("%u \n", byteValue_c);
        write_byte(memAddress, byteValue_c);
    }


}

void readCommand(struct fileReader *self) {

    char address[5];
    unsigned int memAddress;

    fileReaderGetAddress(self, address);
    memAddress = atoi(address);
    if(memAddress > MAIN_MEMORY_SIZE || memAddress < 0){
        printf("Error, la direccion: %d supera el tamaño de memoria principal \n", memAddress);
    }
    else{
        printf("%d \n", memAddress);
        printf("Value: %u \n",read_byte(memAddress));
    }

}


