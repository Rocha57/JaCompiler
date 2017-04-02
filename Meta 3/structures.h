#ifndef HEADER_FILE
#define HEADER_FILE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/* META 2  estruturas auxiliares*/
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

/*alterado para meta 3*/
typedef struct _Node Node;
typedef struct _Node{
	tag tipo;
	Info* token;
	symbol type;
	Node* filho;
	Node* irmao;
}Node;


/* META 3  estruturas auxiliares*/
typedef enum{_boolean_,_integer_,_real_,_function_,_program_,_type_,_false_,_true_,
	_constant_,_return_,_param_,_varparam_,_erro_,_outer_,_null_}symbol;
//TODO possivelmente corrigir estes nomes e na função insereOuter
char* tabelaTipos[] ={ "_boolean_","_integer_","_real_","_function_","_program_","_type_","_false_","_true_",
	"constant","return","param","varparam","Erro","_outer_","null"};

typedef struct _info{
	int linha,coluna;
	char* token;
}Info;

typedef struct _table Table;
typedef struct _element Elemento;
typedef struct _element{
	char* token; //Token
	symbol tType,tFlag,tValue; //Atributos

	Elemento* next;
	Table* table;

}Elemento;

//Tabela de simbolos
typedef struct _table{
	char* idTable; //Id pelo qual é identificada a tabela, normalmente o nome da funcao

	symbol tableType;
	Elemento* simbolo; //Simbolos na tabela
	Table* parent;
}Table;


/* META 2  declaração funções*/

Node* createNode(tag tipo,char* token,Node* filho,Node* irmao);
void joinIrmao(Node* eu, Node* irmao);
void printTree(Node* root,int altura);
void elimina(Node* root);
char* getTipo(tag etag);

/* META 3  declaração funções*/
char* getTipoTabela(symbol simbolo);
Table* createTable(char* id,symbol tableType,Table* parent);
Elemento* createElement(char* token,symbol tType,symbol tFlag,symbol tValue);
void insertElement(Elemento* element,Table* inserir,Table* next);
Table* insereOuter();
Elemento* searchLocal(Table* tabela,char* aProcurar);
Elemento* searchGlobalID(Table* tabela,char* aProcurar);
void printTabela(Table* tabela);
void printSimbolos(Elemento* elemento);
void toSmall(char* string);


/* META 2  funções*/

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

/* META 3  funções*/

char* getTipoTabela(symbol simbolo){
	return tabelaTipos[simbolo];
}

Table* createTable(char* id,symbol tableType,Table* parent){
	Table* table = malloc(sizeof(Table));
	table->idTable = strdup(id);
	toSmall(table->idTable);
	table->tableType = tableType;
	table->parent = parent;
	table->simbolo = NULL;

	return table;
}

Elemento* createElement(char* token,symbol tType,symbol tFlag,symbol tValue){
	Elemento* element = malloc(sizeof(Elemento));
	element->token = strdup(token);
	toSmall(element->token);
	element->tType = tType;
	element->tFlag = tFlag;
	element->tValue = tValue;
	element->next = NULL;

	return element;
}

/*possivelmente mudar o nome do elemento de onde para atual ou algo que o valha*/
void insertElement(Elemento* element,Table* inserir,Table* next){
	Elemento* onde = inserir->simbolo;
	if(onde == NULL){
		inserir->simbolo = element;
	}
	else{
		while(onde->next != NULL){
			onde = onde->next;
		}
		onde->next = element;
	}
	element->table = next;
}

Table* insereOuter(){
	Table* tabela = createTable("_outer_",_outer_,NULL);
	insertElement(createElement("boolean",_type_,_constant_,_boolean_),tabela,NULL);
	insertElement(createElement("integer",_type_,_constant_,_integer_),tabela,NULL);
	insertElement(createElement("real",_type_,_constant_,_real_),tabela,NULL);
	insertElement(createElement("false",_boolean_,_constant_,_false_),tabela,NULL);
	insertElement(createElement("true",_boolean_,_constant_,_true_),tabela,NULL);

	Table* paramcountTable = createTable("paramcount",_function_,tabela);
	insertElement(createElement("paramcount",_integer_,_return_,_null_),paramcountTable,NULL);

	insertElement(createElement("paramcount",_function_,_null_,_null_),tabela,paramcountTable);

	return tabela;
}
/*percorre a local e compara com o elemento*/
Elemento* searchLocal(Table* tabela,char* aProcurar){
	if(tabela != NULL){
		Elemento* pElem = tabela->simbolo;
		char* aux = strdup(aProcurar);
		toSmall(aux);
		while(pElem != NULL){
			if(strcmp(pElem->token,aux) == 0){
				return pElem;
			}
			pElem = pElem->next;
		}
	}

	return NULL;
}

/*percorre a global e compara com o elemento (percorre das "folhas" até à raiz para verificar tudo)*/
Elemento* searchGlobalID(Table* tabela,char* aProcurar){

	if(tabela != NULL){
		Elemento* pElem = tabela->simbolo;
		char* aux = strdup(aProcurar);
		toSmall(aux);
		while(pElem != NULL){
			if(strcmp(aux,pElem->token) == 0 && pElem->tFlag != _return_){
				return pElem;
			}
			pElem = pElem->next;
		}
		return searchGlobalID(tabela->parent,aProcurar);
	}else{
		return NULL;
	}

}

void printTabela(Table* tabela){

	if(tabela != NULL){
		if(tabela->tableType == _outer_){
			printf("===== Outer Symbol Table =====");
		}else if(tabela->tableType == _function_){
			printf("\n");
			printf("\n===== Function Symbol Table =====");
		}else{
			printf("\n");
			printf("\n===== Program Symbol Table =====");
		}
		printSimbolos(tabela->simbolo);
	}

}

void printSimbolos(Elemento* elemento){
	Elemento* auxiliar = elemento;

	while(elemento != NULL){
		printf("\n");
		printf("%s\t%s",elemento->token,getTipoTabela(elemento->tType));
		if(elemento->tFlag != _null_){
			printf("\t%s",getTipoTabela(elemento->tFlag));

			if(elemento->tValue != _null_){
				printf("\t%s",getTipoTabela(elemento->tValue));
			}
		}
		elemento = elemento->next;
	}

	while(auxiliar != NULL){
		printTabela(auxiliar->table);
		auxiliar = auxiliar->next;
	}
}

/*teoricamente isto é uma mera função para meter todas as letras como minusculas, só não percebo o porquê de estar assim construida...*/
void toSmall(char* string){
    char* aux = string;

	while((long)*aux != (long)NULL){
		*aux = tolower((int)*aux);
		++aux;
	}
}





#endif
