/*************************************************************************
**                                                                       **
**  Mark Morley's Expression Evaluator in a handy library                **
**                                                                       **
**  Modified version by:  M.Trifon. (mtrif@katamail.com)                 **
**                                                                       **
**  last update: 2008-07-04                                              **
*************************************************************************/

#ifndef __EE2_Hdr
#define __EE2_Hdr

/* basic datum managed herein */
struct eeTYPE {
        union {   /* Type of result to work with */
                double x;
                char *p;
        } dat;
        unsigned char type;
};

typedef struct eeTYPE eeTYPE;
typedef struct eeTYPE * peeTYPE;

struct eeTYPE * eet_alloc(void);
struct eeTYPE * eet_dup(struct eeTYPE * tp);
void eet_dup_value(struct eeTYPE *dest, struct eeTYPE *src);
void eet_free(struct eeTYPE * tp);
void eet_reset(struct eeTYPE * tp);

#define eet_set_nil(tp) \
    (tp)->type=0,(tp)->dat.x=0.0

#define eet_set_num(tp,v) \
    (tp)->type=1,(tp)->dat.x=(v)

#define eet_set_str(tp,s) \
    (tp)->type=2,(tp)->dat.p=(s)

#define eet_get_num(tp) \
    ((tp)->type==1 ? (tp)->dat.x : 0.0)

#define eet_get_str(tp) \
    ((tp)->type==2 ? (tp)->dat.p : (char *)"")

#define eet_get_type(tp) ((tp)->type)
#define eet_is_num(tp) ((tp)->type==1)
#define eet_is_str(tp) ((tp)->type==2)


/* String and vector sizes */
#define eeVARLEN       10              /* Max length of variable, const and funcs names */
#define eeMAXVARS      50              /* Max user-defined variables */
#define eeTOKLEN       100             /* Max token length */
#define eeMAXCONSTS    4               /* Max constants */
#define eeMAXFUNCS     50              /* Max functions */

/* Codes returned from the evaluator */
#define eeE_OK         0        /* Successful evaluation */
#define eeE_SYNTAX    -1        /* Syntax error */
#define eeE_UNBALAN   -2        /* Unbalanced parenthesis */
#define eeE_DIVZERO   -3        /* Attempted division by zero */
#define eeE_UNKNOWN   -4        /* Reference to unknown variable */
#define eeE_MAXVARS   -5        /* Maximum variables exceeded */
#define eeE_BADFUNC   -6        /* Unrecognised function */
#define eeE_NUMARGS   -7        /* Wrong number of arguments to funtion */
#define eeE_NOARG     -8        /* Missing an argument to a funtion */
#define eeE_EMPTY     -9        /* Empty expression */
#define eeE_BADTYPE   -10       /* wrong argument type for a function */
#define eeE_EDOM      -11       /* domain error - see clib */
#define eeE_ERANGE    -12       /* range error - see clib */
#define eeE_NONMODIF  -13       /* attempt to modify a constant */
#define eeE_ERRTYPE   -14       /* returned when type of operands is wrong */
#define eeE_LASTERR   -14       /* last valid error message */

#define eeE_NONE       0        /* Not an error ! No action was performed */
#define eeE_DONE       1        /* Not an error ! Go on with evaluation */


typedef struct {
        char name[eeVARLEN + 1];   /* Variable name */
        eeTYPE value;              /* Variable value */
} eeVARIABLE;


typedef struct {
        char name[eeVARLEN + 1];  /* Function name */
        int  args;                /* Number of arguments to expect, mx allowed == 3 */
        int  (*func)();           /* Pointer to function */
        /* function is actually called as follows:
func(eeTYPE, ...)
the integer retruend by func is the error code
*/
} eeFUNCTION;


struct eeSTATE {
        eeTYPE result;                /* evaluation result */

        int   ERROR;                  /* The error number */
        char  ERTOK[eeTOKLEN + 1];    /* The token that generated the error */
        int   ERPOS;                  /* The offset from the start of the expression */
        char* ERANC;                  /* Used to calculate ERPOS */

        eeVARIABLE Consts[eeMAXCONSTS];
        int iConsts;

        eeFUNCTION Funcs[eeMAXFUNCS];
        int iFuncs;

        eeVARIABLE  Vars[eeMAXVARS];   /* Array for user-defined variables */
        int iVars;

        char * expression;            /* Pointer to the user's expression */
        int pos;                      /* current position inside expression */
        char token[eeTOKLEN + 1];     /* Holds the current token */
        char type;                    /* Type of the current token */
        char oper;                    /* operator ID */
}; // struct eeSTATE

typedef struct eeSTATE * peeSTATE;

// custom math functions
int ee_deg(peeTYPE et, peeTYPE x);
int ee_rad(peeTYPE et, peeTYPE x);
int ee_if(peeTYPE et, peeTYPE cond, peeTYPE xtrue, peeTYPE xfalse);
int ee_sin(peeTYPE et, peeTYPE x);
int ee_cos(peeTYPE et, peeTYPE x);
int ee_tan(peeTYPE et, peeTYPE x);
int ee_asin(peeTYPE et, peeTYPE x);
int ee_acos(peeTYPE et, peeTYPE x);
int ee_atan(peeTYPE et, peeTYPE x);
int ee_atan2(peeTYPE et, peeTYPE y, peeTYPE x);
int ee_sinh(peeTYPE et, peeTYPE x);
int ee_cosh(peeTYPE et, peeTYPE x);
int ee_tanh(peeTYPE et, peeTYPE x);
int ee_exp(peeTYPE et, peeTYPE x);
int ee_log(peeTYPE et, peeTYPE x);
int ee_log10(peeTYPE et, peeTYPE x);
int ee_sqrt(peeTYPE et, peeTYPE x);
int ee_hypot(peeTYPE et, peeTYPE y, peeTYPE x);
int ee_floor(peeTYPE et, peeTYPE x);
int ee_ceil(peeTYPE et, peeTYPE x);
int ee_fabs(peeTYPE et, peeTYPE x);
int ee_num(peeTYPE et, peeTYPE x);
int ee_str(peeTYPE et, peeTYPE x);

// evaluator allocator & deallocator
struct eeSTATE *eeAlloc(void);
void eeReset(struct eeSTATE *EST);
void eeFree(struct eeSTATE *EST);

// variables management
void eeClearAllVars(struct eeSTATE *EST);
int eeClearVar(struct eeSTATE *EST, char* name);
int eeGetValue(struct eeSTATE *EST, char* name, eeTYPE* value);
int eeSetValue(struct eeSTATE *EST, char* name, eeTYPE* value);

// one-call-solution
int eeEvaluate(struct eeSTATE *EST, char* e, int* a);

// quick access to the guts
#define eeResult(e) ((e)->result)
#define eeErrorNo(e) ((e)->ERROR)
#define eeErrorToken(e) ((e)->ERTOK)

char * eeErrorMsg(struct eeSTATE *EST, int ierr);

#endif /* ifndef __EE2_Hdr */
