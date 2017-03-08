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
				DecLit, 	Id, 		Int, 		RealLit,   		StrLit,			Void, 			Null
}tag;

char* tipos[] = {"Program", 	"FieldDecl",	"VarDecl",	"MethodDecl", 	"MethodHeader",		"MethodParams", 	"ParamDecl", 
				"MethodBody", 	"Block", 		"DoWhile",	"If",  	 		"Print", 			"Return",  			"While", 	 
				"Assign",		"Call", 		"ParseArgs","Or", 			"And",   			"Eq", 				"Neq",
				"Lt", 			"Leq", 	 		"Geq", 		"Add", 		   	"Mul", 		 		"Div", 				"Mod", 		
				"Not",   		"Minus", 		"Plus", 	"Length", 		"Bool", 	 		"BoolLit", 	 		"Double",	   
				"DecLit", 		"Id", 			"Int", 		"RealLit",   	"StrLit",			"Void", 			"Null"
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
		if(root->tipo == Id || root->tipo == IntLit || root->tipo == ChrLit || root->tipo == StrLit){
			for(i=0; i < altura;i++){
				printf(".");
			}
			printf("%s(%s)\n",getTipo(root->tipo),root->token );
		}else{
			for(i=0; i < altura;i++){
					printf(".");
			}
			printf("%s\n",getTipo(root->tipo) );
			if(root->irmao != NULL){
				printf("%s\n",getTipo(root->tipo) );
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




char* getTipo(tag etag){ //Vir aqui buscar token para imprimir
	return tipos[etag];
}


#endif