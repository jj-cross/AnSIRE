#ifndef MATRIZ_C_H
#define MATRIZ_C_H

#include "elementos.h"
#include "matrix.h"

void asig_matriz_CV(struct MATRIZ **matrix, struct REGISTRO_ELEMENTO *elementos,struct REGISTRO_NODO **indice);
void asig_matriz_CI(struct MATRIZ **matrix, struct REGISTRO_ELEMENTO *elementos,struct REGISTRO_NODO **indice, int num_v);
void asig_matriz_Coa(struct MATRIZ **matrix, struct REGISTRO_ELEMENTO *elementos, struct REGISTRO_NODO **indice, int num_v);
void asig_matriz_CVCVS(struct MATRIZ **matrix, struct REGISTRO_ELEMENTO *elementos, struct REGISTRO_NODO **indice, int num_v);
void asig_matriz_CCCCS(struct MATRIZ **matrix, struct REGISTRO_ELEMENTO *elementos, struct REGISTRO_NODO **indice,  int num_v);
void asig_matriz_CCCVS(struct MATRIZ **matrix, struct REGISTRO_ELEMENTO *elementos, struct REGISTRO_NODO **indice,  int num_v);
void asig_matriz_CN(struct MATRIZ **matrix, struct REGISTRO_ELEMENTO *elementos, struct REGISTRO_NODO **indice, int num_v);
void asig_matriz_CK(struct MATRIZ **matrix, struct REGISTRO_ELEMENTO *elementos, struct REGISTRO_NODO **indice,  int num_v, struct REGISTRO_ELEMENTO *eleme);

#endif // MATRIZ_C_H
