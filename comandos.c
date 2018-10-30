#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>

#include"comandos.h"
#include "cadenas.h"
#include "elementos.h"
#include "matrix.h"
#include "gauss.h"
#include "matho.h"
#include "DEF.H"
#include "laplace.h"

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
                   FILE *salida)
{
    struct REGISTRO_ELEMENTO *band_elemento;
    struct REGISTRO_ELEMENTO *aux;
    struct REGISTRO_ELEMENTO *aux1;
    int i,npuntos,m;
    double valpas,valfin;
    double tiempo;
    char *ress;
    char resptf[MAX_LONG_LINEA];
    char respimp[MAX_LONG_LINEA];
    char res[MAX_LONG_LINEA];
    char temp2[MAX_LONG_LINEA];
    char temp3[MAX_LONG_LINEA];
    char temp4[25];
    char tiempo1[12];
    char pantalla[MAX_LONG_LINEA];
    char temp1[15];
    char ini[10];
    char fin[10];

    aux=comandos;

    if(aux!=NULL)
    {
        while (aux->sig != NULL)
        {
            sprintf(pantalla,"%s %s %s\n",aux->elemento,aux->nodo_1,aux->nodo_2);
            printf("%s",pantalla);
            fputs(pantalla,salida);

            i=0;
            if(strcmp(aux->elemento,"TF")==0)
            {
                strcpy(res,aux->nodo_1);
                ress=strtok(res,"(");
                ress=strtok(NULL,")");
                //Busca la funciond de transferencia en un nodo
                i=busca_tension (mat_x,num_nodos,aux->nodo_1);
                if(i!= num_nodos && i!=1000)
                {
                    //si existe la fuente de tension independiente o el voltaje en elemento
                    //j=0;
                    if (fuente_tension!=NULL)
                    {
                        band_elemento=fuente_tension;
                        while (strcmp(band_elemento->elemento,aux->nodo_2)!=0)
                        {
                            if(band_elemento->sig!=NULL)
                                band_elemento=(struct REGISTRO_ELEMENTO *) band_elemento->sig;
                            else
                                break;
                        }
                        if(strcmp(band_elemento->elemento,aux->nodo_2)==0)
                        {
                            if(strcmp(band_elemento->valor,"SYMBOLIC")==0)
                            {
                                strcpy(resptf,ft1(fuente_tension,fuente_corriente,&(*(mat_x[i])->otro_dato),aux->nodo_2));
                                sprintf(pantalla,"           %s/%s=%s\n\n\n",&(*(mat_x[i])->expresion),aux->nodo_2,resptf);
                                printf("%s",pantalla);
                                //COMPRUEBA SI ES SIMBOLICO EL ELEMENTO Y SE CALCULA LA RESPUESTA IMPULSIVA
                                if(com_numerico(elementos,indice_VCVS,indice_CCCS,indice_VCCS,indice_CCVS,indice_N,indice_K)==0)
                                {
                                    printf("No se puede calcular la respuesta impulsiva, uno o mas elementos estan definidos de manera simbolica\n\n\n");
                                }
                                else
                                {
                                    strcpy(respimp,fun_respuesta_tiempo(resptf));
                                    printf("Respuesta impulsiva=%s\n\n\n",respimp);

                                }

                                aux1 =comandos;
                                while (aux1->sig != NULL)
                                {
                                    if(strcmp("TRAN",aux1->elemento)==0)
                                    {
                                        strcpy(ini,aux1->nodo_1);
                                        strcpy(fin,aux1->nodo_2);
                                    }
                                    if(aux->sig != 0)
                                    {
                                        aux1 = (struct REGISTRO_ELEMENTO *)aux1->sig;
                                    }

                                }
                                if((aux1->sig == NULL))
                                {
                                    if(strcmp("TRAN",aux1->elemento)==0)
                                    {
                                        strcpy(ini,aux1->nodo_1);
                                        strcpy(fin,aux1->nodo_2);
                                    }
                                }
                                valpas = atof(ini);
                                valfin = atof(fin);
                                if (valpas>valfin)
                                {
                                    printf("ERROR el tiempo de paso es mayor que el valor final");
                                }

                                double *x,*y;

                                npuntos=(int)(valfin/valpas);
                                x=(double*)calloc(npuntos,sizeof(double));
                                y=(double*)calloc(npuntos,sizeof(double));
                                tiempo=0;
                                for(m=0;m<npuntos;m++)
                                {
                                    //calculo de la respuesta
                                    *(x+m)=tiempo;
                                    //char *transitorio(char *expresion, char* entrada)
                                    //strcpy(temp3,ft1(fuente_tension,fuente_corriente,temp2,aux->nodo_2));
                                    sprintf(tiempo1,"%f",tiempo);
                                    strcpy(temp4,transitorio(respimp,tiempo1));
                                    double tempy;
                                    tempy = atof(temp4);
                                    *(y+m)=tempy;
                                    tiempo=tiempo+valpas;
                                }
                                //guardar tiempo
                                guardar_tiempo(npuntos,valfin,valpas,&x[0],&y[0],aux->nodo_1);


                                //llamado a graficar
                                system("plot-examples");

                                fputs(pantalla,salida);
                                if (aux->sig!=NULL)
                                {
                                    aux = (struct REGISTRO_ELEMENTO *)aux->sig;
                                    continue;
                                }
                                else
                                {
                                    break;
                                }
                            }
                            else
                            {
                                sprintf(pantalla,"           ERROR_1 no existe VARIABLE SIMBOLICA %s \n\n\n",aux->nodo_2);
                                printf("%s",pantalla);
                                fputs(pantalla,salida);
                            }
                        }
                        else
                        {
                            //si existe la fuente de corriente independiente
                            if (fuente_corriente!=NULL)
                            {
                                band_elemento=fuente_corriente;
                                while (strcmp(band_elemento->elemento,aux->nodo_2)!=0)
                                {
                                    if(band_elemento->sig!=NULL)
                                        band_elemento=(struct REGISTRO_ELEMENTO *) band_elemento->sig;
                                    else
                                        break;
                                }
                                if(strcmp(band_elemento->elemento,aux->nodo_2)==0)
                                {
                                    if(strcmp(band_elemento->valor,"SYMBOLIC")==0)
                                    {
                                        strcpy(resptf,ft1(fuente_tension,fuente_corriente,&(*(mat_x[i])->otro_dato),aux->nodo_2));
                                        sprintf(pantalla,"           %s/%s=%s\n\n\n",&(*(mat_x[i])->expresion),aux->nodo_2,resptf);
                                        printf("%s",pantalla);
                                        //COMPRUEBA SI ES SIMBOLICO EL ELEMENTO Y SE CALCULA LA RESPUESTA IMPULSIVA
                                        if(com_numerico(elementos,indice_VCVS,indice_CCCS,indice_VCCS,indice_CCVS,indice_N,indice_K)==0)
                                        {
                                            printf("No se puede calcular la respuesta impulsiva, uno o mas elementos estan definidos de manera simbolica\n\n\n");
                                        }
                                        else
                                        {
                                            strcpy(respimp,fun_respuesta_tiempo(resptf));
                                            printf("Respuesta impulsiva=%s\n\n\n",respimp);
                                        }
                                        fputs(pantalla,salida);
                                        if (aux->sig!=NULL)
                                        {
                                            aux = (struct REGISTRO_ELEMENTO *)aux->sig;
                                            continue;
                                        }
                                        else
                                        {
                                            break;
                                        }
                                    }
                                    else
                                    {
                                        sprintf(pantalla,"           ERROR_1 no existe VARIABLE SIMBOLICA %s \n\n\n",aux->nodo_2);
                                        printf("%s",pantalla);
                                        fputs(pantalla,salida);
                                    }
                                }
                            }
                        }
                    }
                }
                else
                {
                    //Busca la FT sobre un elemento
                    band_elemento=busca_elemento2(elementos,ress);
                    if(band_elemento != NULL)
                    {
                        sprintf(temp1,"V(%s)",band_elemento->nodo_1);
                        i=busca_tension (mat_x,num_nodos,temp1);
                        strcpy(temp2,"(");
                        strcat(temp2,&(*(mat_x[i])->otro_dato));
                        strcat(temp2,"-");
                        sprintf(temp1,"V(%s)",band_elemento->nodo_2);
                        i=busca_tension (mat_x,num_nodos,temp1);
                        strcat(temp2,&(*(mat_x[i])->otro_dato));
                        strcat(temp2,")");
                        if(aux->nodo_1[0]=='V')
                        {
                            strcpy(temp3,ft1(fuente_tension,fuente_corriente,temp2,aux->nodo_2));
                        }
                        else
                        {
                            strcat(temp2,"*");
                            strcpy(temp3,busca_impedancia(temp3,band_elemento));
                            strcat(temp2, temp3);
                            strcpy(temp3,ft1(fuente_tension,fuente_corriente,temp2,aux->nodo_2));
                        }
                        sprintf(pantalla,"           %s/%s=%s\n\n\n",aux->nodo_1,aux->nodo_2,temp3);
                        printf("%s",pantalla);
                        //COMPRUEBA SI ES SIMBOLICO EL ELEMENTO Y SE CALCULA LA RESPUESTA IMPULSIVA
                        if(com_numerico1(elementos)==0)
                        {
                            printf("No se puede calcular la respuesta impulsiva, uno o mas elementos estan definidos de manera simbolica\n\n\n");
                        }
                        else
                        {

                            strcpy(respimp,fun_respuesta_tiempo(temp3));
                            printf("Respuesta impulsiva=%s\n\n\n",respimp);
                        }
                        fputs(pantalla,salida);

                    }
                    else
                    {
                        sprintf(pantalla,"           ERROR_8 No existe el nodo %s \n\n\n",ress);
                        printf("%s",pantalla);
                        fputs(pantalla,salida);
                    }
                }
            }
            //SE GRAFICA EL ANALISIS TRANSITORIO
            //            if(strcmp(aux->elemento,"TRAN")==0)
            //            {
            //                double *x,*y;


            //                strcpy(ini,aux->nodo_1);
            //                strcpy(fin,aux->nodo_2);
            //                valpas = atoi(ini);
            //                valfin = atof(fin);
            //                if (valpas>valfin)
            //                {
            //                    printf("ERROR el tiempo de paso es mayor que el valor final");
            //                }
            //                npuntos=(int)(valfin/valpas);
            //                x=(float*)calloc(npuntos,sizeof(float));
            //                y=(float*)calloc(npuntos,sizeof(float));
            //                tiempo=0;
            //                for(m=0;m<npuntos;m++)
            //                {
            //                    //calculo de la respuesta
            //                    *(x+m)=tiempo;
            //                    //char *transitorio(char *expresion, char* entrada)
            //                    //strcpy(temp3,ft1(fuente_tension,fuente_corriente,temp2,aux->nodo_2));
            //                    strcpy(temp4,transitorio(respimp,tiempo));
            //                    double tempy;
            //                    tempy = atof(temp4);
            //                    *(y+m)=tempy;
            //                    tiempo=+valpas;


            //                }
            //                //guardar tiempo
            //                  guardar_tiempo(npuntos,valfin,valpas,&x[0],&y[0]);
            //                //llamado a graficar
            //            }

            aux = (struct REGISTRO_ELEMENTO *)aux->sig;
        }
        if(aux->sig == 0)
        {
            sprintf(pantalla,"%s %s %s\n",aux->elemento,aux->nodo_1,aux->nodo_2);
            printf("%s",pantalla);
            fputs(pantalla,salida);

            i=0;
            if(strcmp(aux->elemento,"TF")==0)
            {
                strcpy(res,aux->nodo_1);
                ress=strtok(res,"(");
                ress=strtok(NULL,")");
                //Busca la funciond de transferencia en un nodo
                i=busca_tension (mat_x,num_nodos,aux->nodo_1);
                if(i!= num_nodos && i!=1000)
                {
                    //si existe la fuente de tension independiente
                    //j=0;
                    if (fuente_tension!=NULL)
                    {
                        band_elemento=fuente_tension;
                        while (strcmp(band_elemento->elemento,aux->nodo_2)!=0)
                        {
                            if(band_elemento->sig!=NULL)
                                band_elemento=(struct REGISTRO_ELEMENTO *) band_elemento->sig;
                            else
                                break;
                        }
                        if(strcmp(band_elemento->elemento,aux->nodo_2)==0)
                        {
                            if(strcmp(band_elemento->valor,"SYMBOLIC")==0)
                            {
                                strcpy(resptf,ft1(fuente_tension,fuente_corriente,&(*(mat_x[i])->otro_dato),aux->nodo_2));
                                sprintf(pantalla,"           %s/%s=%s\n\n\n",&(*(mat_x[i])->expresion),aux->nodo_2,resptf);
                                printf("%s",pantalla);
                                //COMPRUEBA SI ES SIMBOLICO EL ELEMENTO Y SE CALCULA LA RESPUESTA IMPULSIVA
                                if(com_numerico(elementos,indice_VCVS,indice_CCCS,indice_VCCS,indice_CCVS,indice_N,indice_K)==0)
                                {
                                    printf("No se puede calcular la respuesta impulsiva, uno o mas elementos estan definidos de manera simbolica\n\n\n");
                                }
                                else
                                {

                                    strcpy(respimp,fun_respuesta_tiempo(resptf));
                                    printf("Respuesta impulsiva=%s\n\n\n",respimp);
                                }
                                fputs(pantalla,salida);
                            }
                            else
                            {
                                sprintf(pantalla,"           ERROR_1 no existe VARIABLE SIMBOLICA %s \n\n\n",aux->nodo_2);
                                printf("%s",pantalla);
                                fputs(pantalla,salida);
                            }
                        }
                        else
                        {
                            //si existe la fuente de corriente independiente
                            if (fuente_corriente!=NULL)
                            {
                                band_elemento=fuente_corriente;
                                while (strcmp(band_elemento->elemento,aux->nodo_2)!=0)
                                {
                                    if(band_elemento->sig!=NULL)
                                        band_elemento=(struct REGISTRO_ELEMENTO *) band_elemento->sig;
                                    else
                                        break;
                                }
                                if(strcmp(band_elemento->elemento,aux->nodo_2)==0)
                                {
                                    if(strcmp(band_elemento->valor,"SYMBOLIC")==0)
                                    {
                                        strcpy(resptf,ft1(fuente_tension,fuente_corriente,&(*(mat_x[i])->otro_dato),aux->nodo_2));
                                        sprintf(pantalla,"           %s/%s=%s\n\n\n",&(*(mat_x[i])->expresion),aux->nodo_2,resptf);
                                        printf("%s",pantalla);
                                        //COMPRUEBA SI ES SIMBOLICO EL ELEMENTO Y SE CALCULA LA RESPUESTA IMPULSIVA
                                        if(com_numerico(elementos,indice_VCVS,indice_CCCS,indice_VCCS,indice_CCVS,indice_N,indice_K)==0)
                                        {
                                            printf("No se puede calcular la respuesta impulsiva, uno o mas elementos estan definidos de manera simbolica\n\n\n");
                                        }
                                        else
                                        {
                                            strcpy(respimp,fun_respuesta_tiempo(resptf));
                                            printf("Respuesta impulsiva=%s\n\n\n",respimp);
                                        }
                                        fputs(pantalla,salida);
                                    }
                                    else
                                    {
                                        sprintf(pantalla,"           ERROR_1 no existe VARIABLE SIMBOLICA %s \n\n\n",aux->nodo_2);
                                        printf("%s",pantalla);
                                        fputs(pantalla,salida);
                                    }
                                }
                            }
                        }
                    }
                }
                else
                {
                    //Busca la FT sobre un elemento
                    band_elemento=busca_elemento2(elementos,ress);
                    if(band_elemento != NULL)
                    {
                        sprintf(temp1,"V(%s)",band_elemento->nodo_1);
                        i=busca_tension (mat_x,num_nodos,temp1);
                        strcpy(temp2,"(");
                        strcat(temp2,&(*(mat_x[i])->otro_dato));
                        strcat(temp2,"-");
                        sprintf(temp1,"V(%s)",band_elemento->nodo_2);
                        i=busca_tension (mat_x,num_nodos,temp1);
                        strcat(temp2,&(*(mat_x[i])->otro_dato));
                        strcat(temp2,")");
                        if(aux->nodo_1[0]=='V')
                        {
                            strcpy(temp3,ft1(fuente_tension,fuente_corriente,temp2,aux->nodo_2));
                        }
                        else
                        {
                            strcat(temp2,"*");
                            strcpy(temp3,busca_impedancia(temp3,band_elemento));
                            strcat(temp2, temp3);
                            strcpy(temp3,ft1(fuente_tension,fuente_corriente,temp2,aux->nodo_2));
                        }
                        sprintf(pantalla,"           %s/%s=%s\n\n\n",aux->nodo_1,aux->nodo_2,temp3);
                        printf("%s",pantalla);
                        //COMPRUEBA SI ES SIMBOLICO EL ELEMENTO Y SE CALCULA LA RESPUESTA IMPULSIVA
                        if(com_numerico1(elementos)==0)
                        {
                            printf("No se puede calcular la respuesta impulsiva, uno o mas elementos estan definidos de manera simbolica\n\n\n");
                        }
                        else
                        {
                            strcpy(respimp,fun_respuesta_tiempo(temp3));
                            printf("Respuesta impulsiva=%s\n\n\n",respimp);
                        }
                        fputs(pantalla,salida);

                    }
                    else
                    {
                        sprintf(pantalla,"           ERROR_8 No existe el nodo %s \n\n\n",ress);
                        printf("%s",pantalla);
                        fputs(pantalla,salida);
                    }
                }
            }
            //SE GRAFICA EL ANALISIS TRANSITORIO
            if(strcmp(aux->elemento,"TRAN")==0)
            {
                strcpy(ini,aux->nodo_1);
                strcpy(fin,aux->nodo_2);
                valpas = atoi(ini);
                valfin = atof(fin);
                if (valpas>valfin)
                {
                    printf("ERROR el tiempo de paso es mayor que el valor final");
                }
            }
        }
    }
}

