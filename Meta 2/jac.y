%{
    #include "structures.h"
    
    
    extern void yyerror( char *s);
    int yylex(void);
    extern Node* raiz;
    Node* temp;
    Node* temp1;
    /*int yydebug=1;
    */

    /*
    IF ELSE AMBIGUITY - http://epaperpress.com/lexandyacc/if.html
    */
%}

%union{
    char *yylval; /*token*/
    struct _Node* node;
    
}

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

%token <yylval> RESERVED
%token <yylval> ID
%token <yylval> BOOLLIT
%token <yylval> REALLIT
%token <yylval> DECLIT
%token <yylval> STRLIT

%type <node> Program
%type <node> ProgramList
%type <node> FieldDecl
%type <node> MethodDecl
%type <node> MethodHeader
%type <node> MethodBody
%type <node> MethodBodyList
%type <node> FormalParams
%type <node> FormalParamsList
%type <node> VarDecl
%type <node> IdList
%type <node> Type
%type <node> Statement
%type <node> StatementList
%type <node> Assignment
%type <node> MethodInvocation
%type <node> ExprList
%type <node> ParseArgs
%type <node> Expr



%start Program
 
%left COMMA
%right ASSIGN 
%left OR
%left AND
%left EQ NEQ
%left GEQ LEQ GT LT
%left PLUS MINUS
%left STAR DIV MOD
%right NOT
%left OCURV OSQUARE CCURV CSQUARE
%nonassoc IFX
%nonassoc ELSE 


 
%%
Program: CLASS ID OBRACE ProgramList CBRACE                                 {temp = createNode(Id,$2,NULL,$4); 
                                                                            raiz = createNode(Program,NULL,temp,NULL);}

ProgramList: ProgramList FieldDecl                                          {$$ = $1; joinIrmao($$,createNode(FieldDecl,NULL,$2,NULL));}
           | ProgramList MethodDecl                                         {$$ = $1; joinIrmao($$,$2);}
           | ProgramList SEMI                                               {$$ = $1;}
           | %empty                                                         {;}
           ;

FieldDecl: PUBLIC STATIC Type ID IdList SEMI                                {$$ = $3;
                                                                            joinIrmao($$,createNode(Id,$4,NULL,$5));}
         | error SEMI                                                       {;}
         ;

MethodDecl: PUBLIC STATIC MethodHeader MethodBody                           {temp = createNode(MethodHeader,NULL,$3,NULL);
                                                                            temp1 = createNode(MethodBody, NULL, $4, NULL);
                                                                            joinIrmao(temp,temp1);
                                                                            $$ = createNode(MethodDecl,NULL,temp,NULL);}

MethodHeader: Type ID OCURV CCURV                                           {;}
            | Type ID OCURV FormalParams CCURV                              {;}
            | VOID ID OCURV CCURV                                           {;}
            | VOID ID OCURV FormalParams CCURV                              {;}
            ;

MethodBody: OBRACE MethodBodyList CBRACE                                    {;}

MethodBodyList: MethodBodyList VarDecl                                       {;}
              | MethodBodyList Statement                                     {;}
              | %empty                                                      {;}
              ;

FormalParams: Type ID FormalParamsList                                      {;}
            | STRING OSQUARE CSQUARE ID                                     {;}
            ;

FormalParamsList: FormalParamsList COMMA Type ID                             {;}
                | %empty                                                          {;}
                ;

VarDecl: Type ID IdList SEMI                                                {;}

IdList: IdList COMMA ID                                                      {;}
      | %empty                                                                    {;}
      ;

Type: BOOL                                                                  {$$ = createNode(Bool,NULL,NULL,NULL);}
    | INT                                                                   {$$ = createNode(Int,NULL,NULL,NULL);}
    | DOUBLE                                                                {$$ = createNode(Double,NULL,NULL,NULL);}
    ;

Statement: OBRACE StatementList CBRACE                                      {;}
         | IF OCURV Expr CCURV Statement ELSE Statement                     {;}
         | IF OCURV Expr CCURV Statement %prec IFX                          {;}
         | WHILE OCURV Expr CCURV Statement                                 {;}
         | DO Statement WHILE OCURV Expr CCURV SEMI                         {;}
         | PRINT OCURV Expr CCURV SEMI                                      {;}
         | PRINT OCURV STRLIT CCURV SEMI                                    {;}         
         | SEMI                                                             {;}
         | Assignment SEMI                                                  {;}
         | MethodInvocation SEMI                                            {;}
         | ParseArgs SEMI                                                   {;}
         | RETURN Expr SEMI                                                 {;}
         | RETURN SEMI                                                      {;}
         | error SEMI                                                       {;}
         ;

StatementList: StatementList Statement                                      {;}
             | %empty                                                             {;}
             ;

Assignment: ID ASSIGN Expr                                                  {;}

MethodInvocation: ID OCURV ExprList CCURV                                   {;}
                | ID OCURV CCURV                                            {;}
                | ID OCURV error CCURV                                      {;}
                ;

ExprList: ExprList COMMA Expr                                                {;}
        | Expr                                                              {$$ = $1;}
        ;

ParseArgs: PARSEINT OCURV ID OSQUARE Expr CSQUARE CCURV                     {;}
         | PARSEINT OCURV error CCURV                                       {;}
         ;

Expr: Assignment                                                            {;}
    | MethodInvocation                                                      {;}
    | ParseArgs                                                             {;}
    | Expr AND Expr                                                         {;}
    | Expr OR Expr                                                          {;}
    | Expr EQ Expr                                                          {;}
    | Expr GEQ Expr                                                         {;}
    | Expr GT Expr                                                          {;}
    | Expr LEQ Expr                                                         {;}
    | Expr LT Expr                                                          {;}
    | Expr NEQ Expr                                                         {;}
    | Expr PLUS Expr                                                        {;}
    | Expr MINUS Expr                                                       {;}
    | Expr STAR Expr                                                        {;}
    | Expr DIV Expr                                                         {;}
    | Expr MOD Expr                                                         {;}
    | PLUS Expr  %prec NOT                                                  {;}
    | MINUS Expr %prec NOT                                                  {;}
    | NOT Expr                                                              {;}
    | ID DOTLENGTH                                                          {;}
    | ID                                                                    {;}
    | OCURV Expr CCURV                                                      {;}
    | BOOLLIT                                                               {;}
    | DECLIT                                                                {;}
    | REALLIT                                                               {;}
    | OCURV error CCURV                                                     {;}
    ;


%%
//A main está no lex
    