#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "gauss.h"
#include "elementos.h"
#include "matrix.h"
#include "cadenas.h"
#include "matho.h"

//struct MATRIZ **gauss(struct MATRIZ **matrix,struct MATRIZ **mat_gauss, int num_nodos)
struct MATRIZ **gauss(struct MATRIZ **matx, struct MATRIZ **mat_gauss, int num_nodos, FILE *salida)
{
    int k,i,w,var;

    var=num_nodos;

    char h[MAX_LONG_LINEA],
            gg[MAX_LONG_LINEA],
            pantalla[MAX_LONG_LINEA];
    int c=0;
    char g[MAX_LONG_LINEA];

    //    aux = REGISTRO_nodo;
    FILE *matho_f;
    matho_f = fopen("debug$.in", "w" );

    printf("\n\n");
    if (var!=1)
    {
        for (k = 0; k < var; k++)
        {
            for (i = 0; i < var; i++)
            {
                strcpy(h,leer_ele_matriz(mat_gauss, i+1, k+1));

                for (w = 0; w <= var; w++)
                {
                    if (i != k)
                    {
                        //system("cls");
                        // imprime matriz antes del cambio
                        for (int kk = 0; kk < var; kk++)
                        {
                            for (int ii = 0; ii < var+1; ii++)
                            {
                                printf("%25s ",leer_ele_matriz(mat_gauss, kk+1, ii+1));
                            }
                            printf("\n");
                        }
                        printf("\n\n");

                        c++;
                        sprintf(pantalla,"\n\n%d--------------\n",c);
                        printf("%s",pantalla);
                        fputs(pantalla,matho_f);
                        strcpy(g,"(");
                        strcat(g,leer_ele_matriz(mat_gauss, k+1, k+1));
                        strcat(g,")*");
                        strcat(g,"(");
                        strcat(g,leer_ele_matriz(mat_gauss, i+1, w+1));
                        strcat(g,")-(");
                        strcat(g,h);
                        strcat(g,")*(");
                        strcat(g,leer_ele_matriz(mat_gauss, k+1, w+1));
                        strcat(g,")");

                        printf("m[%d][%d]=%s\n",k,k,leer_ele_matriz(mat_gauss, k+1, k+1));
                        printf("m[%d][%d]=%s\n",i,w,leer_ele_matriz(mat_gauss, i+1, w+1));
                        printf("h=%s\n",h);
                        printf("m[%d][%d]=%s\n\n",k,w,leer_ele_matriz(mat_gauss, k+1, w+1));

                        sprintf(pantalla,"%d NUEVO VALOR:%d,%d\n%8s",strlen(g), i+1, w+1,g);
                        printf("%s",pantalla);
                        fputs(pantalla,matho_f);
                        strcpy(gg,matho2(g));
                        squeeze(gg,' ');
                        sprintf(pantalla,"\n--> %s",gg);
                        printf("\n");
                        printf("%s\n",pantalla);
                        printf("\n");
                        fputs(pantalla,matho_f);
                        asig_matriz_din(mat_gauss, i+1, w+1,gg);
                        sprintf(pantalla,"%03dfile$.out",c);
                        rename("file$.out",pantalla);
                        // imprime matriz despues del cambio
                        for (int kk = 0; kk < var; kk++)
                        {
                            for (int ii = 0; ii < var+1; ii++)
                            {
                                printf("%25s ",leer_ele_matriz(mat_gauss, kk+1, ii+1));
                            }
                            printf("\n");
                        }
                        printf("\n\n");
                        //fflush(stdin);
                        //getchar();
                    }
                }
                sprintf(pantalla,"\n\n");
                printf("%s",pantalla);
                fputs(pantalla,matho_f);
            }
            elimina_factor_comun(mat_gauss, var);
        }
    }

