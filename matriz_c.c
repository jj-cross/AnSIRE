#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include "matrix.h"
#include "cadenas.h"
#include "elementos.h"

void asig_matriz_CV(struct MATRIZ **matrix, struct REGISTRO_ELEMENTO *elementos,struct REGISTRO_NODO **indice)
{
    int i=1;
    static struct REGISTRO_ELEMENTO *aux;

    if(elementos==NULL)
        return;

    aux=elementos; //Se para en el primero

    do
    {
        if((strcmp(aux->nodo_1,"0"))!=0)
        {

            asig_matriz_din(matrix,i, indice_nodo(indice,aux->nodo_1),(char *)"1");
        }
        if((strcmp(aux->nodo_2,"0"))!=0){
            asig_matriz_din(matrix, i, indice_nodo(indice,aux->nodo_2), (char *)"-1");
        }

        if (aux->sig != NULL){
            aux=(struct REGISTRO_ELEMENTO *)aux->sig;
            i++;
        }
    }while(aux->sig !=NULL );

    if((strcmp(aux->nodo_1,"0"))!=0)
    {
        asig_matriz_din(matrix,i, indice_nodo(indice,aux->nodo_1),(char *)"1");
    }
    if((strcmp(aux->nodo_2,"0"))!=0)
    {
        asig_matriz_din(matrix, i, indice_nodo(indice,aux->nodo_2), (char *)"-1");
    }
}

void asig_matriz_CI(struct MATRIZ **matrix, struct REGISTRO_ELEMENTO *elementos,struct REGISTRO_NODO **indice, int num_v)
{
    int i=1;
    static struct REGISTRO_ELEMENTO *aux;

    if(elementos==NULL)
        return;

    aux=elementos; //Se para en el primero

    do
    {
        if((strcmp(aux->nodo_1,"0"))!=0)
        {
            asig_matriz_din(matrix,num_v+i, indice_nodo(indice,aux->nodo_1),(char *)"+1");
        }
        if((strcmp(aux->nodo_2,"0"))!=0)
        {
            asig_matriz_din(matrix, num_v+i, indice_nodo(indice,aux->nodo_2), (char *)"-1");
        }

        if (aux->sig != NULL)
        {
            aux=(struct REGISTRO_ELEMENTO *)aux->sig;
            i++;
        }
    }while(aux->sig !=NULL );

    if((strcmp(aux->nodo_1,"0"))!=0)
    {
        asig_matriz_din(matrix,num_v+i, indice_nodo(indice,aux->nodo_1),(char *)"+1");
    }
    if((strcmp(aux->nodo_2,"0"))!=0)
    {
        asig_matriz_din(matrix, num_v+i, indice_nodo(indice,aux->nodo_2), (char *)"-1");
    }
}

void asig_matriz_Coa(struct MATRIZ **matrix, struct REGISTRO_ELEMENTO *elementos,struct REGISTRO_NODO **indice, int num_v)
{
    int i=1;
    static struct REGISTRO_ELEMENTO *aux;
    char val_actual [50],val_nuevo [50];
    //strcpy(tem,"");

    if(elementos==NULL)
        return;

    aux=elementos; //Se para en el primero

    do
    {
        if((strcmp(aux->nodo_1,"0"))!=0)
        {
            asig_matriz_din(matrix, num_v+i, indice_nodo(indice,aux->nodo_1),(char *)"+1");
        }
        if((strcmp(aux->nodo_2,"0"))!=0)
        {
            asig_matriz_din(matrix, num_v+i, indice_nodo(indice,aux->nodo_2), (char *)"-1");
        }

        if (aux->sig != NULL)
        {
            aux=(struct REGISTRO_ELEMENTO *)aux->sig;
            i++;
        }
    }while(aux->sig !=NULL );

    if((strcmp(aux->nodo_1,"0"))!=0)
    {
        asig_matriz_din(matrix, num_v+i, indice_nodo(indice,aux->nodo_1),(char *)"+1");
    }
    if((strcmp(aux->nodo_2,"0"))!=0)
    {
        strcpy(val_actual,(leer_ele_matriz(matrix,num_v+i,indice_nodo(indice,aux->nodo_2))));
        if (strcmp(val_actual,"0")==0)
            strcpy(val_actual,"0");
        strcpy(val_nuevo,(char *)"-1");
        asig_matriz_din(matrix, num_v+i, indice_nodo(indice,aux->nodo_2),val_nuevo);
    }
}