char *ft1(struct REGISTRO_ELEMENTO *fuente_tension, struct REGISTRO_ELEMENTO *fuente_corriente, char *expresion, char* entrada)
{
    FILE *matho_f;
    char l1[]="clear all\n";
    char l11[]="set fraction=0\n";
    char l2[]="simplify fraction\n";

    char l4[]="quit\n";
    char respuesta[MAX_LONG_LINEA];
    char salida[MAX_LONG_LINEA];
    char *resp;

    static struct REGISTRO_ELEMENTO *aux;

    //Se crea un archivo de texto con extension .in para ser usado
    //por el mathomatic

    matho_f = fopen("file$.in", "w" );

    fputs(l1,matho_f);
    fputs(l11,matho_f);
    fputs(expresion,matho_f);
    fputs("\n",matho_f);
    //RECORRE TODAS LAS LISTAS DE FUENTES DE TENSION Y CORRIENTE SI EXISTE entrada EN LAS LISTAS
    //SE COLOCA replace entrada with 1 y si no existe replace entrada with 0 en el archivo
    //file$.in

    if((fuente_tension)!=NULL)
    {
        aux= fuente_tension;
        while (aux!=NULL)
        {
            if(strstr(expresion,aux->elemento)!=0)
            {
                fputs("replace ",matho_f);
                fputs( aux->elemento,matho_f);
                fputs(" with ",matho_f);
                if(strcmp(aux->elemento,entrada)!=0)
                {
                    fputs(" 0\n",matho_f);
                }
                else
                {
                    fputs(" 1\n",matho_f);
                }
            }
            aux = aux->sig;
        }
    }
    if((fuente_corriente)!=NULL)
    {
        aux= fuente_corriente;
        while (aux!=NULL)
        {
            fputs("replace ",matho_f);
            fputs(aux->elemento,matho_f);
            fputs(" with ",matho_f);
            if(strcmp( aux->elemento,entrada)!=0)
            {
                fputs(" 0\n",matho_f);
            }
            else
            {
                fputs(" 1\n",matho_f);
            }
            aux = aux->sig;
        }
    }

    fputs(l2,matho_f);
    fputs(l4,matho_f);
    fclose(matho_f) ;

    //
    system("mathomatic -t file$.in >file$.out");

    matho_f = fopen("file$.out", "r" );
    obtener_linea(respuesta,matho_f);
    while(strcmp(respuesta,"1-> SIMPLIFY FRACTION")!=0)
    {
        obtener_linea(respuesta,matho_f);
    }

    obtener_linea(respuesta,matho_f);
    obtener_linea(respuesta,matho_f);

    int xxx=strncmp(respuesta,"#1:",3);
    if (xxx==0)
    {
        strtok(respuesta," ");
        resp=respuesta;
    }
    else
    {
        strcpy(salida,"(");
        strcat(salida,respuesta);
        strcat(salida,")/(");
        obtener_linea(respuesta,matho_f);
        obtener_linea(respuesta,matho_f);
        strcat(salida,respuesta);
        strcat(salida,")");
    }

    if (xxx==0)
    {
        resp=strtok(NULL,"\0");

        int xxx=strncmp(resp,"ANSWER",6);
        if (xxx==0)
        {
            char bbb[MAX_LONG_LINEA];
            strcpy(bbb,resp);
            resp=strtok(bbb," = ");
            resp=strtok(NULL,"= ");
        }

        if(strcmp(resp,"ANSWER")==0)
        {
            resp=strtok(NULL," ");
            resp=strtok(NULL," ");
            fclose(matho_f) ;
            return(resp);
        }
        else
        {
            fclose(matho_f) ;
            return(resp);
        }
    }
    else
    {
        resp=salida;
        fclose(matho_f) ;
        return(resp);
    }
}

