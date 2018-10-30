#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include "matrix.h"
#include "cadenas.h"
#include "elementos.h"

//void asig_matriz_D_CCVS(struct MATRIZ **matrix, struct REGISTRO_ELEMENTO *elementos, struct REGISTRO_NODO **indice, int num_v)
void asig_matriz_D_CCVS(struct MATRIZ **matrix, struct REGISTRO_ELEMENTO *elementos, int num_v)
{
    int i=num_v+1;
    static struct REGISTRO_ELEMENTO *aux;
    int x,y;
    char val_actual [50],val_nuevo [50];
    char numero[16];

    if(elementos==NULL)
        return;

    aux=elementos; //Se para en el primero

    do{
        strcpy(val_actual,(leer_ele_matriz(matrix,i, i)));

        if (strcmp(val_actual,"0")==0)
            strcpy(val_actual,"");
        strcpy(val_nuevo,(aux->valor));
        sprintf(numero, "%s-%s",val_actual,val_nuevo);
        x=i;
        y=i;
        asig_matriz_din(matrix, y, x,numero);
        //imprime_matrix("Matriz B 2",matrix,num_nodos,num_v+1);

        if (aux->sig != NULL)
        {
            aux=(struct REGISTRO_ELEMENTO *)aux->sig;
            i=i+2;
        }
    }while(aux->sig !=NULL );

    strcpy(val_actual,(leer_ele_matriz(matrix,i, i)));

    if (strcmp(val_actual,"0")==0)
        strcpy(val_actual,"");
    strcpy(val_nuevo,(aux->valor));
    sprintf(numero, "%s-%s",val_actual,val_nuevo);
    x=i;
    y=i;
    asig_matriz_din(matrix, y, x,numero);

}

//void asig_matriz_D_K(struct MATRIZ **matrix, struct REGISTRO_ELEMENTO *elementos, struct REGISTRO_NODO **indice, int num_v,struct REGISTRO_ELEMENTO *eleme)
void asig_matriz_D_K(
        struct MATRIZ **matrix,
        struct REGISTRO_ELEMENTO *elementos,
        int num_v,
        struct REGISTRO_ELEMENTO *eleme)
{
    int i=num_v+1;
    static struct REGISTRO_ELEMENTO *aux,*band_elemento;;
    int x,y;
    char val_actual [50],val_nuevo [50];
    char numero[16];
    char aux1[30];


    if(*matrix==NULL)
        return;

    aux=elementos; //Se para en el primero

    if(elementos==NULL)
        return;

    while(aux->sig != NULL)
    {
        if(aux->sig == NULL){

            band_elemento=busca_elemento2(eleme,aux->nodo_1);
            if(band_elemento != NULL)
            {
                //busca el elemento que existe en nodo 1

                strcpy(val_actual,(leer_ele_matriz(matrix,i, i)));

                if (strcmp(val_actual,"0")==0)
                    strcpy(val_actual,"0");
                if((strcmp(aux->valor,"SYMBOLIC")==0))
                {
                    strcpy(val_nuevo,(band_elemento->elemento));
                    sprintf(numero, "%s+S*%s",val_actual,val_nuevo); //imprime el valor del promario
                }
                else
                {
                    sprintf(aux1,"%f",band_elemento->valornum);
                    strcpy(val_nuevo,aux1);
                    sprintf(numero, "%s+S*%s",val_actual,val_nuevo); //imprime el valor del promario
                }
                x=i;
                y=i;
                asig_matriz_din(matrix, y, x,numero);
                //imprime_matrix("Matriz B 2",matrix,num_nodos,num_v+1);

            }
        }

        if (aux->sig != NULL){
            aux=(struct REGISTRO_ELEMENTO *)band_elemento->sig;
            i=i+2;
        }
    }

    if(aux->sig == NULL)
    {
        band_elemento=busca_elemento2(eleme,aux->nodo_1);
        if(band_elemento != NULL)
        {
            //busca el elemento que existe en nodo 1

            strcpy(val_actual,(leer_ele_matriz(matrix,i, i)));

            if (strcmp(val_actual,"0")==0)
                strcpy(val_actual,"0");
            if((strcmp(band_elemento->valor,"SYMBOLIC")==0))
            {
                strcpy(val_nuevo,(band_elemento->elemento));
                sprintf(numero, "%s-S*%s",val_actual,val_nuevo); //imprime el valor del promario
            }
            else
            {
                sprintf(aux1,"%f",band_elemento->valornum);
                strcpy(val_nuevo,aux1);
                sprintf(numero, "%s-S*%s",val_actual,val_nuevo); //imprime el valor del promario
            }
            x=i;
            y=i;
            asig_matriz_din(matrix, y, x,numero);
        }

        i++;
        band_elemento=busca_elemento2(eleme,aux->nodo_2);
        if(band_elemento != NULL){
            //busca el elemento que existe en nodo 1

            strcpy(val_actual,(leer_ele_matriz(matrix,i, i)));

            if (strcmp(val_actual,"0")==0)
                strcpy(val_actual,"0");
            if((strcmp(band_elemento->valor,"SYMBOLIC")==0))
            {
                strcpy(val_nuevo,(band_elemento->elemento));
                sprintf(numero, "%s-S*%s",val_actual,val_nuevo); //imprime el valor del promario
            }
            else
            {
                sprintf(aux1,"%f",band_elemento->valornum);
                strcpy(val_nuevo,aux1);
                sprintf(numero, "%s-S*%s",val_actual,val_nuevo); //imprime el valor del promario
            }

            x=i;
            y=i;
            asig_matriz_din(matrix, y, x,numero);
            //imprime_matrix("Matriz B 2",matrix,num_nodos,num_v+1);

            //busca el elemento que existe en nodo 1
            strcpy(val_actual,(leer_ele_matriz(matrix,i, i-1)));
            if (strcmp(val_actual,"0")==0)
                strcpy(val_actual,"0");
            strcpy(val_nuevo,(aux->nodo_3));
            sprintf(numero, "%s-S*%s",val_actual,val_nuevo); //imprime el valor del promario

            x=i;
            y=i-1;
            asig_matriz_din(matrix, y, x,numero);
        }

        if(aux != NULL)
        {
            //busca el elemento que existe en nodo 1
            strcpy(val_actual,(leer_ele_matriz(matrix,i, i-1)));
            if (strcmp(val_actual,"0")==0)
                strcpy(val_actual,"0");
            strcpy(val_nuevo,(aux->nodo_3));
            sprintf(numero, "%s-S*%s",val_actual,val_nuevo); //imprime el valor del promario
        }
        x=i-1;
        y=i;
        asig_matriz_din(matrix, y, x,numero);
    }
}
