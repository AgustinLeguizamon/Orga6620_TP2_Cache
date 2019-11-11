//
// Created by agustin on 10/11/19.
//

#ifndef ORGA6620_TP2_CACHE_FILEREADER_H
#define ORGA6620_TP2_CACHE_FILEREADER_H

#include <stdio.h>
#include <string.h>

typedef struct fileReader {
    FILE* file;
    char currentCommand;
}fileReader;

void fileReaderInit(struct fileReader *self, const char* fileName);

//PRE: el char* destino debe tener por lo menos tamanio 3

void fileReaderGetNextCommand(struct fileReader *self, char destiny[]);

void fileReaderGetAddress(struct fileReader *self, char destiny[]);

void fileReaderGetValue(struct fileReader *self, char destiny[]);

void fileReaderRelease(struct fileReader *self);

/*Agustin*/

void readCommand(struct fileReader *self);

void writeCommand(struct fileReader *self);

#endif //ORGA6620_TP2_CACHE_FILEREADER_H
