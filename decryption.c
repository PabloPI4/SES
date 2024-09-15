#include <stdlib.h>
#include <stdio.h>
#include "decryption.h"
#include "funcAux.h"

unsigned char *decrypt(unsigned char *text, char *key, int keylength) {
    int meanKey = 0;

    for (int i = 0; i < keylength; i++) {
        meanKey += (int)*(key + i);
    }
    meanKey /= keylength;
    int numOrder = meanKey%keylength;

    int textlength = strlen2(text);
    unsigned char **encryptedText = (unsigned char **) calloc(0, 1);
    int nsegments = 0;
    int keyChar = 0;

    for (int i = 0; i < textlength; i++) {
        //EL ERROR CONSISTE EN UN ERROR DE ROTACION DE LOS SEGMENTOS
        //CUANDO SE ELIGE EL KEYCHAR DE CADA SEGMENTO SE ELIGE MAL
        //NO CREO QUE SEA POSIBLE EL DESCIFRADO DEL TEXTO
        keyChar = keylength - numOrder + nsegments;
        int nspaces = (key[keyChar%keylength] - meanKey > 0) ? (key[keyChar%keylength] - meanKey)%4 : (meanKey - key[keyChar%keylength])%4;
        fprintf(stderr, "nspaces: %d\n", nspaces);

        encryptedText = (unsigned char **) realloc(encryptedText, sizeof(unsigned char *) * (nsegments + 1));
        encryptedText[nsegments] = (unsigned char *) calloc(keylength * (1 + nspaces) + 1, 1);

        //bucle para ir separando los segmentos del texto sin saber la longitud. Tambien se puede hacer con un bucle con otra variable como j

        strncpy2(encryptedText[nsegments], text + i, keylength * (1 + nspaces));

        i += keylength * (1 + nspaces) - 1;
        encryptedText[nsegments][keylength * (1 + nspaces)] = '\0';
        nsegments++;
        keyChar++;
    }

    fprintf(stderr, "NSEGMENTS: %d\n", nsegments);

    //bucle para descifrar cada segmento

    unsigned char *decryptedText = (unsigned char *) malloc(nsegments * keylength + 1);

    for (int i = 0; i < nsegments; i++) {
        fprintf(stderr, "longitud del segmento enc: %d\n", strlen2(encryptedText[(i + numOrder)%nsegments]));
        unsigned char *segment = decryptSegment(encryptedText[(i + numOrder) % nsegments], key[i%keylength], meanKey, keylength);
        strcpy2(decryptedText + i * keylength, encryptedText[i]);
        free(encryptedText[i]);
    }

    free(encryptedText);

    decryptedText[nsegments * keylength] = '\0';

    return decryptedText;
}

unsigned char *decryptSegment(unsigned char *segment, int keyChar, int meanKey, int keylength) {
    unsigned int lengthSegment = strlen2(segment);
    fprintf(stderr, "long: %d\n", lengthSegment);
    int nrotationsSegment = (keyChar / keylength);
    int rotationsLogic = ((keyChar - meanKey) > 0) ? (keyChar - meanKey)%7 + 1 : (meanKey - keyChar)%7+ 1;
    int nspaces = (keyChar - meanKey > 0) ? (keyChar - meanKey)%4 : (meanKey - keyChar)%4;

    int numrot = 0;
    unsigned char last = segment[0];
    unsigned char next;

    do {
        int newNum = (numrot - nrotationsSegment) % lengthSegment;
        next = segment[newNum];
        segment[newNum] = last;
        last = next;
        numrot = newNum;
    } while (numrot != 0);

    int nmetidos = 0;
    unsigned char *newSegment = (unsigned char *) malloc(keylength);

    for (int i = 0; i < lengthSegment; i += nspaces + 1) {
        int newChar = (int) segment[i];
        newSegment[nmetidos] = ((int) segment[i] >> (8 - rotationsLogic)) | (newChar << rotationsLogic);
        nmetidos++;
    }

    return newSegment;
}