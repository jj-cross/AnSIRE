#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include "matrix.h"
#include "cadenas.h"
#include "elementos.h"
#include "gauss.h"
#include "matho.h"

void ini_matrix_simb(struct MATRIZ **matrix, int f, int c)
{

    int i;
    int j;

    for(i=0; i < f; i++)
    {
        for(j=0; j < c; j++)
        {
            strcpy( &(*(matrix[i]+j)->expresion),"0");
        }
    }
    //    imprime_matrix(matrix,f,c);
}

struct MATRIZ **crear_matriz_din(struct MATRIZ **matrix, int f, int c)
{
    int i;

    matrix=(struct MATRIZ **)calloc(f,sizeof(struct MATRIZ *));
    if(matrix==NULL){
        printf("No hay memoria disponible para crear la matriz \n");
        getch();
        exit(12);
    }

    for( i=0; i < f; i++)
    {
        matrix[i]=(struct MATRIZ *)calloc(c,sizeof(struct MATRIZ));
        if(matrix[i]==NULL){
            printf("No hay memoria disponible para crear la matriz \n");
            getch();
            exit(12);
        }
    }
    return matrix;
}

void asig_matriz_din(struct MATRIZ **matrix, int i, int j, char *valor)
{
    strcpy( &(*(matrix[i-1]+j-1)->expresion),valor);
}

char * leer_ele_matriz(struct MATRIZ **matrix, int i, int j)
{

    return &(*(matrix[i-1]+j-1)->expresion);
}

void ximprime_matrix(char *titulo, struct MATRIZ **matrix, int f, int c)
{
    int i;
    int j;
    char *indice;
    printf("\n\n%s\n",titulo);

    for(i=0; i < f; i++)
    {
        for(j=0; j < c; j++)
        {
            indice=&(*(matrix[i]+j)->expresion);
            printf("%20s ", indice);
        }
        printf("\n");
    }
    printf("\n");
}

void asig_matriz_A(struct MATRIZ **A ,struct MATRIZ **G, struct MATRIZ **B, struct MATRIZ **C, struct MATRIZ **D, int num_nodos, int num_v)
{
    int i=1;
    int j=1;

    for(i=1; i <=num_nodos; i++){
        for(j=1; j <=num_nodos; j++){

            asig_matriz_din(A, i, j, leer_ele_matriz(G, i, j));
        }
    }

    for(i=1; i <=num_nodos; i++){
        for(j=num_nodos+1; j <=num_nodos+num_v; j++){

            asig_matriz_din(A, i, j, leer_ele_matriz(B, i, j-num_nodos));
        }
    }

    for(i=num_nodos+1; i <=num_nodos+num_v; i++){
        for(j=1; j <=num_nodos; j++){

            asig_matriz_din(A, i, j, leer_ele_matriz(C, i-num_nodos, j));
        }
    }

    for(i=num_nodos+1; i <=num_nodos+num_v; i++){
        for(j=num_nodos+1; j <=num_nodos+num_v; j++){

            asig_matriz_din(A, i, j, leer_ele_matriz(D, i-num_nodos, j-num_nodos));
        }
    }


}

void asig_matriz_x(struct MATRIZ **x ,struct MATRIZ **v, struct MATRIZ **j, int num_nodos, int num_v)
{
    int i=1;
    int m=1;

    for(i=1; i <=num_nodos; i++){
        asig_matriz_din(x, i, m, leer_ele_matriz(v, i, m));
    }

    for(i=num_nodos+1; i <=num_nodos+num_v; i++){
        asig_matriz_din(x, i, m, leer_ele_matriz(j, i-num_nodos, m));
    }
}

