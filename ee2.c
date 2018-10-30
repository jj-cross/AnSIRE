/*************************************************************************
**                                                                       **
**  Mark Morley's Expression Evaluator in a handy library                **
**  Modified version by:  M.Trifon. (mtrif@katamail.com)                 **
**  SEE BELOW THE ORIGINAL INFO & LICENSE TEXT FROM AUTHOR               **
**                                                                       **
**  last update: 2009-02-01                                              **
*************************************************************************/

/*************************************************************************
**                                                                       **
** EE.C         Expression Evaluator                                     **
**                                                                       **
** AUTHOR:      Mark Morley                                              **
** COPYRIGHT:   (c) 1992 by Mark Morley                                  **
** DATE:        December 1991                                            **
** HISTORY:     Jan 1992 - Made it squash all command line arguments     **
**                         into one big long string.                     **
**                       - It now can set/get VMS symbols as if they     **
**                         were variables.                               **
**                       - Changed max variable name length from 5 to 15 **
**              Jun 1992 - Updated comments and docs                     **
**                                                                       **
** You are free to incorporate this code into your own works, even if it **
** is a commercial application.  However, you may not charge anyone else **
** for the use of this code!  If you intend to distribute your code,     **
** I'd appreciate it if you left this message intact.  I'd like to       **
** receive credit wherever it is appropriate.  Thanks!                   **
**                                                                       **
** I don't promise that this code does what you think it does...         **
**                                                                       **
** Please mail any bug reports/fixes/enhancments to me at:               **
**      morley@camosun.bc.ca                                             **
** or                                                                    **
**      Mark Morley                                                      **
**      3889 Mildred Street                                              **
**      Victoria, BC  Canada                                             **
**      V8Z 7G1                                                          **
**      (604) 479-7861                                                   **
**                                                                       **
*************************************************************************/
/* #define VAX */             /* Uncomment this line if you're using VMS */

#pragma GCC diagnostic ignored "-Wunused-parameter"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


#ifdef VAX
#include <ssdef.h>
#include <descrip.h>
#endif

#include "ee2.h"

#define eeERR(e,n) { (e)->ERROR = (n); \
    (e)->ERPOS = (e)->pos; \
    strcpy((e)->ERTOK,(e)->token); return (n); }

#define eeSETERR(e,n) { (e)->ERROR = (n); \
    (e)->ERPOS = (e)->pos; \
    strcpy((e)->ERTOK,(e)->token); }

#define eeCURRCHAR(e) (((e)->expression)[(e)->pos])
#define eeNEXTCHAR(e) (((e)->expression)[(e)->pos+1])

char* eeErrMsgs[] =
{
    (char *)"Syntax error",
    (char *)"Unbalanced parenthesis",
    (char *)"Division by zero",
    (char *)"Unknown variable",
    (char *)"Maximum variables exceeded",
    (char *)"Unrecognised funtion",
    (char *)"Wrong number of arguments to funtion",
    (char *)"Missing an argument",
    (char *)"Empty expression",
    (char *)"Wrong argument type for function",
    (char *)"Domain error (EDOM)",
    (char *)"Range error (ERANGE)",
    (char *)"Cannot modify a CONST",
    (char *)"Wrong type for operand",
    (char *)"?"
};

/* These defines only happen if the values are not already defined!  You may
want to add more precision here, if your machine supports it. */
#ifndef M_PI
#define M_PI    3.14159265358979323846
#endif
#ifndef M_E
#define M_E     2.71828182845904523536
#endif


/* The following macros are ASCII dependant, no EBCDIC here! */
#define ee_iswhite(c)  (c == ' ' || c == '\t')
#define ee_isnumer(c)  ((c >= '0' && c <= '9') || c == '.')
#define ee_isalpha(c)  ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') \
    || c == '_')
#define ee_isdelim(c)  (c == '+' || c == '-' || c == '*' || c == '/' \
    || c == '%' || c == '^' || c == '(' || c == ')' \
    || c == ',' || c == '=' \
    || c == '!' || c == '<' || c == '>')
#define ee_isquote(c)  (c == '"' || c == '\'')


/* token types */
#define eeVAR          1
#define eeDEL          2
#define eeNUM          3
#define eeSTR          4

/* operators */
#define eeOP_PLUS          1   //  +
#define eeOP_MINUS         2   //  -
#define eeOP_MUL           3   //  *
#define eeOP_DIV           4   //  /
#define eeOP_MOD           5   //  %
#define eeOP_EQ            6   //  =
#define eeOP_EXIST         7   //  ? -- NOT YET
#define eeOP_NOT           8   //  !
#define eeOP_EQEQ          9   //  ==
#define eeOP_NOTEQ        10   //  !=
#define eeOP_GT           11   //  >
#define eeOP_LT           12   //  <
#define eeOP_GTEQ         13   //  >=
#define eeOP_LTEQ         14   //  <=

#define eeFIRST_OPER  eeOP_PLUS        /* first valid operator */
#define eeLAST_OPER   eeOP_LTEQ        /* last valid operator */
#define eeOPERS_TOTAL (eeLAST_OPER-eeFIRST_OPER+1)

/* execute assignment */
int eeLevel1(struct eeSTATE *EST, eeTYPE* r);
/* relational operators */
int eeLevel2(struct eeSTATE *EST, eeTYPE* r);
/* arithmetic + and - */
int eeLevel3(struct eeSTATE *EST, eeTYPE* r);
/* multiplication, division, modulus */
int eeLevel4(struct eeSTATE *EST, eeTYPE* r);
/* ^ (i.e. power) */
int eeLevel5(struct eeSTATE *EST, eeTYPE* r);
/* unary + and - and ! */
int eeLevel6(struct eeSTATE *EST, eeTYPE* r);
/* parens, functions calls and variables */
int eeLevel7(struct eeSTATE *EST, eeTYPE* r);


/*************************************************************************
**                                                                       **
** Basics datum                                                          **
**                                                                       **
*************************************************************************/
struct eeTYPE * eet_alloc()
{
    peeTYPE pt;
    pt = (struct eeTYPE *)malloc(sizeof(struct eeTYPE));
    if (pt!=NULL) eet_set_nil(pt);
    return pt;
}

struct eeTYPE * eet_dup(struct eeTYPE * tp)
{
    peeTYPE pdup;
    char *ss;
    pdup = eet_alloc();
    if (tp!=NULL && pdup!=NULL) {
        if (eet_is_str(tp)) {
            ss = strdup(eet_get_str(tp));
            eet_set_str( pdup, ss );
        }
        else if (eet_is_num(tp))
            eet_set_num( pdup, eet_get_num(tp) );
    }
    return pdup;
}

