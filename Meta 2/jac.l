%{
	/*
		({PARAGRAPH}|{CARRIAGE}|{CARRIAGE}{PARAGRAPH}) -> Line terminator
	*/
	#include "y.tab.h"
	int col = 1;
	int line = 1;
	int aux1 = 0;
	int aux2 = 0;

	int erros = 0;
	int erros_sintax = 0;
	int sintax = 0;

	struct Node* raiz;
	void printTree(struct Node* root,int altura);
	void elimina(struct Node* root);
%}

ID					[_$a-zA-Z][_$0-9a-zA-Z]*
DECLIT				0|[1-9]|[1-9][0-9_]*[0-9]
AUXLIT				[0-9]|[0-9][0-9_]*[0-9]
EXP 				[eE]("+"|"-")?{AUXLIT}
REALLIT 			{AUXLIT}"."({AUXLIT}|{EXP})?|"."{AUXLIT}{EXP}?|{AUXLIT}{EXP}|{AUXLIT}"."{AUXLIT}{EXP}

ESCAPE 				\\\"|\\n|\\t|\\r|\\\\|\\f

BOOLIT				true|false


CBRACE				"}"
CCURV				")"
CSQUARE				"]"
OBRACE				"{"
OCURV				"("
OSQUARE				"["

AND 				"&&"
ASSIGN				"="
BOOL 				boolean
CLASS 				class
COMMA				","
DIV 				"/"
DO 					do
DOTLENGTH			".length"
DOUBLE 				double
ELSE				else
EQ 					"=="
GEQ 				">="
GT 					">"
IF					if
INT 				int
LEQ 				"<="
LT 					"<"
MINUS 				"-"
MOD 				"%"
NEQ 				"!="
NOT					"!"
OR 					"||"
RETURN 				return
PARSEINT 			"Integer.parseInt"
PLUS 				"+"
PRINT 				"System.out.println"
PUBLIC 				public
SEMI				";"
STAR 				"*"
STATIC 				static
STRING 				String
VOID				void
WHILE 				while

FORM				"\f"
CARRIAGE			"\r"
SPACE				" "
TAB					"\t"
PARAGRAPH			"\n"

RESERVED			null|abstract|assert|break|byte|case|catch|char|const|continue|default|enum|extends|final|finally|float|for|goto|implements|import|instanceof|Integer|interface|long|native|new|package|private|protected|short|strictfp|super|switch|synchronized|System|this|throws?|transient|try|volatile|"++"|"--"

INITCOMMENT			"/*"
ENDCOMMENT			"*/"

STRLIT 				\"([^\\\n\"\r]|{ESCAPE})*\"

%X					COMMENT
%X					STR


%%
"//"([^\n\r])*					//if it catches a // ignores everything until a \n|\r
{INITCOMMENT}					{aux1 = line; aux2 = col; col += yyleng; BEGIN COMMENT;}
<COMMENT><<EOF>>				{printf("Line %d, col %d: unterminated comment\n", aux1, aux2); col += yyleng; return 0;}
<COMMENT>{ENDCOMMENT}			{col += yyleng; BEGIN 0;}
<COMMENT>({PARAGRAPH}|{CARRIAGE}|{CARRIAGE}{PARAGRAPH})			{col = 1; line++;}
<COMMENT>.						{col += yyleng;}


{STRLIT} 						{if (erros==0) printf("STRLIT(%s)\n", yytext);col += yyleng; BEGIN 0; yylval.yylval = strdup(yytext); if (sintax==1)return STRLIT;}
\"								{aux1 = line; aux2 = col; col += yyleng; BEGIN STR;} 
<STR>{ESCAPE}					{ col += yyleng; }
<STR>\"							{col += yyleng; BEGIN 0;}
<STR>({PARAGRAPH}|{CARRIAGE}|{CARRIAGE}{PARAGRAPH})			    {printf("Line %d, col %d: unterminated string literal\n", aux1, aux2);col=1; line++; BEGIN 0;}
<STR><<EOF>>					{printf("Line %d, col %d: unterminated string literal\n", aux1, aux2); col += yyleng; return 0;}
<STR>\\.?						{printf("Line %d, col %d: invalid escape sequence (%s)\n", line, col,yytext); col += yyleng;} //If it catches a \ with somthing other than a escape sequence, or nothing at all, prints this error
<STR>.							{col += yyleng;}


{CBRACE}						{if(erros==0) printf("CBRACE\n"); col += yyleng; if (sintax==1)return CBRACE;}
{CCURV}							{if(erros==0) printf("CCURV\n"); col += yyleng; if (sintax==1)return CCURV;}
{CSQUARE}						{if(erros==0) printf("CSQUARE\n"); col += yyleng; if (sintax==1)return CSQUARE;}
{OBRACE}						{if(erros==0) printf("OBRACE\n"); col += yyleng; if (sintax==1)return OBRACE;}
{OCURV}							{if(erros==0) printf("OCURV\n"); col += yyleng; if (sintax==1)return OCURV;}
{OSQUARE}						{if(erros==0) printf("OSQUARE\n"); col += yyleng; if (sintax==1)return OSQUARE;}
 