void asig_matriz_G(struct MATRIZ **matrix, int f, int c, struct REGISTRO_ELEMENTO *elementos, struct REGISTRO_NODO **indice, struct REGISTRO_ELEMENTO *K)
{
    int i;
    int j;
    char linea[MAX_LONG_LINEA];

     for(i=1; i <=f; i++)
    {
        for(j=1; j <=c; j++)
        {
            if(i==j)
            {
                char temp_dim [MAX_LONG_LINEA];
                char sd [MAX_LONG_LINEA];
                char sdd [MAX_LONG_LINEA];

                strcpy(sdd,nombre_nodo(indice,i));
                strcpy(sd,busca_valor_dp(linea, elementos,sdd,K));
                strcpy(temp_dim,sd);
                asig_matriz_din(matrix, i, j,temp_dim);

                //strcpy(linea,busca_valor_dp(linea, elementos,nombre_nodo(indice,i)));

            }
            else
            {
                char temp_dim [MAX_LONG_LINEA];
                char GAO [MAX_LONG_LINEA];
                char nodo1 [MAX_LONG_LINEA];
                char nodo2 [MAX_LONG_LINEA];
                char nodo3 [MAX_LONG_LINEA];
                strcpy(nodo1,nombre_nodo(indice,i));
                strcpy(nodo2,nombre_nodo(indice,j));
                strcpy(nodo3,busca_valor_ad(linea, elementos,nodo1,nodo2,K));
                strcpy(GAO,nodo3);
                strcpy(temp_dim,GAO);
                asig_matriz_din(matrix, i, j,temp_dim);
                //strcpy(linea,busca_valor_ad(linea, elementos,nombre_nodo(indice,i),nombre_nodo(indice,j)));
            }
        }
    }

    //
    for(i=1; i <=f; i++)
    {
        for(j=1; j <=c; j++)
        {
            char temp_dim [MAX_LONG_LINEA];
            char temp_dim1 [MAX_LONG_LINEA];
            strcpy(temp_dim,leer_ele_matriz(matrix, i, j));
            if(strcmp(temp_dim,"0")!=0)
            {
                strcpy(temp_dim1,matho2(temp_dim));
                asig_matriz_din(matrix, i, j,temp_dim1);
            }
        }
    }

    //

}

void asig_matriz_e(struct MATRIZ **matrix, struct REGISTRO_ELEMENTO *elementos, int num_v)
{
    int i=1;
    int j=1;
    char numero[12];
    static struct REGISTRO_ELEMENTO *aux;
    //char aux1[30];
    char tem[30];
    strcpy(tem,"");
    aux=elementos; //Se para en el primero

    for(i=1; i <=num_v; i++){
        if(strcmp(aux->valor,"SYMBOLIC")==0)
            sprintf(numero,aux->elemento);
        else
        {
            sprintf(numero,"%g",aux->valornum);
//            strcat(numero,aux1);
        }
        //  sprintf(numero, "%f",valor);
        asig_matriz_din(matrix, i, j,numero);
        if(aux->sig !=NULL)
            aux=(struct REGISTRO_ELEMENTO *)aux->sig;
    }
}

void asig_matriz_z(struct MATRIZ **z , struct MATRIZ **i, struct MATRIZ **e, int num_nodos, int num_v)
{
    int n=1;
    int m=1;

    for(n=1; n <=num_nodos; n++){
        asig_matriz_din(z, n, m, leer_ele_matriz(i, n, m));
    }

    for(n=num_nodos+1; n <=num_nodos+num_v; n++){
        asig_matriz_din(z, n, m, leer_ele_matriz(e, n-num_nodos, m));
    }
}

void asig_matriz_v(struct MATRIZ **matrix, struct REGISTRO_NODO **indice, int num_nodos)
{
    int i=1;
    int j=1;
    char numero[10];
    static struct REGISTRO_NODO **aux; //Se para en el primero
    aux=indice;
    for(i=1; i <=num_nodos; i++){
        sprintf(numero, "V(%s)",(*aux)->nodo);
        asig_matriz_din(matrix, i, j,numero);
        aux++;
    }
}

