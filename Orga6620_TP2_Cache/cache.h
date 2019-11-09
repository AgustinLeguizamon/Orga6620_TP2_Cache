//
// Created by agustin on 6/11/19.
//

#ifndef ORGA6620_TP2_CACHE_CACHE_H
#define ORGA6620_TP2_CACHE_CACHE_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "cache_constants.h"


typedef struct CacheBlock{

    char blockByte[OFFSET]; /*guarda el value*/
    char validateBit;
    char dirtyBit;
    char counter;
    int Tag; /*El tag guarda el address base del bloque de memoria principal que es unico e irrepetible y sera el mismo para los 64*/

}CacheBlock;

typedef struct Way{
    CacheBlock cacheBlocks[SETS];
}Way;

typedef struct Cache{
    float access;
    float misses;
    Way ways[WAYS];
}Cache;

typedef struct MainMemory{
    char blockByte [MAIN_MEMORY_SIZE]; /*guarda el value*/
}MainMemory;


/*Funciones catedra*/

void init();

unsigned int get_offset (unsigned int address);

unsigned int find_set(unsigned int address);

unsigned int select_oldest(unsigned int setnum);

/*leer de memoria principal y guardarlo en el conjunto(set) y via(way) en memoria cache*/
void read_tocache(unsigned int blocknum, unsigned int way, unsigned int set);

/*Lee cache y carga de memoria ante un read miss*/
unsigned char read_byte(unsigned int address);

/*Escribe en cache*/
void write_byte (unsigned int address, unsigned char value);

/*Escribe en memoria principal*/
void write_tomem(unsigned int address, unsigned char value);

float get_miss_rate();


#endif //ORGA6620_TP2_CACHE_CACHE_H
