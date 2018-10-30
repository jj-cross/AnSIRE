//#pragma GCC diagnostic ignored "-Wunused-variable"
//#pragma GCC diagnostic ignored "-Wunused-parameter"
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include "elementos.h"

// Crea la lista correspondiente de los elemntos de la red que permite saber que nodos
// existen o no en la misma.
struct REGISTRO_ELEMENTO *asigna_elemento(char *nombre, char *valor,struct REGISTRO_ELEMENTO *REGISTRO_ele, char *nodo_1,char *nodo_2,char *nodo_3,char *nodo_4)
{
    struct REGISTRO_ELEMENTO *componente;
    struct REGISTRO_ELEMENTO *guia;
    struct REGISTRO_ELEMENTO *N1;
    //	int band;

    if (strcmp(nombre,"") == 0)
    {
        return REGISTRO_ele;
    }

    //q2 = REGISTRO_elemento_res;

    componente = (struct REGISTRO_ELEMENTO *) malloc(sizeof(struct REGISTRO_ELEMENTO));
    if (componente == NULL)
    {

        printf("No hay memoria disponible \n");
        printf("para crear el elemento %s", nombre);
        getch();
        exit(11);
    }

    strcpy(componente->elemento,nombre);
    strcpy(componente->valor,valor);
    componente->valornum=0;
    strcpy(componente->nodo_1,nodo_1);
    strcpy(componente->nodo_2,nodo_2);
    strcpy(componente->nodo_3,nodo_3);
    strcpy(componente->nodo_4,nodo_4);
    componente->sig = NULL;

    N1=REGISTRO_ele;

    // primer elemento
    if(N1==NULL)
    {
        REGISTRO_ele=componente;
        return REGISTRO_ele;
    }
    else
    {
        guia = REGISTRO_ele;
        while(guia->sig != NULL)
        {
            guia = guia->sig;
        }
        guia->sig = componente;
    }
    return REGISTRO_ele;
}