void asig_matriz_CVCVS(struct MATRIZ **matrix, struct REGISTRO_ELEMENTO *elementos, struct REGISTRO_NODO **indice, int num_v)
{
    int i=1;
    char tem [8];
    static struct REGISTRO_ELEMENTO *aux;
    char val_actual [50],val_nuevo [50];
    char numero[16];
    strcpy(tem,"");

    if(elementos==NULL)
        return;

    aux=elementos; //Se para en el primero

    do
    {
        if((strcmp(aux->nodo_1,"0"))!=0)
        {
            strcpy(val_actual,(leer_ele_matriz(matrix, num_v+i, indice_nodo(indice,aux->nodo_1))));

            if (strcmp(val_actual,"0")==0)
                strcpy(val_actual,"0");
            strcpy(val_nuevo,(char *)"+1");
            //            asig_matriz_din(matrix, num_v+i, indice_nodo(indice,aux->nodo_2), (char *)"-1");
            //            imprime_matrix(matrix, num_v+num_VCVS, num_nodos);
            asig_matriz_din(matrix, num_v+i, indice_nodo(indice,aux->nodo_1),val_nuevo);
        }
        if((strcmp(aux->nodo_2,"0"))!=0)
        {
            strcpy(val_actual,(leer_ele_matriz(matrix,num_v+i,indice_nodo(indice,aux->nodo_2))));
            if (strcmp(val_actual,"0")==0)
                strcpy(val_actual,"0");
            strcpy(val_nuevo,(char *)"-1");
            asig_matriz_din(matrix, num_v+i, indice_nodo(indice,aux->nodo_2),val_nuevo);
        }
        if((strcmp(aux->nodo_3,"0"))!=0)
        {
            strcpy(val_actual,(leer_ele_matriz(matrix,num_v+i, indice_nodo(indice,aux->nodo_3))));

            if (strcmp(val_actual,"0")==0)
                strcpy(val_actual,"0");
            strcpy(val_nuevo,(aux->valor));
            sprintf(numero, "%s-%s",val_actual,val_nuevo);
            asig_matriz_din(matrix, num_v+i, indice_nodo(indice,aux->nodo_3),numero);
        }
        if((strcmp(aux->nodo_4,"0"))!=0)
        {
            strcpy(val_actual,(leer_ele_matriz(matrix,num_v+i, indice_nodo(indice,aux->nodo_4))));

            if (strcmp(val_actual,"0")==0)
                strcpy(val_actual,"0");
            strcpy(val_nuevo,(aux->valor));
            sprintf(numero, "%s+%s",val_actual,val_nuevo);
            asig_matriz_din(matrix, num_v+i, indice_nodo(indice,aux->nodo_4),numero);
        }

        if (aux->sig != NULL)
        {
            aux=(struct REGISTRO_ELEMENTO *)aux->sig;
            i++;
        }
    }
    while(aux->sig !=NULL );
    if((strcmp(aux->nodo_1,"0"))!=0)
    {
        strcpy(val_actual,(leer_ele_matriz(matrix, num_v+i, indice_nodo(indice,aux->nodo_1))));

        if (strcmp(val_actual,"0")==0)
            strcpy(val_actual,"0");
        strcpy(val_nuevo,(char *)"+1");
        //            asig_matriz_din(matrix, num_v+i, indice_nodo(indice,aux->nodo_2), (char *)"-1");
        //            imprime_matrix(matrix, num_v+num_VCVS, num_nodos);
        asig_matriz_din(matrix, num_v+i, indice_nodo(indice,aux->nodo_1),val_nuevo);
    }
    if((strcmp(aux->nodo_2,"0"))!=0)
    {
        strcpy(val_actual,(leer_ele_matriz(matrix,num_v+i,indice_nodo(indice,aux->nodo_2))));
        if (strcmp(val_actual,"0")==0)
            strcpy(val_actual,"0");
        strcpy(val_nuevo,(char *)"-1");
        asig_matriz_din(matrix, num_v+i, indice_nodo(indice,aux->nodo_2),val_nuevo);
    }
    if((strcmp(aux->nodo_3,"0"))!=0)
    {
        strcpy(val_actual,(leer_ele_matriz(matrix,num_v+i, indice_nodo(indice,aux->nodo_3))));

        if (strcmp(val_actual,"0")==0)
            strcpy(val_actual,"0");
        strcpy(val_nuevo,(aux->valor));
        sprintf(numero, "%s-%s",val_actual,val_nuevo);
        asig_matriz_din(matrix, num_v+i, indice_nodo(indice,aux->nodo_3),numero);
    }
    if((strcmp(aux->nodo_4,"0"))!=0)
    {
        strcpy(val_actual,(leer_ele_matriz(matrix,num_v+i, indice_nodo(indice,aux->nodo_4))));

        if (strcmp(val_actual,"0")==0)
            strcpy(val_actual,"0");
        strcpy(val_nuevo,(aux->valor));
        sprintf(numero, "%s+%s",val_actual,val_nuevo);
        asig_matriz_din(matrix, num_v+i, indice_nodo(indice,aux->nodo_4),numero);
    }
}

