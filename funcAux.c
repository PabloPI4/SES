#include <stdio.h>

unsigned int strlen2(unsigned char *text) {
    register unsigned char *s;

    for (s = text; *s; ++s);
	return(s - text);
}

void strcpy2(unsigned char *dest, unsigned char *src) {
    int num = 0;

    while (src[num] != '\0') {
        dest[num] = src[num];
        num++;
    }
}

void strncpy2(unsigned char *dest, unsigned char *src, int n) {
    int count = 0;

    while (src[count] != '\0' && count != n) {
        fprintf(stderr, "char es: %d\n", src[count]);
        dest[count] = src[count];
        count++;
    }
}

void strcpy3(unsigned char *dest, char *src) {
    int num = 0;

    while (src[num] != '\0') {
        dest[num] = src[num];
        num++;
    }
}