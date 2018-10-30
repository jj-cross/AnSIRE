#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include "matrix.h"
#include "cadenas.h"
#include "elementos.h"

void asig_matriz_BV(struct MATRIZ **matrix, struct REGISTRO_ELEMENTO *elementos, struct REGISTRO_NODO **indice)
{
    int i=1;
    static struct REGISTRO_ELEMENTO *aux;

    if (*matrix==NULL)
        return;

    aux=elementos; //Se para en el primero
    if (aux==NULL)
        return;

    while(aux->sig !=NULL )
    {
        if((strcmp(aux->nodo_1,"0"))!=0)
        {
            //asig_matriz_din(matrix, i, j,busca_valor_dp(linea, elementos,nombre_nodo(indice,i), f));
            asig_matriz_din(matrix, indice_nodo(indice,aux->nodo_1), i,(char *)"1");
        }
        if((strcmp(aux->nodo_2,"0"))!=0)
        {
            asig_matriz_din(matrix, indice_nodo(indice,aux->nodo_2), i,(char *)"-1");
        }


        if (aux->sig != NULL)
        {
            aux=(struct REGISTRO_ELEMENTO *)aux->sig;
            i++;
        }
    }

    if(aux->sig ==NULL)
    {

        if((strcmp(aux->nodo_1,"0"))!=0)
        {
            asig_matriz_din(matrix, indice_nodo(indice,aux->nodo_1), i,(char *)"1");
        }
        if((strcmp(aux->nodo_2,"0"))!=0){
            asig_matriz_din(matrix, indice_nodo(indice,aux->nodo_2), i,(char *)"-1");
        }
    }
}

//void asig_matriz_BC(struct MATRIZ **matrix, struct REGISTRO_ELEMENTO *elementos, struct REGISTRO_NODO **indice, int num_v)
//{
//    int i=1;
//    static struct REGISTRO_ELEMENTO *aux;

//    if (*matrix==NULL)
//        return;

//    aux=elementos; //Se para en el primero
//    if (aux==NULL)
//        return;

//    while(aux->sig !=NULL ) {
//        if((strcmp(aux->nodo_1,"0"))!=0){
//            //asig_matriz_din(matrix, i, j,busca_valor_dp(linea, elementos,nombre_nodo(indice,i), f));
//            asig_matriz_din(matrix, indice_nodo(indice,aux->nodo_1), num_v+i,(char *)"+1");
//        }
//        if((strcmp(aux->nodo_2,"0"))!=0){
//            asig_matriz_din(matrix, indice_nodo(indice,aux->nodo_2), num_v+i,(char *)"-1");
//        }


//        if (aux->sig != NULL){
//            aux=(struct REGISTRO_ELEMENTO *)aux->sig;
//            i++;
//        }
//    }if(aux->sig ==NULL){

//        if((strcmp(aux->nodo_1,"0"))!=0){
//            asig_matriz_din(matrix, indice_nodo(indice,aux->nodo_1), num_v+i,(char *)"+1");
//        }
//        if((strcmp(aux->nodo_2,"0"))!=0){
//            asig_matriz_din(matrix, indice_nodo(indice,aux->nodo_2), num_v+i,(char *)"-1");
//        }
//    }
//}