    for (i = 0; i < var; i++)
    {
        char variable[MAX_LONG_LINEA];
        char variable1[MAX_LONG_LINEA];

        c++;
        printf("\n");
        sprintf(variable,"(%s)/(%s)",leer_ele_matriz(mat_gauss, i+1, var+1) ,leer_ele_matriz(mat_gauss, i+1, i+1));
        //printf("%s",variable);
        fputs(variable,matho_f);
        strcpy(variable1,matho2(variable));
        squeeze(variable1,' ');
        sprintf(pantalla,"\n\n  %s=%s\n\n\n",&(*(matx[i])->expresion),variable1);
        printf("%s",pantalla);
        fputs(pantalla,matho_f);
        sprintf(pantalla,"%04dfile$.out",1000+c);
        rename("file$.out",pantalla);

        strcpy (& (*(matx[i])->otro_dato),variable1);
    }
    sprintf(pantalla,"\n\n");
    printf("%s",pantalla);
    fputs(pantalla,matho_f);

    return (mat_gauss);
}

void gauss1(struct MATRIZ **mat_gauss, int num_nodos, FILE *salida)
{
    int i,j,k,l,n;
    int c=1;

    n=num_nodos;

    char gg[MAX_LONG_LINEA];
    char variable1[MAX_LONG_LINEA];
    char factor[MAX_LONG_LINEA],
            pantalla[MAX_LONG_LINEA];

    for (i=0;i<n;i++)
    {
        strcpy(gg,"1");
        asig_matriz_din(mat_gauss, i+1, i+1+n,gg);
    }

    for(i=1;i<=n;i++)
    {
        if(strcmp(leer_ele_matriz(mat_gauss, i, i),"0")==0)
        {
            for(k=i+1;k<n;k++)
            {
                for(l=1;l<=2*n;l++)
                {
                    strcpy(gg,leer_ele_matriz(mat_gauss, i, l));
                    strcat(gg,"+");
                    strcat(gg,leer_ele_matriz(mat_gauss, k, l));
                    strcpy(variable1,matho2(gg));
                    squeeze(variable1,' ');
                    asig_matriz_din(mat_gauss, i, l,variable1);
                    sprintf(pantalla,"%03dfile$.out",c);
                    rename("file$.out",pantalla);
                    c++;
                }
            }
        }
        strcpy(factor,(leer_ele_matriz(mat_gauss, i, i)));
        for(l=1;l<=2*n;l++)
        {
            strcpy(gg,"(");
            strcat(gg,leer_ele_matriz(mat_gauss, i, l));
            strcat(gg,")/(");
            strcat(gg,factor);
            strcat(gg,")");
            strcpy(variable1,matho2(gg));
            squeeze(variable1,' ');
            asig_matriz_din(mat_gauss, i, l,variable1);
            sprintf(pantalla,"%03dfile$.out",c);
            rename("file$.out",pantalla);
            c++;
        }

        for(j=i;j<=n;j++)
        {
            if(i!=j)
            {
                if(strcmp(leer_ele_matriz(mat_gauss, j, i),"0")!=0)
                {
                    strcpy(gg,"(");
                    strcat(gg,leer_ele_matriz(mat_gauss, i, i));
                    strcat(gg,")/(");
                    strcat(gg,(leer_ele_matriz(mat_gauss, j, i)));
                    strcat(gg,")");
                    strcpy(factor,matho2(gg));
                    squeeze(factor,' ');
                    sprintf(pantalla,"%03dfile$.out",c);
                    rename("file$.out",pantalla);
                    c++;

                    for(l=1;l<=2*n;l++)
                    {
                        strcpy(gg,"(");
                        strcat(gg,factor);
                        strcat(gg,")*(");
                        strcat(gg,(leer_ele_matriz(mat_gauss, j, l)));
                        strcat(gg,")-(");
                        strcat(gg,(leer_ele_matriz(mat_gauss, i, l)));
                        strcat(gg,")");
                        strcpy(variable1,matho2(gg));
                        squeeze(variable1,' ');
                        asig_matriz_din(mat_gauss, j, l,variable1);
                        sprintf(pantalla,"%03dfile$.out",c);
                        rename("file$.out",pantalla);
                        c++;
                    }
                    sprintf(variable1,"%d,%d",j,i);
                    imprime_matrix_arch("yy",mat_gauss,n,2*n,salida);
                }
            }
        }
    }

    imprime_matrix_arch("yy",mat_gauss,n,2*n,salida);

    for(i=n;i>=2;i--)
    {
        for(j=n-1;j>=1;j--)
        {
            if(i!=j)
            {
                if(strcmp(leer_ele_matriz(mat_gauss, j, i),"0")!=0)
                {
                    strcpy(gg,"(");
                    strcat(gg,(leer_ele_matriz(mat_gauss, i, i)));
                    strcat(gg,")/(");
                    strcat(gg,(leer_ele_matriz(mat_gauss, j, i)));
                    strcat(gg,")");
                    strcpy(factor,matho2(gg));
                    squeeze(factor,' ');
                    sprintf(pantalla,"%03dfile$.out",c);
                    rename("file$.out",pantalla);
                    c++;

                    for(l=1;l<=2*n;l++)
                    {
                        strcpy(gg,"(");
                        strcat(gg,leer_ele_matriz(mat_gauss, j, l));
                        strcat(gg,")-(");
                        strcat(gg,leer_ele_matriz(mat_gauss, i, l));
                        strcat(gg,")/(");
                        strcat(gg,factor);
                        strcat(gg,")");
                        strcpy(variable1,matho2(gg));
                        squeeze(variable1,' ');
                        asig_matriz_din(mat_gauss, j, l,variable1);
                        sprintf(pantalla,"%03dfile$.out",c);
                        rename("file$.out",pantalla);
                        c++;
                    }
                    sprintf(variable1,"%d,%d",j,i);
                    imprime_matrix_arch("yy",mat_gauss,n,2*n,salida);
                }
            }
        }
    }
}

