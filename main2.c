#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>

#include "cadenas.h"
#include "guardar.h"
#include "elementos.h"
#include "matrix.h"
#include "matriz_d.h"
#include "matriz_b.h"
#include "matriz_c.h"
#include "matriz_j.h"
#include "gauss.h"
#include "comandos.h"
#include "matho.h"
#include "laplace.h"


//void coef_pol_pot (char expresion[]);

int maximo(char *a,char *b,int c);

int maximo(char *a,char *b,int c)
{
    int max;
    max=atoi(a);
    if (atoi(b)>max)
        max=atoi(b);
    if (c>max)
        max=c;
    return max;
}

/*******************************PROGRAMA_PRINCIPAL*************************/
int main(int argc,char *argv[])
{
    FILE *input, *salida;

    int comienzo; //numero de parametros
    char nombre[10];
    char ini [10];
    char fin [10];
    char k [10];
    char kp [10];

    //int nptos;
    //double fini;
    //double ffin;

    char vari[10];
    double valini;
    double valfin;
    //double increm;

    int tam;
    int num_v=0,num_i=0,num_pasivo=0,num_ao=0;
    int num_nodos=0;
    //int num_corrientes=0;
    int num_VCVS=0;
    int num_CCCS=0;
    int num_VCCS=0;
    int num_CCVS=0;
    int num_N=0;
    int num_K=0;
    int num_com=0;

    char *resto;

    char valor[15];
    char f_cir[80];
    char f_cir_sal[80];
    char linea[MAX_LONG_LINEA];
    char linea_simple[MAX_LONG_LINEA];
    char pantalla[MAX_LONG_LINEA];

    struct REGISTRO_ELEMENTO *elementos,
            *fuente_tension,
            *fuente_corriente,
            *O,                 //Amplificador ideal
            *E,                 //Fuente de tension controlada por tension VCVS
            *F,                 //Fuente de corriente controlada por corriente CCCS
            *G,                 //Fuente de corriente controlada por tension
            *H,                 //Fuente de tension controlada por corriente
            *N,                 //Transformador ideal
            *K,                 //Coeficiente de acoplamiento del transformador
            *comandos;          //Almacenamos la lista de comandos
    //*conductancia;


    struct REGISTRO_NODO *nodos;

    nodos= NULL;
    //corrientes= NULL;
    elementos = NULL;
    fuente_corriente = NULL;
    fuente_tension = NULL;
    O = NULL;
    E = NULL;
    F = NULL;
    G = NULL;
    H = NULL;
    N = NULL;
    K = NULL;
    comandos = NULL;
    // conductancia = NULL;

    strcpy(ini,"0");
    strcpy(fin,"0");
    strcpy(k,"0");
    strcpy(kp,"0");
    system("del *file$.out");

    // Comienzo del Programa
    if (argc==1){
        printf("\nArchivo que desea analizar:");
        scanf("%s",f_cir);
    }

    if (argc==2){
        strcpy(f_cir,*(argv+1));
    }

    if ((input = fopen(f_cir,"rt")) == NULL)
    {
        fprintf(stderr,"NO EXISTE EL CIRCUITO O LE FALTA LA EXTENSION .CIR \n");
        getch();
        return(1);
    }

    resto=strtok(f_cir,".");
    strcpy(f_cir_sal,resto);
    strcat(f_cir_sal,".out");
    salida = fopen(f_cir_sal, "w" );

    while (!feof(input))
    {
        strcpy(ini,"0");
        strcpy(fin,"0");
        strcpy(k,"0");
        strcpy(kp,"0");
        strcpy(linea,"");
        obtener_linea(linea,input);
        eliminar_espacios(linea,linea_simple);
        //strcpy(linea,linea_simple);
        tam = (int)strlen(linea);
        if ( (linea[0] != '*') && (tam > 0) )
        {
            comienzo=1;
            if(  (linea[0] != 'R') && (linea[0] != 'A') && (linea[0] != 'L') && (linea[0] != 'C') && (linea[0] != 'I') && (linea[0] != 'V') &&
                 (linea[0] != 'O') && (linea[0] != 'E') &&(linea[0] != 'F') &&(linea[0] != 'G') &&(linea[0] != 'H') &&
                 (linea[0] != 'N') && (linea[0] != 'K') && (linea[0] != '.'))
            {
                printf("Se introdujeron en forma erronea los parametros de la red \n ");
                printf("En la linea \n");
                printf("%s \n ",linea);
                getch();
                exit(2);
            }

            // Elementos Pasivos
            if ( (linea[0] !='.') )
            {
                resto=strtok(linea," ");
                strcpy(nombre,resto);
                resto=strtok(NULL," ");
                while (resto!= NULL)
                {
                    switch(comienzo)
                    {
                        case 1:
                            strcpy(ini,resto);
                            break;
                        case 2:
                            strcpy(fin,resto);
                            break;
                        case 3:
                            if(  (linea[0] == 'O') || (linea[0] == 'E') || (linea[0] == 'F') ||
                                 (linea[0] == 'G') || (linea[0] == 'H') || (linea[0] == 'N') ||
                                 (linea[0] == 'K') || (linea[0] == 'T' && linea[1] == 'F'))
                                strcpy(k,resto);
                            else
                                strcpy(valor,resto);
                            break;

                        case 4:
                            if(  (linea[0] == 'E') || (linea[0] == 'F') || (linea[0] == 'G') ||
                                 (linea[0] == 'H') || (linea[0] == 'N') || (linea[0] == 'K') ||
                                 (linea[0] == 'T' && linea[1] == 'F')
                                 )
                                strcpy(kp,resto);
                            break;
                        case 5:
                            if(  (linea[0] == 'E') || (linea[0] == 'F') || (linea[0] == 'G') ||
                                 (linea[0] == 'H') || (linea[0] == 'N') || (linea[0] == 'K') )
                                strcpy(valor,resto);
                            break;
                    }
                    resto=strtok(NULL," ");
                    comienzo++;
                }
                // Validaciones
                if (comienzo == 1)
                {
                    printf("Falta colocar los numeros de los nodos \n");
                    printf("donde se encuentra y el valor del elemento \n ");
                    printf("%s \n",nombre);
                    getch();
                    exit(3);
                }
                if (comienzo == 2)
                {
                    printf("Falta colocar el numero del nodo \n");
                    printf("definido como negativo y el valor del elemento \n ");
                    printf("%s \n",nombre);
                    getch();
                    exit(4);
                }
                if (comienzo == 3)
                {
                    printf("Falta colocar el valor del elemento \n");
                    printf("%s \n",nombre);
                    getch();
                    exit(5);
                }
                // Estan completos los parametros
                if (comienzo == 4)
                {
                    if (strlen(nombre) > 10)
                    {
                        printf("El nombre del elemento \n");
                        printf("%s \n",nombre);
                        printf("es mayor a 10 caracteres .. \n ");
                        getch();
                        exit(6);
                    }
                    if (strlen(valor) > 15)
                    {
                        printf("El valor del elemento \n");
                        printf("%s \n",nombre);
                        printf("es mayor a 15 caracteres .. \n ");
                        getch();
                        exit(7);
                    }

                    if (strcmp(ini,fin)==0)
                    {
                        printf("Los nodos estan mal colocados ... \n");
                        printf("%s %s \n ",ini,fin);
                        getch();
                        exit(8);
                    }
                    if(  (linea[0] == 'O') || (linea[0] == 'E') || (linea[0] == 'F') ||
                         (linea[0] == 'G') || (linea[0] == 'H') || (linea[0] == 'N') || (linea[0] == 'K') )
                    {
                        if (strcmp(k,"0")==0)
                        {
                            printf("La salida del amplificador no puede estar a tierra... \n");
                            printf("%s %s \n ",ini,fin);
                            getch();
                            exit(17);
                        }
                    }
                }
                if (comienzo > 7)
                {
                    printf("ERROR ........ \n ");
                    printf("Demasiados parametros al definir el elemento \n");
                    printf("%s \n",nombre);
                    getch();
                    exit(9);
                }

                if ( (linea[0] == 'R') || (linea[0] == 'A') || (linea[0] == 'L') || (linea[0] == 'C') )
                {
                    elementos=crear_elemento(nombre,ini,fin,k,kp,valor,elementos);
                    num_pasivo++;
                    nodos = crear_nodo(ini,nodos,&num_nodos);
                    nodos = crear_nodo(fin,nodos,&num_nodos);
                }

                if ( (linea[0] == 'I') )
                {
                    fuente_corriente=crear_elemento(nombre,ini,fin,k,kp,valor,fuente_corriente);
                    num_i++;
                    nodos = crear_nodo(ini,nodos,&num_nodos);
                    nodos = crear_nodo(fin,nodos,&num_nodos);
                }

                if ( (linea[0] == 'V') )
                {
                    fuente_tension=crear_elemento(nombre,ini,fin,k,kp,valor,fuente_tension);
                    num_v++;
                    nodos = crear_nodo(ini,nodos,&num_nodos);
                    nodos = crear_nodo(fin,nodos,&num_nodos);
                }

                if ( (linea[0] == 'O') )
                {
                    O=crear_elemento(nombre,ini,fin,k,kp,valor,O);
                    num_ao++;
                    nodos = crear_nodo(ini,nodos,&num_nodos);
                    nodos = crear_nodo(fin,nodos,&num_nodos);
                    nodos = crear_nodo(k,nodos,&num_nodos);
                }

                if ( (linea[0] == 'E') )
                {
                    E=crear_elemento(nombre,ini,fin,k,kp,valor,E);
                    num_VCVS++;
                    nodos = crear_nodo(ini,nodos,&num_nodos);
                    nodos = crear_nodo(fin,nodos,&num_nodos);
                    nodos = crear_nodo(k,nodos,&num_nodos);
                    nodos = crear_nodo(kp,nodos,&num_nodos);
                }

                if ( (linea[0] == 'F') )
                {
                    F=crear_elemento(nombre,ini,fin,k,kp,valor,F);
                    num_CCCS++;
                    nodos = crear_nodo(ini, nodos, &num_nodos);
                    nodos = crear_nodo(fin, nodos, &num_nodos);
                    nodos = crear_nodo(k, nodos, &num_nodos);
                    nodos = crear_nodo(kp, nodos, &num_nodos);
                }

                if ( (linea[0] == 'G') )
                {
                    G=crear_elemento(nombre,ini,fin,k,kp,valor,G);
                    num_VCCS++;
                    nodos = crear_nodo(ini, nodos, &num_nodos);
                    nodos = crear_nodo(fin, nodos, &num_nodos);
                    nodos = crear_nodo(k, nodos, &num_nodos);
                    nodos = crear_nodo(kp, nodos, &num_nodos);
                }

                if ( (linea[0] == 'H') )
                {
                    H=crear_elemento(nombre,ini,fin,k,kp,valor,H);
                    num_CCVS++;
                    nodos = crear_nodo(ini, nodos, &num_nodos);
                    nodos = crear_nodo(fin, nodos, &num_nodos);
                    nodos = crear_nodo(k, nodos, &num_nodos);
                    nodos = crear_nodo(kp, nodos, &num_nodos);
                }

                if ( (linea[0] == 'N') )
                {
                    N=crear_elemento(nombre,ini,fin,k,kp,valor,N);
                    num_N++;
                    nodos = crear_nodo(ini, nodos, &num_nodos);
                    nodos = crear_nodo(fin, nodos, &num_nodos);
                    nodos = crear_nodo(k, nodos, &num_nodos);
                    nodos = crear_nodo(kp, nodos, &num_nodos);
                }

                if ( (linea[0] == 'K') )
                {
                    K=crear_elemento(nombre,ini,fin,k,"0","0",K);
                    num_K++;
                }
            }

            // Procesa un comando
            else if (linea[0] =='.')
            {
                resto=strtok(linea," ");
                strcpy(nombre,resto);
                resto=strtok(NULL," ");
                while (resto!= NULL)
                {
                    switch(comienzo)
                    {
                        case 1:
                            strcpy(ini,resto);
                            break;
                        case 2:
                            strcpy(fin,resto);
                            break;
                        case 3:
                            strcpy(k,resto);
                            break;

                        case 4:
                            strcpy(kp,resto);
                            break;

                        case 5:
                            break;
                    }
                    resto=strtok(NULL," ");
                    comienzo++;
                }

                if ( (linea[1] == 'T') && (linea[2] == 'F') )
                {
                    comandos=crear_elemento("TF",ini,fin,k,kp,valor,comandos);
                }
                if ( (linea[1] == 'D') && (linea[2] == 'C') )
                {
                    comandos=crear_elemento("DC",ini,fin,k,kp,valor,comandos);

                    if(strcmp(ini,"LIN")==0)
                    {
                        strcpy(vari,fin);
                        valini = atof(k);
                        valfin = atof(kp);
                        //increm = atof(valor);
                        if (valini>valfin)
                        {
                            printf("ERROR el valor inicial es mayor que el valor final");
                        }
                    }
                    if(strcmp(ini,"OCT")==0)
                    {
                        strcpy(vari,fin);
                        valini = atof(k);
                        valfin = atof(kp);
                        //increm = atof(valor);
                        if (valini>valfin)
                        {
                            printf("ERROR el valor inicial es mayor que el valor final");
                        }
                    }
                    if(strcmp(ini,"DEC")==0)
                    {
                        strcpy(vari,fin);
                        valini = atof(k);
                        valfin = atof(kp);
                        //increm = atof(valor);
                        if (valini>valfin)
                        {
                            printf("ERROR el valor inicial es mayor que el valor final");
                        }
                    }
                }
                if ( (linea[1] == 'T') && (linea[2] == 'R') && (linea[3] == 'A')&& (linea[4] == 'N') )
                {
                    comandos=crear_elemento("TRAN",ini,fin,"0","0","0",comandos);
                }
                if ( (linea[1] == 'A') && (linea[2] == 'C') )
                {

                    comandos=crear_elemento("AC",ini,fin,k,kp,"0",comandos);
                    num_com++;

//                    if(strcmp(ini,"DEC")==0)
//                    {
//                        nptos = atoi(fin);
//                        fini = atof(k);
//                        ffin = atof(kp);
//                        if (fini>ffin)
//                        {
//                            printf("ERROR el valor finicial es mayor que el de ffinal");
//                        }
//                    }
//                    else
//                    {
//                        nptos = atoi(ini);
//                        fini = atof(fin);
//                        ffin = atof(k);
//                    }
                }
                if ( (linea[1] == 'S') && (linea[2] == 'I') && (linea[3] == 'M') )
                {
                    strtok(linea," ");
                    if (strlen(linea)>4)
                    {
                        printf("Exede el numero de caracteres para el comando .SIM \n");
                        printf("\n ");
                        getch();
                        exit(111);
                    }
                    else
                    {
                    }
                }
            }
        }
    }
    fclose(input);

    struct eeSTATE *EST;
    EST = eeAlloc();
    eeClearAllVars(EST);   // Limpia las variables
    int i;

    if((elementos!=NULL))
    {
        for(i=1; i <=num_pasivo; i++)
        {
            static struct REGISTRO_ELEMENTO *aux;
            aux=elementos;

            if(strcmp(aux->valor,"SYMBOLIC")!=0)
                inicia_simbolico(elementos, EST);
            if(aux->sig !=NULL)
                aux=(struct REGISTRO_ELEMENTO *)aux->sig;
        }
    }

    if((fuente_tension!=NULL))
    {
        for(i=1; i <=num_v; i++)
        {
            static struct REGISTRO_ELEMENTO *aux;
            aux=fuente_tension;

            if(strcmp(aux->valor,"SYMBOLIC")!=0)
                inicia_simbolico(fuente_tension, EST);
            if(aux->sig !=NULL)
                aux=(struct REGISTRO_ELEMENTO *)aux->sig;
        }
    }

    if((fuente_corriente!=NULL))
    {
        for(i=1; i <=num_i; i++)
        {
            static struct REGISTRO_ELEMENTO *aux;
            aux=fuente_corriente;

            if(strcmp(aux->valor,"SYMBOLIC")!=0)
                inicia_simbolico(fuente_corriente, EST);
            if(aux->sig !=NULL)
                aux=(struct REGISTRO_ELEMENTO *)aux->sig;
        }
    }

    //int i;
    for ( i = 0; i < eeMAXVARS; i++ )
    {
        if ( *(EST->Vars[i].name) ) {
            if ( eet_is_num(&(EST->Vars[i].value)) )
                printf( "%s = %lg\n", EST->Vars[i].name, EST->Vars[i].value.dat.x );
            else if ( eet_is_str(&(EST->Vars[i].value)) )
                printf( "%s = \"%s\"\n", EST->Vars[i].name, EST->Vars[i].value.dat.p );
            else
                printf( "%s = NIL\n", EST->Vars[i].name );
        } // if
    }

    struct REGISTRO_NODO     **indice_nodo    = (struct REGISTRO_NODO     **)malloc(num_nodos  * sizeof(struct REGISTRO_NODO     *));
    struct REGISTRO_ELEMENTO **indice_elemento= (struct REGISTRO_ELEMENTO **)malloc(num_pasivo * sizeof(struct REGISTRO_ELEMENTO *));
    struct REGISTRO_ELEMENTO **indice_factv   = (struct REGISTRO_ELEMENTO **)malloc(num_v      * sizeof(struct REGISTRO_ELEMENTO *));
    struct REGISTRO_ELEMENTO **indice_facti   = (struct REGISTRO_ELEMENTO **)malloc(num_i      * sizeof(struct REGISTRO_ELEMENTO *));
    struct REGISTRO_ELEMENTO **indice_ao      = (struct REGISTRO_ELEMENTO **)malloc(num_ao     * sizeof(struct REGISTRO_ELEMENTO *));
    struct REGISTRO_ELEMENTO **indice_VCVS    = (struct REGISTRO_ELEMENTO **)malloc(num_VCVS   * sizeof(struct REGISTRO_ELEMENTO *));
    struct REGISTRO_ELEMENTO **indice_CCCS    = (struct REGISTRO_ELEMENTO **)malloc(num_CCCS   * sizeof(struct REGISTRO_ELEMENTO *));
    struct REGISTRO_ELEMENTO **indice_VCCS    = (struct REGISTRO_ELEMENTO **)malloc(num_VCCS   * sizeof(struct REGISTRO_ELEMENTO *));
    struct REGISTRO_ELEMENTO **indice_CCVS    = (struct REGISTRO_ELEMENTO **)malloc(num_CCVS   * sizeof(struct REGISTRO_ELEMENTO *));
    struct REGISTRO_ELEMENTO **indice_N       = (struct REGISTRO_ELEMENTO **)malloc(num_N      * sizeof(struct REGISTRO_ELEMENTO *));
    struct REGISTRO_ELEMENTO **indice_K       = (struct REGISTRO_ELEMENTO **)malloc(num_K      * sizeof(struct REGISTRO_ELEMENTO *));

    indice_nodo     = ordenar_nodos(nodos,           indice_nodo,    num_nodos );
    indice_elemento = ordenar      (elementos,       indice_elemento,num_pasivo);
    indice_factv    = ordenar      (fuente_tension,  indice_factv,   num_v     );
    indice_facti    = ordenar      (fuente_corriente,indice_facti,   num_i     );
    indice_ao       = ordenar      (O    ,           indice_VCVS,    num_ao  );
    indice_VCVS     = ordenar      (E    ,           indice_VCVS,    num_VCVS  );
    indice_CCCS     = ordenar      (F    ,           indice_CCCS,    num_CCCS  );
    indice_VCCS     = ordenar      (G    ,           indice_VCCS,    num_VCCS  );
    indice_CCVS     = ordenar      (H    ,           indice_CCVS,    num_CCVS  );
    indice_N        = ordenar      (N    ,           indice_N   ,    num_N     );
    indice_K        = ordenar      (K    ,           indice_K   ,    num_K     );

    exibir_nodos    (indice_nodo    ,num_nodos ,salida);
    exibir_elementos(indice_elemento,num_pasivo,salida);
    exibir_elementos(indice_factv   ,num_v     ,salida);
    exibir_elementos(indice_facti   ,num_i     ,salida);
    exibir_elementos(indice_ao      ,num_ao    ,salida);
    exibir_elementos(indice_VCVS    ,num_VCVS  ,salida);
    exibir_elementos(indice_CCCS    ,num_CCCS  ,salida);
    exibir_elementos(indice_VCCS    ,num_VCCS  ,salida);
    exibir_elementos(indice_CCVS    ,num_CCVS  ,salida);
    exibir_elementos(indice_N       ,num_N     ,salida);
    exibir_elementos(indice_K       ,num_K     ,salida);

    int num_fuentes_equi=num_v+num_ao+num_VCVS+num_CCCS+(num_CCVS)*2+num_N+(num_K)*2;

    //char *indice;
    struct MATRIZ **mat_G=NULL; //Matriz G                    /* puntero de puntero */
    struct MATRIZ **mat_B=NULL; //Matriz B
    struct MATRIZ **mat_C=NULL; //Matriz C
    struct MATRIZ **mat_D=NULL; //Matriz D
    struct MATRIZ **mat_A=NULL; //Matriz A
    struct MATRIZ **mat_v=NULL; //Matriz v
    struct MATRIZ **mat_j=NULL; //Matriz j
    struct MATRIZ **mat_x=NULL; //Matriz x
    struct MATRIZ **mat_i=NULL; //Matriz i
    struct MATRIZ **mat_e=NULL; //Matriz e
    struct MATRIZ **mat_z=NULL; //Matriz z
    struct MATRIZ **mat_gauss=NULL; //Matriz gauss
    struct MATRIZ **inv_mat_A=NULL; //inversa Matriz A

    //Matriz G
    mat_G=crear_matriz_din (mat_G,num_nodos,num_nodos);
    ini_matrix_simb (mat_G,num_nodos,num_nodos);
    asig_matriz_G(mat_G,num_nodos,num_nodos,elementos,indice_nodo,K);
    asig_matriz_VCCS(mat_G,G,indice_nodo);
    imprime_matrix_arch("Matriz G",mat_G,num_nodos,num_nodos,salida);

    //Matriz B
    mat_B=crear_matriz_din (mat_B,num_nodos,num_fuentes_equi);
    ini_matrix_simb  (mat_B,num_nodos,num_fuentes_equi);
    asig_matriz_BV   (mat_B,fuente_tension  ,indice_nodo);
    asig_matriz_Boa  (mat_B,O               ,indice_nodo,num_v);
    asig_matriz_BVCVS(mat_B,E               ,indice_nodo,num_v+num_ao);
    asig_matriz_BCCCS(mat_B,F               ,indice_nodo,num_v+num_ao+num_VCVS);

    //aqui parece que hay un error num_v+num_ao+num_VCVS+num_CCCS+(num_CCVS)*2+num_N+(num_K)*2
    asig_matriz_BCCVS(mat_B,H               ,indice_nodo,num_v+num_ao+num_VCVS+num_CCCS,num_nodos);
    asig_matriz_BN   (mat_B,N               ,indice_nodo,num_v+num_ao+num_VCVS+num_CCCS+num_CCVS);
    asig_matriz_BK   (mat_B,K               ,indice_nodo,num_v+num_ao+num_VCVS+num_CCCS+num_CCVS+num_N,elementos);
    imprime_matrix_arch("Matriz B",mat_B,num_nodos,num_fuentes_equi,salida);

    //Matriz C
    mat_C=crear_matriz_din (mat_C, num_fuentes_equi, num_nodos);
    ini_matrix_simb (mat_C, num_fuentes_equi, num_nodos);
    asig_matriz_CV        (mat_C, fuente_tension  ,indice_nodo);
    asig_matriz_Coa       (mat_C, O               ,indice_nodo   ,num_v);
    asig_matriz_CVCVS     (mat_C, E               ,indice_nodo   ,num_v+num_ao);
    asig_matriz_CCCCS     (mat_C, F               ,indice_nodo   ,num_v+num_ao+num_VCVS);

    //aqui parece que hay un error num_v+num_ao+num_VCVS+num_CCCS+(num_CCVS)*2+num_N+(num_K)*2
    asig_matriz_CCCVS     (mat_C, H               ,indice_nodo   ,num_v+num_ao+num_VCVS+num_CCCS);
    asig_matriz_CN        (mat_C, N               ,indice_nodo   ,num_v+num_ao+num_VCVS+num_CCCS+num_CCVS);
    asig_matriz_CK        (mat_C, K               ,indice_nodo   ,num_v+num_ao+num_VCVS+num_CCCS+num_CCVS+num_N,elementos);
    imprime_matrix_arch("Matriz C",mat_C,num_fuentes_equi, num_nodos,salida);

    //Matriz D
    mat_D=crear_matriz_din (mat_D, num_fuentes_equi, num_fuentes_equi);
    ini_matrix_simb (mat_D, num_fuentes_equi, num_fuentes_equi);
    asig_matriz_D_CCVS(mat_D,H,num_v+num_ao+num_VCVS+num_CCCS);

    //aqui parece que hay un error num_v+num_ao+num_VCVS+num_CCCS+(num_CCVS)*2+num_N+(num_K)*2
    asig_matriz_D_K(mat_D,K,num_v+num_ao+num_VCVS+num_CCCS+num_CCVS+num_N,elementos);
    imprime_matrix_arch("Matriz D",mat_D, num_fuentes_equi, num_fuentes_equi,salida);

    //    //Matriz A
    mat_A=crear_matriz_din (mat_A, (num_nodos+num_fuentes_equi), (num_nodos+num_fuentes_equi));
    ini_matrix_simb (mat_A, (num_nodos+num_fuentes_equi), (num_nodos+num_fuentes_equi));
    asig_matriz_A(mat_A, mat_G, mat_B, mat_C, mat_D, num_nodos, num_fuentes_equi);
    imprime_matrix_arch("Matriz A",mat_A,num_nodos+num_fuentes_equi, num_nodos+num_fuentes_equi,salida);

    //    //Matriz v
    mat_v=crear_matriz_din (mat_v, num_nodos, 1);
    ini_matrix_simb (mat_v, num_nodos,1);
    asig_matriz_v(mat_v, indice_nodo, num_nodos);
    imprime_matrix_arch("Matriz V",mat_v,num_nodos,1,salida);

    //    //Matriz j
    mat_j=crear_matriz_din (mat_j, num_fuentes_equi, 1);
    ini_matrix_simb        (mat_j, num_fuentes_equi,1);
    asig_matriz_jV   (mat_j, fuente_tension  , num_v);
    asig_matriz_joa  (mat_j, O               , num_ao  , num_v);
    asig_matriz_jVCVS(mat_j, E               , num_VCVS, num_v+num_ao);
    asig_matriz_jCCCS(mat_j, F               , num_CCCS, num_v+num_ao+num_VCVS);
    asig_matriz_jCCVS(mat_j, H               , num_CCVS, num_v+num_ao+num_VCVS+num_CCCS);

    //aqui parece que hay un error num_v+num_ao+num_VCVS+num_CCCS+(num_CCVS)*2+num_N+(num_K)*2
    asig_matriz_jN   (mat_j, N               , num_N   , num_v+num_ao+num_VCVS+num_CCCS+num_CCVS);
    asig_matriz_jK   (mat_j, K               , num_K   , num_v+num_ao+num_VCVS+num_CCCS+num_CCVS+num_N);
    imprime_matrix_arch("Matriz J",mat_j,num_v+num_ao+num_VCVS+num_CCCS+(num_CCVS)*2, 1,salida);

    //    //Matriz x
    mat_x=crear_matriz_din (mat_x, (num_nodos+num_fuentes_equi), 1);
    ini_matrix_simb (mat_x, (num_nodos+num_fuentes_equi), 1);
    asig_matriz_x(mat_x, mat_v, mat_j, num_nodos, num_fuentes_equi);
    imprime_matrix_arch("Matriz X",mat_x,num_nodos+num_fuentes_equi, 1,salida);

    //  //Matriz i
    mat_i=crear_matriz_din (mat_i, num_nodos, 1);
    ini_matrix_simb (mat_i, num_nodos,1);
    asig_matriz_i(mat_i, fuente_corriente,indice_nodo);
    imprime_matrix_arch("Matriz i",mat_i,num_nodos,1,salida);

    //Matriz e
    mat_e=crear_matriz_din (mat_e, num_fuentes_equi, 1);
    ini_matrix_simb (mat_e, num_fuentes_equi,1);
    asig_matriz_e(mat_e, fuente_tension,num_v);
    imprime_matrix_arch("Matriz e",mat_e,num_v+num_ao+num_VCVS+num_CCCS, 1,salida);

    //Matriz z
    mat_z=crear_matriz_din (mat_z, (num_nodos+num_fuentes_equi), 1);
    ini_matrix_simb (mat_z, (num_nodos+num_fuentes_equi), 1);
    asig_matriz_z(mat_z, mat_i, mat_e, num_nodos, num_fuentes_equi);
    imprime_matrix_arch("Matriz Z",mat_z,(num_nodos+num_fuentes_equi), 1,salida);

    //Matriz Gauss
    //resuelve gauss metodo 1
    //        mat_gauss=crear_matriz_din (mat_gauss, (num_nodos+num_fuentes_equi), (num_nodos+num_fuentes_equi)+1);
    //        ini_matrix_simb (mat_gauss, (num_nodos+num_fuentes_equi), (num_nodos+num_fuentes_equi)+1);
    //        asig_matriz_gauss(mat_gauss, mat_A, mat_z, (num_nodos+num_fuentes_equi));
    //        imprime_matrix_arch("Matriz Gauss", mat_gauss, (num_nodos+num_fuentes_equi), (num_nodos+num_fuentes_equi)+1,salida);
    //        gauss(mat_x, mat_gauss, (num_nodos+num_fuentes_equi),salida);
    //        fflush(stdin);
    //        getchar();

    //    //    resuelve gauss metodo 2
    mat_gauss=crear_matriz_din (mat_gauss, (num_nodos+num_fuentes_equi), (num_nodos+num_fuentes_equi)*2);
    ini_matrix_simb (mat_gauss, (num_nodos+num_fuentes_equi), (num_nodos+num_fuentes_equi)*2);
    asig_matriz_gauss1(mat_gauss, mat_A, (num_nodos+num_fuentes_equi));
    imprime_matrix_arch("Matriz Gauss1", mat_gauss, (num_nodos+num_fuentes_equi), (num_nodos+num_fuentes_equi)*2,salida);

    gauss1(mat_gauss,(num_nodos+num_fuentes_equi),salida);
    imprime_matrix_arch("resultados", mat_gauss, (num_nodos+num_fuentes_equi), (num_nodos+num_fuentes_equi)*2,salida);
    //    //    fflush(stdin);
    //    //    getchar();

    //    //    Inversa Matriz A
    inv_mat_A=crear_matriz_din       (inv_mat_A, (num_nodos+num_fuentes_equi), (num_nodos+num_fuentes_equi));
    ini_matrix_simb                  (inv_mat_A, (num_nodos+num_fuentes_equi), (num_nodos+num_fuentes_equi));
    //imprime_matrix("Inversa Matriz A",inv_mat_A,num_nodos+num_fuentes_equi, num_nodos+num_fuentes_equi);


    char sd [MAX_LONG_LINEA];
    char variable1[MAX_LONG_LINEA];

    for(i=0;i<num_nodos+num_fuentes_equi;i++)
    {
        for(int j=0;j<num_nodos+num_fuentes_equi;j++)
        {
            strcpy(sd,leer_ele_matriz(mat_gauss, i+1, j+1+num_nodos+num_fuentes_equi));
            asig_matriz_din(inv_mat_A, i+1, j+1,sd);
        }
    }
    imprime_matrix_arch("Inversa Matriz A",inv_mat_A,num_nodos+num_fuentes_equi, num_nodos+num_fuentes_equi,salida);

    //    //calcula solucion
    for(i=0;i<num_nodos+num_fuentes_equi;i++)
    {
        strcpy(sd,"");
        for(int j=0;j<num_nodos+num_fuentes_equi;j++)
        {
            strcat(sd,"(");
            strcat(sd,leer_ele_matriz(inv_mat_A, i+1, j+1));
            strcat(sd,")*(");
            strcat(sd,&(*(mat_z[j])->expresion));
            strcat(sd,")+");
        }
        strcat(sd,"0");
        strcpy(variable1,matho2(sd));
        strcpy(&(*(mat_x[i])->otro_dato),variable1);
    }

    //    //imprime solucion
    sprintf(pantalla,"\n\nTensiones en nodos y Corrientes\n");
    fputs(pantalla,salida);
    printf("%s",pantalla);

    for (i = 0; i < (num_nodos+num_fuentes_equi); i++)
    {
        sprintf(pantalla,"          %s=%s\n\n\n",&(*(mat_x[i])->expresion),&(*(mat_x[i])->otro_dato));
        fputs(pantalla,salida);
        printf("%s",pantalla);
    }

    eval_comandos(comandos,
                  fuente_tension,
                  fuente_corriente,
                  mat_x,
                  num_nodos+num_fuentes_equi,
                  elementos,
                  indice_VCVS,
                  indice_CCCS,
                  indice_VCCS,
                  indice_CCVS,
                  indice_N,
                  indice_N,
                  salida);

//    printf("%s",fun_respuesta_tiempo("1/(S^2+3*S+2)"));

    for(i = 0; i < num_nodos; i++)
    {
        free( mat_G[i] );
    }
    free(mat_G);
    return 0;
}