void asig_matriz_i(struct MATRIZ **matrix, struct REGISTRO_ELEMENTO *elemento, struct REGISTRO_NODO **indice)
{

    char numero[16];
    static struct REGISTRO_ELEMENTO *aux;
    char val_actual [50],val_nuevo [50];
    //   float val_actual,val_nuevo;
    int i;
    char aux1[30];
    char tem[30];

    aux=elemento; //Se para en el primero
    if(aux==NULL)
        return;
    //Se suman las corrietes que llegan o salen del nodo

    do{
        if((strcmp(aux->nodo_1,"0"))!=0){

            i=indice_nodo(indice,aux->nodo_1);

            strcpy(val_actual,(leer_ele_matriz(matrix, i, 1)));
            if (strcmp(val_actual,"0")==0)
                strcpy(val_actual,"");

            if(strcmp(aux->valor,"SYMBOLIC")==0)
                strcpy(val_nuevo,(aux->elemento));
            else
            {
                sprintf(aux1,"%g",aux->valornum);
                strcat(tem,aux1);
            }

            sprintf(numero, "%s0+%s",val_actual,val_nuevo);
            asig_matriz_din(matrix, i, 1,numero);
        }
        if((strcmp(aux->nodo_2,"0"))!=0){
            i=indice_nodo(indice,aux->nodo_2);
            strcpy(val_actual,(leer_ele_matriz(matrix, i, 1)));
            if (strcmp(val_actual,"0")==0)
                strcpy(val_actual,"");
            if(strcmp(aux->valor,"SYMBOLIC")==0)
                strcpy(val_nuevo,(aux->elemento));
            else
            {
                sprintf(aux1,"%g",aux->valornum);
                strcat(tem,aux1);
            }
            sprintf(numero, "%s0-%s",val_actual,val_nuevo);
            asig_matriz_din(matrix, i, 1,numero);
        }
        if(aux->sig !=NULL )
            aux=(struct REGISTRO_ELEMENTO *)aux->sig;
    }while(aux->sig !=NULL );
    if(aux->sig !=NULL ){
        if((strcmp(aux->nodo_1,"0"))!=0){
            i=indice_nodo(indice,aux->nodo_1);
            strcpy(val_actual,(leer_ele_matriz(matrix, i, 1)));
            if (strcmp(val_actual,"0")==0)
                strcpy(val_actual,"");

            if(strcmp(aux->valor,"SYMBOLIC")==0)
                strcpy(val_nuevo,(aux->elemento));
            else
            {
                sprintf(aux1,"%g",aux->valornum);
                strcat(tem,aux1);;
            }

            sprintf(numero, "%s+0+%s",val_actual,val_nuevo);
            asig_matriz_din(matrix, i, 1,numero);
        }
        if((strcmp(aux->nodo_2,"0"))!=0){
            i=indice_nodo(indice,aux->nodo_2);
            strcpy(val_actual,(leer_ele_matriz(matrix, i, 1)));
            if (strcmp(val_actual,"0")==0)
                strcpy(val_actual,"");

            if(strcmp(aux->valor,"SYMBOLIC")==0)
                strcpy(val_nuevo,(aux->elemento));
            else
            {
                sprintf(aux1,"%g",aux->valornum);
                strcat(tem,aux1);
            }

            sprintf(numero, "%s+0-%s",val_actual,val_nuevo);
            asig_matriz_din(matrix, i, 1,numero);
        }
    }
}

