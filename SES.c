#include <stdio.h>
#include <string.h>
#include "decryption.h"
#include "encryption.h"

int main() {
    int nescritos = 0;
    unsigned char textoSin[34];
    strcpy3(textoSin, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    textoSin[33] = '\0';

    for (int i = 0; i < strlen(textoSin); i++) {
        fprintf(stderr, "%d ", (int) textoSin[i]);
    }

    fprintf(stderr, "---------------------------------------------------------------------\n");

    unsigned char *texto = encrypt("cascos", 6, textoSin);
    for (long unsigned int i = 0; i < strlen2(texto); i++) {
        fprintf(stderr, "%d ", (int) texto[i]);
        nescritos++;
    }
    fprintf(stderr, "nescritos: %d\n", nescritos);
    fprintf(stderr, "%s\n", texto);

    fprintf(stderr, "---------------------------------------------------------------------\n");

    unsigned char *newText = decrypt(texto, "cascos", 6);
    nescritos = 0;
    for (long unsigned int i = 0; i < strlen2(newText); i++) {
        fprintf(stderr, "%d ", (int) newText[i]);
        nescritos++;
    }
    fprintf(stderr, "nescritos: %d\n", nescritos);
    fprintf(stderr, "%s\n", newText);

    return 0;
}