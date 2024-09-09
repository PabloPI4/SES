#include <stdlib.h>
#include <string.h>
#include <stdio.h>

unsigned char *encryptSegment(unsigned char *, int, int, int);
unsigned char *encrypt(char *, int, unsigned char *);
unsigned int strlen2(unsigned char *);
void strcpy2(unsigned char *, unsigned char *);
void strcpy3(unsigned char *, char *);

int main() {
    int nescritos = 0;
    unsigned char *textoSin = (unsigned char *) malloc(34);
    strcpy3(textoSin, "Me gustan los platanos amarillos");
    textoSin[33] = '\0';
    unsigned char *texto = encrypt("cascos", 6, textoSin);
    for (long unsigned int i = 0; i < strlen2(texto); i++) {
        fprintf(stderr, "%d\n", (int) texto[i]);
        nescritos++;
    }
    fprintf(stderr, "nescritos: %d\n", nescritos);
    fprintf(stderr, "%s\n", texto);

    return 0;
}

unsigned char *encrypt(char *key, int keylength, unsigned char *text) {
    int textlength = strlen2(text);
    int encryptedTextlength = 0;
    int meanKey = 0;

    for (int i = 0; i < keylength; i++) {
        meanKey += (int)*(key + i);
    }
    meanKey /= keylength;

    int reps = (textlength % keylength > 0) ? (textlength / keylength) + 1: (textlength / keylength);
    unsigned char **encryptedSegmentedText = (unsigned char **) malloc(sizeof(unsigned char *) * reps);

    for (int i = 0; i < reps; i++) {
        unsigned char *segment = encryptSegment(text + i*keylength, key[i%keylength], meanKey, keylength);
        int lenSegment = strlen2(segment);

        encryptedSegmentedText[i] = (unsigned char *) malloc(lenSegment + 1);
        strcpy2(encryptedSegmentedText[i], segment);
        encryptedSegmentedText[i][lenSegment] = '\0';
        encryptedTextlength += lenSegment;
    }

    unsigned char *encryptedText = (unsigned char *) malloc(encryptedTextlength + 1);
    encryptedTextlength = 0;
    int numOrder = reps - meanKey%reps;

    for (int i = 0; i < reps; i++) {
        unsigned char *segment = encryptedSegmentedText[(i+numOrder)%reps];
        strcpy2(encryptedText + encryptedTextlength, segment);
        encryptedTextlength += strlen2(segment);
        free(segment);
    }
    encryptedText[encryptedTextlength] = '\0';

    free(encryptedSegmentedText);

    return encryptedText;
}

unsigned char *encryptSegment(unsigned char *segment, int keyChar, int meanKey, int keylength) {
    if (strlen2(segment) < (unsigned int)keylength) {
        keylength = strlen2(segment);
    }

    /*Se calcula el numero de espacios entre caracteres y de rotaciones en el segmento que van a haber*/
    int nspaces = (keyChar - meanKey > 0) ? (keyChar - meanKey)%4 : (meanKey - keyChar)%4;
    int nrotationsSegment = (keyChar / keylength);
    /*Se calcula el numero de rotaciones logicas que se le aplicara a cada caracter*/
    int rotations = ((keyChar - meanKey) > 0) ? (keyChar - meanKey)%7 + 1 : (meanKey - keyChar)%7+ 1;

    unsigned char *encryptedText = (unsigned char *) malloc(keylength * (1+nspaces) + 1);

    for (int i = 0; i < keylength; i++) {
        int newChar = (int) segment[i];
        encryptedText[i * (nspaces + 1)] = ((int) segment[i] >> rotations) | (newChar << (8 - rotations));
    }

    unsigned char space;
    for (int i = 0; i < keylength; i++) {
        for (int j = 1; j < nspaces + 1; j++) {
            space = (unsigned char) ((rand() % 255) + 1);
            encryptedText[i*(nspaces + 1) + j] = space;
        }
    }

    int numrot = 0;
    char rotcharLast = encryptedText[0];
    char rotcharNext;

    do {
        numrot = (numrot + nrotationsSegment) % (keylength * (1+nspaces));
        rotcharNext = encryptedText[numrot];
        encryptedText[numrot] = rotcharLast;
        rotcharLast = rotcharNext;
    } while (numrot != 0);
    encryptedText[numrot + nrotationsSegment] = rotcharLast;
    encryptedText[keylength * (1+nspaces)] = '\0';

    return encryptedText;
}



unsigned int strlen2(unsigned char *text) {
    unsigned int num = 0;

    while (text[num] != '\0') {
        num++;
    }

    return num;
}

void strcpy2(unsigned char *dest, unsigned char *src) {
    int num = 0;

    while (src[num] != '\0') {
        dest[num] = src[num];
        num++;
    }
}

void strcpy3(unsigned char *dest, char *src) {
    int num = 0;

    while (src[num] != '\0') {
        dest[num] = src[num];
        num++;
    }
}