#include <stdio.h>
#include "cache.h"

#define MAXCHAR 100

int main() {

    /*void init()*/

    FILE *fp;
    char str[MAXCHAR];
    char* filename = "/home/agustin/Documents/Orga6620/tp2/prueba1.mem";

    fp = fopen(filename, "r");

    if (fp == NULL){
        printf("Could not open file %s",filename);
        return 1;
    }


    /*ver si char* funciona
     * ver pq carajo tengo basura en las lecturas*/
    while( fgets(str, MAXCHAR, fp) != NULL){

        /* [0]=R
         * [1]=ddddd
         * [2]=vvv */

        char* textLineCommands[3];

        /**O tal vez mejor primero leer el primer argumento y a partir de ahi decidir
         * e.g si leo R, se que los proximos 5 son address (ddddd)*/
        int i=0;
        for(int j=0; j<3; j++) {
            while (str[i] != ' '  && str[i] != '\0') {
                if(str[i] != ','){
                    textLineCommands[j] = textLineCommands[j] + str[i];
                    printf("%c", str[i]);
                }
                i++;
            }
            i++;
        }
        printf("\n");

    } //str tiene la linea W 2048, 048

    fclose(fp);

    return 0;
}