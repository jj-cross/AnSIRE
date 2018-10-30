#include <stdio.h>
#include <string.h>
#include <math.h>

#include "def.h"
#include "laplace.h"

// Funcion donde se realiza la transformada inversa de Laplace para
// obtener la respuesta impulsiva del circuito.
char *laplace_inv(double nu[], double de[], int gr_nu, int gr_de)
{
    int KK;
    register int var;
    complejo raices_polb1[GRADO];
    double G1[GRADO];
    double A1[10];
    double G2[GRADO];
    double A2[10];
    double ARE[GRADO];
    double AIM[GRADO];

    char mas[5];
    char funcion[ANCHO];
    char *fun;
    fun=funcion;

    strcpy(funcion,"");
    strcpy(mas," + ");

    memset(G1,0,sizeof(G1));
    memset(G2,0,sizeof(G2));
    memset(ARE,0,sizeof(ARE));
    memset(AIM,0,sizeof(AIM));
    memset(AIM,0,sizeof(AIM));
    memset(A1,0,sizeof(A1));
    memset(A2,0,sizeof(A2));

    // Valor de bandera -1e200
    for(var=0;var<=INDICE;var++)
    {
        raices_polb1[var].r = -1e200;
        raices_polb1[var].i = -1e200;
    }

    if (gr_de > gr_nu)
        separa_en_fracciones_parciales(nu,de,gr_nu,gr_de,ARE,AIM,raices_polb1);

    // Funcion impropia (se va a limitar a solo gr_nu = gr_de)
    // o a lo sumo gr_nu = gr_de+1)
    else if (gr_de == gr_nu)
    {
        division_larga(nu,de,gr_nu,gr_de,G1,A1);
        // Cociente
        for (var=0;var<=1;var++)
        {
            G2[var]=G1[var];
        }

        // Resto
        for (var=(gr_de-1);var>=0;var--)
        {
            // AR[5] = nu[0];
            KK=gr_de-1-var;
            A2[KK]=A1[var];
        }
        separa_en_fracciones_parciales(A2,de,(gr_de-1),gr_de,ARE,AIM,raices_polb1);
        // Desde G2[0] hasta G2[gr_nu-gr_de] hay terminos que hay que sumarle a la
        // expansion en fracciones parciales(donde esta guardado el cociente)
        // Desde A2[0] hasta A2[gr_de-1] es donde esta guardado el resto
        // que va a ser ahora el numerador
    }
    else if (gr_de == gr_nu-1)
    {
        division_larga(nu,de,gr_nu,gr_de,G1,A1);
        // Cociente
        for (var=1;var>=0;var--)
        {
            // AR[5] = nu[0];
            KK=1-var;
            G2[KK]=G1[var];
        }

        // Resto
        for (var=(gr_de-1);var>=0;var--)
        {
            // AR[5] = nu[0];
            KK=gr_de-1-var;
            A2[KK]=A1[var];
        }
        separa_en_fracciones_parciales(A2,de,(gr_de-1),gr_de,ARE,AIM,raices_polb1);
        // Desde G2[0] hasta G2[gr_nu-gr_de] hay terminos que hay que sumarle a la
        // expansion en fracciones parciales(donde esta guardado el cociente)
        // Desde A2[0] hasta A2[gr_de-1] es donde esta guardado el resto
        // que va a ser ahora el numerador
    }

    // Parte donde se hace la antitransformada de Laplace, que se escribe
    // en una cadena para posteriormente mostrarla por pantalla, ser
    // evaluada y graficada.

    strcpy(funcion,"");

    // Impulso en el origen
    if ( (G2[0] != 0) && (G2[1] == 0) )
        sprintf(funcion,"%f*impulso",G2[0]);

    /*
// Doblete en el origen
if ( (G2[0] == 0) && (G2[1] != 0) )
sprintf(funcion,"%f*delta'(t)",G2[1]);

// Impulso en el origen mas un doblete en el origen
if ( (G2[0] != 0) && (G2[1] != 0) )
sprintf(funcion,"%f*delta(t) + %f*delta'(t)",G2[0],G2[1]);
*/

























    for(var=0;var<=5;var++)
    {
        if( (raices_polb1[var].r != -1e200) && (raices_polb1[var].i != -1e200) )
        {
            // Raiz real pura
            if (raices_polb1[var].i == 0)
            {
                if ( strcmp(funcion,"") != 0 )
                {
                    strcat(funcion,mas);
                    strcat(funcion,exponencial(raices_polb1[var].r,ARE[var]));
                }
                else if ( strcmp(funcion,"") == 0 )
                    strcat(funcion,exponencial(raices_polb1[var].r,ARE[var]));
            }
            // Raiz imaginaria pura
            if ( (raices_polb1[var].r == 0) && (raices_polb1[var].i != 0) )
            {
                if ( strcmp(funcion,"") != 0 )
                {
                    strcat(funcion,mas);
                    strcat(funcion,coseno_seno(ARE[var],AIM[var],raices_polb1[var].r,raices_polb1[var].i));
                    var++;
                }
                else if ( strcmp(funcion,"") == 0 )
                {
                    strcat(funcion,coseno_seno(ARE[var],AIM[var],raices_polb1[var].r,raices_polb1[var].i));
                    var++;
                }
            }
            // Raices con componente real e imaginaria
            if ( (raices_polb1[var].r != 0) && (raices_polb1[var].i != 0) )
            {
                if ( strcmp(funcion,"") != 0 )
                {
                    strcat(funcion,mas);
                    strcat(funcion,coseno_seno(ARE[var],AIM[var],raices_polb1[var].r,raices_polb1[var].i));
                    var++;
                }
                else if ( strcmp(funcion,"") == 0 )
                {
                    strcat(funcion,coseno_seno(ARE[var],AIM[var],raices_polb1[var].r,raices_polb1[var].i));
                    var++;
                }
            }
        }
    }
    return(fun);
}

