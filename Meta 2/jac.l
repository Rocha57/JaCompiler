%{
	/*
		({PARAGRAPH}|{CARRIAGE}|{CARRIAGE}{PARAGRAPH}) -> Line terminator
	*/
	int col = 1;
	int line = 1;
	int aux1 = 0;
	int aux2 = 0;

	int erros = 0;
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


{STRLIT} 						{if (erros==0) printf("STRLIT(%s)\n", yytext);col += yyleng; BEGIN 0;}
\"								{aux1 = line; aux2 = col; col += yyleng; BEGIN STR;} 
<STR>{ESCAPE}					{ col+= yyleng; }
<STR>\"							{col += yyleng; BEGIN 0;}
<STR>({PARAGRAPH}|{CARRIAGE}|{CARRIAGE}{PARAGRAPH})			    {printf("Line %d, col %d: unterminated string literal\n", aux1, aux2);col=1; line++; BEGIN 0;}
<STR><<EOF>>					{printf("Line %d, col %d: unterminated string literal\n", aux1, aux2); return 0;}
<STR>\\.?						{printf("Line %d, col %d: invalid escape sequence (%s)\n", line, col,yytext); col+=yyleng;} //If it catches a \ with somthing other than a escape sequence, or nothing at all, prints this error
<STR>.							{col+=yyleng;}


{CBRACE}						{if(erros==0) printf("CBRACE\n"); col += yyleng;}
{CCURV}							{if(erros==0) printf("CCURV\n"); col += yyleng;}
{CSQUARE}						{if(erros==0) printf("CSQUARE\n"); col += yyleng;}
{OBRACE}						{if(erros==0) printf("OBRACE\n"); col += yyleng;}
{OCURV}							{if(erros==0) printf("OCURV\n"); col += yyleng;}
{OSQUARE}						{if(erros==0) printf("OSQUARE\n"); col += yyleng;}

{AND}							{if(erros==0) printf("AND\n"); col += yyleng;}
{ASSIGN}						{if(erros==0) printf("ASSIGN\n"); col += yyleng;}
{BOOL} 							{if(erros==0) printf("BOOL\n"); col += yyleng;}
{CLASS} 						{if(erros==0) printf("CLASS\n"); col += yyleng;}
{COMMA}							{if(erros==0) printf("COMMA\n"); col += yyleng;}
{DIV}							{if(erros==0) printf("DIV\n"); col += yyleng;}
{DO} 							{if(erros==0) printf("DO\n"); col += yyleng;}
{DOTLENGTH}						{if(erros==0) printf("DOTLENGTH\n"); col += yyleng;}
{DOUBLE}						{if(erros==0) printf("DOUBLE\n"); col += yyleng;}
{ELSE}							{if(erros==0) printf("ELSE\n"); col += yyleng;}
{EQ}							{if(erros==0) printf("EQ\n"); col += yyleng;}
{GEQ}							{if(erros==0) printf("GEQ\n"); col += yyleng;}
{GT}							{if(erros==0) printf("GT\n"); col += yyleng;}
{IF}							{if(erros==0) printf("IF\n"); col += yyleng;}
{INT} 							{if(erros==0) printf("INT\n"); col += yyleng;}
{LEQ}							{if(erros==0) printf("LEQ\n"); col += yyleng;}
{LT}							{if(erros==0) printf("LT\n"); col += yyleng;}
{MINUS}							{if(erros==0) printf("MINUS\n"); col += yyleng;}
{MOD}							{if(erros==0) printf("MOD\n"); col += yyleng;}
{NEQ}							{if(erros==0) printf("NEQ\n"); col += yyleng;}
{NOT}							{if(erros==0) printf("NOT\n"); col += yyleng;}
{OR}							{if(erros==0) printf("OR\n"); col += yyleng;}
{RETURN} 						{if(erros==0) printf("RETURN\n"); col += yyleng;}
{PARSEINT} 						{if(erros==0) printf("PARSEINT\n"); col += yyleng;}
{PLUS}							{if(erros==0) printf("PLUS\n"); col += yyleng;}
{PRINT}							{if(erros==0) printf("PRINT\n"); col += yyleng;}
{PUBLIC} 						{if(erros==0) printf("PUBLIC\n"); col += yyleng;}
{SEMI}							{if(erros==0) printf("SEMI\n"); col += yyleng;}
{STAR}							{if(erros==0) printf("STAR\n"); col += yyleng;}
{STATIC} 						{if(erros==0) printf("STATIC\n"); col += yyleng;}
{STRING} 						{if(erros==0) printf("STRING\n"); col += yyleng;}
{VOID}							{if(erros==0) printf("VOID\n"); col += yyleng;}
{WHILE}							{if(erros==0) printf("WHILE\n"); col += yyleng;}

{REALLIT}						{if(erros==0) printf("REALLIT(%s)\n", yytext); col += yyleng;}
{DECLIT}						{if(erros==0) printf("DECLIT(%s)\n", yytext); col += yyleng;}
{BOOLIT}						{if(erros==0) printf("BOOLLIT(%s)\n", yytext); col += yyleng;}				
{SPACE}|{TAB}|{FORM}			{col += yyleng;}
{PARAGRAPH}|{CARRIAGE}|{CARRIAGE}{PARAGRAPH}	{line++; col = 1;}
{RESERVED}						{if(erros==0) printf("RESERVED(%s)\n", yytext); col += yyleng;}
{ID}							{if(erros==0) printf("ID(%s)\n", yytext); col += yyleng;}
.								{printf("Line %d, col %d: illegal character (%s)\n", line, col, yytext); col += yyleng;} //Mas este está certo eheh


%%
int main(int argc, char** argv){

	if (argc > 1){
		if (strcmp(argv[1], "-l") == 0)
			erros = 0;
		else if (strcmp(argv[1], "-1") == 0)
			erros = 1;
	}
	else
		erros = 1;
	yylex();

	return 0;
}

int yywrap(){
	return 1;
}