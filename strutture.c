#include "strutture.h"
#include "y.tab.h"
#include <stdarg.h>

/* entry table */
entry table[DIM];
int size = 0;

entry * insertVariable(char * n){
	if(size >= DIM) return NULL;
	
	table[size].name = n;
	table[size].defined = false;
	table[size].data = NULL;
	
	size++;
	return &table[size];
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
	if (trovato)
		return &table[i];
    else
        return NULL;
}


nodeType * con(valueU d, typeData tipo){
    nodeType *p;
    /* allocate node space in memory */
    if((p=malloc(sizeof(nodeType))) == NULL){
        yyerror("out of memory");
    }
    /* copy information */
    p->type = typeCon;
    if((p->con.data = malloc(sizeof(valueU))) == NULL){
    		yyerror("out of memory");
    }
    p->con.data->type = tipo;
    switch (tipo) {
		case (t_int):  p->con.data->ivalue = d.ivalue;
			break;
		case (t_bool): p->con.data->bvalue = d.bvalue;
			break;
		case (t_real): p->con.data->rvalue = d.rvalue;
			break;
	}
    return p;
}

nodeType * id (char * nome){
    nodeType *p;
    if((p=malloc(sizeof(nodeType))) == NULL){
        yyerror("out of memory");
    }
    p->type = typeId;
    p->id.index = cerca(nome);
    if(p->id.index == NULL) {
    		p->id.index = insertVariable(nome);
    }
    return p;
}

nodeType * tipo(typeData t){
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


valueU * ex(nodeType * p){
	if(!p) return NULL;
	valueU * tmp;
	switch (p->type) {
        case typeCon:       
            return p->con.data;
        case typeId:
        		return p->id.index->data;
        case typeType: 
        		return NULL;
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
                		tmp = ex(p->opr.op[0]);
                		switch(tmp->type){
                			case (t_int):  printf("%d\n",tmp->ivalue);
							break;
						case (t_bool): printf("%d\n",tmp->bvalue);
							break;
						case (t_real): printf("%f\n",tmp->rvalue);
							break;
					}
                    break;
                case '=':
                    p->opr.op[0]->id.index->data = ex(p->opr.op[1]);
                    break;
                case UMINUS:
                    tmp = ex(p->opr.op[0]);
                    switch(tmp->type){
                			case (t_int):  tmp->ivalue = - tmp->ivalue;
							break;
						case (t_bool): tmp->bvalue = !tmp->bvalue;
							break;
						case (t_real): tmp->rvalue = - tmp->rvalue;
							break;
					}
                    break;
                case 'D':
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
    return NULL;
}