void eet_dup_value(struct eeTYPE *dest, struct eeTYPE *src)
{
    char *ss;
    if (eet_is_str(dest)) free(dest->dat.p);
    eet_set_nil(dest);
    if (src!=NULL) {
        if (eet_is_str(src)) {
            ss = strdup(eet_get_str(src));
            eet_set_str( dest, ss );
        }
        else if (eet_is_num(src))
            eet_set_num( dest, eet_get_num(src) );
    }
}

void eet_force_string(struct eeTYPE * tp)
{
    static char ss[60];
    char *sss;

    if (eet_is_str(tp))
        return;
    else if (eet_is_num(tp)) {
        sprintf(ss, "%-lg", eet_get_num(tp));
        sss = strdup(ss);
        eet_set_str( tp, sss );
    }
    else {
        sss = strdup("\0");
        eet_set_str( tp, sss );
    }
}

void eet_force_number(struct eeTYPE * tp)
{
    double xx;

    if (eet_is_num(tp))
        return;
    else if (eet_is_str(tp)) {
        xx = atof( eet_get_str(tp) );
        free(tp->dat.p);
        eet_set_num( tp, xx );
    }
    else {
        eet_set_num( tp, 0.0 );
    }
}


void eet_free(struct eeTYPE * tp)
{
    eet_reset(tp);
    free(tp);
}

void eet_reset(struct eeTYPE * tp)
{
    if (tp!=NULL) {
        if (eet_is_str(tp)) free(tp->dat.p);
        eet_set_nil(tp);
    }
}

/*************************************************************************
**                                                                       **
** Some custom math functions...   Note that they must be prototyped     **
** above (if your compiler requires it)                                  **
**                                                                       **
** deg( x )                  Converts x radians to degrees.              **
** rad( x )                  Converts x degrees to radians.              **
** if(cond, xtrue, xfalse)   returns xtrue if cond!=0, xfalse otherwise  **
**                                                                       **
*************************************************************************/
int ee_deg(peeTYPE et, eeTYPE* x)
{
    double xx;
    if (!eet_is_num(x)) return eeE_BADTYPE;
    xx = 180.0 * eet_get_num(x);
    xx /= M_PI;
    eet_reset(et);
    eet_set_num(et,xx);
    return eeE_OK;
}

int ee_rad(peeTYPE et, eeTYPE* x)
{
    double xx;
    if (!eet_is_num(x)) return eeE_BADTYPE;
    xx = M_PI * eet_get_num(x);
    xx /= 180.0;
    eet_reset(et);
    eet_set_num(et,xx);
    return eeE_OK;
}

int ee_if(peeTYPE et, eeTYPE* cond, eeTYPE* xtrue, eeTYPE* xfalse)
{
    char *ss;
    if (!eet_is_num(cond)) return eeE_BADTYPE;
    eet_reset(et);
    if (eet_get_num(cond)) {
        if (eet_is_num(xtrue)) eet_set_num(et,eet_get_num(xtrue));
        else if (eet_is_str(xtrue)) {
            ss = eet_get_str(xtrue);
            ss = strdup(ss);
            eet_reset(et);
            eet_set_str(et, ss);
        }
    }
    else {
        if (eet_is_num(xfalse)) eet_set_num(et,eet_get_num(xfalse));
        else if (eet_is_str(xfalse)) {
            ss = eet_get_str(xfalse);
            ss = strdup(ss);
            eet_reset(et);
            eet_set_str(et, ss);
        }
    }
    return eeE_OK;
}

int ee_sin(peeTYPE et, peeTYPE x)
{
    double xx;
    if (!eet_is_num(x)) return eeE_BADTYPE;
    xx = sin(eet_get_num(x));
    eet_reset(et);
    eet_set_num(et,xx);
    return eeE_OK;
}

int ee_cos(peeTYPE et, peeTYPE x)
{
    double xx;
    if (!eet_is_num(x)) return eeE_BADTYPE;
    xx = cos(eet_get_num(x));
    eet_reset(et);
    eet_set_num(et,xx);
    return eeE_OK;
}

int ee_tan(peeTYPE et, peeTYPE x)
{
    double xx;
    if (!eet_is_num(x)) return eeE_BADTYPE;
    xx = tan(eet_get_num(x));
    eet_reset(et);
    eet_set_num(et,xx);
    return eeE_OK;
}

int ee_asin(peeTYPE et, peeTYPE x)
{
    double xx;
    if (!eet_is_num(x)) return eeE_BADTYPE;
    if ( fabs(eet_get_num(x)) > 1.0) return eeE_EDOM;
    xx = asin(eet_get_num(x));
    eet_reset(et);
    eet_set_num(et,xx);
    return eeE_OK;
}

int ee_acos(peeTYPE et, peeTYPE x)
{
    double xx;
    if (!eet_is_num(x)) return eeE_BADTYPE;
    if ( fabs(eet_get_num(x)) > 1.0) return eeE_EDOM;
    xx = acos(eet_get_num(x));
    eet_reset(et);
    eet_set_num(et,xx);
    return eeE_OK;
}

int ee_atan(peeTYPE et, peeTYPE x)
{
    double xx;
    if (!eet_is_num(x)) return eeE_BADTYPE;
    xx = atan(eet_get_num(x));
    eet_reset(et);
    eet_set_num(et,xx);
    return eeE_OK;
}

int ee_atan2(peeTYPE et, peeTYPE y, peeTYPE x)
{
    double xx;
    if (!eet_is_num(x)) return eeE_BADTYPE;
    if (!eet_is_num(y)) return eeE_BADTYPE;
    if ( (eet_get_num(y) == 0.0) && (eet_get_num(x) == 0.0) ) return eeE_EDOM;
    xx = atan2(eet_get_num(y), eet_get_num(x));
    eet_reset(et);
    eet_set_num(et,xx);
    return eeE_OK;
}

int ee_sinh(peeTYPE et, peeTYPE x)
{
    double xx;
    if (!eet_is_num(x)) return eeE_BADTYPE;
    xx = sinh(eet_get_num(x));
    eet_reset(et);
    eet_set_num(et,xx);
    return eeE_OK;
}

int ee_cosh(peeTYPE et, peeTYPE x)
{
    double xx;
    if (!eet_is_num(x)) return eeE_BADTYPE;
    xx = cosh(eet_get_num(x));
    eet_reset(et);
    eet_set_num(et,xx);
    return eeE_OK;
}

int ee_tanh(peeTYPE et, peeTYPE x)
{
    double xx;
    if (!eet_is_num(x)) return eeE_BADTYPE;
    xx = tanh(eet_get_num(x));
    eet_reset(et);
    eet_set_num(et,xx);
    return eeE_OK;
}

int ee_exp(peeTYPE et, peeTYPE x)
{
    double xx;
    if (!eet_is_num(x)) return eeE_BADTYPE;
    xx = exp(eet_get_num(x));
    eet_reset(et);
    eet_set_num(et,xx);
    return eeE_OK;
}

