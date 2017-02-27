%{
    #include "structures.h"
    
    
    extern void yyerror( char *s);
    int yylex(void);
    int erro =0, primeira = 0, errosyn = 0;
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
 
Start:   FunctionDefinition  Start                       {/*temp = createNode(FuncDefinition,NULL,$1,NULL);*/temp=$1; joinIrmao(temp,$2); raiz = createNode(Program,NULL,temp,NULL);/*temp = joinIrmao(createNode(FuncDefinition,NULL,NULL,NULL),$2);raiz = createNode(Program,NULL,temp,NULL);*/}
    |    FunctionDeclaration Start                       {temp = $1; joinIrmao(temp,$2); raiz = createNode(Program,NULL,temp,NULL);}
    |    Declaration  Start                              {temp = $1; joinIrmao(temp,$2); raiz = createNode(Program,NULL,temp,NULL);}

    |   FunctionDefinition                              {if(raiz !=NULL){
                                                            raiz = createNode(Program,NULL,$1,NULL);
                                                        }
                                                         else{
                                                            $$=createNode(FuncDefinition,NULL,$1,NULL);
                                                         }
                                                        } 
    |   FunctionDeclaration                             {if(raiz !=NULL){
                                                            raiz = createNode(Program,NULL,$1,NULL);}
                                                         else{
                                                            $$=createNode(FuncDeclaration,NULL,$1,NULL);
                                                            }    
                                                         }
    |   Declaration                                     {if(raiz !=NULL){
                                                            raiz = createNode(Program,NULL,$1,NULL);
                                                        }
                                                         else{
                                                            $$=createNode(Declaration,NULL,$1,NULL);
                                                         }
                                                     }
    
 
/*não são filhas do function body mas sim irmas e aparecem primeiro    CONFIRMAR ESTA MERDA*/
FunctionDefinition: TypeSpec FunctionDeclarator LBRACE DeclarationList StatementList RBRACE             {
                                                                                                        $$ = $1;
                                                                                                        joinIrmao($1, $2);
                                                                                                        joinIrmao($4,$5);
                                                                                                        temp = createNode(FuncBody,NULL,$4,NULL);
                                                                                                        joinIrmao($2, temp);
                                                                                                    }
                                                                                                       
                                                                                                        


    |               TypeSpec FunctionDeclarator LBRACE DeclarationList RBRACE                           {
                                                                                                        $$ = $1;
                                                                                                        joinIrmao($1, $2);
                                                                                                        temp = createNode(FuncBody,NULL,$4,NULL);
                                                                                                        joinIrmao($2, temp);
                                                                                                        }

    |               TypeSpec FunctionDeclarator LBRACE StatementList RBRACE                             {
                                                                                                        $$ = $1;
                                                                                                        joinIrmao($1, $2);
                                                                                                        temp = createNode(FuncBody,NULL,$4,NULL);
                                                                                                        joinIrmao($2, temp);
                                                                                                    }
    |               TypeSpec FunctionDeclarator LBRACE RBRACE                                           { 
                                                                                                        $$ = $1;
                                                                                                        joinIrmao($1, $2);
                                                                                                        temp = createNode(FuncBody,NULL,NULL,NULL);
                                                                                                        joinIrmao($2, temp);
                                                                                                        }
    |               TypeSpec FunctionDeclarator LBRACE error RBRACE                                     {$$=NULL;errosyn = 1;}
    ;

 
FunctionDeclaration: TypeSpec FunctionDeclarator SEMI                                                   {                                                                                                        
                                                                                                        $$=$1;
                                                                                                        if($2!=NULL)
                                                                                                            {joinIrmao($$,$2);}
                                                                                                        
                                                                                                        }
                ;

                                                                                            /*o parameterlist já está bem*/
FunctionDeclarator: AstList ID LPAR ParameterList RPAR                                                  {
                                                                                                        if($1!=NULL){
                                                                                                            $$=$1;
                                                                                                            temp=createNode(Id,$2,NULL,NULL);
                                                                                                            joinIrmao($$,temp);
                                                                                                            temp1=createNode(ParamList,NULL,$4,NULL);
                                                                                                            joinIrmao($$,temp1);  
                                                                                                        }
                                                                                                        else{
                                                                                                            $$=createNode(Id,$2,NULL,NULL);
                                                                                                            temp1=createNode(ParamList,NULL,$4,NULL);
                                                                                                            joinIrmao($$,temp1); 
                                                                                                            }
                                                                                                        }
                ;
 
ParameterList: ParameterDeclaration CommaParameterDeclaration                                           {$$=createNode(ParamDeclaration,NULL,$1,NULL); if($2!=NULL){joinIrmao($$,$2);}}
            ;
 
CommaParameterDeclaration: CommaParameterDeclaration COMMA ParameterDeclaration                          {if($1!=NULL)
                                                                                                                {
                                                                                                                    $$=$1;
                                                                                                                temp = createNode(ParamDeclaration,NULL,$3,NULL);
                                                                                                                 joinIrmao($$,temp);
                                                                                                             }
                                                                                                             else
                                                                                                             {
                                                                                                                $$ = createNode(ParamDeclaration,NULL,$3,NULL);
                                                                                                             }
                                                                                                         }
    |                                                                                                   {$$ = NULL;}   
    ;
 
ParameterDeclaration: TypeSpec AstList ID                                                               {$$ = $1;
                                                                                                            if($2!=NULL)
                                                                                                                {
                                                                                                                    joinIrmao($$,$2);
                                                                                                                }
                                                                                                                temp = createNode(Id,$3,NULL,NULL);
                                                                                                                joinIrmao($$,temp);
                                                                                                        }
                
                |     TypeSpec AstList                                                                  {
                                                                                                        $$ = $1;
                                                                                                        if($2!=NULL)
                                                                                                        {
                                                                                                            joinIrmao($$,$2);
                                                                                                        }
                                                                                                    }
                ;

DeclarationList: DeclarationList Declaration                                                            {
                                                                                                            if($1!=NULL)
                                                                                                            {   $$ = $1;
                                                                                                                
                                                                                                                temp = createNode(Declaration,NULL,NULL,NULL);
                                                                                                                joinIrmao($$,temp);
                                                                                                                joinIrmao($$,$2->irmao);
                                                                                                                
                                                                                                            }
                                                                                                            else{
                                                                                                                $$ = createNode(Declaration,NULL,NULL,$2);
                                                                                                                /*joinIrmao($$,$2->irmao);
                                                                                                               */
                                                                                                            }
                                                                                                        }
                                                                                                            
    |            Declaration                                                                            {$$ = createNode(Declaration,NULL,NULL,$1);/*Confirmar se não é necessario criar no*/}
    ;


Declaration: TypeSpec CommaDeclarator SEMI                                                              {/*$$ = $2;
                                                                                                        $1->irmao=$2->filho;
                                                                                                        $$->filho=$1;*/
                                                                                                        $$ = $2;
                                                                                                        typeFunc($1,$2);

                                                                                                                                                                                                               /*if($3!=NULL)
                                                                                                         {
                                                                                                          joinIrmao($$,$3);  
                                                                                                         }*/
                                                                                                        }
    |        error SEMI                                                                                 {$$=NULL;errosyn = 1;}
    ;

StatementList: StatementList ERRStatement                                                               {$$ = $1;
                                                                                                            joinIrmao($$,$2);
                                                                                                                }
    |          Statement                                                                                {$$ = $1;}
    ;
 
CommaDeclarator: Declarator                                                                              {$$=$1;}
                |CommaDeclarator COMMA Declarator                                                        {if ($1 != NULL){
                                                                                                            $$=$1;
                                                                                                            joinIrmao($$,$3);
                                                                                                            
                                                                                                            }
                                                                                                         else{
                                                                                                            $$ = createNode(Declaration,NULL,NULL,$3);
                                                                                                         
                                                                                                         }
                                                                                                        }
    ;
 
TypeSpec: CHAR                                                                                          {$$ = createNode(Char,NULL,NULL,NULL);}
    |     INT                                                                                           {$$ = createNode(Int,NULL,NULL,NULL);}
    |     VOID                                                                                          {$$ = createNode(Void,NULL,NULL,NULL);}
    ;
 
Declarator: AstList ID LSQ INTLIT RSQ                                                                   {if ($1!=NULL){
                                                                                                            $$=$1;
                                                                                                            temp1=createNode(IntLit,$4,NULL,NULL);
                                                                                                                printf("CONA1\n");

                                                                                                            temp=createNode(Id,$2,NULL,temp1);
                                                                                                            joinIrmao($$,createNode(ArrayDeclaration,NULL,temp,NULL));
                                                                                                            }
                                                                                                         else{
                                                                                                            temp1=createNode(IntLit,$4,NULL,NULL);
                                                                                                                printf("CONA2\n");


                                                                                                            temp=createNode(Id,$2,NULL,temp1);
                                                                                                            $$=createNode(ArrayDeclaration,NULL,temp,NULL);

                                                                                                         }
                                                                                                        }
        |   AstList ID                                                                                   {
                                                                                                            if($1!=NULL)
                                                                                                            {
                                                                                                                $$ = $1;
                                                                                                                printf("CONA3\n");

                                                                                                                temp = createNode(Id,$2,NULL,NULL);
                                                                                                                joinIrmao($$,temp);
                                                                                                            }
                                                                                                            else
                                                                                                            {
                                                                                                                printf("CONA4\n");

                                                                                                                $$ = createNode(Id,$2,NULL,NULL); 
                                                                                                            }
                                                                                                        }
        ;
 /*confirmar */
    AstList:AstList AST                                                                                 {   temp=createNode(Pointer,NULL,NULL,NULL); 
                                                                                                            if( $1 != NULL){
                                                                                                                $$ = $1;
                                                                                                                joinIrmao($$,temp);
                                                                                                             }
                                                                                                             else{
                                                                                                                $$ = temp;
                                                                                                             }
                                                                                                             
                                                                                                        }
    |                                                                                                   {$$ = NULL;}
    ;
 
Statement: Expression SEMI                                      {$$ = $1;} 
    |      SEMI                                                 {$$ = NULL;}
    |      LBRACE InsideStatement RBRACE                        {$$ = $2;}
    |      LBRACE RBRACE                                        {$$ = NULL;}

    |      IF LPAR Expression RPAR Statement ELSE Statement     {

                                                                joinIrmao($5,$7);
                                                                joinIrmao($3,$5);
                                                                $$=createNode(If,NULL,$3,NULL);


                                                                }
    |      IF LPAR Expression RPAR Statement                    {
                                                                joinIrmao($3,$5);
                                                                $$=createNode(If,NULL,$3,NULL);
                                                                }
    /*confirmar*/
    |      FOR LPAR ForList RPAR Statement                      {   joinIrmao($3,$5);
                                                                    temp=createNode(For,NULL,$3,NULL);
                                                                    $$ = createNode(StatList,NULL,temp,NULL);
}
    |      RETURN Expression SEMI                               {$$ = createNode(Return,NULL,$2,NULL);

                                                                }
    |      RETURN SEMI                                          {$$ = createNode(Return,NULL,NULL,NULL);}
    |      LBRACE error RBRACE                                  {$$=NULL;errosyn = 1;}
    ;

ERRStatement: error SEMI                                        {$$=NULL;errosyn = 1;}
    |         Statement                                         {$$ = $1;}
    ;

InsideStatement: InsideStatement ERRStatement                   {

                                                                 if( $2 != NULL){
                                                                        $$ = $1;
                                                                        joinIrmao($$,$2);
                                                                    }
                                                                    else{
                                                                        $$ = $1;
                                                                    }
                                                                }

    |            ERRStatement                                   {$$ = $1;}
    ;

ForList: Expression SEMI Expression SEMI Expression             {
                                                                    $$=$1; 
                                                                    joinIrmao($$,$3);
                                                                    joinIrmao($$,$5); 
                                                                
                                                                }

    |    SEMI Expression SEMI Expression                        {joinIrmao($2,$4);
                                                                $$=createNode(Null,NULL,NULL,$2);


                                                                }
    |    Expression SEMI SEMI Expression                        {
                                                                    joinIrmao($1,createNode(Null,NULL,NULL,$4));
                                                                    $$ = $1;
                                                                }
    
    |    Expression SEMI Expression SEMI                        {   $$=$1;
                                                                    joinIrmao($$,$3);
                                                                    temp = createNode(Null,NULL,NULL,NULL);
                                                                    joinIrmao($$,temp);
                                                                }
    
    |    Expression SEMI SEMI                                   {$$ = $1;
                                                                temp = createNode(Null,NULL,NULL,NULL);
                                                                temp1 = createNode(Null,NULL,NULL,NULL);
                                                                joinIrmao(temp,temp1);
                                                                joinIrmao($$,temp);
                                                                }

    |    SEMI Expression SEMI                                   {
                                                                 
                                                                $$ = createNode(Null,NULL,NULL,$2);
                                                                temp = createNode(Null,NULL,NULL,NULL);
                                                                joinIrmao($$,temp);                  
                                                                }

    |    SEMI SEMI Expression                                   {
                                                                    $$ = createNode(Null,NULL,NULL,NULL);
                                                                    joinIrmao($$,createNode(Null,NULL,NULL,$3));

                                                                }
    |    SEMI SEMI                                              {
                                                                    $$ = createNode(Null,NULL,NULL,createNode(Null,NULL,NULL,createNode(Null,NULL,NULL,NULL)));
                                                                }
    ;   


Expression: ExpressionCall                                      {$$ = $1;}
/*nao deve ser necessario verificar o nulo*/
    |       Expression COMMA ExpressionCall                     {if( $1 != NULL){
                                                                    $$ = $1;
                                                                    joinIrmao($$,$3);
                                                                 }
                                                                 else{
                                                                    $$ = $3;
                                                                 }
                                                                }
    

/*a = a - b 
a = irmaos

*/
ExpressionCall: ExpressionCall ASSIGN ExpressionCall            {
                                                                joinIrmao($1,$3);
                                                                $$ = createNode(Store,NULL,$1,NULL);}
    
    |           ExpressionCall AND ExpressionCall               {joinIrmao($1,$3);
                                                                    $$ = createNode(And,NULL,$1,NULL);}
    
    |           ExpressionCall OR ExpressionCall                {joinIrmao($1,$3);
                                                                    $$ = createNode(Or,NULL,$1,NULL);
                                                                }
    |           ExpressionCall EQ ExpressionCall                {
                                                                    joinIrmao($1,$3);
                                                                    $$ = createNode(Eq,NULL,$1,NULL);
                                                                }            
    
    |           ExpressionCall NE ExpressionCall                {joinIrmao($1,$3);
                                                                $$ = createNode(Ne,NULL,$1,NULL);}            
    
    |           ExpressionCall LT ExpressionCall                {joinIrmao($1,$3);
                                                                $$ = createNode(Lt,NULL,$1,NULL);}                
    
    |           ExpressionCall GT ExpressionCall                {joinIrmao($1,$3);
                                                                    $$ = createNode(Gt,NULL,$1,NULL);}
    
    |           ExpressionCall LE ExpressionCall                {joinIrmao($1,$3);
                                                                    $$ = createNode(Le,NULL,$1,NULL);}
    
    |           ExpressionCall GE ExpressionCall                {joinIrmao($1,$3);
                                                                    $$ = createNode(Ge,NULL,$1,NULL);}
    
    |           ExpressionCall PLUS ExpressionCall              {
                                                                    joinIrmao($1,$3);
                                                                    $$ = createNode(Add,NULL,$1,NULL);}

    |           ExpressionCall MINUS ExpressionCall             {joinIrmao($1,$3);
                                                                    $$ = createNode(Sub,NULL,$1,NULL);}
    
    |           ExpressionCall AST ExpressionCall               {joinIrmao($1,$3);
                                                                    $$ = createNode(Mul,NULL,$1,NULL);}        
    
    |           ExpressionCall DIV ExpressionCall               {joinIrmao($1,$3);
                                                                    $$ = createNode(Div,NULL,$1,NULL);}
    
    |           ExpressionCall MOD ExpressionCall               {joinIrmao($1,$3);
                                                                    $$ = createNode(Mod,NULL,$1,NULL);}

/*
CONFIRMAR SE E FILHO OU IRMAO 
*/
    |           AMP ExpressionCall                              {$$ = createNode(Addr,NULL,$2,NULL);}
    |           AST  ExpressionCall                             {$$ = createNode(Pointer,NULL,$2,NULL);}    
    |           PLUS ExpressionCall                             {$$ = createNode(Add,NULL,$2,NULL);}
    |           MINUS ExpressionCall                            {$$ = createNode(Sub,NULL,$2,NULL);}
    |           NOT ExpressionCall                              {$$ = createNode(Not,NULL,$2,NULL);} 

    |           ExpressionCall LSQ Expression RSQ               {   
                                                                        temp = createNode(Id,$1->token,NULL,$3);
                                                                    $$ = createNode(Deref,NULL,createNode(Add,NULL,temp,NULL),NULL);}
    /*Confirmar*/            
    |           ID LPAR ExpressionCall CommaExpression RPAR     {       if($4!=NULL){
                                                                            joinIrmao($3,$4);
                                                                        }
                                                                        temp=createNode(Id,$1,NULL,$3);
                                                                    $$=createNode(Call,NULL,temp,NULL);
                                                                }


    |           ID LPAR RPAR                                    {$$ = createNode(Call,NULL,createNode(Id,$1,NULL,NULL),NULL);}                  
    |           ID                                              {$$ = createNode(Id,$1,NULL,NULL);}
    |           INTLIT                                          {$$ = createNode(IntLit,$1,NULL,NULL);}
    |           CHRLIT                                          {$$ = createNode(ChrLit,$1,NULL,NULL);}            
    |           STRLIT                                          {$$ = createNode(StrLit,$1,NULL,NULL);}
    |           LPAR Expression RPAR                            {$$ = $2;}        
    |           ID LPAR error RPAR                              {$$=NULL;errosyn = 1;}
    |           LPAR error RPAR                                 {$$=NULL;errosyn = 1;}    
    ;
 
CommaExpression:CommaExpression COMMA Expression                {if( $1 != NULL){
                                                                    $$ = $1;
                                                                    joinIrmao($$,$3);
                                                                 }
                                                                 else{
                                                                    $$ = $3;
                                                                 }
                                                                }
    |                                                           {$$ = NULL;}
    ;

%%
int main(int argc, char** argv){
    if(argc>1){
        if(strcmp(argv[1],"-1")==0){
            primeira=1;
            erro = 0;
            yylex();
        }
        if(strcmp(argv[1],"-l")==0){
            primeira=1;
            erro = 1;
            yylex();
        }
        if( strcmp(argv[1],"-t") == 0 && erro == 0 && primeira == 0){
            yyparse(); 
            if(errosyn == 0){
                printTree(raiz,0);
            }
        }
            
    }
    else{
       primeira=0;
       erro = 0;
       yyparse(); 
    }
    return 0;
}
    