int busca_tension (struct MATRIZ **mat_x, int num_nodos,char *nodo)
{

    int i=0;
    while (strcmp(&(*(mat_x[i])->expresion),nodo)!=0)
    {
        i++;
        if(i== num_nodos)
            break;
    }
    if(i!= num_nodos)
    {
        return i;
    }
    else
    {
        return 1000;
    }
}

char *busca_impedancia(char *tem, struct REGISTRO_ELEMENTO *elemento)
{
    struct REGISTRO_ELEMENTO *aux;
    char aux1[30];

    strcpy(tem,"");
    aux=elemento;

    if (aux->elemento[0] == 'R')
    {
        if ((aux->elemento[0] == 'R') && (strcmp(aux->valor,"SYMBOLIC")==0))
        {
            strcat(tem,"1/");
            strcat(tem,aux -> elemento);
        }
        else
        {
            strcat(tem,"1/");
            sprintf(aux1,"%g",aux->valornum);
            strcat(tem,aux1);
        }
    }

    if (aux->elemento[0] == 'A')
    {
        if ((aux->elemento[0] == 'A') && (strcmp(aux->valor,"SYMBOLIC")==0))
        {
            strcat(tem,"1*");
            strcat(tem,aux -> elemento);
        }
        else
        {
            strcat(tem,"1*");
            sprintf(aux1,"%g",aux->valornum);
            strcat(tem,aux1);
        }
    }

    if (aux->elemento[0] == 'L')
    {
        if((strcmp(aux->valor,"SYMBOLIC")==0))
        {
            strcat(tem,"1/(S*");
            strcat(tem,aux -> elemento);
            strcat(tem,")");
        }
        else
        {
            strcat(tem,"1/(S*");
            strcat(tem,aux -> valor);
            strcat(tem,")");
        }
    }

    if (aux->elemento[0] == 'C')
    {
        if ((aux->elemento[0] == 'C') && (strcmp(aux->valor,"SYMBOLIC")==0))
        {
            strcat(tem,"S*");
            strcat(tem,aux -> elemento);
        }
        else
        {
            strcat(tem,"S*");
            strcat(tem,aux -> valor);
        }
    }

    if (strcmp(tem, "")==0)
        strcpy(tem, "0");

    return (tem);
}