int ee_log(peeTYPE et, peeTYPE x)
{
    double xx;
    if (!eet_is_num(x)) return eeE_BADTYPE;
    if (eet_get_num(x) < 0.0)return eeE_EDOM;
    if (eet_get_num(x) == 0.0)return eeE_ERANGE;
    xx = log(eet_get_num(x));
    eet_reset(et);
    eet_set_num(et,xx);
    return eeE_OK;
}

int ee_log10(peeTYPE et, peeTYPE x)
{
    double xx;
    if (!eet_is_num(x)) return eeE_BADTYPE;
    if (eet_get_num(x) < 0.0)return eeE_EDOM;
    if (eet_get_num(x) == 0.0)return eeE_ERANGE;
    xx = log10(eet_get_num(x));
    eet_reset(et);
    eet_set_num(et,xx);
    return eeE_OK;
}

int ee_sqrt(peeTYPE et, peeTYPE x)
{
    double xx;
    if (!eet_is_num(x)) return eeE_BADTYPE;
    if (eet_get_num(x) < 0.0)return eeE_EDOM;
    xx = sqrt(eet_get_num(x));
    eet_reset(et);
    eet_set_num(et,xx);
    return eeE_OK;
}

int ee_floor(peeTYPE et, peeTYPE x)
{
    double xx;
    if (!eet_is_num(x)) return eeE_BADTYPE;
    xx = floor(eet_get_num(x));
    eet_reset(et);
    eet_set_num(et,xx);
    return eeE_OK;
}

int ee_ceil(peeTYPE et, peeTYPE x)
{
    double xx;
    if (!eet_is_num(x)) return eeE_BADTYPE;
    xx = ceil(eet_get_num(x));
    eet_reset(et);
    eet_set_num(et,xx);
    return eeE_OK;
}

int ee_fabs(peeTYPE et, peeTYPE x)
{
    double xx;
    if (!eet_is_num(x)) return eeE_BADTYPE;
    xx = fabs(eet_get_num(x));
    eet_reset(et);
    eet_set_num(et,xx);
    return eeE_OK;
}

int ee_hypot(peeTYPE et, peeTYPE y, peeTYPE x)
{
    double xx;
    if (!eet_is_num(x)) return eeE_BADTYPE;
    if (!eet_is_num(y)) return eeE_BADTYPE;
    xx = hypot(eet_get_num(y), eet_get_num(x));
    eet_reset(et);
    eet_set_num(et,xx);
    return eeE_OK;
}

int ee_num(peeTYPE et, peeTYPE x)
{
    double xx=0.0;
    if (eet_is_num(x)) xx = eet_get_num(x);
    else if (eet_is_str(x)) xx = atof(eet_get_str(x));
    eet_reset(et);
    eet_set_num(et,xx);
    return eeE_OK;
}

int ee_str(peeTYPE et, peeTYPE x)
{
    static char sbuf[20];
    char *ss;
    eet_reset(et);
    if (eet_is_num(x)) {
        sprintf(sbuf, "%.8lg", eet_get_num(x));
        ss = strdup(sbuf);
        eet_set_str(et, ss);
    }
    else if (eet_is_str(x)) {
        ss = strdup(eet_get_str(x));
        eet_set_str(et, ss);
    }
    else
        eet_set_nil(et);
    return eeE_OK;
}

/*************************************************************************
**                                                                       **
** eeAlloc()                                                             **
**                                                                       **
** This routine returns a new evaluator                                  **
************************************************************************/
struct eeSTATE *eeAlloc()
{
    struct eeSTATE *eret;
    int ii;

    eret = (struct eeSTATE *)malloc( sizeof(struct eeSTATE) );
    if (eret == NULL) return NULL;

    eret->ERROR = eeE_OK;
    eret->ERTOK[0]=0;
    eret->ERPOS=0;
    eret->ERANC=NULL;

    /*
Add any "constants" here...  These are "read-only" values that are
provided as a convienence to the user.  Their values can not be
permanently changed.  The first field is the variable name, the second
is its value.
*/
    ii=0;
    eet_set_num(&(eret->Consts[ii].value), M_PI);
    strcpy(eret->Consts[ii++].name, "pi");

    eet_set_num(&(eret->Consts[ii].value), M_E);
    strcpy(eret->Consts[ii++].name, "e");

    eret->iConsts = ii;
    for (;ii<eeMAXCONSTS;ii++) {
        eret->Consts[ii].name[0] = 0;
        eet_set_nil(&(eret->Consts[ii].value));
    }

    /*
Add any math functions that you wish to recognise here...  The first
field is the name of the function as it would appear in an expression.
The second field tells how many arguments to expect.  The third is
a pointer to the actual function to use.
*/
    ii=0;
    strcpy(eret->Funcs[ii].name, "sin");
    eret->Funcs[ii].args = 1;
    eret->Funcs[ii++].func = ee_sin;

    strcpy(eret->Funcs[ii].name, "cos");
    eret->Funcs[ii].args = 1;
    eret->Funcs[ii++].func = ee_cos;

    strcpy(eret->Funcs[ii].name, "tan");
    eret->Funcs[ii].args = 1;
    eret->Funcs[ii++].func = ee_tan;

    strcpy(eret->Funcs[ii].name, "asin");
    eret->Funcs[ii].args = 1;
    eret->Funcs[ii++].func = ee_asin;

    strcpy(eret->Funcs[ii].name, "acos");
    eret->Funcs[ii].args = 1;
    eret->Funcs[ii++].func = ee_acos;

    strcpy(eret->Funcs[ii].name, "atan");
    eret->Funcs[ii].args = 1;
    eret->Funcs[ii++].func = ee_atan;

    strcpy(eret->Funcs[ii].name, "atan2");
    eret->Funcs[ii].args = 2;
    eret->Funcs[ii++].func = ee_atan2;

    strcpy(eret->Funcs[ii].name, "sinh");
    eret->Funcs[ii].args = 1;
    eret->Funcs[ii++].func = ee_sinh;

    strcpy(eret->Funcs[ii].name, "cosh");
    eret->Funcs[ii].args = 1;
    eret->Funcs[ii++].func = ee_cosh;

    strcpy(eret->Funcs[ii].name, "tanh");
    eret->Funcs[ii].args = 1;
    eret->Funcs[ii++].func = ee_tanh;

    strcpy(eret->Funcs[ii].name, "exp");
    eret->Funcs[ii].args = 1;
    eret->Funcs[ii++].func = ee_exp;

    strcpy(eret->Funcs[ii].name, "log");
    eret->Funcs[ii].args = 1;
    eret->Funcs[ii++].func = ee_log;

    strcpy(eret->Funcs[ii].name, "log10");
    eret->Funcs[ii].args = 1;
    eret->Funcs[ii++].func = ee_log10;

