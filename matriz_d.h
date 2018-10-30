#ifndef MATRIZ_D_H
#define MATRIZ_D_H

#include "elementos.h"
#include "matrix.h"

void asig_matriz_D(struct MATRIZ **matrix, struct REGISTRO_ELEMENTO **elementos, struct REGISTRO_NODO **indice, int num_v);
void asig_matriz_D_CCVS(struct MATRIZ **matrix, struct REGISTRO_ELEMENTO *elementos, int num_v);
void asig_matriz_D_K(struct MATRIZ **matrix, struct REGISTRO_ELEMENTO *elementos, int num_v, struct REGISTRO_ELEMENTO *eleme);

#endif // MATRIZ_D_H