int com_numerico (struct REGISTRO_ELEMENTO *elemento,
                  struct REGISTRO_ELEMENTO **indice_VCVS,
                  struct REGISTRO_ELEMENTO **indice_CCCS,
                  struct REGISTRO_ELEMENTO **indice_VCCS,
                  struct REGISTRO_ELEMENTO **indice_CCVS,
                  struct REGISTRO_ELEMENTO **indice_N,
                  struct REGISTRO_ELEMENTO **indice_K)

{
    static struct REGISTRO_ELEMENTO *aux;
    static struct REGISTRO_ELEMENTO **aux1;
    static struct REGISTRO_ELEMENTO **aux2;
    static struct REGISTRO_ELEMENTO **aux3;
    static struct REGISTRO_ELEMENTO **aux4;
    static struct REGISTRO_ELEMENTO **aux5;
    static struct REGISTRO_ELEMENTO **aux6;

    aux=elemento;
    aux1=indice_VCVS;
    aux2=indice_CCCS;
    aux3=indice_VCCS;
    aux4=indice_CCVS;
    aux5=indice_N;
    aux6=indice_K;

    if (aux!=NULL)
    {
        while (aux->sig != NULL)
        {
            if(strcmp(aux->valor,"SYMBOLIC")==0)
            {
                return 0;
            }
            aux = (struct REGISTRO_ELEMENTO *)aux->sig;
        }
        if(strcmp(aux->valor,"SYMBOLIC")==0)
        {
            return 0;
        }
    }

