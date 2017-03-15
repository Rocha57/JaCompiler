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
void joinIrmao(Node* eu, Node* irmao);
void printTree(Node* root,int altura);
void elimina(Node* root);
char* getTipo(tag etag);


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
	if(eu == NULL)
		return;
	else{
		while(eu->irmao != NULL){
			eu= eu->irmao;
		}
		eu->irmao = irmao;
	}
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
			}
			
		}

		printTree(root->filho,altura+2);
		printTree(root->irmao,altura);
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

char* getTipo(tag etag){ //Vir aqui buscar token para imprimir
	return tipos[etag];
}


#endif