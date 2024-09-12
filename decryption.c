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

    int textlength = strlen2(text);
    unsigned char **encryptedText = (unsigned char **) malloc(0);
    int nsegments = 0;
    int keyChar = 0;

    for (int i = 0; i < textlength; i++) {
        int nescritos = 0;
        int nspaces = (key[keyChar] - meanKey > 0) ? (key[keyChar] - meanKey)%4 : (meanKey - keyChar)%4;

        encryptedText = (unsigned char **) realloc(encryptedText, sizeof(unsigned char *) * (nsegments + 1));
        encryptedText[nsegments] = (unsigned char *) malloc(keylength * (1 + nspaces));

        //bucle para ir separando los segmentos del texto sin saber la longitud. Tambien se puede hacer con un bucle con otra variable como j

        for (; i < textlength; i++) {
            if (nescritos == keylength * (1 + nspaces)) {
                i += (nspaces + 1);
                break;
            }

            encryptedText[nsegments][nescritos] = text[i];
            nescritos++;
        }

        nsegments++;
        keyChar++;
    }

    fprintf(stderr, "NSEGMENTS: %d\n", nsegments);

    //bucles para ordenar los segmentos

    int numOrder = nsegments - meanKey%nsegments;
    unsigned char **ordenador = (unsigned char **) malloc(sizeof(unsigned char *) * numOrder);
    int npuestos = 0;

    for (int i = 0; i < numOrder; i++) {
        ordenador[i] = encryptedText[i + numOrder%nsegments];
        encryptedText[i + numOrder%nsegments] = encryptedText[i];
    }

    unsigned char *last;
    for (int i = numOrder; i < nsegments; i++) {
        last = ordenador[npuestos%numOrder];
        ordenador[npuestos%numOrder] = encryptedText[i + numOrder%nsegments];
        encryptedText[i + numOrder%nsegments] = last;
    }

    //bucle para descifrar cada segmento

    unsigned char *decryptedText = (unsigned char *) malloc(nsegments * keylength + 1);

    for (int i = 0; i < nsegments; i++) {
        unsigned char *segment = decryptSegment(encryptedText[i], key[i%keylength], meanKey, keylength);
        strcpy2(decryptedText + i * keylength, segment);
        free(encryptedText[i]);
    }

    free(encryptedText);

    decryptedText[nsegments * keylength] = '\0';

    return decryptedText;
}

unsigned char *decryptSegment(unsigned char *segment, int keyChar, int meanKey, int keylength) {
    unsigned int lengthSegment = strlen2(segment);
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