    if (aux1!=NULL)
    {
        while ((*aux1)->sig != NULL)
        {
            if(strcmp((*aux1)->valor,"SYMBOLIC")==0)
            {
                return 0;
            }
            *aux1 = (struct REGISTRO_ELEMENTO *)(*aux1)->sig;
        }
        if(strcmp((*aux1)->valor,"SYMBOLIC")==0)
        {
            return 0;
        }
    }

    if (aux2!=NULL)
    {
        while ((*aux2)->sig != NULL)
        {
            if(strcmp((*aux2)->valor,"SYMBOLIC")==0)
            {
                return 0;
            }
            (*aux2) = (struct REGISTRO_ELEMENTO *)(*aux2)->sig;
        }
        if(strcmp((*aux2)->valor,"SYMBOLIC")==0)
        {
            return 0;
        }
    }

    if (aux3!=NULL)
    {
        while ((*aux3)->sig != NULL)
        {
            if(strcmp((*aux3)->valor,"SYMBOLIC")==0)
            {
                return 0;
            }
            (*aux3) = (struct REGISTRO_ELEMENTO *)(*aux3)->sig;
        }
        if(strcmp((*aux3)->valor,"SYMBOLIC")==0)
        {
            return 0;
        }
    }

    if (aux4!=NULL)
    {
        while ((*aux4)->sig != NULL)
        {
            if(strcmp((*aux4)->valor,"SYMBOLIC")==0)
            {
                return 0;
            }
            (*aux4) = (struct REGISTRO_ELEMENTO *)(*aux4)->sig;
        }
        if(strcmp((*aux4)->valor,"SYMBOLIC")==0)
        {
            return 0;
        }
    }