{AND}							{if(erros==0) printf("AND\n"); col += yyleng; if (sintax==1)return AND;}
{ASSIGN}						{if(erros==0) printf("ASSIGN\n"); col += yyleng; if (sintax==1)return ASSIGN;}
{BOOL} 							{if(erros==0) printf("BOOL\n"); col += yyleng; if (sintax==1)return BOOL;}
{CLASS} 						{if(erros==0) printf("CLASS\n"); col += yyleng; if (sintax==1)return CLASS;}
{COMMA}							{if(erros==0) printf("COMMA\n"); col += yyleng; if (sintax==1)return COMMA;}
{DIV}							{if(erros==0) printf("DIV\n"); col += yyleng; if (sintax==1)return DIV;}
{DO} 							{if(erros==0) printf("DO\n"); col += yyleng; if (sintax==1)return DO;}
{DOTLENGTH}						{if(erros==0) printf("DOTLENGTH\n"); col += yyleng; if (sintax==1)return DOTLENGTH;}
{DOUBLE}						{if(erros==0) printf("DOUBLE\n"); col += yyleng; if (sintax==1)return DOUBLE;}
{ELSE}							{if(erros==0) printf("ELSE\n"); col += yyleng; if (sintax==1)return ELSE;}
{EQ}							{if(erros==0) printf("EQ\n"); col += yyleng; if (sintax==1)return EQ;}
{GEQ}							{if(erros==0) printf("GEQ\n"); col += yyleng; if (sintax==1)return GEQ;}
{GT}							{if(erros==0) printf("GT\n"); col += yyleng; if (sintax==1)return GT;}
{IF}							{if(erros==0) printf("IF\n"); col += yyleng; if (sintax==1)return IF;}
{INT} 							{if(erros==0) printf("INT\n"); col += yyleng; if (sintax==1)return INT;}
{LEQ}							{if(erros==0) printf("LEQ\n"); col += yyleng; if (sintax==1)return LEQ;}
{LT}							{if(erros==0) printf("LT\n"); col += yyleng; if (sintax==1)return LT;}
{MINUS}							{if(erros==0) printf("MINUS\n"); col += yyleng; if (sintax==1)return MINUS;}
{MOD}							{if(erros==0) printf("MOD\n"); col += yyleng; if (sintax==1)return MOD;}
{NEQ}							{if(erros==0) printf("NEQ\n"); col += yyleng; if (sintax==1)return NEQ;}
{NOT}							{if(erros==0) printf("NOT\n"); col += yyleng; if (sintax==1)return NOT;}
{OR}							{if(erros==0) printf("OR\n"); col += yyleng; if (sintax==1)return OR;}
{RETURN} 						{if(erros==0) printf("RETURN\n"); col += yyleng; if (sintax==1)return RETURN;}
{PARSEINT} 						{if(erros==0) printf("PARSEINT\n"); col += yyleng; if (sintax==1)return PARSEINT;}
{PLUS}							{if(erros==0) printf("PLUS\n"); col += yyleng; if (sintax==1)return PLUS;}
{PRINT}							{if(erros==0) printf("PRINT\n"); col += yyleng; if (sintax==1)return PRINT;}
{PUBLIC} 						{if(erros==0) printf("PUBLIC\n"); col += yyleng; if (sintax==1)return PUBLIC;}
{SEMI}							{if(erros==0) printf("SEMI\n"); col += yyleng; if (sintax==1)return SEMI;}
{STAR}							{if(erros==0) printf("STAR\n"); col += yyleng; if (sintax==1)return STAR;}
{STATIC} 						{if(erros==0) printf("STATIC\n"); col += yyleng; if (sintax==1)return STATIC;}
{STRING} 						{if(erros==0) printf("STRING\n"); col += yyleng; if (sintax==1)return STRING;}
{VOID}							{if(erros==0) printf("VOID\n"); col += yyleng; if (sintax==1)return VOID;}
{WHILE}							{if(erros==0) printf("WHILE\n"); col += yyleng; if (sintax==1)return WHILE;}

{REALLIT}						{if(erros==0) printf("REALLIT(%s)\n", yytext);col += yyleng; yylval.yylval = strdup(yytext); if (sintax==1)return REALLIT;}
{DECLIT}						{if(erros==0) printf("DECLIT(%s)\n", yytext);col += yyleng; yylval.yylval = strdup(yytext); if (sintax==1)return DECLIT;}
{BOOLIT}						{if(erros==0) printf("BOOLLIT(%s)\n", yytext);col += yyleng; yylval.yylval = strdup(yytext); if (sintax==1)return BOOLLIT;}				
{SPACE}|{TAB}|{FORM}			{col += yyleng;}
{PARAGRAPH}|{CARRIAGE}|{CARRIAGE}{PARAGRAPH}	{line++; col = 1;}
{RESERVED}						{if(erros==0) printf("RESERVED(%s)\n", yytext); col += yyleng; yylval.yylval = strdup(yytext); if (sintax==1)return RESERVED;}
{ID}							{if(erros==0) printf("ID(%s)\n", yytext); col += yyleng; yylval.yylval = strdup(yytext); if (sintax==1)return ID;}
<<EOF>>							{col += yyleng; return yytext[0];}
.								{printf("Line %d, col %d: illegal character (%s)\n", line, col, yytext); col += yyleng;}


%%
int main(int argc, char** argv){

	if (argc > 1){
		if (strcmp(argv[1], "-l") == 0){
			erros = 0;
			yylex();
		}
		else if (strcmp(argv[1], "-1") == 0){
			erros = 1;
			yylex();
		}
		else if (strcmp(argv[1], "-t") == 0){
			sintax = 1;
			erros = 1;
			yyparse();
			if (erros_sintax==0){
				printTree(raiz,0);
			}
			elimina(raiz);
		}
	}
	else{
		sintax = 1;
		erros = 1;
		yyparse();
		elimina(raiz);
	}

	return 0;
}

void yyerror (const char *s){
	if (sintax == 1){
		printf ("Line %d, col %d: %s: %s\n", line, col-(int)yyleng, s, yytext);
		erros_sintax = 1;
	}
}

int yywrap(){
	return 1;
}