    strcpy(eret->Funcs[ii].name, "sqrt");
    eret->Funcs[ii].args = 1;
    eret->Funcs[ii++].func = ee_sqrt;

    strcpy(eret->Funcs[ii].name, "floor");
    eret->Funcs[ii].args = 1;
    eret->Funcs[ii++].func = ee_floor;

    strcpy(eret->Funcs[ii].name, "ceil");
    eret->Funcs[ii].args = 1;
    eret->Funcs[ii++].func = ee_ceil;

    strcpy(eret->Funcs[ii].name, "abs");
    eret->Funcs[ii].args = 1;
    eret->Funcs[ii++].func = ee_fabs;

    strcpy(eret->Funcs[ii].name, "fabs");
    eret->Funcs[ii].args = 1;
    eret->Funcs[ii++].func = ee_fabs;

    strcpy(eret->Funcs[ii].name, "hypot");
    eret->Funcs[ii].args = 2;
    eret->Funcs[ii++].func = ee_hypot;

    strcpy(eret->Funcs[ii].name, "deg");
    eret->Funcs[ii].args = 1;
    eret->Funcs[ii++].func = ee_deg;

    strcpy(eret->Funcs[ii].name, "rad");
    eret->Funcs[ii].args = 1;
    eret->Funcs[ii++].func = ee_rad;

    strcpy(eret->Funcs[ii].name, "if");
    eret->Funcs[ii].args = 3;
    eret->Funcs[ii++].func = ee_if;

    strcpy(eret->Funcs[ii].name, "num");
    eret->Funcs[ii].args = 1;
    eret->Funcs[ii++].func = ee_num;

    strcpy(eret->Funcs[ii].name, "str");
    eret->Funcs[ii].args = 1;
    eret->Funcs[ii++].func = ee_str;

    eret->iFuncs = ii;
    for (;ii<eeMAXCONSTS;ii++) {
        eret->Funcs[ii].name[0]=0;
        eret->Funcs[ii].args = 0;
        eret->Funcs[ii].func = NULL;
    }

    for (ii=0; ii<eeMAXVARS; ii++) {
        eret->Vars[ii].name[0] = 0;
        eet_set_nil(&(eret->Vars[ii].value));
    }

    eret->token[0] = 0;
    eret->type = 0;
    eret->oper = 0;
    eret->expression = NULL;
    eret->pos = 0;
    eet_set_nil(&(eret->result));

    return eret;
}

/*************************************************************************
**                                                                       **
** eeReset(struct eeSTATE *EST)                                          **
**                                                                       **
** Make the evalautor as good as new                                     **
************************************************************************/
void eeReset(struct eeSTATE *EST)
{
    EST->ERROR = eeE_OK;
    EST->ERTOK[0]=0;
    EST->ERPOS=0;
    EST->ERANC=NULL;

    eeClearAllVars(EST);

    EST->token[0] = 0;
    EST->type = 0;
    EST->oper = 0;

    free(EST->expression);
    EST->expression = NULL;
}

/*************************************************************************
**                                                                       **
** eeFree(struct eeSTATE *EST)                                           **
**                                                                       **
** Cleanup the mess                                                      **
************************************************************************/
void eeFree(struct eeSTATE *EST)
{
    int ii;

    eet_reset( &(EST->result) );
    free(EST->expression);
    for (ii=0; ii<EST->iConsts; ii++)
        eet_reset(&(EST->Consts[ii].value));
    for (ii=0; ii<eeMAXVARS; ii++)
        if (EST->Vars[ii].name[0]) eet_reset(&(EST->Vars[ii].value));
    free(EST);
}

/*************************************************************************
**                                                                       **
** GetSymbol( char* s, eeTYPE* v )                                       **
**                                                                       **
** This routine obtains a value from the program's environment.          **
** This works for DOS and VMS (and other OS's???)
**                                                                       **
************************************************************************/
int eeGetSymbol( char* s, eeTYPE* v )
{
    char* e;

    e = getenv( s );
    if ( !e )
        return( 0 );
    eet_reset(v);
    eet_set_num(v,atof( e ));
    return( 1 );
}

/*************************************************************************
**                                                                       **
** SetSymbol( char* s, char* v )                                         **
**                                                                       **
** This VMS specific routine sets (or updates) a VMS symbol to a given   **
** value                                                                 **
**                                                                       **
*************************************************************************/
#ifdef VAX
eeSetSymbol( char* s, char* v )
{
    struct dsc$descriptor_s sym;
    struct dsc$descriptor_s val;
    long                    typ = 1;

    sym.dsc$w_length = strlen( s );
    sym.dsc$a_pointer = s;
    sym.dsc$b_class = DSC$K_CLASS_S;
    sym.dsc$b_dtype = DSC$K_DTYPE_T;
    val.dsc$w_length = strlen( v );
    val.dsc$a_pointer = v;
    val.dsc$b_class = DSC$K_CLASS_S;
    val.dsc$b_dtype = DSC$K_DTYPE_T;
    return( LIB$SET_SYMBOL( &sym, &val, &typ ) );
}
#endif

/*************************************************************************
**                                                                       **
** ee_strlwr( char* s )   Internal use only                              **
**                                                                       **
** This routine converts a string to lowercase.  I know many compilers   **
** offer their own routine, but my VMS compiler didn't so...             **
** Again, this one is ASCII specific!                                    **
**                                                                       **
*************************************************************************/
static void ee_strlwr( char* s )
{
    while( *s )
    {
        if( *s >= 'A' && *s <= 'Z' )
            *s += (char)32;
        s++;
    }
}

/*************************************************************************
**                                                                       **
** ee_strcat(char *s1, char *s2, char *s3)    Internal use only          **
**                                                                       **
** Returns the concatenation of the non-NULL arguments                   **
**                                                                       **
*************************************************************************/
//#pragma warn -8060
char * ee_strcat( char* s1, char *s2, char *s3)
{
    char *sret, *ss;
    int ii=1;
    if (s1 != NULL) ii += strlen(s1);
    if (s2 != NULL) ii += strlen(s2);
    if (s3 != NULL) ii += strlen(s3);
    sret = ss = (char *)malloc(ii * sizeof(char));
    if (s1 !=NULL) { while (*s1) *ss++ = *s1++;  }
    if (s2 !=NULL) { while (*s2) *ss++ = *s2++;  }
    if (s3 !=NULL) { while (*s3) *ss++ = *s3++;  }
    *ss = '\0';
    return sret;
}
//#pragma warn .8060



/*************************************************************************
**                                                                       **
** eeClearAllVars(struct eeSTATE *EST)                                   **
**                                                                       **
** Erases all user-defined variables from memory. Note that constants    **
** can not be erased or modified in any way by the user.                 **
**                                                                       **
** Returns nothing.                                                      **
**                                                                       **
*************************************************************************/
void eeClearAllVars(struct eeSTATE *EST)
{
    int ii;

    for ( ii = 0; ii < eeMAXVARS; ii++ ) {
        if ( *(EST->Vars[ii].name) )
            eet_reset( &(EST->Vars[ii].value) );
        *(EST->Vars[ii].name) = 0;
    }
    EST->iVars = 0;
}