    if (aux5!=NULL)
    {
        while ((*aux5)->sig != NULL)
        {
            if(strcmp((*aux5)->valor,"SYMBOLIC")==0)
            {
                return 0;
            }
            (*aux5) = (struct REGISTRO_ELEMENTO *)(*aux5)->sig;
        }
        if(strcmp((*aux5)->valor,"SYMBOLIC")==0)
        {
            return 0;
        }
    }

    if (aux6!=NULL)
    {
        while ((*aux6)->sig != NULL)
        {
            if(strcmp((*aux6)->valor,"SYMBOLIC")==0)
            {
                return 0;
            }
            (*aux6) = (struct REGISTRO_ELEMENTO *)(*aux6)->sig;
        }
        if(strcmp((*aux6)->valor,"SYMBOLIC")==0)
        {
            return 0;
        }
    }
    return 1;
}

int com_numerico1 (struct REGISTRO_ELEMENTO *elemento)

{
    static struct REGISTRO_ELEMENTO *aux;

    aux=elemento;

    if (aux!=NULL)
    {
        while (aux->sig != NULL)
        {
            if(strcmp(aux->valor,"SYMBOLIC")==0)
            {
                return 0;
            }
            aux = (struct REGISTRO_ELEMENTO *)aux->sig;
        }
        if(strcmp(aux->valor,"SYMBOLIC")==0)
        {
            return 0;
        }
    }
    return 1;
}