// Separa en fracciones parciales y calcula las constantes
void separa_en_fracciones_parciales(double pola[],double polb[],int grado_a,int grado_b,double AR[],double AI[],complejo raices_polb[])
{
    register int i,j;
    register int i1,j1;

    static int n; // Grado del denominador-1
    int K;
    static double PR[6], PI[6];
    static double AR2[6], AI2[6];
    double AS, DR, DI, YR, YI, Y2;

    register int ijk;
    double pold[GRADO];
    double polb_aux[GRADO];

    //clrscr();

    memset(pold,0,sizeof(pold));
    memset(polb_aux,0,sizeof(polb_aux));

    if (polb[grado_b] != 1)
        for(ijk=0;ijk<=grado_b;ijk++)
            polb_aux[ijk]=polb[ijk]/polb[grado_b];

    // (Grado del denominador - 1)
    n = grado_b-1;   // Numero de raices del denominador - 1

    // Para hallar las raices del denominador
    if (polb[grado_b] != 1)
        polrt(polb_aux,pold,grado_b,raices_polb);
    else if (polb[grado_b] == 1)
        polrt(polb,pold,grado_b,raices_polb);

    memset(PR,0,sizeof(PR));
    memset(PI,0,sizeof(PI));
    memset(AR2,0,sizeof(AR2));
    memset(AI2,0,sizeof(AR2));

    for (ijk=0;ijk<=n;ijk++)
    {
        if (raices_polb[ijk].r !=-1e200)
            PR[ijk]=raices_polb[ijk].r;
        if (raices_polb[ijk].i !=-1e200)
            PI[ijk]=raices_polb[ijk].i;
    }

    // Se va ordenando el numerador de mayor a menor en el arreglo
    for (ijk=n;ijk>=0;ijk--)
    {
        // AR[5] = nu[0];
        K=n-ijk;
        AR[K]=pola[ijk];
        AI[K]=0;
    }

    for (i=0;i<=n;i++)
    {
        i1=n-i;
        if (i1 == 0)
            goto uno;
        for(j=1;j<=i1;j++)
        {
            AR[j]=AR[j]+(PR[i]*AR[j-1])-(PI[i]*AI[j-1]);
            AI[j]=AI[j]+(PI[i]*AR[j-1])+(PR[i]*AI[j-1]);
        }
uno:
        for(j=0;j<=i;j++)
        {
            j1=n-j;
            if(j == 0)
                goto dos;
            if ( ( (PR[j]-PR[j-1])*(PR[j]-PR[j-1]) + (PI[j]-PI[j-1])*(PI[j]-PI[j-1]) ) <= EP )
                goto tres;
            AR[i1] = AR[i1] - AR[j1+1];
            AI[i1] = AI[i1] - AI[j1+1];
dos:
            if ( ( (PR[j]-PR[i])*(PR[j]-PR[i]) + (PI[j]-PI[i])*(PI[j]-PI[i]) ) <= EP )
                goto cuatro;
            YR = PR[j]-PR[i];
            YI = PI[j]-PI[i];
            Y2 = (YR*YR)+(YI*YI);
            AS = ((AR[j1]*YR)+(AI[j1]*YI))/Y2;
            AI[j1] = ((AI[j1]*YR)-(AR[j1]*YI))/Y2;
            AR[j1] = AS;
            goto cinco;
tres:
            DR = AR[j1]-AR[j1+1];
            DI = AI[j1]-AI[j1+1];
            AR[j1] = (DR*YR+DI*YI)/Y2;
            AI[j1] = (DI*YR-DR*YI)/Y2;
cinco:
            ;
        }
cuatro:
        ;
    }

    if (polb[grado_b] != 1)
        for(ijk=0;ijk<=grado_b;ijk++)
        {
            AR[ijk]=AR[ijk]/polb[grado_b];
            AI[ijk]=AI[ijk]/polb[grado_b];
        }

    /*
clrscr();

printf("La parte (Real,Imag) de las constantes en el orden ");
printf("de como se introdujeron las raices respecto a la multiplicidad \n\n ");

// AR parte real de la constante
// AI parte imaginaria de la constante

for(i=0;i<=n;i++)
{
K=n-i;
printf("# %d %e %e   %e %e \n ",i,AR[K],AI[K],raices_polb[i].r,raices_polb[i].i);
}

getch();
*/

    for(i=0;i<=n;i++)
    {
        K=n-i;
        AR2[i] = AR[K];
        AI2[i] = AI[K];
    }

    for(i=0;i<=n;i++)
    {
        AR[i] = AR2[i];
        AI[i] = AI2[i];
    }

    /*
clrscr();

for(i=0;i<=n;i++)
printf("# %d %e %e   %e %e \n ",i,AR[i],AI[i],raices_polb[i].r,raices_polb[i].i);

getch();
*/
}

