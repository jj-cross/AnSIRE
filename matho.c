#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include "gauss.h"
//#include "elementos.h"
//#include "matrix.h"
#include "cadenas.h"

char *matho(char *expresion)
{
    FILE *matho_f;
    char l1[]="clear all\n";
    char l11[]="set fraction=0\n";
    char l2[]="simplify fraction\n";
    char l3[]="expand\n";
    char l4[]="quit\n";
    char respuesta[MAX_LONG_LINEA];
    char salida[MAX_LONG_LINEA];
    char *resp;

    //Se crea un archivo de texto con extension .in para ser usado
    //por el mathomatic

    matho_f = fopen("file$.in", "w" );

    fputs(l1,matho_f);
    fputs(l11,matho_f);
    fputs(expresion,matho_f);
    fputs("\n",matho_f);
    fputs(l2,matho_f);
    fputs(l3,matho_f);
    fputs(l4,matho_f);
    fclose(matho_f) ;

    system("mathomatic -t file$.in >file$.out");

    matho_f = fopen("file$.out", "r" );
    obtener_linea(respuesta,matho_f);

    //    while(strcmp(respuesta,"1-> SIMPLIFY FRACTION")!=0){
    while(strcmp(respuesta,"1-> EXPAND")!=0)
    {
        obtener_linea(respuesta,matho_f);
    }

    obtener_linea(respuesta,matho_f);
    //    strcpy(respuesta," ");
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
#ifdef NUMERO
    while((resp=strtok(NULL," "))!=0)
    {
        if(strcmp(resp,"=")==0)
        {
            break;
        }
    }
    resp=strtok(NULL," ");
#else
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
#endif

}

