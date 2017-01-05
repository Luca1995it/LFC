#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#define bool int
#define false 0
#define true 1
#define DIM 100

typedef enum { t_int, t_bool, t_real } typeData;

typedef union { 
	int ivalue; 
	double rvalue; 
	bool bvalue; 
} valueU;

typedef struct {
	char * name;
	typeData tipo;
	union{
		int ivalue;
		double rvalue;
		bool bvalue;
	};
} entry;

extern entry table[];
extern int size;


bool insertVariable(char * n, typeData t, valueU d){
	if(size >= DIM) return false;
	
	table[size].name = n;
	table[size].tipo = t;
	switch (t) {
		case (t_int): table[size].ivalue = d.ivalue;
			break;
		case (t_bool): table[size].bvalue = d.bvalue;
			break;
		case (t_real): table[size].rvalue = d.rvalue;
			break;
	}
	size++;
	return true;
}

entry * cerca(char * var){
	int i = 0;
	bool trovato = false;
	while ( (i<size) && (!trovato) ){
		int ris = strcmp(table[i].name, var);

		if(ris == 0){
			trovato = true;
		} else {
			i++;
		}
	}
	entry * p = &table[i];
	return p;
}

typedef enum { typeCon, typeId, typeOpr, typeType } nodeEnum;


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
    struct nodeTypeTag **op;	/* operands */
} oprNodeType;

typedef struct nodeTypeTag {
    nodeEnum type;              /* type of node */
    union {
        conNodeType con;        /* constants */
        idNodeType id;          /* identifiers */
        oprNodeType opr;        /* operators */
        typeNodeType ty;
    };
} nodeType;



