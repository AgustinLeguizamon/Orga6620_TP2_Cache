//
// Created by agustin on 6/11/19.
//

#ifndef ORGA6620_TP2_CACHE_CACHE_H
#define ORGA6620_TP2_CACHE_CACHE_H

#include <stdint.h>
#include "cache_constants.h"

const char CACHE [CACHE_SIZE];

/*Funciones catedra*/

void init();

unsigned int get_offset (unsigned int address);

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

/*Funciones TP*/


#endif //ORGA6620_TP2_CACHE_CACHE_H