char *matho2(char *expresion)
{
    FILE *matho_f;
    char l1[]="clear all\n";
    char l11[]="set fraction=0\n";
    char l2[]="simplify fraction\n";

    char l4[]="quit\n";
    char respuesta[MAX_LONG_LINEA];
    char salida[MAX_LONG_LINEA];
    char *resp;

    //Se crea un archivo de texto con extension .in para ser usado
    //por el mathomatic
    matho_f = fopen("file$.in", "w" );

    fputs(l1,matho_f);
    fputs(l11,matho_f);
    fputs(expresion,matho_f);
    fputs("\n",matho_f);
    fputs(l2,matho_f);
    fputs(l4,matho_f);

    fclose(matho_f);

    system("mathomatic -t file$.in >file$.out");

    matho_f = fopen("file$.out", "r" );
    obtener_linea(respuesta,matho_f);
    while(strcmp(respuesta,"1-> SIMPLIFY FRACTION")!=0)
    {
        obtener_linea(respuesta,matho_f);
    }

    obtener_linea(respuesta,matho_f);
    //    strcpy(respuesta," ");
    obtener_linea(respuesta,matho_f);

    int xxx=strncmp(respuesta,"#1:",3);
    if (xxx==0)
    {
        strtok(respuesta," ");
        resp=respuesta;
    }
    else
    {
        //resp=strtok(respuesta," ");
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

char *matho3(char *expresion1, char *expresion2)
{
    FILE *matho_f;
    char l1[]="clear all\n";
    char l11[]="set fraction=0\n";
    char l2[]="inter=b/a\n";
    char l3[]="eliminate a\n";
    char l4[]="eliminate b\n";
    //    char l5[]="simplify\n";
    char l6[]="b/inter\n";
    char l7[]="eliminate b\n";
    char l8[]="eliminate inter\n";
    char l9[]="simplify fraction\n";

    char l14[]="quit\n";
    char respuesta[MAX_LONG_LINEA];
    char salida[MAX_LONG_LINEA];
    char *resp;

    //Se crea un archivo de texto con extension .in para ser usado
    //por el mathomatic
    matho_f = fopen("fileabc$.in", "w" );

    fputs(l1,matho_f);
    fputs(l11,matho_f);
    fputs("a=",matho_f);
    fputs(expresion1,matho_f);
    fputs("\nb=",matho_f);
    fputs(expresion2,matho_f);
    fputs("\n",matho_f);
    fputs(l2,matho_f);
    fputs(l3,matho_f);
    fputs(l4,matho_f);
    //    fputs(l5,matho_f);
    fputs(l6,matho_f);
    fputs(l7,matho_f);
    fputs(l8,matho_f);
    fputs(l9,matho_f);

    fputs(l14,matho_f);
    fputs("\n",matho_f);
    fclose(matho_f) ;

    system("mathomatic -t fileabc$.in >fileabc$.out");

    matho_f = fopen("fileabc$.out", "r" );
    obtener_linea(respuesta,matho_f);
    while(strcmp(respuesta,"4-> SIMPLIFY FRACTION")!=0)
    {

        obtener_linea(respuesta,matho_f);
    }

    obtener_linea(respuesta,matho_f);
    //    strcpy(respuesta," ");
    obtener_linea(respuesta,matho_f);

    int xxx=strncmp(respuesta,"#4:",3);
    if (xxx==0)
    {
        strtok(respuesta," ");
        resp=strtok(NULL," ");
        return(resp);
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
#ifdef NUMERO
    while((resp=strtok(NULL," "))!=0)
    {
        if(strcmp(resp,"=")==0)
        {
            break;
        }
    }
    resp=strtok(NULL," ");
#else
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
#endif
}

char *matho4(char *expresion1, char *expresion2)
{
    FILE *matho_f;
    char l1[]="clear all\n";
    char l11[]="set fraction=0\n";
    char l2[]="b/a\n";
    char l3[]="eliminate a\n";
    char l4[]="eliminate b\n";
    char l9[]="simplify fraction\n";

    char l14[]="quit\n";
    char respuesta[MAX_LONG_LINEA];
    char salida[MAX_LONG_LINEA];
    char *resp;

    //Se crea un archivo de texto con extension .in para ser usado
    //por el mathomatic
    matho_f = fopen("fileabd$.in", "w" );

    fputs(l1,matho_f);
    fputs(l11,matho_f);
    fputs("a=",matho_f);
    fputs(expresion1,matho_f);
    fputs("\nb=",matho_f);
    fputs(expresion2,matho_f);
    fputs("\n",matho_f);
    fputs(l2,matho_f);
    fputs(l3,matho_f);
    fputs(l4,matho_f);
    //    fputs(l5,matho_f);
    fputs(l9,matho_f);

    fputs(l14,matho_f);
    fputs("\n",matho_f);
    fclose(matho_f) ;

    system("mathomatic -t fileabd$.in >fileabd$.out");

    matho_f = fopen("fileabd$.out", "r" );
    obtener_linea(respuesta,matho_f);
    while(strcmp(respuesta,"3-> SIMPLIFY FRACTION")!=0)
    {

        obtener_linea(respuesta,matho_f);
    }

    obtener_linea(respuesta,matho_f);
    obtener_linea(respuesta,matho_f);

    int xxx=strncmp(respuesta,"#3:",3);
    if (xxx==0)
    {
        strtok(respuesta," ");
        resp=strtok(NULL,"\000");
        return(resp);
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
#ifdef NUMERO
    while((resp=strtok(NULL," "))!=0)
    {
        if(strcmp(resp,"=")==0)
        {
            break;
        }
    }
    resp=strtok(NULL," ");
#else
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
#endif
}

char *mathopol(char *expresion)
{
    FILE *matho_f;
    char l1[]="clear all\n";
    char l11[]="set fraction=0\n";
   // char l2[]="simplify fraction\n";

    char l4[]="quit\n";
    char respuesta[MAX_LONG_LINEA];
    //char salida[MAX_LONG_LINEA];
    char *resp;

    //Se crea un archivo de texto con extension .in para ser usado
    //por el mathomatic
    matho_f = fopen("file$.in", "w" );

    fputs(l1,matho_f);
    fputs(l11,matho_f);
    fputs(expresion,matho_f);
    fputs("\n",matho_f);
   // fputs(l2,matho_f);
    fputs(l4,matho_f);

    fclose(matho_f);

    system("mathomatic -t file$.in >file$.out");

    matho_f = fopen("file$.out", "r" );
    obtener_linea(respuesta,matho_f);
//    while(strcmp(respuesta,"1-> SIMPLIFY FRACTION")!=0)
//    {
//        obtener_linea(respuesta,matho_f);
//    }

    obtener_linea(respuesta,matho_f);
    //    strcpy(respuesta," ");
    obtener_linea(respuesta,matho_f);
    obtener_linea(respuesta,matho_f);
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
        return(expresion);
    }

    if (xxx==0)
    {
        resp=strtok(NULL,"\0");

//        int xxx=strncmp(resp,"ANSWER",6);
//        if (xxx==0)
//        {
//            char bbb[MAX_LONG_LINEA];
//            strcpy(bbb,resp);
//            resp=strtok(bbb," = ");
//            resp=strtok(NULL,"= ");
//        }

//        if(strcmp(resp,"ANSWER")==0)
//        {
//            resp=strtok(NULL," ");
//            resp=strtok(NULL," ");
//            fclose(matho_f) ;
//            return(resp);
//        }
//        else
//        {
//            fclose(matho_f) ;
//            return(resp);
//        }
    }
//    else
//    {
//        resp=salida;
//        fclose(matho_f) ;
//        return(resp);
//    }
    return resp;
}
