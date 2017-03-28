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

Node* createNode(tag tipo,char* token,Node* filho,Node* irmao);
void joinIrmao(Node* eu, Node* irmao);
void printTree(Node* root,int altura);
void elimina(Node* root);
char* getTipo(tag etag);


Node* createNode(tag tipo,char* token,Node* filho,Node* irmao){
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

/**
	createBlock
	returns Node*
	receives Node* -> the node with the list of Statements
			 int = 1 -> if checking sons of If, While or DoWhile (Nodes that have a mandatory empty statement)
			 	 = 0 -> otherwise
**/

Node* createBlock(Node* eu, int statlist){
	int stats = 0;
	Node* temp = malloc(sizeof(Node));
	temp = eu;
	while(temp != NULL){
		if (temp->tipo != Null)
			stats++; 								//Count the list elements that are different from Null node
		temp = temp->irmao;
	}
	free(temp);
	if (stats > 1 && statlist == 0) 				//The list has at least 2 nodes with types different from Null
		return createNode(Block, NULL,eu,NULL); 	// create Block that is a parent of the list
	else if (stats == 0 && statlist == 1)  			//The list only has empty statements, and is mandatory to have one
		return createNode(Block, NULL,NULL,NULL); 	//create Block
	else
		return eu;
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