void asig_matriz_CCCCS(struct MATRIZ **matrix, struct REGISTRO_ELEMENTO *elementos, struct REGISTRO_NODO **indice,  int num_v)
{
    int i=1;
    int x,y;

    static struct REGISTRO_ELEMENTO *aux;

    if (*matrix==NULL)
        return;

    aux=elementos; //Se para en el primero
    if (aux==NULL)
        return;

    do
    {
        if((strcmp(aux->nodo_3,"0"))!=0)
        {
            x=num_v+i;
            y=indice_nodo(indice,aux->nodo_3);
            asig_matriz_din(matrix, x,y, (char *)"+1");
        }
        if((strcmp(aux->nodo_4,"0"))!=0)
        {
            x=num_v+i;
            y=indice_nodo(indice,aux->nodo_4);
            asig_matriz_din(matrix,x,y, (char *)"-1");
        }

        if (aux->sig != NULL)
        {
            aux=(struct REGISTRO_ELEMENTO *)aux->sig;
            i++;
        }
    }while(aux->sig !=NULL );

    if((strcmp(aux->nodo_3,"0"))!=0)
    {
        x=num_v+i;
        y=indice_nodo(indice,aux->nodo_3);
        asig_matriz_din(matrix, x,y,(char *)"+1");
    }
    if((strcmp(aux->nodo_4,"0"))!=0)
    {
        x=num_v+i;
        y=indice_nodo(indice,aux->nodo_4);
        asig_matriz_din(matrix, x,y,(char *)"-1");
    }
}