/*************************************************************************
**                                                                       **
** eeClearVar(struct eeSTATE *EST, char* name)                           **
**                                                                       **
** Erases the user-defined variable that is called NAME from memory.     **
** Note that constants are not affected.                                 **
**                                                                       **
** Returns 1 if the variable was found and erased, or 0 if it didn't     **
** exist.                                                                **
**                                                                       **
*************************************************************************/
int eeClearVar(struct eeSTATE *EST, char* name)
{
    int ii;

    for( ii = 0; ii < eeMAXVARS; ii++ )
        if( *(EST->Vars[ii].name) && ! strcmp( name, EST->Vars[ii].name ) ) {
            *(EST->Vars[ii].name) = 0;
            eet_reset( &(EST->Vars[ii].value) );
            return( eeE_DONE );
        }
    return( eeE_NONE );
}

/*************************************************************************
**                                                                       **
** eeGetValue(struct eeSTATE *EST, char* name, eeTYPE* value)            **
**                                                                       **
** Looks up the specified variable (or constant) known as NAME and       **
** returns its contents in VALUE.                                        **
**                                                                       **
** First the user-defined variables are searched, then the constants are **
** searched.                                                             **
**                                                                       **
** Returns 1 if the value was found, or 0 if it wasn't.                  **
**                                                                       **
*************************************************************************/
int eeGetValue(struct eeSTATE *EST, char* name, eeTYPE* value)
{
    int ii;

    /* First check for an environment variable reference... */
    if( *name == '_' )
        return( eeGetSymbol( name + 1, value ) );

    /* Now check the user-defined variables. */
    for( ii = 0; ii < eeMAXVARS; ii++ )
        if( *(EST->Vars[ii].name) && ! strcmp( name, EST->Vars[ii].name ) ) {
            // NOOOO !!!         *value = EST->Vars[ii].value;
            eet_dup_value( value, &(EST->Vars[ii].value) );
            return( eeE_DONE );
        }

    /* Now check the programmer-defined constants. */
    for( ii = 0; *(EST->Consts[ii].name); ii++ )
        if( *(EST->Consts[ii].name) && ! strcmp( name, EST->Consts[ii].name ) ) {
            // NOOOO !!!         *value = EST->Consts[ii].value;
            eet_dup_value( value, &(EST->Consts[ii].value) );
            return( eeE_DONE );
        }
    return( eeE_NONE );
}

/*************************************************************************
**                                                                       **
** eeSetValue(struct eeSTATE *EST, char* name, eeTYPE* value)            **
**                                                                       **
** First, it erases any user-defined variable that is called NAME.  Then **
** it creates a new variable called NAME and gives it the value VALUE.   **
**                                                                       **
** Returns 1 if the value was added, or 0 if there was no more room.     **
**                                                                       **
*************************************************************************/
int eeSetValue(struct eeSTATE *EST, char* name, eeTYPE* value)
{
    int  ii;
#ifdef VAX
    char b[30];

    if( *name == '_' )
    {
        sprintf( b, "%g", *value );
        if( SetSymbol( name + 1, b ) != SS$_NORMAL )
            return( eeE_NONE );
        return( eeE_DONE );
    }
#endif
    eeClearVar(EST, name);
    for( ii = 0; ii < eeMAXVARS; ii++ )
        if( ! *(EST->Vars[ii].name) ) {
            strcpy( EST->Vars[ii].name, name );
            EST->Vars[ii].name[eeVARLEN] = 0;
            eet_dup_value( &(EST->Vars[ii].value), value);
            return( eeE_DONE );
        }
    return( eeE_NONE );
}

/*************************************************************************
**                                                                       **
** eeIsConstant(struct eeSTATE *EST, char* name)                         **
**                                                                       **
** Check if the variable is modifiable                                   **
**                                                                       **
** Returns 1 if the value is a constant, or 0 if it may be modified.     **
**                                                                       **
*************************************************************************/
int eeIsConstant(struct eeSTATE *EST, char* name)
{
    int  ii;
    for( ii = 0; ii < eeMAXCONSTS; ii++ )
        if ( *(EST->Consts[ii].name) &&
             ! strcmp( name, EST->Consts[ii].name ) ) { return 1; }
    return 0;
}

/*************************************************************************
**                                                                       **
** eeParse(struct eeSTATE *EST)     Internal use only                    **
**                                                                       **
** This function is used to grab the next token from the expression that **
** is being evaluated.                                                   **
**                                                                       **
*************************************************************************/
int eeParse(struct eeSTATE *EST)
{
    char* t;
    char ch;

    EST->type = 0;
    EST->oper = 0;
    t = EST->token;
    // skip spaces
    while( ee_iswhite(eeCURRCHAR(EST)) ) (EST->pos)++;

    // what's next ?
    if ( ee_isdelim(eeCURRCHAR(EST)) ) {
        EST->type = eeDEL;
        *t++ = eeCURRCHAR(EST);
        // flag the operators
        switch ( eeCURRCHAR(EST) ) {
            case '=' :
                if (eeNEXTCHAR(EST) == '=') { (EST->pos)++; *t++=eeCURRCHAR(EST); EST->oper=eeOP_EQEQ; }
                else EST->oper = eeOP_EQ;
                break;
            case '+' :  EST->oper = eeOP_PLUS; break;
            case '-' :  EST->oper = eeOP_MINUS; break;
            case '*' :  EST->oper = eeOP_MUL; break;
            case '/' :  EST->oper = eeOP_DIV; break;
            case '%' :  EST->oper = eeOP_MOD; break;
            case '!' :
                if (eeNEXTCHAR(EST) == '=') { (EST->pos)++; *t++=eeCURRCHAR(EST); EST->oper=eeOP_NOTEQ; }
                else EST->oper = eeOP_NOT;
                break;
            case '<' :
                if (eeNEXTCHAR(EST) == '=') { (EST->pos)++; *t++=eeCURRCHAR(EST); EST->oper=eeOP_LTEQ; }
                else EST->oper = eeOP_LT;
                break;
            case '>' :
                if (eeNEXTCHAR(EST) == '=') { (EST->pos)++; *t++=eeCURRCHAR(EST); EST->oper=eeOP_GTEQ; }
                else EST->oper = eeOP_GT;
                break;
        } // switch
        (EST->pos)++;
    } // if  delim
    else if( ee_isquote(eeCURRCHAR(EST)) ) {
        ch = eeCURRCHAR(EST);
        (EST->pos)++;
        EST->type = eeSTR;
        while (eeCURRCHAR(EST)) {
            if ( eeCURRCHAR(EST) == ch) { (EST->pos)++; break; }
            else if ( t - EST->token < eeTOKLEN ) { *t++ = eeCURRCHAR(EST); }
            (EST->pos)++;
        } // while
    } // else if quote
    else if ( ee_isnumer(eeCURRCHAR(EST)) ) {
        EST->type = eeNUM;
        while ( ee_isnumer(eeCURRCHAR(EST)) ) {
            *t++ = eeCURRCHAR(EST);
            (EST->pos)++;
        } // while
    } // else if  num
    else if ( ee_isalpha(eeCURRCHAR(EST)) ) {
        EST->type = eeVAR;
        while ( ee_isalpha(eeCURRCHAR(EST)) ) {
            *t++ = eeCURRCHAR(EST);
            (EST->pos)++;
        } // while
        EST->token[eeVARLEN] = 0;
    } // else if any
    else if ( eeCURRCHAR(EST) ) {
        *t++ = eeCURRCHAR(EST);
        (EST->pos)++;
        *t = 0;
        eeERR(EST, eeE_SYNTAX); // returned form inside !!!!
    }
    *t = 0;
    while ( ee_iswhite(eeCURRCHAR(EST)) )  (EST->pos)++;

    return eeE_OK;
}

