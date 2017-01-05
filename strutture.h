#ifndef MODULO_I_H
#define MODULO_I_H

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#define bool int
#define false 0
#define true 1
#define DIM 100



/* tipi di dato */
typedef enum { t_int, t_bool, t_real } typeData;

/* tipi di nodo */
typedef enum { typeCon, typeId, typeOpr, typeType } nodeEnum;

/* valore generico tra int, double e bool */
typedef union { 
	int ivalue; 
	double rvalue; 
	bool bvalue; 
} valueU;

/* entry della tabella */
typedef struct {
	char * name;
	typeData tipo;
	union{
		int ivalue;
		double rvalue;
		bool bvalue;
	};
} entry;

/* entry table */
entry table[DIM];
int size = 0;

/* constants */
typedef struct {
	typeData tipo;
	union{
		int ivalue;
		double rvalue;
		bool bvalue;
	};
} conNodeType;

/* variables */
typedef struct {  
	entry * index;     
} idNodeType;

/* types */
typedef struct {
	typeData type;
} typeNodeType;

/* operators */
typedef struct {
    int oper;                   /* operator */
    int nops;                   /* number of operands */
    struct nodeTypeTag ** op;	/* operands */
} oprNodeType;

/* general operators */
typedef struct nodeTypeTag{
    nodeEnum type;              /* type of node */
    union {
        conNodeType con;        /* constants */
        idNodeType id;          /* identifiers */
        oprNodeType opr;        /* operators */
        typeNodeType ty;			/* type */
    };
} nodeType;



bool insertVariable(char * n, typeData t, valueU d);
entry * cerca(char * var);
nodeType * con( valueU d, typeData tipo);
nodeType * id (char * nome);
nodeType * tipo(typeData t);
nodeType * opr(int oper, int nops, ...);
void yyerror(char *);

#endif