void asig_matriz_VCCS(struct MATRIZ **matrix, struct REGISTRO_ELEMENTO *elementos, struct REGISTRO_NODO **indice)
{
    int i=1;
    static struct REGISTRO_ELEMENTO *aux;
    int x,y;
    char val_actual [50],val_nuevo [50];
    char numero[16];
    if(elementos==NULL)
        return;

    aux=elementos; //Se para en el primero


    do{

        if(strcmp(aux->nodo_1,"0")!=0 && strcmp(aux->nodo_3,"0")!=0)
        {
            x=indice_nodo(indice,aux->nodo_1);
            y=indice_nodo(indice,aux->nodo_3);
            strcpy(val_actual,(leer_ele_matriz(matrix,x, y)));

            if (strcmp(val_actual,"0")==0)
                strcpy(val_actual,"");
            strcpy(val_nuevo,(aux->valor));
            sprintf(numero, "%s+0-%s",val_actual,val_nuevo);
            asig_matriz_din(matrix, x, y,numero);
        }

        if(strcmp(aux->nodo_1,"0")!=0 && strcmp(aux->nodo_4,"0")!=0)
        {
            x=indice_nodo(indice,aux->nodo_1);
            y=indice_nodo(indice,aux->nodo_4);
            strcpy(val_actual,(leer_ele_matriz(matrix,x, y)));

            if (strcmp(val_actual,"0")==0)
                strcpy(val_actual,"");
            strcpy(val_nuevo,(aux->valor));
            sprintf(numero, "%s+0+%s",val_actual,val_nuevo);
            asig_matriz_din(matrix, x, y,numero);
        }

        if(strcmp(aux->nodo_2,"0")!=0 && strcmp(aux->nodo_3,"0")!=0)
        {
            x=indice_nodo(indice,aux->nodo_2);
            y=indice_nodo(indice,aux->nodo_3);
            strcpy(val_actual,(leer_ele_matriz(matrix,x, y)));

            if (strcmp(val_actual,"0")==0)
                strcpy(val_actual,"");
            strcpy(val_nuevo,(aux->valor));
            sprintf(numero, "%s+0+%s",val_actual,val_nuevo);
            asig_matriz_din(matrix, x, y,numero);
        }

        if(strcmp(aux->nodo_2,"0")!=0 && strcmp(aux->nodo_4,"0")!=0)
        {
            x=indice_nodo(indice,aux->nodo_2);
            y=indice_nodo(indice,aux->nodo_4);
            strcpy(val_actual,(leer_ele_matriz(matrix,x, y)));

            if (strcmp(val_actual,"0")==0)
                strcpy(val_actual,"");
            strcpy(val_nuevo,(aux->valor));
            sprintf(numero, "%s+0-%s",val_actual,val_nuevo);
            asig_matriz_din(matrix, x, y,numero);
        }

        if (aux->sig != NULL){
            aux=(struct REGISTRO_ELEMENTO *)aux->sig;
            i++;
        }
    } while(aux->sig !=NULL );

    //    if(strcmp(aux->nodo_1,"0")!=0 && strcmp(aux->nodo_3,"0")!=0)
    //    {
    //        x=indice_nodo(indice,aux->nodo_1);
    //        y=indice_nodo(indice,aux->nodo_3);
    //        strcpy(val_actual,(leer_ele_matriz(matrix,x, y)));

    //        if (strcmp(val_actual,"0")==0)
    //            strcpy(val_actual,"");
    //        strcpy(val_nuevo,(aux->valor));
    //        sprintf(numero, "%s+%s",val_actual,val_nuevo);
    //        asig_matriz_din(matrix, x, y,numero);
    //    }

    //    if(strcmp(aux->nodo_1,"0")!=0 && strcmp(aux->nodo_4,"0")!=0)
    //    {
    //        x=indice_nodo(indice,aux->nodo_1);
    //        y=indice_nodo(indice,aux->nodo_4);
    //        strcpy(val_actual,(leer_ele_matriz(matrix,x, y)));

    //        if (strcmp(val_actual,"0")==0)
    //            strcpy(val_actual,"");
    //        strcpy(val_nuevo,(aux->valor));
    //        sprintf(numero, "%s-%s",val_actual,val_nuevo);
    //        asig_matriz_din(matrix, x, y,numero);
    //    }

    //    if(strcmp(aux->nodo_2,"0")!=0 && strcmp(aux->nodo_3,"0")!=0)
    //    {
    //        x=indice_nodo(indice,aux->nodo_2);
    //        y=indice_nodo(indice,aux->nodo_3);
    //        strcpy(val_actual,(leer_ele_matriz(matrix,x, y)));

    //        if (strcmp(val_actual,"0")==0)
    //            strcpy(val_actual,"");
    //        strcpy(val_nuevo,(aux->valor));
    //        sprintf(numero, "%s-%s",val_actual,val_nuevo);
    //        asig_matriz_din(matrix, x, y,numero);
    //    }

    //    if(strcmp(aux->nodo_2,"0")!=0 && strcmp(aux->nodo_4,"0")!=0)
    //    {
    //        x=indice_nodo(indice,aux->nodo_2);
    //        y=indice_nodo(indice,aux->nodo_4);
    //        strcpy(val_actual,(leer_ele_matriz(matrix,x, y)));

    //        if (strcmp(val_actual,"0")==0)
    //            strcpy(val_actual,"");
    //        strcpy(val_nuevo,(aux->valor));
    //        sprintf(numero, "%s+%s",val_actual,val_nuevo);
    //        asig_matriz_din(matrix, x, y,numero);
    //    }

}

