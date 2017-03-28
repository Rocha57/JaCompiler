%{
    #include "structures.h"
    
    
    extern void yyerror( char *s);
    int yylex(void);
    extern Node* raiz;
    Node* temp;
    Node* temp1;
    int stats = 0;
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
%type <node> FieldIdList
%type <node> VarIdList
%type <node> Type
%type <node> Statement
%type <node> StatementList
%type <node> Assignment
%type <node> MethodInvocation
%type <node> ExprList
%type <node> ParseArgs
%type <node> Expr
%type <node> Expr2



%start Program
 
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

ProgramList: ProgramList FieldDecl                                          {joinIrmao($1,$2); $$ = $1;}

           | ProgramList MethodDecl                                         {joinIrmao($1,$2); $$ = $1;}

           | ProgramList SEMI                                               {$$ = $1;}

           | %empty                                                         {$$ = createNode(Null, NULL, NULL, NULL);}
           ;

FieldDecl: PUBLIC STATIC FieldIdList SEMI                                   {$$ = $3;}

         | error SEMI                                                       {$$ = createNode(Null, NULL,NULL,NULL);}
         ;

FieldIdList: FieldIdList COMMA ID                                           {temp = createNode(Id, $3, NULL,NULL); temp1 = createNode($1->filho->tipo, NULL, NULL, temp); joinIrmao($1,createNode(FieldDecl,NULL,temp1,NULL)); $$ = $1;}

      | Type ID                                                             {temp = createNode(Id, $2, NULL,NULL); joinIrmao($1, temp); $$ = createNode(FieldDecl, NULL, $1, NULL);}

MethodDecl: PUBLIC STATIC MethodHeader MethodBody                           {joinIrmao($3,$4); $$ = createNode(MethodDecl, NULL, $3, NULL);}

MethodHeader: Type ID OCURV CCURV                                           {joinIrmao($1,createNode(Id, $2, NULL, createNode(MethodParams, NULL,NULL,NULL))); 
                                                                            $$ = createNode(MethodHeader, NULL, $1,NULL);}

            | Type ID OCURV FormalParams CCURV                              {joinIrmao($1,createNode(Id, $2, NULL, createNode(MethodParams, NULL,$4,NULL))); 
                                                                            $$ = createNode(MethodHeader, NULL, $1,NULL);}

            | VOID ID OCURV CCURV                                           {temp = createNode(Void,NULL,NULL,NULL); 
                                                                            joinIrmao(temp,createNode(Id, $2, NULL, createNode(MethodParams, NULL,NULL,NULL))); 
                                                                            $$ = createNode(MethodHeader, NULL, temp,NULL);}

            | VOID ID OCURV FormalParams CCURV                              {temp = createNode(Void,NULL,NULL,NULL); 
                                                                            joinIrmao(temp,createNode(Id, $2, NULL, createNode(MethodParams, NULL,$4,NULL))); 
                                                                            $$ = createNode(MethodHeader, NULL, temp,NULL);}
            ;

MethodBody: OBRACE MethodBodyList CBRACE                                    {$$ = createNode(MethodBody, NULL, $2, NULL);}

MethodBodyList: MethodBodyList VarDecl                                      {joinIrmao($1,$2); $$ = $1;}

              | MethodBodyList Statement                                    {joinIrmao($1,$2); $$ = $1;}

              | %empty                                                      {$$ = createNode(Null,NULL,NULL,NULL);}
              ;

FormalParams: FormalParamsList                                              {$$ = $1;}

            | STRING OSQUARE CSQUARE ID                                     {$$ = createNode(ParamDecl, NULL, createNode(StringArray, NULL,NULL,createNode(Id, $4,NULL,NULL)), NULL);}
            ;

FormalParamsList: FormalParamsList COMMA Type ID                            {temp = createNode(Id, $4, NULL,NULL); joinIrmao($3,temp); joinIrmao($1,createNode(ParamDecl,NULL,$3,NULL)); $$ = $1;}

                | Type ID                                                   {temp = createNode(Id, $2, NULL,NULL); joinIrmao($1, temp); $$ = createNode(ParamDecl, NULL, $1, NULL);}
                ;

VarDecl: VarIdList SEMI                                                     {$$ = $1;}

VarIdList: VarIdList COMMA ID                                               {temp = createNode(Id, $3, NULL,NULL); temp1 = createNode($1->filho->tipo, NULL, NULL, temp); joinIrmao($1,createNode(VarDecl,NULL,temp1,NULL)); $$ = $1;}

      | Type ID                                                             {temp = createNode(Id, $2, NULL,NULL); joinIrmao($1, temp); $$ = createNode(VarDecl, NULL, $1, NULL);}
      ;

Type: BOOL                                                                  {$$ = createNode(Bool,NULL,NULL,NULL);}

    | INT                                                                   {$$ = createNode(Int,NULL,NULL,NULL);}

    | DOUBLE                                                                {$$ = createNode(Double,NULL,NULL,NULL);}
    ;

Statement: OBRACE StatementList CBRACE                                      {$$ = createBlock($2,0);}

         | IF OCURV Expr CCURV Statement ELSE Statement                     {$7 = createBlock($7,1); $5 = createBlock($5,1); 
                                                                            joinIrmao($5, $7); joinIrmao($3,$5); $$ = createNode(If,NULL,$3,NULL);}

         | IF OCURV Expr CCURV Statement %prec IFX                          {$5 = createBlock($5,1);
                                                                            joinIrmao($3,$5); joinIrmao($3, createNode(Block, NULL,NULL,NULL));
                                                                            $$ = createNode(If,NULL,$3,NULL);}

         | WHILE OCURV Expr CCURV Statement                                 {$5 = createBlock($5,1);
                                                                            joinIrmao($3,$5); $$ = createNode(While,NULL,$3,NULL);}

         | DO Statement WHILE OCURV Expr CCURV SEMI                         {$2 = createBlock($2,1);
                                                                            joinIrmao($2,$5); $$ = createNode(DoWhile,NULL,$2,NULL);}

         | PRINT OCURV Expr CCURV SEMI                                      {$$ = createNode(Print,NULL, $3,NULL);}

         | PRINT OCURV STRLIT CCURV SEMI                                    {$$ = createNode(Print, NULL, createNode(StrLit,$3,NULL,NULL), NULL);}

         | SEMI                                                             {$$ = createNode(Null,NULL,NULL,NULL);}

         | Assignment SEMI                                                  {$$ = $1;}

         | MethodInvocation SEMI                                            {$$ = $1;}

         | ParseArgs SEMI                                                   {$$ = $1;}

         | RETURN Expr SEMI                                                 {$$ = createNode(Return, NULL, $2, NULL);}

         | RETURN SEMI                                                      {$$ = createNode(Return, NULL, NULL, NULL);}

         | error SEMI                                                       {$$ = createNode(Null, NULL,NULL,NULL);}
         ;

StatementList: StatementList Statement                                      {joinIrmao($1,$2); $$ = $1;}

             | %empty                                                       {$$ = createNode(Null,NULL,NULL,NULL);}
             ;

Assignment: ID ASSIGN Expr                                                  {temp = createNode(Id, $1, NULL, $3); 
                                                                            $$ = createNode(Assign, NULL, temp, NULL);}

MethodInvocation: ID OCURV ExprList CCURV                                   {temp = createNode(Id, $1, NULL, $3); $$ = createNode(Call, NULL, temp, NULL);}

                | ID OCURV CCURV                                            {$$ = createNode(Call,NULL,createNode(Id,$1,NULL,NULL),NULL);}

                | ID OCURV error CCURV                                      {$$ = createNode(Null, NULL,NULL,NULL);}
                ;

ExprList: ExprList COMMA Expr                                               {joinIrmao($1,$3); $$ = $1;}

        | Expr                                                              {$$ = $1;}
        ;

ParseArgs: PARSEINT OCURV ID OSQUARE Expr CSQUARE CCURV                     {temp = createNode(Id, $3, NULL, $5); $$ = createNode(ParseArgs, NULL, temp, NULL);}

         | PARSEINT OCURV error CCURV                                       {$$ = createNode(Null, NULL,NULL,NULL);}
         ;

                                                                                        //Factorização do Assignment/Expression
Expr: Assignment                                                            {$$ = $1;}  //Precisamos de separar os assignments de todas as outras expressões de forma a dar maior precedencia ao assignment sobre as expressoões
    | Expr2                                                                 {$$ = $1;}  //Segundo as regras aprendidas nas primeiras aulas acerca da remoção de ambiguidade, criamos um novo símbolo não terminal para tratarmos essa precedência

Expr2: MethodInvocation                                                     {$$ = $1;}

    | ParseArgs                                                             {$$ = $1;}

    | Expr2 AND Expr2                                                       {joinIrmao($1,$3); $$ = createNode(And,NULL,$1,NULL);}

    | Expr2 OR Expr2                                                        {joinIrmao($1,$3); $$ = createNode(Or,NULL,$1,NULL);}

    | Expr2 EQ Expr2                                                        {joinIrmao($1,$3); $$ = createNode(Eq,NULL,$1,NULL);}

    | Expr2 GEQ Expr2                                                       {joinIrmao($1,$3); $$ = createNode(Geq,NULL,$1,NULL);}

    | Expr2 GT Expr2                                                        {joinIrmao($1,$3); $$ = createNode(Gt,NULL,$1,NULL);}

    | Expr2 LEQ Expr2                                                       {joinIrmao($1,$3); $$ = createNode(Leq,NULL,$1,NULL);}

    | Expr2 LT Expr2                                                        {joinIrmao($1,$3); $$ = createNode(Lt,NULL,$1,NULL);}

    | Expr2 NEQ Expr2                                                       {joinIrmao($1,$3); $$ = createNode(Neq,NULL,$1,NULL);}

    | Expr2 PLUS Expr2                                                      {joinIrmao($1,$3); $$ = createNode(Add,NULL,$1,NULL);}

    | Expr2 MINUS Expr2                                                     {joinIrmao($1,$3); $$ = createNode(Sub,NULL,$1,NULL);}

    | Expr2 STAR Expr2                                                      {joinIrmao($1,$3); $$ = createNode(Mul,NULL,$1,NULL);}

    | Expr2 DIV Expr2                                                       {joinIrmao($1,$3); $$ = createNode(Div,NULL,$1,NULL);}

    | Expr2 MOD Expr2                                                       {joinIrmao($1,$3); $$ = createNode(Mod,NULL,$1,NULL);}

    | PLUS Expr2  %prec NOT                                                 {$$ = createNode(Plus,NULL,$2,NULL);}

    | MINUS Expr2 %prec NOT                                                 {$$ = createNode(Minus,NULL,$2,NULL);}

    | NOT Expr2                                                             {$$ = createNode(Not,NULL,$2,NULL);}

    | ID DOTLENGTH                                                          {$$ = createNode(Length,NULL,createNode(Id, $1, NULL, NULL),NULL);}

    | ID                                                                    {$$ = createNode(Id,$1,NULL,NULL);}

    | OCURV Expr CCURV                                                      {$$ = $2;}

    | BOOLLIT                                                               {$$ = createNode(BoolLit,$1, NULL,NULL);}

    | DECLIT                                                                {$$ = createNode(DecLit,$1, NULL,NULL);}

    | REALLIT                                                               {$$ = createNode(RealLit,$1, NULL,NULL);}

    | OCURV error CCURV                                                     {$$ = createNode(Null, NULL,NULL,NULL);}
    ;


%%
//A main está no lex
    