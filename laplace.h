#ifndef LAPLACE_H
#define LAPLACE_H
#include "def.h"

void separa_en_fracciones_parciales(double pola[],double polb[],int grado_a,int grado_b,double AR[],double AI[],complejo raices_polb[]);
void division_larga(double pola[],double polb[],int N1,int N2,double G[],double A[]);
int compara(complejo aux1,complejo aux2);

int polrt(double xcof[],double cof[],int m,complejo root[]);

char *laplace_inv(double nu[], double de[], int gr_nu, int gr_de);
char *exponencial(double b,double K);
char *coseno_seno(double a,double b,double alfa,double beta);

#endif // LAPLACE_H