void asig_matriz_gauss(struct MATRIZ **matrix ,struct MATRIZ **A ,struct MATRIZ **z, int num_nodos)
{
    int i=1;
    int j=1;
    char x[MAX_LONG_LINEA], y[MAX_LONG_LINEA];

    for(i=1; i <=num_nodos; i++){
        for(j=1; j <=num_nodos; j++){

            strcpy(x, leer_ele_matriz(A, i, j));
            asig_matriz_din(matrix, i, j, x);
        }
    }

    for(i=1; i <=num_nodos; i++){
        //            asig_matriz_din(z, i, j, leer_ele_matriz(z, i, j));

        strcpy(y, leer_ele_matriz(z, i, j-num_nodos));
        asig_matriz_din(matrix, i, j, y);

    }

#ifdef NUMERO
    //    asig_matriz_din(matrix, 1, 1, "2");
    //    asig_matriz_din(matrix, 1, 2, "1");
    //    asig_matriz_din(matrix, 1, 3, "7");

    //    asig_matriz_din(matrix, 2, 1, "1");
    //    asig_matriz_din(matrix, 2, 2, "3");
    //    asig_matriz_din(matrix, 2, 3, "11");

    // --
    //    asig_matriz_din(matrix, 1, 1, "1");
    //    asig_matriz_din(matrix, 1, 2, "2");
    //    asig_matriz_din(matrix, 1, 3, "9");

    //    asig_matriz_din(matrix, 2, 1, "3");
    //    asig_matriz_din(matrix, 2, 2, "-1");
    //    asig_matriz_din(matrix, 2, 3, "20");

    //--
    asig_matriz_din(matrix, 1, 1, "5");
    asig_matriz_din(matrix, 1, 2, "3");
    asig_matriz_din(matrix, 1, 3, "1");
    asig_matriz_din(matrix, 1, 4, "2");
    asig_matriz_din(matrix, 1, 5, "1");

    asig_matriz_din(matrix, 2, 1, "2");
    asig_matriz_din(matrix, 2, 2, "-1");
    asig_matriz_din(matrix, 2, 3, "3");
    asig_matriz_din(matrix, 2, 4, "1");
    asig_matriz_din(matrix, 2, 5, "2");

    asig_matriz_din(matrix, 3, 1, "-3");
    asig_matriz_din(matrix, 3, 2, "2");
    asig_matriz_din(matrix, 3, 3, "-2");
    asig_matriz_din(matrix, 3, 4, "3");
    asig_matriz_din(matrix, 3, 5, "3");

    asig_matriz_din(matrix, 4, 1, "2");
    asig_matriz_din(matrix, 4, 2, "5");
    asig_matriz_din(matrix, 4, 3, "-1");
    asig_matriz_din(matrix, 4, 4, "5");
    asig_matriz_din(matrix, 4, 5, "4");

#endif
}

void asig_matriz_gauss1(struct MATRIZ **matrix ,struct MATRIZ **A, int num_nodos)
{
    int i=1;
    int j=1;
    char x[MAX_LONG_LINEA];

    for(i=1; i <=num_nodos; i++){
        for(j=1; j <=num_nodos; j++){

            strcpy(x, leer_ele_matriz(A, i, j));
            asig_matriz_din(matrix, i, j, x);
        }
    }


#ifdef NUMERO
    //    asig_matriz_din(matrix, 1, 1, "2");
    //    asig_matriz_din(matrix, 1, 2, "1");
    //    asig_matriz_din(matrix, 1, 3, "7");

    //    asig_matriz_din(matrix, 2, 1, "1");
    //    asig_matriz_din(matrix, 2, 2, "3");
    //    asig_matriz_din(matrix, 2, 3, "11");

    // --
    //    asig_matriz_din(matrix, 1, 1, "1");
    //    asig_matriz_din(matrix, 1, 2, "2");
    //    asig_matriz_din(matrix, 1, 3, "9");

    //    asig_matriz_din(matrix, 2, 1, "3");
    //    asig_matriz_din(matrix, 2, 2, "-1");
    //    asig_matriz_din(matrix, 2, 3, "20");

    //--
    asig_matriz_din(matrix, 1, 1, "2");
    asig_matriz_din(matrix, 1, 2, "1");
    asig_matriz_din(matrix, 1, 3, "-2");

    asig_matriz_din(matrix, 2, 1, "1");
    asig_matriz_din(matrix, 2, 2, "1");
    asig_matriz_din(matrix, 2, 3, "-2");

    asig_matriz_din(matrix, 3, 1, "-1");
    asig_matriz_din(matrix, 3, 2, "0");
    asig_matriz_din(matrix, 3, 3, "1");


#endif
}

void imprime_matrix_arch(char *titulo, struct MATRIZ **matrix, int f, int c, FILE *salida)
{
    int i;
    int j;
    char *indice;
    char pantalla[MAX_LONG_LINEA];

    sprintf(pantalla,"\n\n%s\n",titulo);
    fputs(pantalla,salida);
    printf("%s",pantalla);

    for(i=0; i < f; i++)
    {
        for(j=0; j < c; j++)
        {
            indice=&(*(matrix[i]+j)->expresion);
            sprintf(pantalla,"%10s ", indice);
            fputs(pantalla,salida);
            printf("%s",pantalla);
        }
        sprintf(pantalla,"\n");
        fputs(pantalla,salida);
        printf("%s",pantalla);
    }
    sprintf(pantalla,"\n");
    fputs(pantalla,salida);
    printf("%s",pantalla);
}
