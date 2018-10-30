#ifndef MATRIX_H
#define MATRIX_H

#include "cadenas.h"
#include "elementos.h"
#include "matrix.h"

#define TAMANO  MAX_LONG_LINEA

struct  MATRIZ{
        char expresion[TAMANO];      // expresion 1
        char otro_dato[MAX_LONG_LINEA];      // expresion 2
};

void ini_matrix_simb(struct MATRIZ **matrix, int f, int c);
struct MATRIZ **crear_matriz_din(struct MATRIZ **matrix, int f, int c);
void asig_matriz_din(struct MATRIZ **matrix, int i, int j, char *valor);
char * leer_ele_matriz(struct MATRIZ **matrix, int i, int j);
void imprime_matrix(char *titulo, struct MATRIZ **matrix, int f, int c);
void imprime_matrix_arch(char *titulo, struct MATRIZ **matrix, int f, int c, FILE *salida);

void asig_matriz_A(struct MATRIZ **A , struct MATRIZ **G, struct MATRIZ **B, struct MATRIZ **C, struct MATRIZ **D, int num_nodos, int num_v);
void asig_matriz_x(struct MATRIZ **x , struct MATRIZ **v, struct MATRIZ **j, int num_nodos, int num_v);
void asig_matriz_G(struct MATRIZ **matrix, int f, int c, struct REGISTRO_ELEMENTO *elementos, struct REGISTRO_NODO **indice, struct REGISTRO_ELEMENTO *K);
void asig_matriz_e(struct MATRIZ **matrix, struct REGISTRO_ELEMENTO *elementos, int num_v);
void asig_matriz_z(struct MATRIZ **z , struct MATRIZ **i, struct MATRIZ **e, int num_nodos, int num_v);
void asig_matriz_gauss(struct MATRIZ **matrix , struct MATRIZ **A , struct MATRIZ **z, int num_nodos);
void asig_matriz_gauss1(struct MATRIZ **matrix ,struct MATRIZ **A, int num_nodos);

void asig_matriz_v(struct MATRIZ **matrix, struct REGISTRO_NODO **indice, int num_nodos);
void asig_matriz_i(struct MATRIZ **matrix, struct REGISTRO_ELEMENTO *elemento, struct REGISTRO_NODO **indice);
void asig_matriz_VCCS(struct MATRIZ **matrix, struct REGISTRO_ELEMENTO *elementos, struct REGISTRO_NODO **indice);

#endif // MATRIX_H