// Hace la busqueda en la lista de elementos para saber que elemento
// existe o no.
char *  busca_valor_dp(char *tem, struct REGISTRO_ELEMENTO *REGISTRO_nodo, char *nodo, struct REGISTRO_ELEMENTO *K)
{
    static struct REGISTRO_ELEMENTO *aux;
    char aux1[30];

    int i;

    strcpy(tem,"");

    if(REGISTRO_nodo==NULL)
    {
        printf("Estructura no definida");
        return (NULL);
    }

    aux = REGISTRO_nodo; //Se para en el primero
    strcpy(tem,"0");
    while (aux->sig != NULL )
    {
        if (strcmp(aux->nodo_1,nodo)==0)
        {
            if (aux->elemento[0] == 'R')
            {
                if ((strcmp(aux->valor,"SYMBOLIC")==0))
                {
                    strcat(tem,"+1/");
                    strcat(tem,aux -> elemento);
                }
                else
                {
                    strcat(tem,"+1/");
                    sprintf(aux1,"%g",aux->valornum);
                    strcat(tem,aux1);
                }
            }

            if (aux->elemento[0] == 'A')
            {
                if ( (strcmp(aux->valor,"SYMBOLIC")==0))
                {
                    strcat(tem,"+1*");
                    strcat(tem,aux -> elemento);
                }
                else
                {
                    strcat(tem,"+1*");
                    sprintf(aux1,"%g",aux->valornum);
                    strcat(tem,aux1);
                }
            }

            if (aux->elemento[0] == 'L')
            {
                if(((i=busca_acopl(K,(char *)aux->elemento)) == NOEXISTE) )
                {
                    if( (strcmp(aux->valor,"SYMBOLIC")==0))
                    {
                        strcat(tem,"+1/(S*");
                        strcat(tem,aux -> elemento);
                        strcat(tem,")");
                    }
                    else
                    {
                        strcat(tem,"+1/(S*");
                        sprintf(aux1,"%g",aux->valornum);
                        strcat(tem,aux1);
                        strcat(tem,")");
                    }
                }
            }
            if (aux->elemento[0] == 'C' )
            {
                if ( (strcmp(aux->valor,"SYMBOLIC")==0))
                {
                    strcat(tem,"+S*");
                    strcat(tem,aux -> elemento);
                }
                else {
                    strcat(tem,"+S*");
                    sprintf(aux1,"%g",aux->valornum);
                    strcat(tem,aux1);
                }
            }
        }

        if (strcmp(aux->nodo_2,nodo)==0)
        {
            if (aux->elemento[0] == 'R')
            {
                if ((aux->elemento[0] == 'R') && (strcmp(aux->valor,"SYMBOLIC")==0))
                {
                    strcat(tem,"+1/");
                    strcat(tem,aux -> elemento);
                }
                else
                {
                    strcat(tem,"+1/");
                    sprintf(aux1,"%g",aux->valornum);
                    strcat(tem,aux1);
                }
            }

            if (aux->elemento[0] == 'A')
            {
                if ((aux->elemento[0] == 'A') && (strcmp(aux->valor,"SYMBOLIC")==0))
                {
                    strcat(tem,"+1*");
                    strcat(tem,aux -> elemento);
                }
                else
                {
                    strcat(tem,"+1*");
                    sprintf(aux1,"%g",aux->valornum);
                    strcat(tem,aux1);
                }
            }

            if (aux->elemento[0] == 'L')
            {
                if(((i=busca_acopl(K,(char *)aux->elemento)) == NOEXISTE) )
                {
                    if((strcmp(aux->valor,"SYMBOLIC")==0))
                    {
                        strcat(tem,"+1/(S*");
                        strcat(tem,aux -> elemento);
                        strcat(tem,")");
                    }
                    else
                    {
                        strcat(tem,"+1/(S*");
                        sprintf(aux1,"%g",aux->valornum);
                        strcat(tem,aux1);
                        strcat(tem,")");
                    }
                }
            }

            if (aux->elemento[0] == 'C')
            {
                if ((aux->elemento[0] == 'C') && (strcmp(aux->valor,"SYMBOLIC")==0))
                {
                    strcat(tem,"+S*");
                    strcat(tem,aux -> elemento);
                }
                else
                {
                    strcat(tem,"+S*");
                    sprintf(aux1,"%g",aux->valornum);
                    strcat(tem,aux1);
                }
            }
        }
        aux = (struct REGISTRO_ELEMENTO *)aux->sig;
    }
    if (strcmp(aux->nodo_1,nodo)==0)
    {
        if (aux->elemento[0] == 'R')
        {
            if ((aux->elemento[0] == 'R') && (strcmp(aux->valor,"SYMBOLIC")==0))
            {
                strcat(tem,"+1/");
                strcat(tem,aux -> elemento);
            }
            else
            {
                strcat(tem,"+1/");
                sprintf(aux1,"%g",aux->valornum);
                strcat(tem,aux1);
            }
        }

        if (aux->elemento[0] == 'A')
        {
            if ((aux->elemento[0] == 'A') && (strcmp(aux->valor,"SYMBOLIC")==0))
            {
                strcat(tem,"+1*");
                strcat(tem,aux -> elemento);
            }
            else
            {
                strcat(tem,"+1*");
                sprintf(aux1,"%g",aux->valornum);
                strcat(tem,aux1);
            }
        }

        if (aux->elemento[0] == 'L')
        {
            if(((i=busca_acopl(K,(char *)aux->elemento)) == NOEXISTE) )
            {
                if((strcmp(aux->valor,"SYMBOLIC")==0))
                {
                    strcat(tem,"+1/(S*");
                    strcat(tem,aux -> elemento);
                    strcat(tem,")");
                }
                else
                {
                    strcat(tem,"+1/(S*");
                    sprintf(aux1,"%g",aux->valornum);
                    strcat(tem,aux1);
                    strcat(tem,")");
                }
            }
        }

        if (aux->elemento[0] == 'C')
        {
            if ((aux->elemento[0] == 'C') && (strcmp(aux->valor,"SYMBOLIC")==0))
            {
                strcat(tem,"+S*");
                strcat(tem,aux -> elemento);
            }
            else
            {
                strcat(tem,"+S*");
                sprintf(aux1,"%g",aux->valornum);
                strcat(tem,aux1);
            }
        }
    }

    if (strcmp(aux->nodo_2,nodo)==0)
    {
        if (aux->elemento[0] == 'R')
        {
            if ((aux->elemento[0] == 'R') && (strcmp(aux->valor,"SYMBOLIC")==0))
            {
                strcat(tem,"+1/");
                strcat(tem,aux -> elemento);
            }
            else
            {
                strcat(tem,"+1/");
                sprintf(aux1,"%g",aux->valornum);
                strcat(tem,aux1);
            }
        }

        if (aux->elemento[0] == 'A')
        {
            if ((aux->elemento[0] == 'A') && (strcmp(aux->valor,"SYMBOLIC")==0))
            {
                strcat(tem,"+1*");
                strcat(tem,aux -> elemento);
            }
            else
            {
                strcat(tem,"+1*");
                sprintf(aux1,"%g",aux->valornum);
                strcat(tem,aux1);
            }
        }

        if (aux->elemento[0] == 'L')
        {
            if(((i=busca_acopl(K,(char *)aux->elemento)) == NOEXISTE) )
            {
                if((strcmp(aux->valor,"SYMBOLIC")==0))
                {
                    strcat(tem,"+1/(S*");
                    strcat(tem,aux -> elemento);
                    strcat(tem,")");
                }
                else
                {
                    strcat(tem,"+1/(S*");
                    sprintf(aux1,"%g",aux->valornum);
                    strcat(tem,aux1);
                    strcat(tem,")");
                }
            }
        }

        if (aux->elemento[0] == 'C')
        {
            if ((aux->elemento[0] == 'C') && (strcmp(aux->valor,"SYMBOLIC")==0))
            {
                strcat(tem,"+S*");
                strcat(tem,aux -> elemento);
            }
            else
            {
                strcat(tem,"+S*");
                sprintf(aux1,"%g",aux->valornum);
                strcat(tem,aux1);
            }
        }
    }
    if (strcmp(tem, "")==0)
        strcpy(tem, "0");

    return (tem);
}

