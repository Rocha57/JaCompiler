%{
    #include "structures.h"
    
    
    extern void yyerror( char *s);
    int yylex(void);
    int erros =0, primeira = 0, errosyn = 0;
    Node* raiz;
    Node* temp;
    Node* temp1;
    /*int yydebug=1;
    */
%}

%union{
    char *yylval; /*token*/
    struct _Node* node;
    
}

/*falta mudar isto*/
%token AMP
%token AND
%token ASSIGN
%token AST
%token CHAR
%token COMMA
%token DIV
%token ELSE
%token EQ
%token FOR
%token GE
%token GT
%token IF
%token INT
%token LBRACE
%token LE
%token LPAR
%token LSQ
%token LT
%token MINUS
%token MOD
%token NE
%token NOT
%token OR
%token PLUS
%token RBRACE
%token RETURN
%token RPAR
%token RSQ
%token SEMI
%token VOID
%token RESERVED
%token <yylval> ID
%token <yylval> INTLIT
%token <yylval> CHRLIT
%token <yylval> STRLIT

%type <node> Start
%type <node> FunctionDefinition 
%type <node> FunctionDeclaration
%type <node> FunctionDeclarator
%type <node> ParameterList
%type <node> CommaParameterDeclaration
%type <node> ParameterDeclaration
%type <node> DeclarationList
%type <node> Declaration
%type <node> StatementList
%type <node> CommaDeclarator
%type <node> TypeSpec
%type <node> Declarator
%type <node> AstList
%type <node> Statement
%type <node> ERRStatement
%type <node> InsideStatement
%type <node> ForList
%type <node> Expression
%type <node> ExpressionCall
%type <node> CommaExpression



%start Start
 
%nonassoc RPAR
%nonassoc ELSE 

%left COMMA
%right ASSIGN 
%left OR
%left AND
%left EQ NE
%left GE LE GT LT
%left PLUS MINUS
%left AST DIV MOD
%left NOT AMP
%left LPAR LSQ


 
%%
/*aqui definir gramatica*/ 
Start:   FunctionDefinition  Start    { }


%%
int main(int argc, char** argv){
    if(argc>1){
        if(strcmp(argv[1],"-1")==0){
            primeira=1;
            erros = 0;
            yylex();
        }
        if(strcmp(argv[1],"-l")==0){
            primeira=1;
            erros = 1;
            yylex();
        }
        if( strcmp(argv[1],"-t") == 0 && erros == 0 && primeira == 0){
            yyparse(); 
            if(errosyn == 0){
                printTree(raiz,0);
            }
        }
            
    }
    else{
       primeira=0;
       erros = 0;
       yyparse(); 
    }
    return 0;
}
    