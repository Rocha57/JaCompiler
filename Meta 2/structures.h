#ifndef HEADER_FILE
#define HEADER_FILE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef enum{	Program, 	FieldDecl, 	VarDecl, 	MethodDecl, 	MethodHeader, 	MethodParams, 	ParamDecl, 
				MethodBody, Block, 		DoWhile,	If,  	 		Print, 			Return,  		While, 	 
				Assign,		Call, 		ParseArgs, 	Or, 			And,   			Eq, 			Neq,
				Lt, 		Leq, 	 	Geq, 		Add, 		   	Mul, 		 	Div, 			Mod, 		
				Not,   		Minus, 		Plus, 	 	Length, 		Bool, 	 		BoolLit, 	 	Double,	   
				DecLit, 	Id, 		Int, 		RealLit,   		StrLit,			Void, 			Null,
				StringArray,Sub, 		Gt
}tag;

char* tipos[] = {"Program", 	"FieldDecl",	"VarDecl",	"MethodDecl", 	"MethodHeader",		"MethodParams", 	"ParamDecl", 
				"MethodBody", 	"Block", 		"DoWhile",	"If",  	 		"Print", 			"Return",  			"While", 	 
				"Assign",		"Call", 		"ParseArgs","Or", 			"And",   			"Eq", 				"Neq",
				"Lt", 			"Leq", 	 		"Geq", 		"Add", 		   	"Mul", 		 		"Div", 				"Mod", 		
				"Not",   		"Minus", 		"Plus", 	"Length", 		"Bool", 	 		"BoolLit", 	 		"Double",	   
				"DecLit", 		"Id", 			"Int", 		"RealLit",   	"StrLit",			"Void", 			"Null",
				"StringArray", 	"Sub", 			"Gt"
};


typedef struct _Node Node;
struct _Node{
	tag tipo;
	char* token; 
	Node* filho;
	Node* irmao;
};

Node* createNode(tag tipo,char* token,Node* filho,Node* irmao );
void printTree(Node* root,int altura);
void printEspacos( int altura);
void elimina(Node* root);


char* getTipo(tag etag);
int countBro(Node* root);
void joinIrmao(Node* eu, Node* irmao);
Node* reverse(Node* root);

Node* createNode(tag tipo,char* token,Node* filho,Node* irmao ){
	Node* no = malloc(sizeof(Node));
	no->tipo = tipo;
	if(token != NULL)
		no->token = strdup(token);
	else
		no->token = NULL;
	no->irmao = irmao;
	no->filho = filho;
	return no;
}

void joinIrmao(Node* eu, Node* irmao){
	if(eu == NULL){
		/*printf("NULO");*/
	}
	if( eu != NULL){
		while(eu->irmao != NULL){
			eu= eu->irmao;
		}
		eu->irmao = irmao;
	}
}

Node* reverse(Node* root) {
  Node* new_root = 0;
  while (root) {
    Node* irmao = root->irmao;
    root->irmao = new_root;
    new_root = root;
    root = irmao;
  } 
  return new_root;
}

void printTree(Node* root,int altura){ 
	int i; 
	if(root != NULL){
		if(root->tipo == Id || root->tipo == BoolLit || root->tipo == DecLit || root->tipo == StrLit || root->tipo == RealLit){
			for(i=0; i < altura;i++){
				printf(".");
			}
			printf("%s(%s)\n",getTipo(root->tipo),root->token );
		}else{
			if (root->tipo != Null){
				for(i=0; i < altura;i++){
						printf(".");
				}
				printf("%s\n",getTipo(root->tipo) );
				/*if(root->irmao != NULL){
					printf("irmao: %s\n",getTipo(root->irmao->tipo) );
				}*/
			}
			
		}

	

		printTree(root->filho,altura+2);
		printTree(root->irmao,altura);
	}
}

void printEspacos(int altura){
	int i;
	
	for(i=0; i < altura;i++){
		printf("%c",'.');
	}
}


void elimina(Node* root){
	if(root == NULL)
		return;
	elimina(root->filho);
	elimina(root->irmao);

	free(root->token);
	if(root->filho != NULL)
		free(root->filho);
	if(root->irmao != NULL)
		free(root->irmao);
}

/*isto foi necessário o ano passado para fazer uma passagem estranha*/
void typeFunc(Node* typeSpec, Node* original){
	Node* aux = original;
	Node* aux2;
	while(aux!=NULL){
		aux2 = createNode(typeSpec->tipo,NULL,NULL,NULL);
		aux2->irmao = aux->filho;
		aux->filho = aux2;
		aux = aux->irmao;
	}
}

/* 1 das 500921873821 tentativas de arranjar os ciclos
Node* multipleDecl(int tipo, char* token, Node* IdList){
	Node* fD = NULL;
	if (IdList->irmao != NULL){
		do{	
			fD = createNode(FieldDecl,NULL, NULL,NULL);
			if (IdList->irmao->irmao != NULL){
				Node* fDBrother = createNode(FieldDecl,NULL, NULL,NULL);
				fD->irmao = fDBrother;
			}
			Node* type = createNode(tipo, token, NULL,NULL);
			fD->filho = type; 
			type->irmao = IdList->irmao;
			IdList = IdList->irmao;
			printf("%s\n", getTipo(fD->tipo));
			printf("%s\n", getTipo(fD->filho->tipo));
			printf("%s %s\n", getTipo(fD->filho->irmao->tipo), fD->filho->irmao->token);
		} while (IdList->irmao != NULL);
	}
	return fD;
}*/




char* getTipo(tag etag){ //Vir aqui buscar token para imprimir
	return tipos[etag];
}


#endif