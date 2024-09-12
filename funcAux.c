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