void asig_matriz_Boa(struct MATRIZ **matrix, struct REGISTRO_ELEMENTO *elementos, struct REGISTRO_NODO **indice, int num_v)
{
    int i=1;
    static struct REGISTRO_ELEMENTO *aux;
    char val_actual [50],val_nuevo [50];
    int x;
    strcpy(val_actual,"0");


    if (*matrix==NULL)
        return;

    aux=elementos; //Se para en el primero
    if (aux==NULL)
        return;

    while(aux->sig !=NULL )
    {
        if((strcmp(aux->nodo_3,"0"))!=0)
        {
            strcpy(val_actual,(leer_ele_matriz(matrix, i, num_v+i)));

            if (strcmp(val_actual,"0")==0)
                strcpy(val_actual,"0");
            strcpy(val_nuevo,(char *)"+1");
            x=indice_nodo(indice,aux->nodo_3);
            //            y=i;
            strcat(val_nuevo,val_actual);
            asig_matriz_din(matrix, x, num_v+i,val_actual);

            //            //asig_matriz_din(matrix, i, j,busca_valor_dp(linea, elementos,nombre_nodo(indice,i), f));

            //            ff=indice_nodo(indice,aux->nodo_3);
            //            asig_matriz_din(matrix, ff, num_v+i,(char *)"+1");
        }

        if (aux->sig != NULL){
            aux=(struct REGISTRO_ELEMENTO *)aux->sig;
            i++;
        }
    }

    if(aux->sig ==NULL)
    {
        if((strcmp(aux->nodo_3,"0"))!=0)
        {
            strcpy(val_actual,(leer_ele_matriz(matrix, i, num_v+i)));

            if (strcmp(val_actual,"0")==0)
                strcpy(val_actual,"0");
            strcpy(val_nuevo,(char *)"+1");
            x=indice_nodo(indice,aux->nodo_3);
            //            y=i;
            strcat(val_actual, val_nuevo);
            asig_matriz_din(matrix, x, num_v+i,val_actual);

            //            //asig_matriz_din(matrix, i, j,busca_valor_dp(linea, elementos,nombre_nodo(indice,i), f));

            //            ff=indice_nodo(indice,aux->nodo_3);
            //            asig_matriz_din(matrix, ff, num_v+i,(char *)"+1");
        }

        if (aux->sig != NULL){
            aux=(struct REGISTRO_ELEMENTO *)aux->sig;
            i++;
        }
    }
}

void asig_matriz_BVCVS(struct MATRIZ **matrix, struct REGISTRO_ELEMENTO *elementos, struct REGISTRO_NODO **indice,  int num_v)
{
    int i=1;
    static struct REGISTRO_ELEMENTO *aux;

    if (*matrix==NULL)
        return;

    aux=elementos; //Se para en el primero
    if (aux==NULL)
        return;

    while(aux->sig !=NULL )
    {
        if((strcmp(aux->nodo_1,"0"))!=0){
            asig_matriz_din(matrix, indice_nodo(indice,aux->nodo_1), num_v+i,(char *)"+1");
        }

        if((strcmp(aux->nodo_2,"0"))!=0){
            asig_matriz_din(matrix, indice_nodo(indice,aux->nodo_2), num_v+i,(char *)"-1");
        }

        if (aux->sig != NULL){
            aux=(struct REGISTRO_ELEMENTO *)aux->sig;
            i++;
        }
    }

    if(aux->sig ==NULL)
    {

        if((strcmp(aux->nodo_1,"0"))!=0){
            asig_matriz_din(matrix, indice_nodo(indice,aux->nodo_1), num_v+i,(char *)"+1");
        }
        if((strcmp(aux->nodo_2,"0"))!=0){
            asig_matriz_din(matrix, indice_nodo(indice,aux->nodo_2), num_v+i,(char *)"-1");
        }
    }
}

