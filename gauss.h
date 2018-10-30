#ifndef GAUSS_H
#define GAUSS_H
#include "elementos.h"
#include "matrix.h"
#include "cadenas.h"

//struct MATRIZ **gauss(MATRIZ **matrix, struct MATRIZ **mat_gauss, int num_nodos);
struct MATRIZ **gauss(struct MATRIZ **matx, struct MATRIZ **mat_gauss, int num_nodos, FILE *salida);
void gauss1(struct MATRIZ **mat_gauss, int num_nodos, FILE *salida);
void squeeze(char s[], char c);
void elimina_factor_comun(struct MATRIZ **mat_gauss, int var);

#endif // GAUSS_H
