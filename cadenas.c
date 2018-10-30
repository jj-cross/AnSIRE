#include <stdio.h>
#include <string.h>
#include "cadenas.h"


// Lee una linea del archivo de entrada y la almacena en la variable linea.
void obtener_linea(char *linea, FILE *input)
{
    char l_linea[MAX_LONG_LINEA];

    fgets(linea,MAX_LONG_LINEA + 1,input);
    if (linea[strlen(linea) - 1] == '\n')
        sub_cad(linea,linea,(int)strlen(linea) - 1);
    strupr(linea);
    eliminar_espacios(linea, l_linea);
    strcpy(linea,l_linea);
}

// Extrae la subcadena de longitud cont de la subcadena apuntada por s2, y la
// almacena en la subcadena apuntada por s1.
char *sub_cad(char *s1, char *s2, int cont)
{
    char *aux = s1;
    for(; *s2 && cont-- > 0; *s1++ = *s2++);
    *s1 = '\0';
    return (aux);
}

void eliminar_espacios(char *cad,char *cad2)
{
    int j,i;

    i=0;
    while(cad[i]==' ' || cad[i]=='\t')
        i++;
    j=0;
    while(cad[i] != 0)
    {
        cad2[j]=cad[i];
        i++;
        j++;
    }
    cad2[j]=0;
}