void asig_matriz_BCCCS(struct MATRIZ **matrix, struct REGISTRO_ELEMENTO *elementos, struct REGISTRO_NODO **indice, int num_v)
{
    int i=1;
    int x,y;
    char tem [8];
    static struct REGISTRO_ELEMENTO *aux;
    char val_actual [50],val_nuevo [50],yy [50];
    char numero[16];
    strcpy(tem,"");
    strcpy(val_actual,"0");

    if(elementos==NULL)
        return;

    aux=elementos; //Se para en el primero

    while(aux->sig !=NULL )
    {
        if((strcmp(aux->nodo_1,"0"))!=0){
            //strcpy(val_actual,(leer_ele_matriz(matrix,num_v+i, indice_nodo(indice,aux->nodo_1))));

            int mm= indice_nodo(indice,aux->nodo_1);
            strcpy(yy,leer_ele_matriz(matrix,mm,num_v+i));
            strcpy(val_actual,yy);

            //            if (strcmp(val_actual,"0")==0)
            //                strcpy(val_actual,"");
            strcpy(val_nuevo,(aux->valor));
            sprintf(numero, "%s+%s",val_actual,val_nuevo);
            x=num_v+i;
            y=indice_nodo(indice,aux->nodo_1);
            asig_matriz_din(matrix, y, x,numero);
            //imprime_matrix("Matriz B 1",matrix,num_nodos,num_v+1);
        }

        if((strcmp(aux->nodo_2,"0"))!=0)
        {
            //strcpy(val_actual,(leer_ele_matriz(matrix,num_v+i, indice_nodo(indice,aux->nodo_2))));

            int mm= indice_nodo(indice,aux->nodo_2);
            strcpy(yy,leer_ele_matriz(matrix,mm,num_v+i));
            strcpy(val_actual,yy);

            //            if (strcmp(val_actual,"0")==0)
            //                strcpy(val_actual,"");
            strcpy(val_nuevo,(aux->valor));
            sprintf(numero, "%s-%s",val_actual,val_nuevo);
            x=num_v+i;
            y=indice_nodo(indice,aux->nodo_2);
            asig_matriz_din(matrix, y, x,numero);
            //imprime_matrix("Matriz B 2",matrix,num_nodos,num_v+1);
        }

        if((strcmp(aux->nodo_3,"0"))!=0)
        {
            //strcpy(val_actual,(leer_ele_matriz(matrix, num_v+i, indice_nodo(indice,aux->nodo_3))));

            int mm= indice_nodo(indice,aux->nodo_3);
            strcpy(yy,leer_ele_matriz(matrix,mm,num_v+i));
            strcpy(val_actual,yy);

            //            if (strcmp(val_actual,"0")==0)
            //                strcpy(val_actual,"");
            strcpy(val_nuevo,(char *)"+1");
            strcat(val_nuevo,(char *)"+");
            strcat(val_nuevo,val_actual);
            x=num_v+i;
            y=indice_nodo(indice,aux->nodo_3);
            //            asig_matriz_din(matrix, num_v+i, indice_nodo(indice,aux->nodo_2), (char *)"-1");
            //            imprime_matrix(matrix, num_v+num_VCVS, num_nodos);
            asig_matriz_din(matrix, y, x,val_nuevo);
            //imprime_matrix("Matriz B 3",matrix,num_nodos,num_v+1);
        }

        if((strcmp(aux->nodo_4,"0"))!=0)
        {
            //strcpy(val_actual,(leer_ele_matriz(matrix,num_v+i,indice_nodo(indice,aux->nodo_4))));

            int mm= indice_nodo(indice,aux->nodo_4);
            strcpy(yy,leer_ele_matriz(matrix,mm,num_v+i));
            strcpy(val_actual,yy);

            //            if (strcmp(val_actual,"0")==0)
            //                strcpy(val_actual,"");
            strcpy(val_nuevo,(char *)"-1");
            strcat(val_nuevo,(char *)"+");
            strcat(val_nuevo,val_actual);
            x=num_v+i;
            y=indice_nodo(indice,aux->nodo_4);
            asig_matriz_din(matrix, y, x,val_nuevo);
            //imprime_matrix("Matriz B 4",matrix,num_nodos,num_v+1);
        }

        if (aux->sig != NULL)
        {
            aux=(struct REGISTRO_ELEMENTO *)aux->sig;
            i++;
        }
    }
    strcpy(val_nuevo, "0");
    if(aux->sig ==NULL)
    {
        if((strcmp(aux->nodo_1,"0"))!=0)
        {

            int mm= indice_nodo(indice,aux->nodo_1);
            strcpy(yy,leer_ele_matriz(matrix,mm,num_v+i));
            strcpy(val_actual,yy);

            //            if (strcmp(val_actual,"0")==0)
            //                strcpy(val_actual,"");
            strcpy(val_nuevo,(aux->valor));
            sprintf(numero, "%s+%s",val_actual,val_nuevo);
            x=num_v+i;
            y=indice_nodo(indice,aux->nodo_1);
            asig_matriz_din(matrix, y, x,numero);
            //imprime_matrix("Matriz B 1",matrix,num_nodos,num_v+1);
        }

        if((strcmp(aux->nodo_2,"0"))!=0)
        {
            //strcpy(val_actual,(leer_ele_matriz(matrix,num_v+i, indice_nodo(indice,aux->nodo_2))));

            int mm= indice_nodo(indice,aux->nodo_2);
            strcpy(yy,leer_ele_matriz(matrix,mm,num_v+i));
            strcpy(val_actual,yy);

            //            if (strcmp(val_actual,"0")==0)
            //                strcpy(val_actual,"");
            strcpy(val_nuevo,(aux->valor));
            sprintf(numero, "%s-%s",val_actual,val_nuevo);
            x=num_v+i;
            y=indice_nodo(indice,aux->nodo_2);
            asig_matriz_din(matrix, y, x,numero);
            //imprime_matrix("Matriz B 2",matrix,num_nodos,num_v+1);
        }

        if((strcmp(aux->nodo_3,"0"))!=0)
        {
            //strcpy(val_actual,(leer_ele_matriz(matrix, num_v+i, indice_nodo(indice,aux->nodo_3))));

            int mm= indice_nodo(indice,aux->nodo_3);
            strcpy(yy,leer_ele_matriz(matrix,mm,num_v+i));
            strcpy(val_actual,yy);

            //            if (strcmp(val_actual,"0")==0)
            //                strcpy(val_actual,"");
            strcpy(val_nuevo,(char *)"+1");
            //strcat(val_nuevo,(char *)"+");
            x=num_v+i;
            y=indice_nodo(indice,aux->nodo_3);
            //            asig_matriz_din(matrix, num_v+i, indice_nodo(indice,aux->nodo_2), (char *)"-1");
            //            imprime_matrix(matrix, num_v+num_VCVS, num_nodos);
            asig_matriz_din(matrix, y, x,val_nuevo);
            //imprime_matrix("Matriz B 3",matrix,num_nodos,num_v+1);
        }

        if((strcmp(aux->nodo_4,"0"))!=0)
        {
            //strcpy(val_actual,(leer_ele_matriz(matrix,num_v+i,indice_nodo(indice,aux->nodo_4))));

            int mm= indice_nodo(indice,aux->nodo_4);
            strcpy(yy,leer_ele_matriz(matrix,mm,num_v+i));
            strcpy(val_actual,yy);

            //            if (strcmp(val_actual,"0")==0)
            //                strcpy(val_actual,"");
            strcpy(val_nuevo,(char *)"-1");
            strcat(val_nuevo,(char *)"+");
            strcat(val_nuevo,val_actual);
            x=num_v+i;
            y=indice_nodo(indice,aux->nodo_4);
            asig_matriz_din(matrix, y, x,val_nuevo);
            //imprime_matrix("Matriz B 4",matrix,num_nodos,num_v+1);
        }
    }
}