// Funcion para hacer la division larga, en el caso que la funcion
// sea impropia (gr_numerador >= gr_denominador)
void division_larga(double pola[],double polb[],int N1,int N2,double G[],double A[])
{
    register int ii;
    register int jj;
    int K;
    double B[10];

    memset(B,0,sizeof(B));

    // Se van ordenando de mayor a menor en el arreglo (numerador)
    for (ii=N1;ii>=0;ii--)
    {
        K=N1-ii;
        // A[N1] = nu[0];
        A[K]=pola[ii];
    }

    // Se van ordenando de mayor a menor en el arreglo (denominador)
    for (ii=N2;ii>=0;ii--)
    {
        K=N2-ii;
        // B[N2] = de[0];
        B[K]=polb[ii];
    }

    // En G se guarda el cociente
    // Algoritmo de la division larga de Vlach

    jj=0;
    ii=0;

    while (jj <= (N1-N2) )
    {
        G[jj] = A[0]/B[0];
        for(ii=0;ii<=N2;ii++)
        {
            A[ii] = A[ii]-G[jj]*B[ii];
        }
        for(ii=1;ii<=N2+1;ii++)
        {
            A[ii-1] = A[ii];
        }
        A[ii-1] = 0;
        jj++;
    }
}

int compara(complejo aux1,complejo aux2)
{
    double result_r, result_i;

    if ( (aux1.r == 0) && (aux2.r == 0) && (aux1.i == 0) && (aux2.i == 0) )
        return(1);

    if ( (aux1.r != 0) && (aux2.r != 0) && (aux1.i == 0) && (aux2.i == 0) )
    {
        result_r = aux1.r/aux2.r;
        // Las raices son iguales
        if ( (result_r < 1.000009) && (result_r > 0.99999) )
            return(1);
        // Las raices son distintas
        else
            return(0);
    }

    if ( (aux1.r == 0) && (aux2.r == 0) && (aux1.i != 0) && (aux2.i != 0) )
    {
        result_i = aux1.i/aux2.i;
        // Las raices son iguales
        if ( (result_i < 1.000009) && (result_i > 0.99999) )
            return(1);
        // Las raices son distintas
        else
            return(0);
    }

    if ( (aux1.r != 0) && (aux2.r != 0) && (aux1.i != 0) && (aux2.i != 0) )
    {
        result_r = aux1.r/aux2.r;
        result_i = aux1.i/aux2.i;
        // Las raices son iguales
        if ( (result_r < 1.000009) && (result_r > 0.99999)
             && (result_i < 1.000009) && (result_i > 0.99999) )
            return(1);
        // Las raices son distintas
        else
            return(0);
    }
    return 0;
}