void asig_matriz_CCCVS(struct MATRIZ **matrix, struct REGISTRO_ELEMENTO *elementos, struct REGISTRO_NODO **indice,  int num_v)
{
    int i=num_v+1;
    //int x;
    char val_actual [50],val_nuevo [50];

    static struct REGISTRO_ELEMENTO *aux;

    if (*matrix==NULL)
        return;

    aux=elementos; //Se para en el primero
    if (aux==NULL)
        return;
    do
    {
        if((strcmp(aux->nodo_1,"0"))!=0)
        {
            strcpy(val_actual,(leer_ele_matriz(matrix, i, indice_nodo(indice,aux->nodo_1))));
            if (strcmp(val_actual,"0")==0)
                strcpy(val_actual,"0");
            strcpy(val_nuevo,(char *)"+1");
            asig_matriz_din(matrix, i, indice_nodo(indice,aux->nodo_1),val_nuevo);
        }
        if((strcmp(aux->nodo_2,"0"))!=0)
        {
            strcpy(val_actual,(leer_ele_matriz(matrix,i,indice_nodo(indice,aux->nodo_2))));
            if (strcmp(val_actual,"0")==0)
                strcpy(val_actual,"0");
            strcpy(val_nuevo,(char *)"-1");
            asig_matriz_din(matrix, i, indice_nodo(indice,aux->nodo_2),val_nuevo);
        }
        i++;
        if((strcmp(aux->nodo_3,"0"))!=0)
        {
            //x=i+1;
            strcpy(val_actual,(leer_ele_matriz(matrix,i,indice_nodo(indice,aux->nodo_3))));
            if (strcmp(val_actual,"0")==0)
                strcpy(val_actual,"0");
            strcpy(val_nuevo,(char *)"+1");
            asig_matriz_din(matrix, i, indice_nodo(indice,aux->nodo_3),val_nuevo);
            //            x=i;
            //            y=indice_nodo(indice,aux->nodo_3);
            //            asig_matriz_din(matrix, x,y, (char *)"+1");
        }
        if((strcmp(aux->nodo_4,"0"))!=0)
        {
            //x=i+1;
            strcpy(val_actual,(leer_ele_matriz(matrix,i,indice_nodo(indice,aux->nodo_4))));
            if (strcmp(val_actual,"0")==0)
                strcpy(val_actual,"0");
            strcpy(val_nuevo,(char *)"+1");
            asig_matriz_din(matrix, i, indice_nodo(indice,aux->nodo_4),val_nuevo);
            //            x=i;
            //            y=indice_nodo(indice,aux->nodo_4);
            //            asig_matriz_din(matrix,x,y, (char *)"-1");
        }

        if (aux->sig != NULL){
            aux=(struct REGISTRO_ELEMENTO *)aux->sig;
            i++;
        }
    }while(aux->sig !=NULL );

    if((strcmp(aux->nodo_1,"0"))!=0)
    {
        strcpy(val_actual,(leer_ele_matriz(matrix, i, indice_nodo(indice,aux->nodo_1))));

        if (strcmp(val_actual,"0")==0)
            strcpy(val_actual,"0");
        strcpy(val_nuevo,(char *)"+1");
        asig_matriz_din(matrix, i, indice_nodo(indice,aux->nodo_1),val_nuevo);
    }
    if((strcmp(aux->nodo_2,"0"))!=0)
    {
        strcpy(val_actual,(leer_ele_matriz(matrix,i,indice_nodo(indice,aux->nodo_2))));
        if (strcmp(val_actual,"0")==0)
            strcpy(val_actual,"0");
        strcpy(val_nuevo,(char *)"-1");
        asig_matriz_din(matrix, i, indice_nodo(indice,aux->nodo_2),val_nuevo);
    }

    if((strcmp(aux->nodo_3,"0"))!=0)
    {
        //x=i+1;
        strcpy(val_actual,(leer_ele_matriz(matrix,i,indice_nodo(indice,aux->nodo_3))));
        if (strcmp(val_actual,"0")==0)
            strcpy(val_actual,"0");
        strcpy(val_nuevo,(char *)"+1");
        asig_matriz_din(matrix, i, indice_nodo(indice,aux->nodo_3),val_nuevo);

        //        x=i+1;
        //        y=indice_nodo(indice,aux->nodo_3);
        //        asig_matriz_din(matrix, x,y, (char *)"+1");
    }
    if((strcmp(aux->nodo_4,"0"))!=0)
    {
        //x=i+1;
        strcpy(val_actual,(leer_ele_matriz(matrix,i,indice_nodo(indice,aux->nodo_4))));
        if (strcmp(val_actual,"0")==0)
            strcpy(val_actual,"0");
        strcpy(val_nuevo,(char *)"+1");
        asig_matriz_din(matrix, i, indice_nodo(indice,aux->nodo_4),val_nuevo);

        //        x=i+1;
        //        y=indice_nodo(indice,aux->nodo_4);
        //        asig_matriz_din(matrix,x,y, (char *)"-1");
    }
}

