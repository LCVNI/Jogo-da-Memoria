#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utilidades.h"

void lerLinha(char *buffer, int tamanho) {
    fgets(buffer, tamanho, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
}

int lerInt() {
    char buffer[100];
    lerLinha(buffer, sizeof(buffer));
    return atoi(buffer);
}

void pausar() {
    printf("\nPressione ENTER para continuar...");
    getchar();
}

int letraParaIndice(char c) {
    if (c >= 'a' && c <= 'z') return c - 'a';
    if (c >= 'A' && c <= 'Z') return c - 'A';
    return -1;
}