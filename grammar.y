%{
    #include "strutture.h"
	int yylex(void);
    void yyerror(char *);
   	
%}

%union {
    int iValue;
    char * sName;
    double rValue;
    struct nodeTypeTag * nPtr;
};

%token <iValue> INTEGER BOOL       
%token <sName> VARIABLE 
%token <rValue> REAL
%token WHILE IF PRINT T_INT T_REAL T_BOOL
%nonassoc IFX
%nonassoc UMINUS
%nonassoc ELSE

%left GE LE EQ NE '>' '<'
%left '+' '-'
%left '*' '/'

%type <nPtr> stmt expr type function program

%%


program: 
    function   {printf("sono un bravo parser\n");}
    ;

function:
    function stmt				{ ex($2); }
    | stmt						{ ex($1); }
    ;

expr:  
    VARIABLE						{ $$ = id($1); }				
    | INTEGER					    { 
    									nodeType node;
    									valueU tmp;
    									tmp.ivalue = $1;
    									$$ = con(tmp,t_int); 
    								}
    | BOOL						    { 
    									valueU tmp;
    									tmp.bvalue = $1;
    									$$ = con(tmp,t_bool); 
    								}
    | REAL						    { 
    									valueU tmp;
    									tmp.rvalue = $1;
    									$$ = con(tmp,t_real); 
    								}
    | expr '+' expr				    { $$ = opr('+',2,$1,$3); }
    | expr '-' expr				    { $$ = opr('-',2,$1,$3); }
    | expr '*' expr				    { $$ = opr('*',2,$1,$3); }
    | expr '/' expr				    { $$ = opr('/',2,$1,$3); }
    | '-' expr %prec UMINUS		    { $$ = opr(UMINUS,1,$2); }
    | expr '<' expr         		{ $$ = opr('<',2,$1,$3); }
    | expr '>' expr         		{ $$ = opr('>',2,$1,$3); }
    | expr GE expr          		{ $$ = opr(GE,2,$1,$3); }
    | expr LE expr          		{ $$ = opr(LE,2,$1,$3); }
    | expr NE expr         	    	{ $$ = opr(NE,2,$1,$3); }
    | expr EQ expr			    	{ $$ = opr(EQ,2,$1,$3); }
    | '(' expr ')'			      	{ $$ = $2; }
    ;

type: 
    T_INT						    { $$ = tipo(t_int); }
    | T_BOOL						{ $$ = tipo(t_bool); }
    | T_REAL						{ $$ = tipo(t_real); }
    ;

stmt: 
    expr ';'								{ $$ = $1; }
    | '{' function '}'				        { $$ = $2; }
    | ';'								    { $$ = opr(';',2,NULL,NULL); }
    | PRINT expr ';'						{ $$ = opr(PRINT,1,$2); }
    | VARIABLE '=' expr ';'				    { $$ = opr('=',2,id($1),$3); }
    | type VARIABLE ';'					    { $$ = opr('D',2,$1,id($2)); }
    | WHILE '(' expr ')' stmt			    { $$ = opr(WHILE,2,$3,$5); }
    | IF '(' expr ')' stmt %prec IFX		{ $$ = opr(IF,2,$3,$5); }
    | IF '(' expr ')' stmt ELSE stmt    	{ $$ = opr(IF,3,$3,$5,$7); }
	;

%%




void yyerror(char * s){
  fprintf(stderr, "%s\n",s);
}

int main(void){
  yyparse();
  return 0;
}
