#ifndef DEF_H
#define DEF_H

#ifndef M_PI
#define M_PI    3.14159265358979323846
#endif
#ifndef M_E
#define M_E     2.71828182845904523536
#endif

#define GRADO 7
#define INDICE 6

#define ANCHO 700

#define FENTO 1e-15
#define PICO  1e-12
#define NANO  1e-9
#define MICRO 1e-6
#define MILI  1e-3

#define KILO  1e3
#define MEGA  1e6
#define GIGA  1e9
#define TERA  1e12

#define ESTABLE ( (raices_polb[0].r<0)&&(raices_polb[1].r<0)&&(raices_polb[2].r<0)&&(raices_polb[3].r<0)&&(raices_polb[4].r<0)&&(raices_polb[5].r<0) )

#define EP 1.0e-10

//#pragma warn -par
//#pragma warn -aus
//#pragma warn -use
//#pragma warn -wcln

typedef struct
{
double r;
double i;
} complejo;

#endif // DEF_H
