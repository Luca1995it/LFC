#include "strutture.h"
#include "y.tab.h"
#include <stdarg.h>

/* entry table */
entry table[DIM];
int size = 0;

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


bool ex(nodeType * p){
	if(!p) return false;
	
	switch (p->type) {
        case typeCon:       
            switch (p->con.tipo) {
                case(t_int):        return p->con.ivalue;
                case(t_bool):       return p->con.bvalue;
                case(t_real):       return p->con.rvalue;
            }
            break;
        case typeId:
        		switch (p->id.index->tipo) {
                case(t_int):        return p->id.index->ivalue;
                case(t_bool):       return p->id.index->bvalue;
                case(t_real):       return p->id.index->rvalue;
            }
            break;
        case typeType: 
        		/* codice per typeType */
        		break;
        case typeOpr:
            switch (p->opr.oper) {
                case WHILE:
                    while(ex(p->opr.op[0]) != 0) ex(p->opr.op[1]);
                    break;
                case IF:
                    if(p->opr.nops > 2){
                    		if(ex(p->opr.op[0]) != 0) ex(p->opr.op[1]);
                    		else ex(p->opr.op[2]);
                    }
                    else{
                        if(ex(p->opr.op[0]) != 0) ex(p->opr.op[1]);
                    }
                    break;
                case PRINT:
                		printf("%d\n", ex(p->opr.op[0]));
                    break;
                case '=':
                    switch(p->opr.op[0]->id.index->tipo){
				        case(t_int):		p->opr.op[0]->id.index->ivalue = ex(p->opr.op[1]);
				        case(t_bool):	p->opr.op[0]->id.index->bvalue = ex(p->opr.op[1]);
				        case(t_real):  	p->opr.op[0]->id.index->rvalue = ex(p->opr.op[1]);
            			}
                    break;
                case UMINUS:
                    return - ex(p->opr.op[0]);
                    break;
                case 'D':
                		/* codice per creare entry nella tabella */
                		break;
                default:
                    switch (p->opr.oper) {
                        case'+':
                            return ex(p->opr.op[0]) + ex(p->opr.op[1]);
                            break;
                        case'-':
                            return ex(p->opr.op[0]) - ex(p->opr.op[1]);
                            break;
                        case'*':
                            return ex(p->opr.op[0]) * ex(p->opr.op[1]);
                            break;
                        case'/':
                            return ex(p->opr.op[0]) / ex(p->opr.op[1]);
                            break;
                        case'<':
                            return ex(p->opr.op[0]) < ex(p->opr.op[1]);
                            break;
                        case'>':
                            return ex(p->opr.op[0]) > ex(p->opr.op[1]);
                            break;
                        case GE:
                            return ex(p->opr.op[0]) >= ex(p->opr.op[1]);
                            break;
                        case LE:
                            return ex(p->opr.op[0]) <= ex(p->opr.op[1]);
                            break;
                        case NE:
                            return ex(p->opr.op[0]) != ex(p->opr.op[1]);
                            break;
                        case EQ:
                            return ex(p->opr.op[0]) == ex(p->opr.op[1]);
                            break;
                    }
            }
    }
    return 0;
}