char *exponencial(double b,double K)
{
    char funcion1[100];
    char *fun;

    fun=funcion1;

    strcpy(funcion1,"");

    if (b != 0.0) // Amortiguado
    {
        if (K == 0.0)
            sprintf(funcion1,"(%f %f)",K,b);
        else if (K != 0)
            sprintf(funcion1,"(%f)*e^(%f*t)",K,b);
    }
    else if (b == 0.0) // No amortiguado
        sprintf(funcion1,"(%f)",K);

    return(fun);
}

char *coseno_seno(double a,double b,double alfa,double beta)
{
    double u, d;
    char funcion2[150];
    char *fun;

    fun=funcion2;

    strcpy(funcion2,"");

    u = 2*a;
    d = 2*b;

    if (beta < 0)
        beta = -beta;  // Ajuste

    if (alfa != 0.0)      // Amortiguamento por la exponencial
    {
        if ( (a != 0.0) && (b != 0.0) )
            sprintf(funcion2,"(%f)*exp(%f*t)*cos(%f*t)+(%f)*exp(%f*t)*sin(%f*t)",u,alfa,beta,d,alfa,beta);
        else if ( (a != 0.0) && (b == 0.0) )
            sprintf(funcion2,"(%f)*exp(%f*t)*cos(%f*t)",u,alfa,beta);
        else if ( (a == 0.0) && (b != 0.0) )
            sprintf(funcion2,"(%f)*exp(%f*t)*sin(%f*t)",d,alfa,beta);
    }
    else if (alfa == 0.0) // No existe amortiguamiento por la exponencial
    {
        if ( (a != 0.0) && (b != 0.0) )
            sprintf(funcion2,"(%f)*cos(%f*t)+(%f)*sin(%f*t)",u,beta,d,beta);
        else if ( (a != 0.0) && (b == 0.0) )
            sprintf(funcion2,"(%f)*cos(%f*t)",u,beta);
        else if ( (a == 0.0) && (b != 0.0) )
            sprintf(funcion2,"(%f)*sin(%f*t)",d,beta);
    }

    return(fun);
}