void asig_matriz_BCCVS(struct MATRIZ **matrix, struct REGISTRO_ELEMENTO *elementos, struct REGISTRO_NODO **indice, int num_v)
{
    int i=num_v+1;
    int x,y;
    char val_actual [50],val_nuevo [50];
    static struct REGISTRO_ELEMENTO *aux;

    if (*matrix==NULL)
        return;

    aux=elementos; //Se para en el primero
    if (aux==NULL)
        return;

    while(aux->sig !=NULL )
    {
        if((strcmp(aux->nodo_3,"0"))!=0)
        {
            x=indice_nodo(indice,aux->nodo_3);
            y=i;
            asig_matriz_din(matrix, x, y,(char *)"+1");
        }

        if((strcmp(aux->nodo_4,"0"))!=0)
        {
            x=indice_nodo(indice,aux->nodo_4);
            y=i;
            asig_matriz_din(matrix, x, y,(char *)"-1");
        }
        i++;

        if((strcmp(aux->nodo_1,"0"))!=0)
        {
            strcpy(val_actual,(leer_ele_matriz(matrix, i, indice_nodo(indice,aux->nodo_1))));

            //            if (strcmp(val_actual,"0")==0)
            //                strcpy(val_actual,"");
            strcpy(val_nuevo,(char *)"+1");
            x=i;
            y=indice_nodo(indice,aux->nodo_1);
            asig_matriz_din(matrix, y, x,val_nuevo);
            //imprime_matrix("Matriz B 3",matrix,num_nodos,num_v+1);
        }

        if((strcmp(aux->nodo_2,"0"))!=0)
        {
            strcpy(val_actual,(leer_ele_matriz(matrix,i,indice_nodo(indice,aux->nodo_2))));
            if (strcmp(val_actual,"0")==0)
                strcpy(val_actual,"0");
            strcpy(val_nuevo,(char *)"-1");
            x=i;
            y=indice_nodo(indice,aux->nodo_2);
            asig_matriz_din(matrix, y, x,val_nuevo);
            //imprime_matrix("Matriz B 4",matrix,num_nodos,num_v+1);
        }

        if (aux->sig != NULL)
        {
            aux=(struct REGISTRO_ELEMENTO *)aux->sig;
            i++;
        }
    }

    if(aux->sig ==NULL)
    {

        if((strcmp(aux->nodo_3,"0"))!=0)
        {
            x=indice_nodo(indice,aux->nodo_3);
            y=i;
            asig_matriz_din(matrix, x, y,(char *)"+1");
        }

        if((strcmp(aux->nodo_4,"0"))!=0)
        {
            x=indice_nodo(indice,aux->nodo_4);
            y=i;
            asig_matriz_din(matrix, x, y,(char *)"-1");
        }
        i++;

        if((strcmp(aux->nodo_1,"0"))!=0)
        {
            strcpy(val_actual,(leer_ele_matriz(matrix, i, indice_nodo(indice,aux->nodo_1))));

            //            if (strcmp(val_actual,"0")==0)
            //                strcpy(val_actual,"");
            strcpy(val_nuevo,(char *)"+1");
            x=i;
            y=indice_nodo(indice,aux->nodo_1);
            asig_matriz_din(matrix, y, x,val_nuevo);
            //imprime_matrix("Matriz B 3",matrix,num_nodos,num_v+1);
        }

        if((strcmp(aux->nodo_2,"0"))!=0)
        {
            strcpy(val_actual,(leer_ele_matriz(matrix,i,indice_nodo(indice,aux->nodo_2))));
            if (strcmp(val_actual,"0")==0)
                strcpy(val_actual,"0");
            strcpy(val_nuevo,(char *)"-1");
            x=i;
            y=indice_nodo(indice,aux->nodo_2);
            asig_matriz_din(matrix, y, x,val_nuevo);
            //imprime_matrix("Matriz B 4",matrix,num_nodos,num_v+1);
        }
    }
}