char * busca_valor_ad(char *tem, struct REGISTRO_ELEMENTO *REGISTRO_nodo, char *nodo1, char *nodo2, struct REGISTRO_ELEMENTO *K)
{
    static struct REGISTRO_ELEMENTO *aux;
    //    char tem[ELE_MATRIZ];

    strcpy(tem,"");
    int i;
    char aux1[30];

    if(REGISTRO_nodo==NULL)
    {
        printf("Estructura no definida");
        return (NULL);
    }

    aux = REGISTRO_nodo; //Se para en el primero

    while (aux->sig != NULL )
    {
        if ( (strcmp(aux->nodo_1,nodo1)==0 && strcmp(aux->nodo_2,nodo2)==0) || (strcmp(aux->nodo_1,nodo2)==0 && strcmp(aux->nodo_2,nodo1)==0))
        {
            if (aux->elemento[0] == 'R')
            {
                if ((aux->elemento[0] == 'R') && (strcmp(aux->valor,"SYMBOLIC")==0))
                {
                    strcat(tem,"-1/");
                    strcat(tem,aux -> elemento);
                }
                else
                {
                    strcat(tem,"-1/");
                    sprintf(aux1,"%g",aux->valornum);
                    strcat(tem,aux1);
                }
            }

            if (aux->elemento[0] == 'A')
            {
                if ((aux->elemento[0] == 'A') && (strcmp(aux->valor,"SYMBOLIC")==0))
                {
                    strcat(tem,"-1*");
                    strcat(tem,aux -> elemento);
                }
                else
                {
                    strcat(tem,"-1*");
                    sprintf(aux1,"%g",aux->valornum);
                    strcat(tem,aux1);
                }
            }

            if (aux->elemento[0] == 'L')
            {
                if(((i=busca_acopl(K,(char *)aux->elemento)) == NOEXISTE) )
                {
                    if((strcmp(aux->valor,"SYMBOLIC")==0))
                    {
                        strcat(tem,"-1/(S*");
                        strcat(tem,aux -> elemento);
                        strcat(tem,")");
                    }
                    else {
                        strcat(tem,"-1/(S*");
                        sprintf(aux1,"%g",aux->valornum);
                        strcat(tem,aux1);
                        strcat(tem,")");
                    }
                }
            }

            if (aux->elemento[0] == 'C')
            {
                if ((aux->elemento[0] == 'C') && (strcmp(aux->valor,"SYMBOLIC")==0))
                {
                    strcat(tem,"-S*");
                    strcat(tem,aux -> elemento);
                }
                else
                {
                    strcat(tem,"-S*");
                    strcat(tem,aux -> valor);
                }
            }
        }

        aux = (struct REGISTRO_ELEMENTO *)aux->sig;
    }

    if (aux->sig == NULL )
    {
        if ( (strcmp(aux->nodo_1,nodo1)==0 && strcmp(aux->nodo_2,nodo2)==0) || (strcmp(aux->nodo_1,nodo2)==0 && strcmp(aux->nodo_2,nodo1)==0))
        {
            if (aux->elemento[0] == 'R')
            {
                if ((aux->elemento[0] == 'R') && (strcmp(aux->valor,"SYMBOLIC")==0))
                {
                    strcat(tem,"-1/");
                    strcat(tem,aux -> elemento);
                }
                else
                {
                    strcat(tem,"-1/");
                    sprintf(aux1,"%g",aux->valornum);
                    strcat(tem,aux1);
                }
            }

            if (aux->elemento[0] == 'A')
            {
                if ((aux->elemento[0] == 'A') && (strcmp(aux->valor,"SYMBOLIC")==0))
                {
                    strcat(tem,"-1*");
                    strcat(tem,aux -> elemento);
                }
                else
                {
                    strcat(tem,"-1*");
                    sprintf(aux1,"%g",aux->valornum);
                    strcat(tem,aux1);
                }
            }

            if (aux->elemento[0] == 'L')
            {
                if(((i=busca_acopl(K,(char *)aux->elemento)) == NOEXISTE) )
                {
                    if((strcmp(aux->valor,"SYMBOLIC")==0))
                    {
                        strcat(tem,"-1/(S*");
                        strcat(tem,aux -> elemento);
                        strcat(tem,")");
                    }
                    else
                    {
                        strcat(tem,"-1/(S*");
                        sprintf(aux1,"%g",aux->valornum);
                        strcat(tem,aux1);
                        strcat(tem,")");
                    }
                }
            }

            if (aux->elemento[0] == 'C')
            {
                if ((aux->elemento[0] == 'C') && (strcmp(aux->valor,"SYMBOLIC")==0))
                {
                    strcat(tem,"-S*");
                    strcat(tem,aux -> elemento);
                }
                else
                {
                    strcat(tem,"-S*");
                    strcat(tem,aux -> valor);
                }
            }
        }
    }

    if (strcmp(tem,"")==0)
        strcpy(tem,"0");
    return (tem);
}