void squeeze(char s[], char c)
{
    unsigned int i, j;
    for(i = j = 0; s[i] != '\0'; i++)
    {
        if(s[i] != c)
        {
            s[j++] = s[i];
        }
    }
    s[j] = '\0';
}

void elimina_factor_comun(struct MATRIZ **mat_gauss, int var)
{
    char variable11[MAX_LONG_LINEA],variable22[MAX_LONG_LINEA],factor[MAX_LONG_LINEA];
    char variable1[MAX_LONG_LINEA],variable2[MAX_LONG_LINEA];
    char cero[MAX_LONG_LINEA];
    int ncero;

//    FILE *matho_f;
//    matho_f = fopen("debug$.in", "w" );

    // elimina factores comunes
    for (int kkk = 0; kkk < var; kkk++)
    {
        ncero=0;
        for (int iii = 0; iii < var+1; iii++)
        {
            strcpy(cero,leer_ele_matriz(mat_gauss, kkk+1, iii+1));
            if(strcmp(cero,"0")==0)
                ncero++;
        }
        if((var+1-ncero)==2)
        {
            int inter=0;
            for (int iii = 0; iii < var+1; iii++)
            {
                strcpy(cero,leer_ele_matriz(mat_gauss, kkk+1, iii+1));
                if(strcmp(cero,"0")!=0)
                {
                    if (inter==0)
                    {
                        strcpy(variable1,leer_ele_matriz(mat_gauss, kkk+1, iii+1));
                        inter=1;
                    }else
                        strcpy(variable2,leer_ele_matriz(mat_gauss, kkk+1, iii+1));
                }
            }
            printf("%s\n",variable1);
            printf("%s\n",variable2);
            strcpy(variable22,matho3(variable1,variable2));
            strcpy(variable11,matho3(variable2,variable1));
            if(strcmp(variable1,"1")!=0)
            {
                if(strlen(variable2)>strlen(variable1))
                {
                    //strcpy(factor,variable1);
                    strcpy(factor,"(");
                    strcat(factor,variable1);
                    strcat(factor,")");
                }
            }

            if((strcmp(variable2,"1")!=0)||(strcmp(variable2,"-1")!=0))
            {
                if(strlen(variable1)>strlen(variable2))
                {
                    strcpy(factor,"(");
                    strcat(factor,variable2);
                    strcat(factor,")");
                }
                else
                {
                    strcpy(factor,"1");
                }
            }
            printf("%s\n\n",factor);
            for (int iii = 0; iii < var+1; iii++)
            {
                strcpy(cero,leer_ele_matriz(mat_gauss, kkk+1, iii+1));
                strcpy(variable22,matho4(factor,cero));
                asig_matriz_din(mat_gauss,kkk+1, iii+1, variable22);
            }
        }
    }
}
