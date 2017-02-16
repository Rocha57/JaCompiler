%{
	/*
		flex jac.l
		clang-3.8 -o jac lex.yy.c
		./jac -l < ficheiro

		REALLIT MAYBE FINISHED
	*/
	int col = 1;
	int line = 1;
	int aux1 = 0;
	int aux2 = 0;

	int erros = 0;
%}

ID					[_$a-zA-Z][_$0-9a-zA-Z]*
DECLIT				0|[1-9]|[1-9][0-9_]*[0-9]
EXP 				[eE]["+""-"]?{DECLIT}
REALLIT 			{DECLIT}"."[{DECLIT}{EXP}]?|"."{DECLIT}{EXP}?|{DECLIT}{EXP}

ESCAPE 				\\"|\\n|\\t|\\r|\\|\\f
STRLIT 				\"([^\\\n\"\r]|{ESCAPE})*\"

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
DOTLENGTH			.length
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
NEW 				new
NOT					"!"
OR 					"||"
RETURN 				return
PARSEINT 			Integer.parseInt
PLUS 				"+"
PRINT 				System.out.println
PUBLIC 				public
SEMI				";"
STAR 				"*"
STATIC 				static
STRING 				String
VOID				void
WHILE 				while

SPACE				" "
TAB					"\t"
PARAGRAPH			"\n"

RESERVED			null|abstract|assert|break|byte|case|catch|char|const|continue|default|enum|extends|final|finally|float|for|goto|implements|instanceof|interface|long|native|package|private|protected|short|strictfp|super|switch|synchronized|this|throws?|transient|try|volatile

INITCOMMENT			"/*"
ENDCOMMENT			"*/"

%X					COMMENT


%%
"//".*							//if IT catches a // ignores everything until a \n
{INITCOMMENT}					{aux1 = line; aux2 = col; col += yyleng; BEGIN COMMENT;}
<COMMENT><<EOF>>				{printf("Line %d, col %d: unterminated comment\n", aux1, aux2); BEGIN 0;}
<COMMENT>{ENDCOMMENT}			{col += yyleng; BEGIN 0;}
<COMMENT>.						{col += yyleng;}
<COMMENT>{PARAGRAPH}			{col = 1; line++;}

{CBRACE}						{if(erros==0) printf("CBRACE\n"); col += yyleng;}
{CCURV}							{if(erros==0) printf("CCURV\n"); col += yyleng;}
{CSQUARE}						{if(erros==0) printf("CSQUARE\n"); col += yyleng;}
{OBRACE}						{if(erros==0) printf("OBRACE\n"); col += yyleng;}
{OCURV}							{if(erros==0) printf("OCURV\n"); col += yyleng;}
{OSQUARE}						{if(erros==0) printf("OSQUARE\n"); col += yyleng;}

{ASSIGN}						{if(erros==0) printf("ASSIGN\n"); col += yyleng;}
{BOOL} 							{if(erros==0) printf("BOOL\n"); col += yyleng;}
{CLASS} 						{if(erros==0) printf("CLASS\n"); col += yyleng;}
{COMMA}							{if(erros==0) printf("COMMA\n"); col += yyleng;}
{DO} 							{if(erros==0) printf("DO\n"); col += yyleng;}
{ELSE}							{if(erros==0) printf("ELSE\n"); col += yyleng;}
{IF}							{if(erros==0) printf("IF\n"); col += yyleng;}
{INT} 							{if(erros==0) printf("INT\n"); col += yyleng;}
{NEW} 							{if(erros==0) printf("NEW\n"); col += yyleng;}
{NOT}							{if(erros==0) printf("NOT\n"); col += yyleng;}
{RETURN} 						{if(erros==0) printf("RETURN\n"); col += yyleng;}
{PARSEINT} 						{if(erros==0) printf("PARSEINT\n"); col += yyleng;}
{PRINT}							{if(erros==0) printf("PRINT\n"); col += yyleng;}
{PUBLIC} 						{if(erros==0) printf("PUBLIC\n"); col += yyleng;}
{SEMI}							{if(erros==0) printf("SEMIC\n"); col += yyleng;}
{STATIC} 						{if(erros==0) printf("STATIC\n"); col += yyleng;}
{STRING} 						{if(erros==0) printf("STRING\n"); col += yyleng;}
{VOID}							{if(erros==0) printf("VOID\n"); col += yyleng;}
{WHILE}							{if(erros==0) printf("WHILE\n"); col += yyleng;}

{INTLIT}						{if(erros==0) printf("INTLIT(%s)\n", yytext); col += yyleng;}
{DECLIT}						{if(erros==0) printf("DECLIT(%s)\n", yytext); col += yyleng;}
{BOOLIT}						{if(erros==0) printf("BOOLLIT(%s)\n", yytext); col += yyleng;}				
{OP1}							{if(erros==0) printf("OP1(%s)\n", yytext); col += yyleng;}
{OP2}							{if(erros==0) printf("OP2(%s)\n", yytext); col += yyleng;}
{OP3}							{if(erros==0) printf("OP3(%s)\n", yytext); col += yyleng;}
{OP4}							{if(erros==0) printf("OP4(%s)\n", yytext); col += yyleng;}				
{SPACE}|{TAB}					{col += yyleng;}
{PARAGRAPH}						{line++; col = 1;}
{ID}							{if(erros==0) printf("ID(%s)\n", yytext); col += yyleng;}
								//DAQUI PARA BAIXO ESTÁ DO ANO PASSADO, AINDA NÃO VOU TOCAR ENQUANTO NÃO APARECER O ENUNCIADO
'({ESCAPE}|[^\\\n']|(\\[^\n']))*' 						{printf("Line %d, col %d: invalid char constant (%s)\n",line, col, yytext); col += yyleng;}
'({ESCAPE}|[^\\\n']|(\\[^\\\n']))*(\\)? 						{printf("Line %d, col %d: unterminated char constant\n", line, col); col = 1;}
\"({ESCAPE}|[^\\\n\"]|(\\[^\n\"]))*\"					{printf("Line %d, col %d: invalid string constant (%s)\n",line, col, yytext); col += yyleng;}
\"({ESCAPE}|[^\\\n\"]|(\\[^\\\n\"]))*(\\)?	 		{printf("Line %d, col %d: unterminated string constant\n", line, col); col = 1;}
.								{printf("Line %d, col %d: illegal character (%s)\n", line, col, yytext); col += yyleng;} //Mas este está certo eheh


%%
int main(int argc, char** argv){

	if (argc > 1 && ((strcmp(argv[1], "-l") == 0)) || (strcmp(argv[1], "-1") == 0)))
		erros = 0;
	else
		erros = 1;
	yylex();

	return 0;
}

int yywrap(){
	return 1;
}