void asig_matriz_BN(struct MATRIZ **matrix, struct REGISTRO_ELEMENTO *elementos, struct REGISTRO_NODO **indice, int num_v)
{
    int i=1;
    int x,y;
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
            sprintf(numero, "%s-%s",val_actual,val_nuevo);
            x=num_v+i;
            y=indice_nodo(indice,aux->nodo_1);
            asig_matriz_din(matrix, y, x,numero);
            //imprime_matrix("Matriz B 1",matrix,num_nodos,num_v+1);
        }

        if((strcmp(aux->nodo_2,"0"))!=0)
        {
            strcpy(val_actual,(leer_ele_matriz(matrix,num_v+i, indice_nodo(indice,aux->nodo_2))));

            //            if (strcmp(val_actual,"0")==0)
            //                strcpy(val_actual,"");
            strcpy(val_nuevo,(aux->valor));
            sprintf(numero, "%s+%s",val_actual,val_nuevo);
            x=num_v+i;
            y=indice_nodo(indice,aux->nodo_2);
            asig_matriz_din(matrix, y, x,numero);
            //imprime_matrix("Matriz B 2",matrix,num_nodos,num_v+1);
        }

        if((strcmp(aux->nodo_3,"0"))!=0)
        {
            strcpy(val_actual,(leer_ele_matriz(matrix, num_v+i, indice_nodo(indice,aux->nodo_3))));

            //            if (strcmp(val_actual,"0")==0)
            //                strcpy(val_actual,"");
            strcpy(val_nuevo,(char *)"+1");
            x=num_v+i;
            y=indice_nodo(indice,aux->nodo_3);
            //            asig_matriz_din(matrix, num_v+i, indice_nodo(indice,aux->nodo_2), (char *)"-1");
            //            imprime_matrix(matrix, num_v+num_VCVS, num_nodos);
            asig_matriz_din(matrix, y, x,val_nuevo);
            //imprime_matrix("Matriz B 3",matrix,num_nodos,num_v+1);
        }

        if((strcmp(aux->nodo_4,"0"))!=0)
        {
            strcpy(val_actual,(leer_ele_matriz(matrix,num_v+i,indice_nodo(indice,aux->nodo_4))));
            if (strcmp(val_actual,"0")==0)
                strcpy(val_actual,"0");
            strcpy(val_nuevo,(char *)"-1");
            x=num_v+i;
            y=indice_nodo(indice,aux->nodo_4);
            asig_matriz_din(matrix, y, x,val_nuevo);
            //imprime_matrix("Matriz B 4",matrix,num_nodos,num_v+1);
        }

        if (aux->sig != NULL){
            aux=(struct REGISTRO_ELEMENTO *)aux->sig;
            i++;
        }
    }
    strcpy(val_nuevo, "0");

    if(aux->sig ==NULL)
    {
        strcpy(val_actual, "0");
        strcpy(numero, "0");
        if((strcmp(aux->nodo_1,"0"))!=0)
        {
            //strcpy(val_actual,(leer_ele_matriz(matrix,num_v+i, indice_nodo(indice,aux->nodo_3))));

            if (strcmp(val_actual,"0")==0)
                strcpy(val_actual,"0");

            strcpy(val_nuevo,(aux->valor));
            sprintf(numero, "%s-%s",val_actual,val_nuevo);
            x=num_v+i;
            y=indice_nodo(indice,aux->nodo_1);
            asig_matriz_din(matrix, y, x,numero);
            //imprime_matrix("Matriz B 1",matrix,num_nodos,num_v+1);
        }

        if((strcmp(aux->nodo_2,"0"))!=0)
        {
            //strcpy(val_actual,(leer_ele_matriz(matrix,num_v+i, indice_nodo(indice,aux->nodo_4))));
            strcpy(val_nuevo,(aux->valor));
            sprintf(numero, "%s+%s",val_actual,val_nuevo);
            x=num_v+i;
            y=indice_nodo(indice,aux->nodo_2);
            asig_matriz_din(matrix, y, x,numero);
            //imprime_matrix("Matriz B 2",matrix,num_nodos,num_v+1);
        }

        if((strcmp(aux->nodo_3,"0"))!=0)
        {
            strcpy(val_actual,(leer_ele_matriz(matrix, num_v+i, indice_nodo(indice,aux->nodo_3))));
            strcpy(val_nuevo,(char *)"+1");
            x=num_v+i;
            y=indice_nodo(indice,aux->nodo_3);
            //            asig_matriz_din(matrix, num_v+i, indice_nodo(indice,aux->nodo_2), (char *)"-1");
            //            imprime_matrix(matrix, num_v+num_VCVS, num_nodos);
            asig_matriz_din(matrix, y, x,val_nuevo);
            //imprime_matrix("Matriz B 3",matrix,num_nodos,num_v+1);
        }

        if((strcmp(aux->nodo_4,"0"))!=0){
            strcpy(val_actual,(leer_ele_matriz(matrix,num_v+i,indice_nodo(indice,aux->nodo_4))));
            if (strcmp(val_actual,"0")==0)
                strcpy(val_actual,"0");
            strcpy(val_nuevo,(char *)"-1");
            x=num_v+i;
            y=indice_nodo(indice,aux->nodo_4);
            asig_matriz_din(matrix, y, x,val_nuevo);
            //imprime_matrix("Matriz B 4",matrix,num_nodos,num_v+1);
        }
    }
}

