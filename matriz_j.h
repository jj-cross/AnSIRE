#ifndef MATRIZ_J_H
#define MATRIZ_J_H

#include "elementos.h"
#include "matrix.h"

void asig_matriz_jV   (struct MATRIZ **matrix, struct REGISTRO_ELEMENTO *elementos, int num_v              );
void asig_matriz_joa  (struct MATRIZ **matrix, struct REGISTRO_ELEMENTO *elementos, int num_ao  , int num_v);
void asig_matriz_jVCVS(struct MATRIZ **matrix, struct REGISTRO_ELEMENTO *elementos, int num_VCVS, int num_v);
void asig_matriz_jCCCS(struct MATRIZ **matrix, struct REGISTRO_ELEMENTO *elementos, int num_CCCS, int num_v);
void asig_matriz_jCCVS(struct MATRIZ **matrix, struct REGISTRO_ELEMENTO *elementos, int num_CCVS, int num_v);
void asig_matriz_jN   (struct MATRIZ **matrix, struct REGISTRO_ELEMENTO *elementos, int num_N   , int num_v);
void asig_matriz_jK   (struct MATRIZ **matrix, struct REGISTRO_ELEMENTO *elementos, int num_CCVS, int num_v);

#endif // MATRIZ_J_H
