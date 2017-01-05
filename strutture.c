#include "strutture.h"

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


nodeType *con( valueU d, typeData tipo){
    nodeType *p;
    /* allocate node space in memory */
    if((p=malloc(sizeof(nodeType))) == NULL){
        yyerror("out of memory");
    }
    /* copy information */
    p->type = typeCon;
    p->con.tipo = tipo;
    switch (tipo) {
		case (t_int): p->con.ivalue = d.ivalue;
			break;
		case (t_bool): p->con.bvalue = d.bvalue;
			break;
		case (t_real): p->con.rvalue = d.rvalue;
			break;
	}

    return p;
}

nodeType *id (char * nome){
    nodeType *p;
    if((p=malloc(sizeof(nodeType))) == NULL){
        yyerror("out of memory");
    }
    p->type = typeId;
    p->id.index = cerca(nome);
    return p;
}

nodeType *tipo(typeData t){
	nodeType *p;
    if((p=malloc(sizeof(nodeType))) == NULL){
        yyerror("out of memory");
    }
    p->type = typeType;
    p->ty.type = t;
    return p;
}

nodeType *opr(int oper, int nops, ...){
    va_list ap; /* (ap = argument pointer) va_list is used to declare a variable
                 which, from time to time, is referring to an argument*/
    nodeType *p;
    int i;

    if ((p = malloc(sizeof(nodeType))) == NULL){
        yyerror("out of memory");
    }
    if((p->opr.op = malloc(nops*sizeof(nodeType)))== NULL){
        yyerror("out of memory");
    }
    p->type = typeOpr;
    p->opr.oper = oper;
    p->opr.nops = nops;
    va_start(ap, nops);/* initialize the sequence, makes ap point to the first
                        anonymous argument. Must call it once before reading all
                        the parameters*/
    for(i = 0; i<nops;i++){
        p->opr.op[i]=va_arg(ap,nodeType*); 
    }
    va_end(ap); /* MUST be called BEFORE THE FUNCTION TERMINATES. it provides
                 the necessary cleaning functions.*/


    return p;
}