/*************************************************************************
**                                                                       **
** eeLevel1(struct eeSTATE *EST, eeTYPE* r)     Internal use only        **
**                                                                       **
** This function handles any variable assignment operations.             **
** It returns a value of 1 if it is a top-level assignment operation,    **
** otherwise it returns 0                                                **
**                                                                       **
*************************************************************************/
int eeLevel1(struct eeSTATE *EST, eeTYPE* r)
{
    int iret;
    char tk[eeVARLEN + 1];

    if ( EST->type == eeVAR )
        if ( (eeCURRCHAR(EST) == '=') && (eeNEXTCHAR(EST) != '=') ) {
            if ( eeIsConstant(EST, EST->token) )
                eeERR(EST, eeE_NONMODIF ); // returned form inside !!!
            strcpy( tk, EST->token );
            if ( (iret=eeParse(EST)) < eeE_OK ) return iret;
            if ( (iret=eeParse(EST)) < eeE_OK ) return iret;
            if ( ! *(EST->token) ) {
                eeClearVar(EST, tk);
                return(eeE_DONE);
            }
            if ( (iret=eeLevel2(EST, r)) < eeE_OK ) return iret;
            if ( ! eeSetValue(EST, tk, r) )
                eeERR(EST, eeE_MAXVARS ); // returned form inside !!!
            return( eeE_DONE );
        } // if  eeOP_EQ

    if ( (iret=eeLevel2(EST, r)) < eeE_OK ) return iret;

    return( eeE_NONE );
}

/*************************************************************************
**                                                                       **
** eeLevel2(struct eeSTATE *EST, eeTYPE* r)     Internal use only        **
**                                                                       **
** This function handles relational operations.                          **
** NOTA BENE : all relational operators have same precedence             **
**                                                                       **
*************************************************************************/
int eeLevel2(struct eeSTATE *EST, eeTYPE* r)
{
    int iret;
    eeTYPE tt;
    //char oo;

    eet_set_nil( &tt );
    if ( (iret=eeLevel3(EST, r)) < eeE_OK ) return iret;
    //oo=EST->oper;

    switch (EST->oper) {
        case eeOP_EQEQ:
            if ( (iret=eeParse(EST)) < eeE_OK ) return iret;
            if ( (iret=eeLevel3(EST, &tt)) < eeE_OK ) { eet_reset(&tt); return iret; }
            if (eet_get_type(r) != eet_get_type(&tt)) iret = 0;
            else if ( eet_is_num(r) )
                iret = ( eet_get_num(r) == eet_get_num(&tt) );
            else if (  eet_is_str(r) )
                iret = ( 0 == stricmp(eet_get_str(r), eet_get_str(&tt)) );
            else
                iret=1;
            break;
        case eeOP_NOTEQ:
            if ( (iret=eeParse(EST)) < eeE_OK ) return iret;
            if ( (iret=eeLevel3(EST, &tt)) < eeE_OK ) { eet_reset(&tt); return iret; }
            if (eet_get_type(r) != eet_get_type(&tt)) iret = 1;
            else if ( eet_is_num(r) )
                iret = ( eet_get_num(r) != eet_get_num(&tt) );
            else if (  eet_is_str(r) )
                iret = ( 0 != stricmp(eet_get_str(r), eet_get_str(&tt)) );
            else
                iret=0;
            break;
        case eeOP_LT:
            if ( (iret=eeParse(EST)) < eeE_OK ) return iret;
            if ( (iret=eeLevel3(EST, &tt)) < eeE_OK ) { eet_reset(&tt); return iret; }
            if (eet_get_type(r) != eet_get_type(&tt)) { eet_reset(&tt); return eeE_ERRTYPE; }
            else if ( eet_is_num(r) )
                iret = ( eet_get_num(r) < eet_get_num(&tt) );
            else if (  eet_is_str(r) )
                iret = ( 0 > stricmp(eet_get_str(r), eet_get_str(&tt)) );
            else
                iret=0;
            break;
        case eeOP_GT:
            if ( (iret=eeParse(EST)) < eeE_OK ) return iret;
            if ( (iret=eeLevel3(EST, &tt)) < eeE_OK ) { eet_reset(&tt); return iret; }
            if (eet_get_type(r) != eet_get_type(&tt)) { eet_reset(&tt); return eeE_ERRTYPE; }
            else if ( eet_is_num(r) )
                iret = ( eet_get_num(r) > eet_get_num(&tt) );
            else if (  eet_is_str(r) )
                iret = ( 0 < stricmp(eet_get_str(r), eet_get_str(&tt)) );
            else
                iret=0;
            break;
        case eeOP_LTEQ:
            if ( (iret=eeParse(EST)) < eeE_OK ) return iret;
            if ( (iret=eeLevel3(EST, &tt)) < eeE_OK ) { eet_reset(&tt); return iret; }
            if (eet_get_type(r) != eet_get_type(&tt)) { eet_reset(&tt); return eeE_ERRTYPE; }
            else if ( eet_is_num(r) )
                iret = ( eet_get_num(r) <= eet_get_num(&tt) );
            else if (  eet_is_str(r) )
                iret = ( 0 >= stricmp(eet_get_str(r), eet_get_str(&tt)) );
            else
                iret=1;
            break;
        case eeOP_GTEQ:
            if ( (iret=eeParse(EST)) < eeE_OK ) return iret;
            if ( (iret=eeLevel3(EST, &tt)) < eeE_OK ) { eet_reset(&tt); return iret; }
            if (eet_get_type(r) != eet_get_type(&tt)) { eet_reset(&tt); return eeE_ERRTYPE; }
            else if ( eet_is_num(r) )
                iret = ( eet_get_num(r) >= eet_get_num(&tt) );
            else if (  eet_is_str(r) )
                iret = ( 0 <= stricmp(eet_get_str(r), eet_get_str(&tt)) );
            else
                iret=1;
            break;
        default:
            return( eeE_DONE );
    } // switch

    eet_reset(r);
    eet_reset(&tt);
    eet_set_num(r, ((double)iret));

    return( eeE_DONE );
}