// Crea el elemento correspondiente en el ARBOL, haciendo las concatenaciones
// respectivas de los nodos.
struct REGISTRO_ELEMENTO *crear_elemento(char *nombre,char *ini,char *fin,char *k,char *kp, char *valor,struct REGISTRO_ELEMENTO *REGISTRO_elemento)
{
    int band_elemento;

    band_elemento = busca_elemento(REGISTRO_elemento,nombre);

    if ( band_elemento == EXISTE )
    {

        printf("Elemnto ya existe \n");
        printf("%s \n",nombre);
        getch();
        exit(10);
    }
    else
        REGISTRO_elemento=asigna_elemento(nombre,valor,REGISTRO_elemento,ini,fin,k,kp);

    return REGISTRO_elemento;
}

struct REGISTRO_NODO *crear_nodo(char *nodo,struct REGISTRO_NODO *REGISTRO_nodo, int *num_nodos)
{
    //    int band_ini;
    //    int band_fin;
    int band_nodo;

    band_nodo = busca_nodo(REGISTRO_nodo,nodo);

    if ( band_nodo == EXISTE )
        return REGISTRO_nodo;

    else
    {
        REGISTRO_nodo=asignar_nodo(REGISTRO_nodo, nodo);
        (*num_nodos)++;
    }

    return REGISTRO_nodo;
}

struct REGISTRO_NODO *asignar_nodo(struct REGISTRO_NODO *REGISTRO_nodo,char *nodo)
{
    struct REGISTRO_NODO *nodo_nuevo;
    struct REGISTRO_NODO *guia;
    struct REGISTRO_NODO *N1;
    //	int band;

    if (strcmp(nodo, "0") == 0)
    {
        return REGISTRO_nodo;
    }

    //q2 = REGISTRO_elemento_res;

    nodo_nuevo = (struct REGISTRO_NODO *) malloc(sizeof(struct REGISTRO_NODO));
    if (nodo_nuevo == NULL)
    {

        printf("No hay memoria disponible \n");
        printf("para crear el elemento %s", nodo);
        getch();
        exit(16);
    }
    strcpy(nodo_nuevo->nodo, nodo);
    strcpy(nodo_nuevo->valor,"0");

    nodo_nuevo->sig = NULL;

    N1=REGISTRO_nodo;

    // primer elemento
    if(N1==NULL)
    {
        REGISTRO_nodo=nodo_nuevo;
        return REGISTRO_nodo;
    }
    else
    {
        guia = REGISTRO_nodo;
        while(guia->sig != NULL)
        {
            guia = guia->sig;
        }
        guia->sig = nodo_nuevo;
    }
    return REGISTRO_nodo;
}

