#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <ctype.h>

#include "elementos.h"
#include "guardar.h"
#include "ee2.h"

#define FEMTO 1e-15
#define PICO  1e-12
#define NANO  1e-9
#define MICRO 1e-6
#define MILI  1e-3

#define KILO  1e3
#define MEGA  1e6
#define GIGA  1e9
#define TERA  1e12

void inicia_simbolico(struct REGISTRO_ELEMENTO *lista_elemento, struct eeSTATE *EST)
{
    int     a,
            err;

    struct REGISTRO_ELEMENTO *aux;

    char  line[1024];
    char valor_ele[15]; // Variable auxiliar para calculos intermedios
    char string[15];
    char string2[15];

    strcpy(valor_ele,"");

    aux = lista_elemento;

    // Guarda en cad_comp los elementos de la red
    // Guarda en cad_val los valores de los elementos de la red

    while(aux->sig != NULL)
    {
        strcpy(string,aux->valor);
        strcpy(string2,aux->elemento);
        if ( (strcmp(string,"")!= 0) && (strcmp(string,"SYMBOLIC")!= 0))
        {
            err=1;
            if (err==1)
                err = convierte(aux->elemento,string,"T"  ,TERA ,EST,&aux->valornum);
            if (err==1)
                err = convierte(aux->elemento,string,"G"  ,GIGA ,EST,&aux->valornum);
            if (err==1)
                err = convierte(aux->elemento,string,"MEG",MEGA ,EST,&aux->valornum);
            if (err==1)
                err = convierte(aux->elemento,string,"M"  ,MILI ,EST,&aux->valornum);
            if (err==1)
                err = convierte(aux->elemento,string,"K"  ,KILO ,EST,&aux->valornum);
            if (err==1)
                err = convierte(aux->elemento,string,"U"  ,MICRO,EST,&aux->valornum);
            if (err==1)
                err = convierte(aux->elemento,string,"N"  ,NANO ,EST,&aux->valornum);
            if (err==1)
                err = convierte(aux->elemento,string,"P"  ,PICO ,EST,&aux->valornum);
            if (err==1)
                err = convierte(aux->elemento,string,"F"  ,FEMTO,EST,&aux->valornum);

            // Caso cuando el elemento no tiene sufijo
            if (aux->valornum == 0)
            {
                aux->valornum = atof(string);
                if (aux->valornum == 0)
                {
                    printf("(1)Error en el valor del componente %s\n",aux->elemento);
                    getch();
                    exit(18);
                }

            }
            strlwr(string2);
            sprintf(line, "%s=%g",string2,aux->valornum);
            eeEvaluate(EST,  line, &a );
        }
        aux = (struct REGISTRO_ELEMENTO *)aux->sig; // pararse en el siguiente
    }

    strcpy(string,aux->valor);
    strcpy(string2,aux->elemento);
    if ( (strcmp(string,"")!= 0) && (strcmp(string,"SYMBOLIC")!= 0))
    {
        err=1;
        if (err==1)
            err = convierte(aux->elemento,string,"T"  ,TERA ,EST,&aux->valornum);
        if (err==1)
            err = convierte(aux->elemento,string,"G"  ,GIGA ,EST,&aux->valornum);
        if (err==1)
            err = convierte(aux->elemento,string,"MEG",MEGA ,EST,&aux->valornum);
        if (err==1)
            err = convierte(aux->elemento,string,"M"  ,MILI ,EST,&aux->valornum);
        if (err==1)
            err = convierte(aux->elemento,string,"K"  ,KILO ,EST,&aux->valornum);
        if (err==1)
            err = convierte(aux->elemento,string,"U"  ,MICRO,EST,&aux->valornum);
        if (err==1)
            err = convierte(aux->elemento,string,"N"  ,NANO ,EST,&aux->valornum);
        if (err==1)
            err = convierte(aux->elemento,string,"P"  ,PICO ,EST,&aux->valornum);
        if (err==1)
            err = convierte(aux->elemento,string,"F"  ,FEMTO,EST,&aux->valornum);

        // Caso cuando el elemento no tiene sufijo
        if (aux->valornum == 0)
        {
            aux->valornum = atof(string);
            if (aux->valornum == 0)
            {
                printf("(2)Error en el valor del componente %s\n",aux->elemento);
                getch();
                exit(18);
            }

        }
        strlwr(string2);
        sprintf(line, "%s=%g",string2,aux->valornum);
        eeEvaluate(EST,  line, &a );
    }
}

int convierte(char *elemento, char *valor, char *delimitador,double orden, struct eeSTATE *EST, double *valorn)
{
    char *separa;
    char string[20];
    char str[20];
    char string2[15];
    char valor_ele[15]; // Variable auxiliar para calculos intermedios
    char  line[1024];
    int   a;
    char * pch;

    strcpy(string,"  ");
    strcpy(string2,"          ");
    strcpy(valor_ele,"          ");

    strcpy(string2,elemento);
    strcpy(string,valor);
    strcpy(str,valor);
    strcat(string,"    ");
    separa = strtok(string,delimitador);
    pch = strstr (str,delimitador);
    int xx=strcmp(separa,valor);

    if ( xx !=0 && pch ==0 )
    {
        return(1);
    }
    else
    {
        strcpy(valor_ele,separa);
        if(isnumeric1(valor_ele)==0)
        {
            *valorn = (atof(valor_ele))*orden;
            if (valorn == 0)
            {
                printf("(3)Error en el valor del componente %s\n",elemento);
                getch();
                exit(18);
            }
            strlwr(string2);
            sprintf(line, "%s=%lf",string2,*valorn);
            eeEvaluate(EST,  line, &a );
            return(0);
        }
        return(1);
    }
}

int isNumeric(const char *val)
{
    unsigned int i = 0, s = 0;

    for( i = 0; i < strlen(val); i++)
        if(isdigit(val[i]) == 0)
            s = -1;

    return s;
}

int isnumeric1(char *st)
{

    int len, ascii_code, decimal_count = -1, negative_count = -1;

    len = strlen(st);
    for (int i = 0; i < len; i++)
    {
        ascii_code = (int)(st[i]);
        switch (ascii_code)
        {

        case 44: // Allow commas.
            // This will allow them anywhere, so ",,," would return true.
            // Write code here to require commas be every 3 decimal places.
            break;

        case 45: // Allow a negative sign.
            negative_count++;
            if (negative_count || i != 0)
            {
                return -1;
            }
            break;

        case 46: // Allow a decimal point.
            decimal_count++;
            if (decimal_count)
            {
                return -1;
            }
            break;

        default:
            if (ascii_code < 48 || ascii_code > 57)
            {
                return -1;
            }

            break;

        }
    }
    return 0;
}
