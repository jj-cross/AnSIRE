#ifndef ELEMENTOS_H
#define ELEMENTOS_H
#include "cadenas.h"

#define EXISTE 1
#define NOEXISTE !EXISTE
#define ELE_MATRIZ 100


//#define NUMERO matriz numerica

// Declaracion de la estructura para ordenar la lista de los elementos de la red
struct  REGISTRO_ELEMENTO{
        char elemento[10];      // Nombre del elemento
        char valor[15];         // Valor del elemento
        double valornum;
        char nodo_1[10];
        char nodo_2[10];
        char nodo_3[10];
        char nodo_4[10];
        struct REGISTRO_ELEMENTO *sig;
};

struct  REGISTRO_NODO{
        char valor[MAX_LONG_LINEA];
        char nodo[10];
        struct REGISTRO_NODO *sig;
};
struct REGISTRO_ELEMENTO *asigna_elemento(char *nombre, char *valor, struct REGISTRO_ELEMENTO *REGISTRO_ele, char *nodo_1, char *nodo_2, char *nodo_3, char *nodo_4);
struct REGISTRO_ELEMENTO *crear_elemento(char *nombre, char *ini, char *fin, char *k, char *kp, char *valor, struct REGISTRO_ELEMENTO *REGISTRO_elemento);
int busca_elemento(struct REGISTRO_ELEMENTO *REGISTRO_nodo, char *palabra);
char * busca_valor_dp(char *tem, struct REGISTRO_ELEMENTO *REGISTRO_nodo, char *nodo, struct REGISTRO_ELEMENTO *K);
char * busca_valor_ad(char *tem, struct REGISTRO_ELEMENTO *REGISTRO_nodo, char *nodo1, char *nodo2, struct REGISTRO_ELEMENTO *K);
void ordenar_elemento(struct REGISTRO_ELEMENTO **indice, const int nodos);
int comparar_elemento(const void *ptr1, const void *ptr2);
void exibir_elementos(struct REGISTRO_ELEMENTO **indice, int num_nodos, FILE *salida);
void ordenar_nod(struct REGISTRO_NODO **indice, const int nodos);
struct REGISTRO_NODO *crear_nodo(char *nodo, struct REGISTRO_NODO *REGISTRO_nodo, int *num_nodos);
int busca_nodo(struct REGISTRO_NODO *REGISTRO_nodo, char *nodo);
struct REGISTRO_NODO *asignar_nodo(struct REGISTRO_NODO *REGISTRO_nodo, char *nodo);
struct REGISTRO_NODO     **ordenar_nodos(struct REGISTRO_NODO     *nodos,     struct REGISTRO_NODO     **indice_nodo,     int num_nodos);
struct REGISTRO_ELEMENTO **ordenar      (struct REGISTRO_ELEMENTO *elementos, struct REGISTRO_ELEMENTO **indice_elemento, int num_pasivo);
int comparar_nodo(const void *ptr1, const void *ptr2);
void exibir_nodos(struct REGISTRO_NODO **indice, int num_nodos, FILE *salida);
char *nombre_nodo(struct REGISTRO_NODO **indice, int nodo);
int indice_nodo(struct REGISTRO_NODO **indice, char *nombre);
int busca_acopl(struct REGISTRO_ELEMENTO *REGISTRO_nodo, char *palabra);
struct REGISTRO_ELEMENTO *busca_elemento2(struct REGISTRO_ELEMENTO *elemento, char *K);

#endif // ELEMENTOS_H