int busca_elemento(struct REGISTRO_ELEMENTO *REGISTRO_nodo, char *palabra)
{
    struct REGISTRO_ELEMENTO *aux;

    if(REGISTRO_nodo==NULL)
        return (NOEXISTE);

    aux = REGISTRO_nodo; //Se para en el primero

    while (aux->sig && strcmp (aux->elemento, palabra) != 0)
    {
        aux = (struct REGISTRO_ELEMENTO *)aux->sig;
        if (strcmp(aux->elemento,palabra)==0)
            return(EXISTE);
        else
            return (NOEXISTE);
    }
    return (NOEXISTE);
}

struct REGISTRO_ELEMENTO **ordenar(struct REGISTRO_ELEMENTO *elementos,struct REGISTRO_ELEMENTO **indice_elemento, int num_pasivo)
{
    struct REGISTRO_ELEMENTO *guia;
    struct REGISTRO_ELEMENTO **indiceptr = indice_elemento;
    if (elementos==NULL)
        return NULL;
    guia = elementos;

    while(guia->sig != NULL)
    {
        *indiceptr++ = guia;
        guia = guia->sig;
    }
    *indiceptr = guia;
    ordenar_elemento(indice_elemento, num_pasivo);
    return indice_elemento;
}

struct REGISTRO_NODO **ordenar_nodos(struct REGISTRO_NODO *nodos,struct REGISTRO_NODO **indice_nodo, int num_nodos)
{
    struct REGISTRO_NODO *guia;
    struct REGISTRO_NODO **indiceptr = indice_nodo;
    if (nodos==NULL)
        return NULL;
    guia = nodos;
    while(guia->sig != NULL)
    {
        *indiceptr++ = guia;
        guia = guia->sig;
    }
    *indiceptr = guia;
    ordenar_nod(indice_nodo, num_nodos);
    return indice_nodo;
}

int comparar_elemento(const void *ptr1, const void *ptr2)
{
    struct REGISTRO_ELEMENTO **ptrdados1 = (struct REGISTRO_ELEMENTO **)ptr1;
    struct REGISTRO_ELEMENTO **ptrdados2 = (struct REGISTRO_ELEMENTO **)ptr2;
    return strcmp((*ptrdados1)->elemento, (*ptrdados2)->elemento);
}

int comparar_nodo(const void *ptr1, const void *ptr2)
{
    struct REGISTRO_NODO **ptrdados1 = (struct REGISTRO_NODO **)ptr1;
    struct REGISTRO_NODO **ptrdados2 = (struct REGISTRO_NODO **)ptr2;
    return strcmp((*ptrdados1)->nodo, (*ptrdados2)->nodo);
}

void ordenar_elemento(struct REGISTRO_ELEMENTO **indice, const int nodos)
{
    qsort(indice, nodos, sizeof(struct REGISTRO_ELEMENTO **), comparar_elemento);
}

void ordenar_nod(struct REGISTRO_NODO **indice, const int nodos)
{
    qsort(indice, nodos, sizeof(struct REGISTRO_NODO **), comparar_nodo);
}

void exibir_elementos(struct REGISTRO_ELEMENTO **indice, int num_nodos, FILE *salida)
{
    char pantalla[MAX_LONG_LINEA];
    int i = 0;
    struct REGISTRO_ELEMENTO **indiceptr = indice;
    if (num_nodos!=0)
    {
        if ((*indiceptr)->elemento[0]=='R' || (*indiceptr)->elemento[0]=='A' || (*indiceptr)->elemento[0]=='L' || (*indiceptr)->elemento[0]=='C' ||
                (*indiceptr)->elemento[0]=='V' || (*indiceptr)->elemento[0]=='I')
        {
            sprintf(pantalla,"%15s %10s %10s %15s\n","elemento","nodo+","nodo-","valor");
            fputs(pantalla,salida);
            printf("%s",pantalla);
        }
        else
        {
            sprintf(pantalla,"%15s %10s %10s %10s %10s %15s\n","elemento","nodo+","nodo-","cont+","cont-","valor");
            fputs(pantalla,salida);
            printf("%s",pantalla);
        }

        while(i++ < num_nodos)
        {
            if ((*indiceptr)->elemento[0]=='R' || (*indiceptr)->elemento[0]=='A' || (*indiceptr)->elemento[0]=='L' || (*indiceptr)->elemento[0]=='C' ||
                    (*indiceptr)->elemento[0]=='V' || (*indiceptr)->elemento[0]=='I')
            {
                sprintf(pantalla,"%15s %10s %10s %15s\n",(*indiceptr)->elemento,(*indiceptr)->nodo_1,(*indiceptr)->nodo_2,(*indiceptr)->valor);
                fputs(pantalla,salida);
                printf("%s",pantalla);
            }
            else
            {
                sprintf(pantalla,"%15s %10s %10s %10s %10s %15s\n",(*indiceptr)->elemento,(*indiceptr)->nodo_1,(*indiceptr)->nodo_2,(*indiceptr)->nodo_3,(*indiceptr)->nodo_4,(*indiceptr)->valor);
                fputs(pantalla,salida);
                printf("%s",pantalla);

            }
            indiceptr++;
        }
        sprintf(pantalla,"\n");
        fputs(pantalla,salida);
        printf("%s",pantalla);
    }
}

