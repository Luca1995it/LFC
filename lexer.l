%{
    #include <stdio.h>
    #include "y.tab.h"
   
    void yyerror(char *);
%}

%%


[0-9]+"."[0-9]+   {
                        yylval.rValue = atof(yytext);
                        return REAL;
                    }


[0-9]+            {
                        yylval.iValue = atoi(yytext);
                        return INTEGER;
                    }     

"true"              {
                        yylval.iValue = 1;
                        return BOOL;
                    }
            
"false"             {
                        yylval.iValue = 0;
                        return BOOL;
                    }

"int"	       		return T_INT;
"bool"          		return T_BOOL;
"real"          		return T_REAL;
"while"         		return WHILE;
"if"            		return IF;
"else"          		return ELSE;
"print"         		return PRINT;

[A-Za-z]+           {
                        yylval.sName = strdup(yytext);
                        return VARIABLE;
                    }


[+()<>=*/;{}.-]   	{ 
						return *yytext; 
					}

">="            		return GE;
"<="            		return LE;
"=="            		return EQ;
"!="            		return NE;


[ \t\n]         		;

.               		yyerror("Unknown character");

%%

int yywrap(void) {
    return 1;
}
