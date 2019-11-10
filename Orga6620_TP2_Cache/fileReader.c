#include "fileReader.h"

void fileReaderInit(struct fileReader *self, const char *fileName) {
    self -> file = fopen(fileName, "r");
}

void fileReaderGetNextCommand(struct fileReader *self, char *destiny) {
    fscanf(self -> file, "%s", destiny);
}

void fileReaderGetAddress(struct fileReader *self, unsigned int *destiny) {
    fscanf(self -> file, "%d", destiny);
}

void fileReaderGetValue(struct fileReader *self, unsigned char *destiny) {
    fscanf(self -> file, "%s", destiny);
}

void fileReaderRelease(struct fileReader *self) {
    fclose(self -> file);
}
