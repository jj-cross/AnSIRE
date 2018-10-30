#ifndef MATRIX_B_H
#define MATRIX_B_H

#include "elementos.h"
#include "matrix.h"

void asig_matriz_BV(struct MATRIZ **matrix, struct REGISTRO_ELEMENTO *elementos, struct REGISTRO_NODO **indice);
void asig_matriz_BC(struct MATRIZ **matrix, struct REGISTRO_ELEMENTO *elementos, struct REGISTRO_NODO **indice, int num_v);
void asig_matriz_Boa(struct MATRIZ **matrix, struct REGISTRO_ELEMENTO *elementos, struct REGISTRO_NODO **indice, int num_v);
void asig_matriz_BVCVS(struct MATRIZ **matrix, struct REGISTRO_ELEMENTO *elementos, struct REGISTRO_NODO **indice,  int num_v);
void asig_matriz_BCCCS(struct MATRIZ **matrix, struct REGISTRO_ELEMENTO *elementos, struct REGISTRO_NODO **indice, int num_v);
void asig_matriz_BCCVS(struct MATRIZ **matrix, struct REGISTRO_ELEMENTO *elementos, struct REGISTRO_NODO **indice, int num_v, int num_nodos);
void asig_matriz_BN(struct MATRIZ **matrix, struct REGISTRO_ELEMENTO *elementos, struct REGISTRO_NODO **indice, int num_v);
void asig_matriz_BK(struct MATRIZ **matrix, struct REGISTRO_ELEMENTO *elementos, struct REGISTRO_NODO **indice, int num_v,struct REGISTRO_ELEMENTO *eleme);
#endif // MATRIX_B_H
