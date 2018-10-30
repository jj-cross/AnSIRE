#ifndef CADENAS_H
#define CADENAS_H
#include <stdio.h>

#define MAX_LONG_LINEA 5000

void obtener_linea(char *linea, FILE *input);
char *sub_cad(char *s1, char *s2, int cont);
void eliminar_espacios(char *cad, char *cad2);

#endif // CADENAS_H
