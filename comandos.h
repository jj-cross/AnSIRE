#ifndef COMANDOS_H
#define COMANDOS_H
#include "elementos.h"
#include"matrix.h"

void eval_comandos(struct REGISTRO_ELEMENTO *comandos,
                   struct REGISTRO_ELEMENTO *fuente_tension,
                   struct REGISTRO_ELEMENTO *fuente_corriente,
                   struct MATRIZ **mat_x,
                   int num_nodos,
                   struct REGISTRO_ELEMENTO *elementos,
                   struct REGISTRO_ELEMENTO **indice_VCVS,
                   struct REGISTRO_ELEMENTO **indice_CCCS,
                   struct REGISTRO_ELEMENTO **indice_VCCS,
                   struct REGISTRO_ELEMENTO **indice_CCVS,
                   struct REGISTRO_ELEMENTO **indice_N,
                   struct REGISTRO_ELEMENTO **indice_K,
                   FILE *salida);

char *ft1(struct REGISTRO_ELEMENTO *fuente_tension, struct REGISTRO_ELEMENTO *fuente_corriente, char *expresion, char* entrada);
char *transitorio(char *expresion, char *entrada);
int busca_tension (struct MATRIZ **mat_x, int num_nodos,char *nodo);
char *busca_impedancia(char *tem, struct REGISTRO_ELEMENTO *elemento);
int com_numerico (struct REGISTRO_ELEMENTO *elemento,
                  struct REGISTRO_ELEMENTO **indice_VCVS,
                  struct REGISTRO_ELEMENTO **indice_CCCS,
                  struct REGISTRO_ELEMENTO **indice_VCCS,
                  struct REGISTRO_ELEMENTO **indice_CCVS,
                  struct REGISTRO_ELEMENTO **indice_N,
                  struct REGISTRO_ELEMENTO **indice_K);
int com_numerico1 (struct REGISTRO_ELEMENTO *elemento);
char *evals(char *expresion, char* entrada);
void guardar_tiempo(int t_ini, double t_fin, double delta_t, double *x, double *y, char nombret[]);
char *fun_respuesta_tiempo(char *expresion);
void coef_pol_pot (char expresion[], double *coeficiente, int *exponente);
int grados (double *apold);

#endif // COMANDOS_H
