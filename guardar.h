#ifndef GUARDAR_H
#define GUARDAR_H

#include "elementos.h"
#include "ee2.h"

void inicia_simbolico(struct REGISTRO_ELEMENTO *lista_elemento, struct eeSTATE *EST);
int convierte(char *elemento, char *valor, char *delimitador,double orden, struct eeSTATE *EST, double *valorn);
int isNumeric(const char *val);
int isnumeric1(char *st);

#endif // GUARDAR_H
