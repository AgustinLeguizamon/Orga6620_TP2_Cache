#ifndef _FILEREADER_H_
#define _FILEREADER_H_

#include <stdio.h>
#include <string.h>

typedef struct fileReader {
    FILE* file;
    char currentCommand;
};

void fileReaderInit(struct fileReader *self, const char* fileName);

//PRE: el char* destino debe tener por lo menos tamanio 3

void fileReaderGetNextCommand(struct fileReader *self, char *destiny);

void fileReaderGetAddress(struct fileReader *self, unsigned int *destiny);

void fileReaderGetValue(struct fileReader *self, unsigned char *destiny);

void fileReaderRelease(struct fileReader *self);

#endif