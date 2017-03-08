%{
    #include "structures.h"
    
    
    extern void yyerror( char *s);
    int yylex(void);
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
%token CBRACE
%token CCURV
%token CSQUARE
%token OBRACE
%token OCURV
%token OSQUARE

%token AND
%token ASSIGN
%token BOOL
%token CLASS
%token COMMA
%token DIV
%token DO 
%token DOTLENGTH
%token DOUBLE
%token ELSE
%token EQ
%token GEQ
%token GT
%token IF 
%token INT 
%token LEQ
%token LT 
%token MINUS
%token MOD
%token NEQ
%token NOT
%token OR 
%token RETURN 
%token PARSEINT
%token PLUS
%token PRINT 
%token PUBLIC
%token SEMI 
%token STAR 
%token STATIC
%token STRING
%token VOID 
%token WHILE

%token RESERVED
%token <yylval> ID
%token <yylval> BOOLLIT
%token <yylval> REALLIT
%token <yylval> DECLIT
%token <yylval> STRLIT

%type <node> Program
%type <node> FieldDecl
%type <node> MethodDecl
%type <node> MethodHeader
%type <node> MethodBody
%type <node> FormalParams
%type <node> VarDecl
%type <node> Type
%type <node> Statement
%type <node> Assignment
%type <node> MethodInvocation
%type <node> ParseArgs
%type <node> Expr



%start Program
 
%nonassoc CCURV
%nonassoc ELSE 

%left COMMA
%right ASSIGN 
%left OR
%left AND
%left EQ NEQ
%left GEQ LEQ GT LT
%left PLUS MINUS
%left STAR DIV MOD
%left NOT
%left OCURV OSQUARE


 
%%
/*Gramatica definida como no enunciado*/ 
Program: CLASS ID OBRACE {FieldDecl | MethodDecl | SEMI} CBRACE             {;}

FieldDecl: PUBLIC STATIC Type ID {COMMA ID} SEMI                            {;}
         | error SEMI                                                       {;}

MethodDecl: PUBLIC STATIC MethodHeader MethodBody                           {;}

MethodHeader: (Type | VOID) ID OCURV [FormalParams] CCURV                   {;}

MethodBody: OBRACE {VarDecl | Statement} CBRACE                            {;}

FormalParams: Type ID {COMMA Type ID}                                       {;}
            | STRING OSQUARE CSQUARE ID                                     {;}

VarDecl: Type ID {COMMA ID} SEMI                                            {;}

Type: BOOL                                                                  {;}
    | INT                                                                   {;}
    | DOUBLE                                                                {;}

Statement: OBRACE {Statement} CBRACE                                        {;}
         | IF OCURV Expr CCURV Statement [ELSE Statement]                   {;} 
         | WHILE OCURV Expr CCURV Statement                                 {;}
         | DO Statement WHILE OCURV Expr CCURV SEMI                         {;}
         | PRINT OCURV (Expr | STRLIT) CCURV SEMI                           {;}
         | [(Assignment | MethodInvocation | ParseArgs)] SEMI               {;}
         | RETURN [Expr] SEMI                                               {;}
         | error SEMI                                                       {;}

Assignment: ID ASSIGN Expr                                                  {;}

MethodInvocation: ID OCURV [Expr {COMMA Expr}] CCURV                        {;}
                | ID OCURV error CCURV                                      {;}

ParseArgs: PARSEINT OCURV ID OSQUARE Expr CSQUARE CCURV                     {;}
         | PARSEINT OCURV error CCURV                                       {;}

Expr: Assignment | MethodInvocation | ParseArgs                             {;}
    | Expr (AND | OR) Expr                                                  {;}
    | Expr (EQ | GEQ | GT | LEQ | LT | NEQ) Expr                            {;}
    | Expr (PLUS | MINUS | STAR | DIV | MOD) Expr                           {;}
    | (PLUS | MINUS | NOT) Expr                                             {;}
    | ID [DOTLENGTH]                                                        {;}
    | OCURV Expr CCURV                                                      {;}
    | BOOLLIT | DECLIT | REALLIT                                            {;}
    | OCURV error CCURV                                                     {;}


%%
//A main está no lex
    