void asig_matriz_CN(struct MATRIZ **matrix, struct REGISTRO_ELEMENTO *elementos, struct REGISTRO_NODO **indice, int num_v)
{
    int i=1;
    char tem [8];
    static struct REGISTRO_ELEMENTO *aux;
    char val_actual [50],val_nuevo [50];
    char numero[16];
    strcpy(tem,"");

    if(elementos==NULL)
        return;

    aux=elementos; //Se para en el primero

    while(aux->sig !=NULL )
    {
        if((strcmp(aux->nodo_1,"0"))!=0)
        {
            strcpy(val_actual,(leer_ele_matriz(matrix,num_v+i, indice_nodo(indice,aux->nodo_1))));

            if (strcmp(val_actual,"0")==0)
                strcpy(val_actual,"0");
            strcpy(val_nuevo,(aux->valor));
            sprintf(numero, "%s+%s",val_actual,val_nuevo);
            asig_matriz_din(matrix, num_v+i, indice_nodo(indice,aux->nodo_1),numero);
        }
        if((strcmp(aux->nodo_2,"0"))!=0)
        {
            strcpy(val_actual,(leer_ele_matriz(matrix,num_v+i, indice_nodo(indice,aux->nodo_2))));

            if (strcmp(val_actual,"0")==0)
                strcpy(val_actual,"0");
            strcpy(val_nuevo,(aux->valor));
            sprintf(numero, "%s-%s",val_actual,val_nuevo);
            asig_matriz_din(matrix, num_v+i, indice_nodo(indice,aux->nodo_2),numero);
        }

        if((strcmp(aux->nodo_3,"0"))!=0)
        {
            strcpy(val_actual,(leer_ele_matriz(matrix, num_v+i, indice_nodo(indice,aux->nodo_3))));

            if (strcmp(val_actual,"0")==0)
                strcpy(val_actual,"0");
            strcpy(val_nuevo,(char *)"-1");
            //            asig_matriz_din(matrix, num_v+i, indice_nodo(indice,aux->nodo_2), (char *)"-1");
            //            imprime_matrix(matrix, num_v+num_VCVS, num_nodos);
            asig_matriz_din(matrix, num_v+i, indice_nodo(indice,aux->nodo_3),val_nuevo);
        }
        if((strcmp(aux->nodo_4,"0"))!=0)
        {
            strcpy(val_actual,(leer_ele_matriz(matrix,num_v+i,indice_nodo(indice,aux->nodo_4))));
            if (strcmp(val_actual,"0")==0)
                strcpy(val_actual,"0");
            strcpy(val_nuevo,(char *)"+1");
            asig_matriz_din(matrix, num_v+i, indice_nodo(indice,aux->nodo_4),val_nuevo);
        }

        if (aux->sig != NULL)
        {
            aux=(struct REGISTRO_ELEMENTO *)aux->sig;
            i++;
        }
    }

    if(aux->sig ==NULL)
    {
        if((strcmp(aux->nodo_1,"0"))!=0){
            strcpy(val_actual,(leer_ele_matriz(matrix,num_v+i, indice_nodo(indice,aux->nodo_1))));

            if (strcmp(val_actual,"0")==0)
                strcpy(val_actual,"0");
            strcpy(val_nuevo,(aux->valor));
            sprintf(numero, "%s+%s",val_actual,val_nuevo);
            asig_matriz_din(matrix, num_v+i, indice_nodo(indice,aux->nodo_1),numero);
        }
        if((strcmp(aux->nodo_2,"0"))!=0)
        {
            strcpy(val_actual,(leer_ele_matriz(matrix,num_v+i, indice_nodo(indice,aux->nodo_2))));
            if (strcmp(val_actual,"0")==0)
                strcpy(val_actual,"0");
            strcpy(val_nuevo,(aux->valor));
            sprintf(numero, "%s-%s",val_actual,val_nuevo);
            asig_matriz_din(matrix, num_v+i, indice_nodo(indice,aux->nodo_2),numero);
        }

        if((strcmp(aux->nodo_3,"0"))!=0)
        {
            strcpy(val_actual,(leer_ele_matriz(matrix, num_v+i, indice_nodo(indice,aux->nodo_3))));
            if (strcmp(val_actual,"0")==0)
                strcpy(val_actual,"0");
            strcpy(val_nuevo,(char *)"-1");
            //            asig_matriz_din(matrix, num_v+i, indice_nodo(indice,aux->nodo_2), (char *)"-1");
            //            imprime_matrix(matrix, num_v+num_VCVS, num_nodos);
            asig_matriz_din(matrix, num_v+i, indice_nodo(indice,aux->nodo_3),val_nuevo);
        }
        if((strcmp(aux->nodo_4,"0"))!=0)
        {
            strcpy(val_actual,(leer_ele_matriz(matrix,num_v+i,indice_nodo(indice,aux->nodo_4))));
            if (strcmp(val_actual,"0")==0)
                strcpy(val_actual,"0");
            strcpy(val_nuevo,(char *)"+1");
            asig_matriz_din(matrix, num_v+i, indice_nodo(indice,aux->nodo_4),val_nuevo);
        }
    }
}

