#include <stdlib.h>

unsigned int strlen2(unsigned char *);

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
        encryptedText[nsegments] = (unsigned char *) malloc(keylength);

        //bucle para ir separando los segmentos del texto sin saber la longitud. Tambien se puede hacer con un bucle con otra variable como j

        for (; i < textlength; i += (nspaces + 1)) {
            if (nescritos == keylength) {
                i += (nspaces + 1);
                break;
            }

            encryptedText[nsegments][nescritos] = text[i];
            nescritos++;
        }

        nsegments++;
    }

    //bucles para ordenar los segmentos

    int numOrder = nsegments - meanKey%nsegments;
    unsigned char **ordenador = (unsigned char *) malloc(sizeof(unsigned char *) * numOrder);
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

    unsigned char *decryptedText = (unsigned char *) malloc(nsegments * keylength);

    for (int i = 0; i < nsegments; i++) {
        unsigned char *segment = decryptSegment(encryptedText[i], text[i%keylength], meanKey, keylength);
        strcpy2(decryptedText + i * keylength, segment);
        free(encryptedText[i]);
    }

    free(encryptedText);

    return decryptedText;
}

unsigned char *decryptSegment(segment, keyChar, meanKey, keylength) {
    return NULL;
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