char *evals(char *expresion, char* entrada)
{
    FILE *matho_f;
    char l1[]="clear all\n";
    char l11[]="set fraction=0\n";
    char l2[]="simplify fraction\n";
    char l4[]="quit\n";
    char respuesta[MAX_LONG_LINEA];
    char salida[MAX_LONG_LINEA];
    char *resp;

    //static struct REGISTRO_ELEMENTO *aux;

    //Se crea un archivo de texto con extension .in para ser usado
    //por el mathomatic

    matho_f = fopen("file$.in", "w" );

    fputs(l1,matho_f);
    fputs(l11,matho_f);
    fputs(expresion,matho_f);
    fputs("\n",matho_f);
    //RECORRE TODAS LAS LISTAS DE FUENTES DE TENSION Y CORRIENTE SI EXISTE entrada EN LAS LISTAS
    //SE COLOCA replace entrada with 1 y si no existe replace entrada with 0 en el archivo
    //file$.in


    if (strstr(expresion,"S")!=0)
    {
        fputs("replace S ",matho_f);
        fputs(" with ",matho_f);
        fputs(" i*2*pi*1000\n",matho_f);
    }

    fputs(l2,matho_f);
    fputs(l4,matho_f);
    fclose(matho_f) ;

    //
    system("mathomatic -t file$.in >file$.out");

    matho_f = fopen("file$.out", "r" );
    obtener_linea(respuesta,matho_f);
    while(strcmp(respuesta,"1-> SIMPLIFY FRACTION")!=0)
    {
        obtener_linea(respuesta,matho_f);
    }

    obtener_linea(respuesta,matho_f);
    obtener_linea(respuesta,matho_f);

    int xxx=strncmp(respuesta,"#1:",3);
    if (xxx==0)
    {
        strtok(respuesta," ");
        resp=respuesta;
    }
    else
    {
        strcpy(salida,"(");
        strcat(salida,respuesta);
        strcat(salida,")/(");
        obtener_linea(respuesta,matho_f);
        obtener_linea(respuesta,matho_f);
        strcat(salida,respuesta);
        strcat(salida,")");
    }

    if (xxx==0)
    {
        resp=strtok(NULL,"\0");

        int xxx=strncmp(resp,"ANSWER",6);
        if (xxx==0)
        {
            char bbb[MAX_LONG_LINEA];
            strcpy(bbb,resp);
            resp=strtok(bbb," = ");
            resp=strtok(NULL,"= ");
        }

        if(strcmp(resp,"ANSWER")==0)
        {
            resp=strtok(NULL," ");
            resp=strtok(NULL," ");
            fclose(matho_f) ;
            return(resp);
        }
        else
        {
            fclose(matho_f) ;
            return(resp);
        }
    }
    else
    {
        resp=salida;
        fclose(matho_f) ;
        return(resp);
    }
}

char *transitorio(char *expresion, char* entrada)
{
    FILE *matho_f;
    char l1[]="clear all\n";
    char l11[]="set fraction=0\n";
    char l2[]="simplify fraction\n";
    char l3[]="calculate\n";

    char l4[]="quit\n";
    char respuesta[MAX_LONG_LINEA];
    char salida[MAX_LONG_LINEA];
    char *resp;

    //static struct REGISTRO_ELEMENTO *aux;

    //Se crea un archivo de texto con extension .in para ser usado
    //por el mathomatic

    matho_f = fopen("file$.in", "w" );

    fputs(l1,matho_f);
    fputs(l11,matho_f);
    fputs(expresion,matho_f);
    fputs("\n",matho_f);
    //RECORRE TODAS LAS LISTAS DE FUENTES DE TENSION Y CORRIENTE SI EXISTE entrada EN LAS LISTAS
    //SE COLOCA replace entrada with 1 y si no existe replace entrada with 0 en el archivo
    //file$.in
    fputs("replace ",matho_f);
    fputs("t",matho_f);
    fputs(" with ",matho_f);
    fputs(entrada,matho_f);
    fputs("\n",matho_f);

    fputs(l2,matho_f);
    fputs(l3,matho_f);
    fputs(l4,matho_f);
    fclose(matho_f) ;

    //
    system("mathomatic -t file$.in >file$.out");

    matho_f = fopen("file$.out", "r" );
    obtener_linea(respuesta,matho_f);
    while(strcmp(respuesta,"1-> CALCULATE")!=0)
    {
        obtener_linea(respuesta,matho_f);
    }

    obtener_linea(respuesta,matho_f);


    int xxx=strncmp(respuesta,"#1:",3);
    if (xxx==0)
    {
        strtok(respuesta," ");
        resp=respuesta;
    }
    else
    {
        strcpy(salida,respuesta);
    }

    if (xxx==0)
    {
        resp=strtok(NULL,"\0");

        int xxx=strncmp(resp,"ANSWER",6);
        if (xxx==0)
        {
            char bbb[MAX_LONG_LINEA];
            strcpy(bbb,resp);
            resp=strtok(bbb," = ");
            resp=strtok(NULL,"= ");
        }

        if(strcmp(resp,"ANSWER")==0)
        {
            resp=strtok(NULL," ");
            resp=strtok(NULL," ");
            fclose(matho_f) ;
            return(resp);
        }
        else
        {
            fclose(matho_f) ;
            return(resp);
        }
    }
    else
    {
        resp=salida;
        fclose(matho_f) ;
        return(resp);
    }
}