/*************************************************************************
**                                                                       **
** eeLevel3(struct eeSTATE *EST, eeTYPE* r)     Internal use only        **
**                                                                       **
** This function handles any addition and subtraction operations.        **
**   basic rule : the return value for an addition is a string if at     **
**                least one of the operands is string;                   **
**                otherwise, the operands are treated as numbers         **
**                                                                       **
*************************************************************************/
int eeLevel3(struct eeSTATE *EST, eeTYPE* r)
{
    int iret;
    eeTYPE tt;
    char oo, *ss;
    double xx;

    eet_set_nil( &tt );
    if ( (iret=eeLevel4(EST, r)) < eeE_OK ) return iret;

    while( (oo = *(EST->token)) == '+' || oo == '-' ) {
        if ( (iret=eeParse(EST)) < eeE_OK ) return iret;
        if ( (iret=eeLevel4(EST, &tt)) < eeE_OK ) { eet_reset(&tt); return iret; }
        if ( (eet_is_str(&tt)) ||
             (eet_is_str(r))  ) { /* adding a string to something : return a string */
            if ( oo == '+' ) {
                eet_force_string(r);
                eet_force_string(&tt);
                ss = ee_strcat( eet_get_str(r), eet_get_str(&tt), NULL);
                eet_reset(r);
                eet_set_str(r, ss);
            }
            else
                eet_reset(r);
        } // if  string
        else {
            if ( !eet_is_num(r) ) eet_reset(r);
            if ( oo == '+' ) {
                xx = eet_get_num(r) + eet_get_num(&tt);
                eet_set_num(r,xx);
            }
            else if ( oo == '-' ) {
                xx = eet_get_num(r) - eet_get_num(&tt);
                eet_set_num(r,xx);
            }
        } // else
        eet_reset(&tt);
    } // while

    return( eeE_DONE );
}

/*************************************************************************
**                                                                       **
** eeLevel4(struct eeSTATE *EST, eeTYPE* r)     Internal use only        **
**                                                                       **
** This function handles any multiplication, division, or modulo.        **
**                                                                       **
*************************************************************************/
int eeLevel4(struct eeSTATE *EST, eeTYPE* r)
{
    eeTYPE tt;
    double xx;
    char o;
    int iret;

    eet_set_nil( &tt );
    if ( (iret=eeLevel5(EST, r)) < eeE_OK ) return iret;

    while( (o = *(EST->token)) == '*' || o == '/' || o == '%' ) {
        if ( (iret=eeParse(EST)) < eeE_OK ) return iret;
        if ( (iret=eeLevel5(EST, &tt)) < eeE_OK ) { eet_reset(&tt); return iret; }
        if ( !eet_is_num(r) ) eet_reset(r);
        if ( o == '*' ) {
            xx = eet_get_num(r) * eet_get_num(&tt);
            eet_set_num(r,xx);
        }
        else if ( o == '/' ) {
            if( eet_get_num(&tt) == 0.0 ) eeERR(EST, eeE_DIVZERO);  // returned from inside !!!
            xx = eet_get_num(r) / eet_get_num(&tt);
            eet_set_num(r,xx);
        }
        else if( o == '%' ) {
            if( eet_get_num(&tt) == 0.0 ) eeERR(EST, eeE_DIVZERO);  // returned from inside !!!
            xx = fmod( eet_get_num(r), eet_get_num(&tt) );
            eet_set_num(r,xx);
        }
        eet_reset(&tt);
    } // while

    return( eeE_DONE );
}

/*************************************************************************
**                                                                       **
** eeLevel5(struct eeSTATE *EST, eeTYPE* r)     Internal use only        **
**                                                                       **
** This function handles any "to the power of" operations.               **
**                                                                       **
*************************************************************************/
int eeLevel5(struct eeSTATE *EST, eeTYPE* r)
{
    eeTYPE tt;
    double xx;
    int iret;

    eet_set_nil(&tt);
    if ( (iret=eeLevel6(EST, r)) < eeE_OK ) return iret;
    if ( *(EST->token) == '^' ) {
        if ( (iret=eeParse(EST)) < eeE_OK ) return iret;
        if ( (iret=eeLevel6(EST, &tt)) < eeE_OK ) { eet_reset(&tt); return iret; }
        if ( !eet_is_num(r) ) eet_reset(r);
        xx = pow( eet_get_num(r), eet_get_num(&tt) );
        eet_set_num(r,xx);
        eet_reset(&tt);
    } // if
    return( eeE_DONE );
}

/*************************************************************************
**                                                                       **
** eeLevel6(struct eeSTATE *EST, eeTYPE* r)     Internal use only        **
**                                                                       **
** This function handles any unary + or - signs.                         **
**                                                                       **
*************************************************************************/

int eeLevel6(struct eeSTATE *EST, eeTYPE* r)
{
    double xx;
    int iret;
    char oo = 0;

    if ( *(EST->token) == '+' || *(EST->token) == '-' || *(EST->token) == '!' ) {
        oo = *(EST->token);
        if ( (iret=eeParse(EST)) < eeE_OK ) return iret;
    }
    if ( (iret=eeLevel7(EST, r)) < eeE_OK ) return iret;
    if ( oo == '-' ) {
        if ( !eet_is_num(r) ) return eeE_SYNTAX;
        xx = eet_get_num(r);
        eet_set_num(r, -xx);
    } // if  minus
    else if ( oo == '+' ) {
        if ( !eet_is_num(r) ) return eeE_SYNTAX;
    } // if  plus
    else if ( oo == '!' ) {
        if ( !eet_is_num(r) ) return eeE_SYNTAX;
        xx = (double)( eet_get_num(r) == 0.0 );
        eet_set_num(r, xx);
    } // if  not
    return( eeE_DONE );
}

