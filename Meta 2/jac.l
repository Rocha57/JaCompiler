%{
	/*
		({PARAGRAPH}|{CARRIAGE}|{CARRIAGE}{PARAGRAPH}) -> Line terminator
	*/
	int col = 1;
	int line = 1;
	int aux1 = 0;
	int aux2 = 0;
	int auxline, auxcol = 0;

	int erros = 0;
	int erros_sintax = 0;
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
<COMMENT><<EOF>>				{printf("Line %d, col %d: unterminated comment\n", aux1, aux2); return 0;}
<COMMENT>{ENDCOMMENT}			{col += yyleng; BEGIN 0;}
<COMMENT>({PARAGRAPH}|{CARRIAGE}|{CARRIAGE}{PARAGRAPH})			{col = 1; line++;}
<COMMENT>.						{col += yyleng;}


{STRLIT} 						{if (erros==0) printf("STRLIT(%s)\n", yytext);auxline = line; auxcol = col;col += yyleng; BEGIN 0; yylval.token = strdup(yytext); return STRLIT;}
\"								{aux1 = line; aux2 = col; col += yyleng; BEGIN STR;} 
<STR>{ESCAPE}					{ col+= yyleng; }
<STR>\"							{col += yyleng; BEGIN 0;}
<STR>({PARAGRAPH}|{CARRIAGE}|{CARRIAGE}{PARAGRAPH})			    {printf("Line %d, col %d: unterminated string literal\n", aux1, aux2);col=1; line++; BEGIN 0;}
<STR><<EOF>>					{printf("Line %d, col %d: unterminated string literal\n", aux1, aux2); return 0;}
<STR>\\.?						{printf("Line %d, col %d: invalid escape sequence (%s)\n", line, col,yytext); col+=yyleng;} //If it catches a \ with somthing other than a escape sequence, or nothing at all, prints this error
<STR>.							{col+=yyleng;}


{CBRACE}						{if(erros==0) printf("CBRACE\n");auxline = line; auxcol = col; col += yyleng; return CBRACE;}
{CCURV}							{if(erros==0) printf("CCURV\n");auxline = line; auxcol = col; col += yyleng; return CCURV;}
{CSQUARE}						{if(erros==0) printf("CSQUARE\n");auxline = line; auxcol = col; col += yyleng; return CSQUARE;}
{OBRACE}						{if(erros==0) printf("OBRACE\n");auxline = line; auxcol = col; col += yyleng; return OBRACE;}
{OCURV}							{if(erros==0) printf("OCURV\n");auxline = line; auxcol = col; col += yyleng; return OCURV;}
{OSQUARE}						{if(erros==0) printf("OSQUARE\n");auxline = line; auxcol = col; col += yyleng; return OSQUARE;}
 
{AND}							{if(erros==0) printf("AND\n");auxline = line; auxcol = col; col += yyleng; return AND;}
{ASSIGN}						{if(erros==0) printf("ASSIGN\n");auxline = line; auxcol = col; col += yyleng; return ASSIGN;}
{BOOL} 							{if(erros==0) printf("BOOL\n");auxline = line; auxcol = col; col += yyleng; return BOOL;}
{CLASS} 						{if(erros==0) printf("CLASS\n");auxline = line; auxcol = col; col += yyleng; return CLASS;}
{COMMA}							{if(erros==0) printf("COMMA\n");auxline = line; auxcol = col; col += yyleng; return COMMA;}
{DIV}							{if(erros==0) printf("DIV\n");auxline = line; auxcol = col; col += yyleng; return DIV;}
{DO} 							{if(erros==0) printf("DO\n");auxline = line; auxcol = col; col += yyleng; return DO;}
{DOTLENGTH}						{if(erros==0) printf("DOTLENGTH\n");auxline = line; auxcol = col; col += yyleng; return DOTLENGTH;}
{DOUBLE}						{if(erros==0) printf("DOUBLE\n");auxline = line; auxcol = col; col += yyleng; return DOUBLE;}
{ELSE}							{if(erros==0) printf("ELSE\n");auxline = line; auxcol = col; col += yyleng; return ELSE;}
{EQ}							{if(erros==0) printf("EQ\n");auxline = line; auxcol = col; col += yyleng; return EQ;}
{GEQ}							{if(erros==0) printf("GEQ\n");auxline = line; auxcol = col; col += yyleng; return GEQ;}
{GT}							{if(erros==0) printf("GT\n");auxline = line; auxcol = col; col += yyleng; return GT;}
{IF}							{if(erros==0) printf("IF\n");auxline = line; auxcol = col; col += yyleng; return IF;}
{INT} 							{if(erros==0) printf("INT\n");auxline = line; auxcol = col; col += yyleng; return INT;}
{LEQ}							{if(erros==0) printf("LEQ\n");auxline = line; auxcol = col; col += yyleng; return LEQ;}
{LT}							{if(erros==0) printf("LT\n");auxline = line; auxcol = col; col += yyleng; return LT;}
{MINUS}							{if(erros==0) printf("MINUS\n");auxline = line; auxcol = col; col += yyleng; return MINUS;}
{MOD}							{if(erros==0) printf("MOD\n");auxline = line; auxcol = col; col += yyleng; return MOD;}
{NEQ}							{if(erros==0) printf("NEQ\n");auxline = line; auxcol = col; col += yyleng; return NEQ;}
{NOT}							{if(erros==0) printf("NOT\n");auxline = line; auxcol = col; col += yyleng; return NOT;}
{OR}							{if(erros==0) printf("OR\n");auxline = line; auxcol = col; col += yyleng; return OR;}
{RETURN} 						{if(erros==0) printf("RETURN\n");auxline = line; auxcol = col; col += yyleng; return RETURN;}
{PARSEINT} 						{if(erros==0) printf("PARSEINT\n");auxline = line; auxcol = col; col += yyleng; return PARSEINT;}
{PLUS}							{if(erros==0) printf("PLUS\n");auxline = line; auxcol = col; col += yyleng; return PLUS;}
{PRINT}							{if(erros==0) printf("PRINT\n");auxline = line; auxcol = col; col += yyleng; return PRINT;}
{PUBLIC} 						{if(erros==0) printf("PUBLIC\n");auxline = line; auxcol = col; col += yyleng; return PUBLIC;}
{SEMI}							{if(erros==0) printf("SEMI\n");auxline = line; auxcol = col; col += yyleng; return SEMI;}
{STAR}							{if(erros==0) printf("STAR\n");auxline = line; auxcol = col; col += yyleng; return STAR;}
{STATIC} 						{if(erros==0) printf("STATIC\n");auxline = line; auxcol = col; col += yyleng; return STATIC;}
{STRING} 						{if(erros==0) printf("STRING\n");auxline = line; auxcol = col; col += yyleng; return STRING;}
{VOID}							{if(erros==0) printf("VOID\n");auxline = line; auxcol = col; col += yyleng; return VOID;}
{WHILE}							{if(erros==0) printf("WHILE\n");auxline = line; auxcol = col; col += yyleng; return WHILE;}

{REALLIT}						{if(erros==0) printf("REALLIT(%s)\n", yytext);auxline = line; auxcol = col; col += yyleng; yylval.token = strdup(yytext); return STRLIT;}
{DECLIT}						{if(erros==0) printf("DECLIT(%s)\n", yytext);auxline = line; auxcol = col; col += yyleng; yylval.token = strdup(yytext); return DECLIT;}
{BOOLIT}						{if(erros==0) printf("BOOLLIT(%s)\n", yytext);auxline = line; auxcol = col; col += yyleng; yylval.token = strdup(yytext); return BOOLLIT;}				
{SPACE}|{TAB}|{FORM}			{auxline = line; auxcol = col;col += yyleng;}
{PARAGRAPH}|{CARRIAGE}|{CARRIAGE}{PARAGRAPH}	{auxline = line; auxcol = col; line++; col = 1;}
{RESERVED}						{if(erros==0) printf("RESERVED(%s)\n", yytext);auxline = line; auxcol = col; col += yyleng; yylval.token = strdup(yytext); return RESERVED;}
{ID}							{if(erros==0) printf("ID(%s)\n", yytext);auxline = line; auxcol = col; col += yyleng; yylval.token = strdup(yytext); return ID;}
<<EOF>>							{auxcol = col; col = 1; return yytext[0];}
.								{printf("Line %d, col %d: illegal character (%s)\n", line, col, yytext);auxline = line; auxcol = col; col += yyleng;}


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
			yyparse();
			if (erros_sintax==0)
				//printTree

		}
	}
	else{
		erros = 1;
		yyparse();
	}

	return 0;
}

void yyerror (char *s){
	if (sintax == 0){
		printf ("Line %d, col %d: %s: %s\n", auxline, auxcol, s, yytext);
		erros_sintax = 1;
	}
}

int yywrap(){
	return 1;
}