void guardar_tiempo(int t_ini,double t_fin,double delta_t, double *x, double *y, char nombret[])
{
    FILE *salida;
    //char nombret[60];

    //strcpy(nombret,"Vo(t)");

    if ((salida = fopen("tansitorio.tim","wb")) == NULL)
    {
        fprintf(stderr,"NO SE PUEDE ABRIR ARCHIVO DE SALIDA. \n");
        exit(1);
    }

    fwrite(&t_ini,sizeof(int),1,salida);
    fwrite(&t_fin,sizeof(double),1,salida);
    fwrite(&delta_t,sizeof(double),1,salida);

    for(int i=0;i<t_ini;i++)
    {
        fwrite(&(*(x+i)),sizeof(double),1,salida);
    }

    for(int i=0;i<t_ini;i++)
    {
        fwrite(&(*(y+i)),sizeof(double),1,salida);
    }
    fwrite(nombret,sizeof(nombret[20]),20,salida);
}

char *fun_respuesta_tiempo(char *expresion)
{
    char numer[MAX_LONG_LINEA];
    char denom[MAX_LONG_LINEA];

    char interm[MAX_LONG_LINEA];
    char auxx1[MAX_LONG_LINEA];

    char *auxx;

    double apoln[10];
    double apold[10];
    double acoef;
    int aexpo;
    int grado_n, grado_d;
    static char impulsiva[MAX_LONG_LINEA];
    //static struct REGISTRO_ELEMENTO *aux1;

    // seleccionamos la funcion a analizar... esto lo da .TF
    int i;

    for(i=0; i < 10; i++)
    {
        apold[i]=0;
    }

    for(i=0; i < 10; i++)
    {
        apoln[i]=0;
    }

    strcpy(interm,expresion);
    auxx=strtok(interm,"/");
    strcpy(numer,auxx);
    auxx=strtok(NULL,"/");
    strcpy(denom,auxx);

    if(strlen(denom)!=4)
    {
        strcpy(interm,mathopol(denom));
        squeeze(interm,' ');
        strcpy(denom,mathopol(interm));
        squeeze(denom,' ');
        strcpy(interm,denom);
        auxx=strtok(denom,"+");
    }
    else
    {
        squeeze(denom,' ');
        strcpy(auxx,denom);
    }
    do
    {
        strcpy(auxx1,auxx);
        squeeze(auxx,'(');
        squeeze(auxx,')');
        coef_pol_pot (auxx,&acoef,&aexpo);
        apold[aexpo]=acoef;
    }while((auxx=strtok(NULL,"+"))!=0);

    strcpy(interm,numer);
    auxx=strtok(numer,"+");
    do
    {
        strcpy(auxx1,auxx);

        squeeze(auxx,'(');
        squeeze(auxx,')');

        coef_pol_pot (auxx,&acoef,&aexpo);
        apoln[aexpo]=acoef;
    }while((auxx=strtok(NULL,"+"))!=0);


    // aqui tenemos los polinomios numerador y denominador

    grado_n=grados (apoln);
    grado_d=grados (apold);

    strcpy(impulsiva,laplace_inv(apoln,apold,grado_n,grado_d));
    return impulsiva;
}


void coef_pol_pot (char expresion[],double *coeficiente, int *exponente)
{
    char coef_s[MAX_LONG_LINEA];
    char expo_s[MAX_LONG_LINEA];
    //char *inter;
    int i=0;
    //char *auxx;
    //char *auxx1;

    if(expresion[i]!='S')
    {
        do
        {
            coef_s[i]=expresion[i];
            i++;
        }while(expresion[i]!='S' && expresion[i]!='\0' );
    }else
    {
        coef_s[i]='1';
        i++;
    }
    if(coef_s[i-1]=='*')
        coef_s[i-1]='\0';
    coef_s[i]='\0';
    i++;
    if(expresion[i]=='^')
        i++;
    if(expresion[i]=='\0')
    {
        if(expresion[i-1]=='S')
            expo_s[0]='1';
        else
            expo_s[0]='0';
    }
    else
        expo_s[0]=expresion[i];
    expo_s[1]='\0';


    *coeficiente=atof(coef_s);
    *exponente=atoi(expo_s);
}

int grados (double *apold)
{
    int i=0;
    int grado=-1;

    do
    {
        if(apold[i]!=0)
            grado++;
        i++;

    }while(i<=GRADO);
    return grado;
}
