#include "strutture.h"
#include "y.tab.h"
#include <stdarg.h>

/* entry table */
entry table[DIM];
int size = 0;

entry * insertVariable(char * n){
	if(size >= DIM) return false;
	
	table[size].name = n;	
    table[size].defined=false;
    table[size].data=NULL;

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


nodeType *con( valueU d, typeData tipo){
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
		case (t_int): p->con.data->ivalue = d.ivalue;
			break;
		case (t_bool): p->con.data->bvalue = d.bvalue;
			break;
		case (t_real): p->con.data->rvalue = d.rvalue;
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
     if(p->id.index == NULL) {
            p->id.index=insertVariable(nome);
    }
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


valueU* ex(nodeType * p){

    valueU * tmp, *tmp0, *tmp1;


	if(!p) return NULL;
	
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
                    tmp=ex(p->opr.op[0]);
                    switch(tmp->type){
                            case (t_int):  tmp->ivalue=-tmp->ivalue;
                            return tmp;
                            case (t_bool): tmp->bvalue=-tmp->bvalue;
                            return tmp;
                            case (t_real): tmp->rvalue=-tmp->rvalue;
                            return tmp;
                        }
                case 'D':
                        if(p->opr.op[0]->id.index->defined){
                            yyerror("variabile gia definita");
                        }
                        p->opr.op[0]->id.index->data->type = p->opr.op[1]->ty.type;
                        p->opr.op[0]->id.index->defined = true;
                        break;
                default:
                    switch (p->opr.oper) {
                        tmp;
                        tmp0=ex(p->opr.op[0]);
                        tmp1=ex(p->opr.op[1]);

                        if((tmp0->type==t_int)&&(tmp1->type==t_int)){
                                tmp->type=t_int;
                        } 

                        if((tmp0->type==t_real)&&(tmp1->type==t_real)){
                                tmp->type=t_real;
                        }

                        if((tmp0->type==t_bool)&&(tmp1->type==t_bool)){
                                tmp->type=t_bool;
                        }

                        if((tmp0->type==t_int)&&(tmp1->type==t_real)){
                                tmp->type=t_real;
                        }

                        if((tmp0->type==t_int)&&(tmp1->type==t_bool)){
                                yyerror("tipi non compatibili");
                        }

                        if((tmp0->type==t_real)&&(tmp1->type==t_bool)){
                                yyerror("tipi non compatibili");
                        }

                        case'+':
                            switch(tmp->type){
                            case (t_int):  tmp->ivalue=tmp0->ivalue+tmp1->ivalue;
                            return tmp;
                            case (t_bool): tmp->bvalue=tmp0->bvalue+tmp1->bvalue;
                            return tmp;
                            case (t_real): tmp->rvalue=tmp0->rvalue+tmp1->rvalue;
                            return tmp;
                        }

                        case'-':
                            switch(tmp->type){
                            case (t_int):  tmp->ivalue=tmp0->ivalue-tmp1->ivalue;
                            break;
                            case (t_bool): tmp->bvalue=tmp0->bvalue-tmp1->bvalue;
                            break;
                            case (t_real): tmp->rvalue=tmp0->rvalue-tmp1->rvalue;
                            break;
                        }

                        case'*':
                            switch(tmp->type){
                            case (t_int):  tmp->ivalue=tmp0->ivalue*tmp1->ivalue;
                            break;
                            case (t_bool): tmp->bvalue=tmp0->bvalue*tmp1->bvalue;
                            break;
                            case (t_real): tmp->rvalue=tmp0->rvalue*tmp1->rvalue;
                            break;
                        }

                        case'/':
                            switch(tmp->type){
                            case (t_int):  tmp->ivalue=tmp0->ivalue/tmp1->ivalue;
                            break;
                            case (t_bool): tmp->bvalue=tmp0->bvalue/tmp1->bvalue;
                            break;
                            case (t_real): tmp->rvalue=tmp0->rvalue/tmp1->rvalue;
                            break;
                        }

                        case'<':
                            switch(tmp->type){
                            case (t_int):  tmp->ivalue=tmp0->ivalue<tmp1->ivalue;
                            break;
                            case (t_bool): tmp->bvalue=tmp0->bvalue<tmp1->bvalue;
                            break;
                            case (t_real): tmp->rvalue=tmp0->rvalue<tmp1->rvalue;
                            break;
                        }

                        case'>':
                            switch(tmp->type){
                            case (t_int):  tmp->ivalue=tmp0->ivalue>tmp1->ivalue;
                            break;
                            case (t_bool): tmp->bvalue=tmp0->bvalue>tmp1->bvalue;
                            break;
                            case (t_real): tmp->rvalue=tmp0->rvalue>tmp1->rvalue;
                            break;
                        }

                        case GE:
                            switch(tmp->type){
                            case (t_int):  tmp->ivalue=tmp0->ivalue>=tmp1->ivalue;
                            break;
                            case (t_bool): tmp->bvalue=tmp0->bvalue>=tmp1->bvalue;
                            break;
                            case (t_real): tmp->rvalue=tmp0->rvalue>=tmp1->rvalue;
                            break;
                        }

                        case LE:
                            switch(tmp->type){
                            case (t_int):  tmp->ivalue=tmp0->ivalue<=tmp1->ivalue;
                            break;
                            case (t_bool): tmp->bvalue=tmp0->bvalue<=tmp1->bvalue;
                            break;
                            case (t_real): tmp->rvalue=tmp0->rvalue<=tmp1->rvalue;
                            break;
                        }

                        case NE:
                            switch(tmp->type){
                            case (t_int):  tmp->ivalue=tmp0->ivalue!=tmp1->ivalue;
                            break;
                            case (t_bool): tmp->bvalue=tmp0->bvalue!=tmp1->bvalue;
                            break;
                            case (t_real): tmp->rvalue=tmp0->rvalue!=tmp1->rvalue;
                            break;
                        }

                        case EQ:
                            switch(tmp->type){
                            case (t_int):  tmp->ivalue=tmp0->ivalue==tmp1->ivalue;
                            break;
                            case (t_bool): tmp->bvalue=tmp0->bvalue==tmp1->bvalue;
                            break;
                            case (t_real): tmp->rvalue=tmp0->rvalue==tmp1->rvalue;
                            break;
                        }
                    }       
                    
            }
    }
    return 0;
}