void exibir_nodos(struct REGISTRO_NODO **indice, int num_nodos, FILE *salida)
{
    int i = 0;
    char pantalla[MAX_LONG_LINEA];

    struct REGISTRO_NODO **indiceptr = indice;

    sprintf(pantalla,"%15s\n","nodo");
    fputs(pantalla,salida);
    printf("%s",pantalla);

    while(i++ < num_nodos)
    {
        sprintf(pantalla,"%15s\n",(*indiceptr)->nodo);
        fputs(pantalla,salida);
        printf("%s",pantalla);
        indiceptr++;
    }
    sprintf(pantalla,"\n");
    fputs(pantalla,salida);
    printf("%s",pantalla);
}

char *nombre_nodo(struct REGISTRO_NODO **indice, int nodo)
{
    int i = 1;
    struct REGISTRO_NODO **indiceptr = indice;
    while(i++ < nodo)
    {
        indiceptr++;
    }
    return (*indiceptr)->nodo;
}

int indice_nodo(struct REGISTRO_NODO **indice, char *nombre)
{
    int i = 1;
    struct REGISTRO_NODO **indiceptr = indice;

    while(strcmp((*indiceptr)->nodo,nombre)!=0)
    {
        //printf("%15s\n",(*indiceptr)->nodo);
        indiceptr++;
        i++;
    }
    return i;
}

int busca_acopl(struct REGISTRO_ELEMENTO *REGISTRO_nodo, char *palabra)
{
    static struct REGISTRO_ELEMENTO *aux;

    if(REGISTRO_nodo==NULL)
        return (NOEXISTE);

    aux = REGISTRO_nodo; //Se para en el primero

    while (aux->sig && strcmp (aux->nodo_1, palabra) != 0)
    {
        aux = (struct REGISTRO_ELEMENTO *)aux->sig;
    }
    if (strcmp(aux->nodo_1,palabra)==0)
        return(EXISTE);

    aux = REGISTRO_nodo; //Se para en el primero

    while (aux->sig && strcmp (aux->nodo_2, palabra) != 0)
    {
        aux = (struct REGISTRO_ELEMENTO *)aux->sig;
    }
    if (strcmp(aux->nodo_2,palabra)==0)
        return(EXISTE);
    else
        return (NOEXISTE);
}

int busca_nodo(struct REGISTRO_NODO *REGISTRO_nodo, char *nodo)
{
    static struct REGISTRO_NODO *aux;

    if(REGISTRO_nodo==NULL)
        return (NOEXISTE);

    if (strcmp(nodo,"0")== 0)
        return(EXISTE);

    aux = REGISTRO_nodo; //Se para en el primero

    while (aux->sig && (strcmp(aux->nodo,nodo)!=0))
    {
        aux = (struct REGISTRO_NODO *)aux->sig;
    }
    if (strcmp(aux->nodo,nodo)==0)
        return(EXISTE);
    else
        return (NOEXISTE);
}

struct REGISTRO_ELEMENTO *busca_elemento2(struct REGISTRO_ELEMENTO *elemento, char *K)
{
    static struct REGISTRO_ELEMENTO *aux;

    if(elemento==NULL)
        return (NOEXISTE);

    aux = elemento; //Se para en el primero

    while (aux->sig && strcmp (aux->elemento, K) != 0)
    {
        aux = (struct REGISTRO_ELEMENTO *)aux->sig;
        if (strcmp(aux->elemento,K)==0)
            return aux;
    }
    return (NOEXISTE);
}