/*************************************************************************
**                                                                       **
** eeLevel7(struct eeSTATE *EST, eeTYPE* r)     Internal use only        **
**                                                                       **
** This function handles any literal numbers, variables, or functions.   **
**                                                                       **
*************************************************************************/
int eeLevel7(struct eeSTATE *EST, eeTYPE* r)
{
    int ii;
    int narg;
    int iret;
    eeTYPE a[3];

    // left paren : recourse
    if ( (*(EST->token) == '(') && (EST->type != eeSTR) ) {
        // execute the operation as long as comma is present
        do {
            if ( (iret=eeParse(EST)) < eeE_OK ) return iret;
            if ( *(EST->token) == ')' ) eeERR(EST, eeE_NOARG);  // returned from inside !!!
            if ( (iret=eeLevel1(EST, r)) < eeE_OK ) return iret;
        } while ( *(EST->token) == ',' );
        if ( *(EST->token) != ')' ) {
            eeERR(EST, eeE_UNBALAN); // returned from inside !!
        }
        if ( (iret=eeParse(EST)) < eeE_OK ) return iret;
        return eeE_DONE;
    } // if left paren
    // string : assign token to r and return
    if ( EST->type == eeSTR ) {
        eet_reset(r);
        eet_set_str( r, strdup(EST->token) );
        if ( (iret=eeParse(EST)) < eeE_OK ) return iret;
        return eeE_DONE;
    } // if  eeSTR
    // number : assign value to the r and return
    if ( EST->type == eeNUM ) {
        eet_reset(r);
        eet_set_num( r, atof(EST->token) );
        if ( (iret=eeParse(EST)) < eeE_OK ) return iret;
        return eeE_DONE;
    } // eeNUM
    // variable or function : process...
    if ( EST->type == eeVAR ) {
        if ( eeCURRCHAR(EST) == '(' ) {
            // search the function
            iret = eeE_BADFUNC;
            for ( ii = 0; *(EST->Funcs[ii].name); ii++ ) {
                if (!strcmp(EST->token, EST->Funcs[ii].name)) { iret = eeE_OK; break; }
            } // for  ii
            if (iret != eeE_OK) eeERR(EST, eeE_BADFUNC);  // returned from inside !!
            // get the arguments; NOTA BENE : max. 3 args by design
            eet_set_nil( &(a[0]) );
            eet_set_nil( &(a[1]) );
            eet_set_nil( &(a[2]) );
            if ( (iret=eeParse(EST)) < eeE_OK ) return iret;
            narg = 0;
            do {
                if ( (iret=eeParse(EST)) < eeE_OK ) goto get_out;
                if ( *(EST->token) == ')' ||
                     *(EST->token) == ',' ) { eeSETERR(EST, (iret=eeE_NOARG)); goto get_out; }
                eet_set_num(&(a[narg]), 0.0);
                if ( (iret=eeLevel1(EST, &a[narg])) < eeE_OK ) goto get_out;
                narg++;
            } while( narg < 4 && *(EST->token) == ',' );
            eeParse(EST);
            if ( narg != EST->Funcs[ii].args ) {
                strcpy( EST->token, EST->Funcs[ii].name );
                eeSETERR(EST, (iret=eeE_NUMARGS));
            } // if  wrong number of args
            // call the function
            iret = EST->Funcs[ii].func(r, &(a[0]), &(a[1]), &(a[2]) );
            if (iret < eeE_OK) {
                strcpy( EST->token, EST->Funcs[ii].name );
                eeSETERR(EST, iret);
            } // if  run time error
            goto get_out;
            /*
for ( ii = 0; *(EST->Funcs[ii].name); ii++ ) {
if ( ! strcmp( EST->token, EST->Funcs[ii].name ) ) {
if ( (iret=eeParse(EST)) < eeE_OK ) return iret;
n = 0;
do {
if ( (iret=eeParse(EST)) < eeE_OK ) goto get_out;
if ( *(EST->token) == ')' || *(EST->token) == ',' ) {
eeSETERR(EST, (iret=eeE_NOARG));
goto get_out;
}
eet_set_num(&(a[n]), 0.0);
if ( (iret=eeLevel1(EST, &a[n])) < eeE_OK ) goto get_out;
n++;
} while( n < 4 && *(EST->token) == ',' );
eeParse(EST);
if ( n != EST->Funcs[ii].args ) {
strcpy( EST->token, EST->Funcs[ii].name );
eeSETERR(EST, (iret=eeE_NUMARGS));
goto get_out;
}
iret = EST->Funcs[ii].func(r, &(a[0]), &(a[1]), &(a[2]) );
return iret;
} // if  found function
} // for
if ( ! *(EST->Funcs[ii].name) )
eeERR(EST, eeE_BADFUNC);  // returned from inside !!
*/
        } // if  function call
        else if ( ! eeGetValue(EST, EST->token, r ) )
            eeERR(EST, eeE_UNKNOWN);  // returned from inside !!
        if ( (iret=eeParse(EST)) < eeE_OK ) return iret;
        return eeE_DONE;
    } // eeVAR
    // error : unknown token
    eeERR(EST, eeE_SYNTAX ); // returned from inside !!

get_out:
    eet_reset( &(a[0]) );
    eet_reset( &(a[1]) );
    eet_reset( &(a[2]) );
    return ( iret<eeE_OK ? iret : eeE_DONE );
}

/*************************************************************************
**                                                                       **
** eeEvaluate(struct eeSTATE *EST, char* e, int* a)                      **
**                                                                       **
** This function is called to evaluate the expression "e"" and           **
** return the  answer in EST->result.
** If the expression was a top-level assignment, value of 1 will
** be returned in a, otherwise it will contain 0.                        **
**                                                                       **
** Returns E_OK if the expression is valid, or an error code.            **
**                                                                       **
*************************************************************************/
int eeEvaluate(struct eeSTATE *EST, char* e, int* a)
{
    int ii;

    if (EST->expression != NULL) free(EST->expression);
    EST->expression = strdup(e);
    EST->ERANC = EST->expression;
    EST->pos=0;
    ee_strlwr( EST->expression );
    eet_reset( &(EST->result) );
    eet_set_num( &(EST->result), 0.0 );
    EST->ERROR = eeE_OK;
    if ( (ii=eeParse(EST)) < eeE_OK ) return ii;
    if( ! *(EST->token) )
        eeERR(EST, eeE_EMPTY); // returned from inside !!
    while ( *(EST->token) ) {
        while ( *(EST->token) == ',' )
            if ( (ii=eeParse(EST)) < eeE_OK ) return ii;
        if ( (ii=eeLevel1(EST, &(EST->result))) < eeE_OK )
            return ( EST->ERROR = ii );
        if (a!=NULL) *a=ii;
    } // while
    return( EST->ERROR = eeE_OK );
}

/*************************************************************************
**                                                                       **
** eeErrorMsg(struct eeSTATE *EST, int ierr)                             **
**                                                                       **
** Get internal error message or NULL if none available                  **
**                                                                       **
*************************************************************************/
char * eeErrorMsg(struct eeSTATE *EST, int ierr)
{
    static char sss[4] =" ";

    if ( (ierr<0)&&(ierr>=eeE_LASTERR) ) {
        return eeErrMsgs[-ierr-1];
    }
    else
        return sss;
}