// Busca las raices de un Polinomio
int polrt(double xcof[],double cof[],int m,complejo root[])
{
    register double *p, *q;
    int i, j, nsav, n, n1, n2, nroot, iter, retry;
    int final;
    double mag, cofj, argumento;
    complejo x0, x, xsav, dx, t, t1, u, ud;

    final = 0;
    n = m;
    if( n <= 0 )
        return(1);
    if( n > 36 )
        return(2);
    if( xcof[m] == 0.0 )
        return(4);

    n1 = n;
    n2 = n;
    nroot = 0;
    nsav = n;
    q = &xcof[0];
    p = &cof[n];
    for( j=0; j<=nsav; j++ )
        *p-- = *q++;	/*	cof[ n-j ] = xcof[j];*/

nxtrut:
    x0.r = 0.00500101;
    x0.i = 0.01000101;
    retry = 0;

tryagn:
    retry += 1;
    x.r = x0.r;

    x0.r = -10.0 * x0.i;
    x0.i = -10.0 * x.r;

    x.r = x0.r;
    x.i = x0.i;

finitr:
    iter = 0;

    while( iter < 500 )
    {
        u.r = cof[n];
        if( u.r == 0.0 )
        {		/* this root is zero */
            x.r = 0;
            n1 -= 1;
            n2 -= 1;
            goto zerrut;
        }
        u.i = 0;
        ud.r = 0;
        ud.i = 0;
        t.r = 1.0;
        t.i = 0;
        p = &cof[n-1];
        for( i=0; i<n; i++ )
        {
            t1.r = x.r * t.r  -  x.i * t.i;
            t1.i = x.r * t.i  +  x.i * t.r;
            cofj = *p--;		/* evaluate polynomial */
            u.r += cofj * t1.r;
            u.i += cofj * t1.i;
            cofj = cofj * (i+1);	/* derivative */
            ud.r += cofj * t.r;
            ud.i -= cofj * t.i;
            t.r = t1.r;
            t.i = t1.i;
        }

        mag = ud.r * ud.r  +  ud.i * ud.i;
        if( mag == 0.0 )
        {
            if( !final )
                goto tryagn;
            x.r = xsav.r;
            x.i = xsav.i;
            goto findon;
        }
        dx.r = (u.i * ud.i  -  u.r * ud.r)/mag;
        x.r += dx.r;
        dx.i = -(u.r * ud.i  +  u.i * ud.r)/mag;
        x.i += dx.i;
        if( (fabs(dx.i) + fabs(dx.r)) < 1.0e-6 )
            goto lupdon;
        iter += 1;
    }	/* while iter < 500 */

    if( final )
        goto lupdon;
    if( retry < 5 )
        goto tryagn;
    return(3);

lupdon:
    /* Swap original and reduced polynomials */
    q = &xcof[nsav];
    p = &cof[0];
    for( j=0; j<=n2; j++ )
    {
        cofj = *q;
        *q-- = *p;
        *p++ = cofj;
    }
    i = n;
    n = n1;
    n1 = i;

    if( !final )
    {
        final = 1;
        if (x.r != 0)
            argumento = x.i/x.r;
        else argumento = 1;
        if( fabs(argumento) < 1.0e-4 )
            x.i = 0.0;
        xsav.r = x.r;
        xsav.i = x.i;
        goto finitr;	/* do final iteration on original polynomial */
    }

findon:
    final = 0;
    if (x.r != 0)
        argumento = x.i/x.r;
    else argumento = 1;
    if( fabs(argumento) >= 1.0e-5 )
    {
        cofj = x.r + x.r;
        mag = x.r * x.r  +  x.i * x.i;
        n -= 2;
    }
    else
    {		/* root is real */
zerrut:
        x.i = 0;
        cofj = x.r;
        mag = 0;
        n -= 1;
    }
    /* divide working polynomial cof(z) by z - x */
    p = &cof[1];
    *p += cofj * *(p-1);
    for( j=1; j<n; j++ )
    {
        *(p+1) += cofj * *p  -  mag * *(p-1);
        p++;
    }

setrut:
    root[nroot].r = x.r;
    root[nroot].i = x.i;
    nroot += 1;
    if( mag != 0.0 )
    {
        x.i = -x.i;
        mag = 0;
        goto setrut;	/* fill in the complex conjugate root */
    }
    if( n > 0 )
        goto nxtrut;
    return(0);
}