void asig_matriz_CK(struct MATRIZ **matrix, struct REGISTRO_ELEMENTO *elementos, struct REGISTRO_NODO **indice,  int num_v, struct REGISTRO_ELEMENTO *eleme)
{
    int i=num_v+1;
    int x,y;
    char val_actual [50],val_nuevo [50];
    char l[10];

    static struct REGISTRO_ELEMENTO *aux,*band_elemento;

    if (*matrix==NULL)
        return;

    aux=elementos; //Se para en el primero
    if (aux==NULL)
        return;
    do
    {
        band_elemento = busca_elemento2(eleme,aux->nodo_1);
        if(band_elemento != NULL)
        {
            //buscar lo que existe en la matriz donde esta conectada L1
            if(strcmp(band_elemento->nodo_1,"0")!=0){

                strcpy(val_actual,(leer_ele_matriz(matrix, i, indice_nodo(indice,band_elemento->nodo_1))));

                if (strcmp(val_actual,"0")==0)
                    strcpy(val_actual,"0");
                strcpy(val_nuevo,(char *)"+1");
                x=i;
                y=indice_nodo(indice,band_elemento->nodo_1);
                asig_matriz_din(matrix, x, y,val_nuevo);
            }

            //buscar lo que existe en la matriz dinde esta conectada L1
            if(strcmp(band_elemento->nodo_2,"0")!=0)
            {
                strcpy(val_actual,(leer_ele_matriz(matrix, i, indice_nodo(indice,band_elemento->nodo_2))));

                if (strcmp(val_actual,"0")==0)
                    strcpy(val_actual,"0");
                strcpy(val_nuevo,(char *)"-1");
                x=i;
                y=indice_nodo(indice,band_elemento->nodo_2);
                asig_matriz_din(matrix, x, y,val_nuevo);
            }
        }
        i++;

        band_elemento = busca_elemento2(eleme,aux->nodo_2);
        if(band_elemento != NULL)
        {
            //buscar lo que existe en la matriz donde esta conectada L1
            if(strcmp(band_elemento->nodo_1,"0")!=0)
            {
                strcpy(val_actual,(leer_ele_matriz(matrix, i, indice_nodo(indice,band_elemento->nodo_1))));
                if (strcmp(val_actual,"0")==0)
                    strcpy(val_actual,"0");
                strcpy(val_nuevo,(char *)"+1");
                x=i;
                y=indice_nodo(indice,band_elemento->nodo_1);
                asig_matriz_din(matrix, x, y,val_nuevo);
            }

            //buscar lo que existe en la matriz donde esta conectada L1
            if(strcmp(band_elemento->nodo_2,"0")!=0)
            {
                strcpy(val_actual,(leer_ele_matriz(matrix, i, indice_nodo(indice,band_elemento->nodo_2))));

                if (strcmp(val_actual,"0")==0)
                    strcpy(val_actual,"0");
                strcpy(val_nuevo,(char *)"-1");
                x=i;
                y=indice_nodo(indice,band_elemento->nodo_2);
                asig_matriz_din(matrix, x, y,val_nuevo);
            }
        }

        if (aux->sig != NULL)
        {
            aux=(struct REGISTRO_ELEMENTO *)aux->sig;
            i++;
        }
    }
    while(aux->sig !=NULL );

    if(aux->sig !=NULL)
    {
        //Buscar el elemento guardado o indicado en nodo_1
        band_elemento = busca_elemento2(eleme,aux->nodo_1);
        if(band_elemento != NULL)
        {
            //buscar lo que existe en la matriz dinde esta conectada L1

            if(strcmp(band_elemento->nodo_1,"0")!=0)
            {
                strcpy(val_actual,(leer_ele_matriz(matrix, i, indice_nodo(indice,band_elemento->nodo_1))));
                if (strcmp(val_actual,"0")==0)
                    strcpy(val_actual,"0");
                strcpy(val_nuevo,(char *)"+1");
                x=i;
                y=indice_nodo(indice,band_elemento->nodo_1);
                asig_matriz_din(matrix, x, y,val_nuevo);
            }

            //buscar lo que existe en la matriz dinde esta conectada L1
            if(strcmp(band_elemento->nodo_2,"0")!=0)
            {
                strcpy(val_actual,(leer_ele_matriz(matrix, i, indice_nodo(indice,band_elemento->nodo_2))));
                if (strcmp(val_actual,"0")==0)
                    strcpy(val_actual,"0");
                strcpy(val_nuevo,(char *)"-1");
                x=i;
                y=indice_nodo(indice,band_elemento->nodo_2);
                asig_matriz_din(matrix, x, y,val_nuevo);
            }
        }

        i++;
        band_elemento = busca_elemento2(eleme,aux->nodo_2);
        if(band_elemento != NULL)
        {
            //buscar lo que existe en la matriz donde esta conectada L2
            if(strcmp(band_elemento->nodo_1,"0")!=0)
            {
                y=indice_nodo(indice,band_elemento->nodo_1);
                strcpy(l,leer_ele_matriz(matrix, i, y));
                strcpy(val_actual,l);

                if (strcmp(val_actual,"0")==0)
                    strcpy(val_actual,"0");
                strcpy(val_nuevo,(char *)"+1");
                x=i;
                asig_matriz_din(matrix, x, y,val_nuevo);
            }

            //buscar lo que existe en la matriz donde esta conectada L2
            if(strcmp(band_elemento->nodo_2,"0")!=0)
            {
                strcpy(val_actual,(leer_ele_matriz(matrix, i, indice_nodo(indice,band_elemento->nodo_2))));

                if (strcmp(val_actual,"0")==0)
                    strcpy(val_actual,"0");
                strcpy(val_nuevo,(char *)"-1");
                x=i;
                y=indice_nodo(indice,band_elemento->nodo_2);
                asig_matriz_din(matrix, x, y,val_nuevo);
            }
        }
    }
}