//asig_matriz_KN
void asig_matriz_BK(struct MATRIZ **matrix, struct REGISTRO_ELEMENTO *elementos, struct REGISTRO_NODO **indice, int num_v,struct REGISTRO_ELEMENTO *eleme)
{
    int i=num_v+1;
    int x,y;
    char val_actual [50],val_nuevo [50];
    static struct REGISTRO_ELEMENTO *aux,*band_elemento;

    if (*matrix==NULL)
        return;

    aux=elementos; //Se para en el primero
    if (aux==NULL)
        return;

    while(aux->sig != NULL )
    {
        if(aux->sig ==NULL)
        {
            //Buscar el elemento guardado o indicado en nodo_1
            band_elemento = busca_elemento2(eleme,aux->nodo_1);
            if(band_elemento != NULL)
            {
                //buscar lo que existe en la matriz dinde esta conectada L1
                if(strcmp(aux->nodo_1,"0")!=0 && strcmp(aux->nodo_2,"0")!=0)
                {
                    strcpy(val_actual,(leer_ele_matriz(matrix, i, indice_nodo(indice,band_elemento->nodo_1))));
                    strcpy(val_nuevo,(char *)"+1");
                    x=i;
                    y=indice_nodo(indice,band_elemento->nodo_1);
                    asig_matriz_din(matrix, y, x,val_nuevo);

                    //buscar lo que existe en la matriz dinde esta conectada L1
                    strcpy(val_actual,(leer_ele_matriz(matrix, i, indice_nodo(indice,band_elemento->nodo_2))));
                    strcpy(val_nuevo,(char *)"-1");
                    x=i;
                    y=indice_nodo(indice,band_elemento->nodo_2);
                    asig_matriz_din(matrix, y, x,val_nuevo);
                }
            }
            i++;

            band_elemento = busca_elemento2(eleme,aux->nodo_2);
            if(band_elemento != NULL)
            {
                //buscar lo que existe en la matriz dinde esta conectada L2
                if(strcmp(aux->nodo_1,"0")!=0 && strcmp(aux->nodo_2,"0")!=0)
                {
                    strcpy(val_actual,(leer_ele_matriz(matrix, i, indice_nodo(indice,band_elemento->nodo_2))));
                    strcpy(val_nuevo,(char *)"+1");
                    x=i;
                    y=indice_nodo(indice,band_elemento->nodo_2);
                    asig_matriz_din(matrix, y, x,val_nuevo);

                    //buscar lo que existe en la matriz dinde esta conectada L2
                    strcpy(val_actual,(leer_ele_matriz(matrix, i, indice_nodo(indice,band_elemento->nodo_2))));
                    if (strcmp(val_actual,"0")==0)
                        strcpy(val_actual,"0");
                    strcpy(val_nuevo,(char *)"-1");
                    x=i;
                    y=indice_nodo(indice,band_elemento->nodo_2);
                    asig_matriz_din(matrix, y, x,val_nuevo);
                }
            }
        }

        if (aux->sig != NULL)
        {
            aux=(struct REGISTRO_ELEMENTO *)aux->sig;
            i++;
        }
    }

    if(aux->sig ==NULL)
    {
        //Buscar el elemento guardado o indicado en nodo_1
        band_elemento = busca_elemento2(eleme,aux->nodo_1);
        if(band_elemento != NULL)
        {
            //buscar lo que existe en la matriz donde esta conectada L1
            if(strcmp(band_elemento->nodo_1,"0")!=0)
            {
                strcpy(val_actual,(leer_ele_matriz(matrix, i, indice_nodo(indice,band_elemento->nodo_1))));
                strcpy(val_nuevo,(char *)"+1");
                x=i;
                y=indice_nodo(indice,band_elemento->nodo_1);
                asig_matriz_din(matrix, y, x,val_nuevo);
            }

            //buscar lo que existe en la matriz donde esta conectada L1
            if(strcmp(band_elemento->nodo_2,"0")!=0)
            {
                strcpy(val_actual,(leer_ele_matriz(matrix, i,indice_nodo(indice,band_elemento->nodo_2))));
                if (strcmp(val_actual,"0")==0)
                    strcpy(val_actual,"0");
                strcpy(val_nuevo,(char *)"-1");
                x=i;
                y=indice_nodo(indice,band_elemento->nodo_2);
                asig_matriz_din(matrix, y, x,val_nuevo);
            }
        }
        i++;
        band_elemento = busca_elemento2(eleme,aux->nodo_2);

        if(band_elemento != NULL)
        {
            //buscar lo que existe en la matriz dinde esta conectada L2
            if(strcmp(band_elemento->nodo_1,"0")!=0)
            {
                strcpy(val_actual,(leer_ele_matriz(matrix, i, indice_nodo(indice,band_elemento->nodo_1))));
                strcpy(val_nuevo,(char *)"+1");
                x=i;
                y=indice_nodo(indice,band_elemento->nodo_1);
                asig_matriz_din(matrix, y, x,val_nuevo);
            }

            //buscar lo que existe en la matriz dinde esta conectada L2
            if(strcmp(band_elemento->nodo_2,"0")!=0)
            {
                strcpy(val_actual,(leer_ele_matriz(matrix, i, indice_nodo(indice,band_elemento->nodo_2))));
                if (strcmp(val_actual,"0")==0)
                    strcpy(val_actual,"0");
                strcpy(val_nuevo,(char *)"-1");
                x=i;
                y=indice_nodo(indice,band_elemento->nodo_2);
                asig_matriz_din(matrix, y, x,val_nuevo);
            }
        }
    }
}
