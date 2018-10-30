#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include "matrix.h"
#include "cadenas.h"
#include "elementos.h"

void asig_matriz_jV(struct MATRIZ **matrix, struct REGISTRO_ELEMENTO *elementos, int num_v)
{
    int i=1;
    int j=1;
    char numero[12];
    static struct REGISTRO_ELEMENTO *aux;

    aux=elementos; //Se para en el primero

    for(i=1; i <=num_v; i++){
        sprintf(numero, "i_%s",aux->elemento);
        asig_matriz_din(matrix, i, j,numero);
        aux=(struct REGISTRO_ELEMENTO *)aux->sig;
    }
}

void asig_matriz_joa(struct MATRIZ **matrix, struct REGISTRO_ELEMENTO *elementos, int num_ao, int num_v)
{
    int i=1;
    int j=1;
    char numero[12];
    static struct REGISTRO_ELEMENTO *aux;

    aux=elementos; //Se para en el primero

    for(i=1; i <=num_ao; i++){
        sprintf(numero, "i_%s",aux->elemento);
        asig_matriz_din(matrix, i+num_v, j,numero);
        aux=(struct REGISTRO_ELEMENTO *)aux->sig;
    }
}

void asig_matriz_jVCVS(struct MATRIZ **matrix, struct REGISTRO_ELEMENTO *elementos, int num_VCVS, int num_v)
{
    int i=1;
    int j=1;
    char numero[12];
    static struct REGISTRO_ELEMENTO *aux;

    aux=elementos; //Se para en el primero

    for(i=1; i <=num_VCVS; i++){
        sprintf(numero, "i_%s",aux->elemento);
        asig_matriz_din(matrix, i+num_v, j,numero);
        aux=(struct REGISTRO_ELEMENTO *)aux->sig;
    }
}

void asig_matriz_jCCCS(struct MATRIZ **matrix, struct REGISTRO_ELEMENTO *elementos, int num_CCCS, int num_v)
{
    int i=1;
    int j=1;
    char numero[12];
    static struct REGISTRO_ELEMENTO *aux;

    aux=elementos; //Se para en el primero

    for(i=1; i <=num_CCCS; i++){
        sprintf(numero, "ic_%s",aux->elemento);                 //ic=corriente por el control
        asig_matriz_din(matrix, i+num_v, j,numero);
        aux=(struct REGISTRO_ELEMENTO *)aux->sig;
    }
}

void asig_matriz_jCCVS(struct MATRIZ **matrix, struct REGISTRO_ELEMENTO *elementos, int num_CCVS, int num_v)
{
    int i=1;
    int j=1;
    char numero[12];
    static struct REGISTRO_ELEMENTO *aux;

    aux=elementos; //Se para en el primero

    for(i=1; i <=num_CCVS; i++){
        sprintf(numero, "ic_%s",aux->elemento);                 //ic=corriente por el control
        asig_matriz_din(matrix, num_v+2*(i-1)+1, j,numero);
        sprintf(numero, "is_%s",aux->elemento);                 //is=corriente por la fuente
        asig_matriz_din(matrix, num_v+2*(i-1)+2, j,numero);
        aux=(struct REGISTRO_ELEMENTO *)aux->sig;
    }
}

void asig_matriz_jN (struct MATRIZ **matrix, struct REGISTRO_ELEMENTO *elementos, int num_N, int num_v)
{
    int i=1;
    int j=1;
    char numero[12];
    static struct REGISTRO_ELEMENTO *aux;

    aux=elementos; //Se para en el primero

    for(i=1; i <=num_N; i++){
        sprintf(numero, "i2_%s",aux->elemento);
        asig_matriz_din(matrix, i+num_v, j,numero);
        aux=(struct REGISTRO_ELEMENTO *)aux->sig;
    }
}

void asig_matriz_jK (struct MATRIZ **matrix, struct REGISTRO_ELEMENTO *elementos, int num_K, int num_v)
{
    int i=1;
    int j=1;
    char numero[12];
    static struct REGISTRO_ELEMENTO *aux;

    aux=elementos; //Se para en el primero

    for(i=1; i <=num_K; i++){
        sprintf(numero, "ip_%s",aux->elemento);                 //ic=corriente por el primario
        asig_matriz_din(matrix, num_v+2*(i-1)+1, j,numero);
        sprintf(numero, "is_%s",aux->elemento);                 //is=corriente por el secundario
        asig_matriz_din(matrix, num_v+2*(i-1)+2, j,numero);
        aux=(struct REGISTRO_ELEMENTO *)aux->sig;
    }
}
