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
				"Assign",		"Call", 		"ParseArgs", "Or", 			"And",   			"Eq", 				"Neq",
				"Lt", 			"Leq", 	 		"Geq", 		"Add", 		   	"Mul", 		 		"Div", 				"Mod",
				"Not",   		"Minus", 		"Plus", 	"Length", 		"Bool", 	 		"BoolLit", 	 		"Double",
				"DecLit", 		"Id", 			"Int", 		"RealLit",   	"StrLit",			"Void", 			"Null",
				"StringArray", 	"Sub", 			"Gt"
};




/* META 3  estruturas auxiliares*/
	typedef enum{_boolean_,_int_,_StringArray_,_String_,_Method_,_Class_,_return_,_param_,_void_,_erro_,_null_,_Double_,_undef_}symbol;
//TODO possivelmente corrigir estes nomes e na função insereOuter
char* tabelaTipos[] ={ "boolean","int","String[]","String","Method","Class","return","param","void","Erro","","double","undef"};
char* llvmTypes[] = {"i1","i32","i32 %argc, i8** %argv","","","","","","void","","","double",""};

typedef struct _ParamList ParamList;
struct _ParamList{
	symbol simbolo;
	ParamList* next;
};

typedef struct _info{
	int linha,coluna;
	char* token;
	symbol annotation;
	ParamList* params;
}Info;

/*alterado para meta 3*/
typedef struct _Node Node;
struct _Node{
	tag tipo;
	Info* token;
	symbol type;
	Node* filho;
	Node* irmao;
	char* result;
	int boolean;
};

typedef struct _table Table;
typedef struct _element Elemento;
struct _element{
	char* token; //Token
	symbol tType,tFlag; //Atributos
	ParamList* tParams;

	Elemento* next;
	Table* table;
};

//Tabela de simbolos
struct _table{
	char* idTable; //Id pelo qual é identificada a tabela, normalmente o nome da funcao

	symbol tableType;
	Elemento* simbolo; //Simbolos na tabela
	Table* parent;
	ParamList* params;
};


/* META 2  declaração funções*/

Node* createNode(tag tipo,Info* token,Node* filho,Node* irmao );
void joinIrmao(Node* eu, Node* irmao);
void printTree(Node* root,int altura);
void elimina(Node* root);
char* getTipo(tag etag);

/* META 3  declaração funções*/

Info* createInfo(int linha,int coluna,char* token);
char* getTipoTabela(symbol simbolo);
Table* createTable(char* id,symbol tableType,Table* parent, ParamList* params);
Elemento* createElement(char* token,ParamList* tParams, symbol tType,symbol tFlag);
void insertElement(Elemento* element,Table* inserir,Table* next);
Table* insereOuter();
Elemento* searchLocal(Table* tabela,char* aProcurar);
Elemento* searchGlobalID(Table* tabela,char* aProcurar);
void printTabela(Table* tabela);
void printSimbolos(Elemento* elemento);
void addAnnotation(Node* no, symbol annotation);
void printTreeAnnot(Node* root,int altura);
Elemento* searchMethod(Table* tabela,char* aProcurar, ParamList* params, int erros);
char* annotMethod(ParamList* params);
void checkArithmetic(Node* no);
void checkUnary(Node* no);
int compareParamList(ParamList* first, ParamList* second);
int compareExactParamList(ParamList* first, ParamList* second);
int calculateParamLength(ParamList* params);

/*META 4 declaração funções*/
char* getLLVMConstant(symbol simbolo);

/* META 2  funções*/
/*alterado para meta 3*/
Node* createNode(tag tipo,Info* token,Node* filho,Node* irmao ){
	Node* no = malloc(sizeof(Node));
	no->tipo = tipo;
	if(token != NULL)
		no->token = token;
	else
		no->token = NULL;
	no->irmao = irmao;
	no->filho = filho;
	no->type = _null_;
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
			printf("%s(%s)\n",getTipo(root->tipo),root->token->token );
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
Info* createInfo(int linha,int coluna,char* token){
	Info* info = malloc(sizeof(Info));

	info->coluna = coluna;
	info->linha = linha;
	if (token!=NULL)
		info->token = (char*) strdup(token);
	else
		info->token = NULL;
	info->annotation = _null_;
	info->params = NULL;

	return info;
}

/*função para inserir elementos*/
symbol getTipoInserirTabela(char* tipo){
	symbol simbolo;
	if (strcmp(tipo, "Int")==0 ){
		simbolo = _int_;
	}
	if (strcmp(tipo, "Bool")==0 ){
		simbolo = _boolean_;
	}
	if (strcmp(tipo, "Double")==0 ){
		simbolo = _Double_;
	}
	if (strcmp(tipo, "StringArray")==0 ){
		simbolo = _StringArray_;
	}
	if (strcmp(tipo, "String")==0 ){
		simbolo = _String_;
	}
	if (strcmp(tipo, "Void")==0 ){
		simbolo = _void_;
	}

	return simbolo;
}

char* getTipoTabela(symbol simbolo){
	return tabelaTipos[simbolo];
}

char* getLLVMConstant(symbol simbolo){
	return llvmTypes[simbolo];
}

Table* createTable(char* id,symbol tableType,Table* parent, ParamList* params){
	Table* table = malloc(sizeof(Table));
	table->idTable = strdup(id);
	table->tableType = tableType;
	table->parent = parent;
	table->params = params;
	table->simbolo = NULL;

	return table;
}

Elemento* createElement(char* token, ParamList* tParams, symbol tType,symbol tFlag){
	Elemento* element = malloc(sizeof(Elemento));
	element->token = strdup(token);
	element->tParams = tParams;
	element->tType = tType;
	element->tFlag = tFlag;
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
	Table* tabela =  malloc(sizeof(Table));
	return tabela;
}
/*percorre a local e compara com o elemento*/
Elemento* searchLocal(Table* tabela,char* aProcurar){
	if(tabela != NULL){
		Elemento* pElem = tabela->simbolo;
		char* aux = strdup(aProcurar);
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
		while(pElem != NULL){
			//printf("PROCURANDO VARIAVEL %s: Tabela: %s Elemento %s\n", aProcurar, tabela->idTable ,pElem->token);
			if(strcmp(aux,pElem->token) == 0 && pElem->tFlag != _return_ && pElem->tParams == NULL){
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
	ParamList* temp;
	if(tabela != NULL){
		if(tabela->tableType == _Class_){
			printf("===== Class %s Symbol Table =====", tabela -> idTable);
		}
		else if(tabela->tableType == _Method_){
			printf("\n");
			printf("\n===== Method %s(", tabela -> idTable);
			if (tabela->params != NULL){
				temp = tabela->params;
				printf("%s", getTipoTabela(temp->simbolo));
				while (temp->next != NULL){
					printf(",%s", getTipoTabela(temp->next->simbolo));
					temp = temp->next;
				}
			}
			printf(") Symbol Table =====");
		}
		printSimbolos(tabela->simbolo);
	}

}


void printSimbolos(Elemento* elemento){
	Elemento* auxiliar = elemento;
	ParamList* temp;

	while(elemento != NULL){
		printf("\n");
		printf("%s\t",elemento->token);
		if (elemento->tParams != NULL){
			temp = elemento->tParams;
			printf("(");
			printf("%s", getTipoTabela(temp->simbolo));
			while (temp->next != NULL){
				printf(",%s", getTipoTabela(temp->next->simbolo));
				temp = temp->next;
			}
			printf(")");
		}
		printf("\t%s",getTipoTabela(elemento->tType));
		if(elemento->tFlag != _null_){
			printf("\t%s",getTipoTabela(elemento->tFlag));
		}
		elemento = elemento->next;
	}

	while(auxiliar != NULL){
		printTabela(auxiliar->table);
		auxiliar = auxiliar->next;
	}
}



ParamList* createSymbolList(Node* MethodParams){
	ParamList* lista = malloc(sizeof(ParamList));
	ParamList* temp1 = lista;
	Node* temp = MethodParams;
	temp1->simbolo = _null_;
	while (temp != NULL){
		if (temp->filho->tipo != Null){
			temp1->simbolo = getTipoInserirTabela(getTipo(temp->filho->tipo));
			if (temp->irmao != NULL)
				temp1->next = malloc(sizeof(ParamList));
			temp1 = temp1->next;
			temp = temp->irmao;
		}
		else{
			temp1->simbolo = _null_;
		}
	}
	return lista;
}


void addAnnotation(Node* no, symbol annotation){
	if(no != NULL){
		if (no->token != NULL){
			no->token->annotation = annotation;
		}
	}
}

void printTreeAnnot(Node* root,int altura){
	int i;
	ParamList* temp;
	if(root != NULL){
		if(root->tipo == Id || root->tipo == BoolLit || root->tipo == DecLit || root->tipo == StrLit || root->tipo == RealLit){
			for(i=0; i < altura;i++){
				printf(".");
			}
			if (root->token == NULL)
				printf("%s(%s)\n",getTipo(root->tipo),root->token->token );
			else{
				if (root->token->params != NULL){
					if (root->token->params->simbolo != _null_ && root->token->params->simbolo != _void_){ //COMO SE DIFERENCIOU OS METODOS COM PARAMETROS
						printf("%s(%s) - (", getTipo(root->tipo),root->token->token); 					   //COM OS SEM PARAMETROS (VOID)
						temp = root->token->params;
						if (temp != NULL)
							printf("%s", getTipoTabela(temp->simbolo));
						while (temp->next != NULL){
							printf(",%s", getTipoTabela(temp->next->simbolo));
							temp = temp->next;
						}
						printf(")\n");
					}
					else if (root->token->params->simbolo == _void_) 									   //TEMOS QUE OS TRATAR DE FORMA DIFERENTE
						printf("%s(%s) - ()\n",getTipo(root->tipo),root->token->token);
					else{
						printf("%s(%s) - %s\n", getTipo(root->tipo),root->token->token, getTipoTabela(root->token->annotation));
					}
				}
				else if (root->token->annotation != _null_)
					printf("%s(%s) - %s\n",getTipo(root->tipo),root->token->token, getTipoTabela(root->token->annotation));
				else
					printf("%s(%s)\n",getTipo(root->tipo),root->token->token);
				/*if(root->token->linha > -1) 
					printf("linha: %d,coluna: %d\n", root->token->linha,root->token->coluna);*/
			}
		}else{
			if (root->tipo != Null){
				for(i=0; i < altura;i++){
					printf(".");
				}
				if (root->token == NULL)
					printf("%s\n",getTipo(root->tipo) );


				else{
					if (root->token->annotation != _null_)
						printf("%s - %s\n",getTipo(root->tipo), getTipoTabela(root->token->annotation));
					else
						printf("%s\n",getTipo(root->tipo));
					/*if(root->token->linha > -1) 
						printf("linha: %d,coluna: %d\n", root->token->linha,root->token->coluna);*/
				}
			}
		}

		printTreeAnnot(root->filho,altura+2);
		printTreeAnnot(root->irmao,altura);
	}
}

// retorno da funcao pElem->tType, lista com paramlist pElem->tParams
Elemento* searchMethod(Table* tabela,char* aProcurar, ParamList* params, int erros){
	Table* temp = tabela;
	while(temp != NULL){
		Elemento* pElem = temp->simbolo;
		char* aux = strdup(aProcurar);
		while(pElem != NULL){
			//printf("PROCURANDO METODO %s: Tabela: %s Elemento %s\n", aProcurar, tabela->idTable ,pElem->token);
			//if(strcmp(aux,pElem->token) == 0 && pElem->tParams==params){
			if(strcmp(aux,pElem->token) == 0 && compareExactParamList(pElem->tParams,params)){
				//printf("elemento: %s", pElem->token);
				return pElem;
			}
			pElem = pElem->next;
		}
		temp = temp-> parent;
	}
	if (erros > 0){
		while(tabela != NULL){
			Elemento* pElem = tabela->simbolo;
			char* aux = strdup(aProcurar);
			while(pElem != NULL){
				//printf("PROCURANDO METODO %s: Tabela: %s Elemento %s\n", aProcurar, tabela->idTable ,pElem->token);
				//if(strcmp(aux,pElem->token) == 0 && pElem->tParams==params){
				if(strcmp(aux,pElem->token) == 0 && compareParamList(pElem->tParams,params)){
					//printf("elemento: %s", pElem->token);
					return pElem;
				}
				pElem = pElem->next;
			}
			tabela = tabela-> parent;
		}
	}
	return NULL;
}

int calculateParamLength(ParamList* params){
	int i = 0;
	while (params != NULL){
		i++;
		params = params->next;
	}
	return i;
}

int compareParamList(ParamList* first, ParamList* second){
	int size_first = calculateParamLength(first);
	int size_second = calculateParamLength(second);
	if (size_first == 0)
		return 0;
	if (size_first != size_second)
		return 0;

	while(first != NULL){
		if (first->simbolo != second->simbolo)
			if (!(first->simbolo == _Double_ && second->simbolo == _int_))
				return 0;
		first = first->next;
		second = second->next;
	}

	return 1;
}

int compareExactParamList(ParamList* first, ParamList* second){
	int size_first = calculateParamLength(first);
	int size_second = calculateParamLength(second);
	ParamList* temp = first;
	ParamList* temp1 = second;
	if (size_first == 0)
		return 0;
	if (size_first != size_second)
		return 0;
	/*PROCURA PARAMETROS EXACTOS*/
	while(temp != NULL){
		if (temp->simbolo != temp1->simbolo)
			return 0;
		temp = temp->next;
		temp1 = temp1->next;
	}
	return 1; //ENCONTRA EXACTAMENTE OS PARAMETROS CORRECTOS
}


// ====================== FUNÇÕES DE CHECK PARA ERRORS E OUTRAS COISAS ==============================

Elemento* checkFieldDecl(Node* var, Table* tabela){
	return searchGlobalID(tabela,var->token->token);
}

Elemento* checkMethodDecl(char* nome, ParamList* params, Table* tabela){
	return searchMethod(tabela, nome, params,0);
}

Elemento* checkParamDecl(Table* tabela,Node* no){
	return searchLocal(tabela, no->token->token);
}

Elemento* checkVarDecl(Table* tabela,Node* no){
	return searchLocal(tabela, no->token->token);
}

void checkArithmetic(Node* no){
	Node* expr1 = no->filho;
	Node* expr2 = no->filho->irmao;
	//no->token = malloc(sizeof(Info));

	if (expr1->token->annotation == _int_){
		if (expr2->token->annotation == _int_)
			addAnnotation(no, _int_);
		else if (expr2->token->annotation == _Double_){
			addAnnotation(no, _Double_);
		}
	}
	else if (expr2->token->annotation == _int_){
		if (expr1->token->annotation == _int_)
			addAnnotation(no, _int_);
		else if (expr1->token->annotation == _Double_){
			addAnnotation(no, _Double_);
		}
	}
	else if (expr1->token->annotation == _Double_ && expr2->token->annotation == _Double_){
		addAnnotation(no, _Double_);
	}
}

void checkUnary(Node* no){
	Node* expr = no->filho;
	//no->token = malloc(sizeof(Info));

	if (expr->token->annotation == _int_)
		addAnnotation(no, _int_);
	else if (expr->token->annotation == _Double_)
		addAnnotation(no,_Double_);
}

void checkCall(Node* no, Table* tabela){
	if (no->token == NULL)
		no->token = malloc(sizeof(Info));
	no->filho->token->params = malloc(sizeof(ParamList));
	no->filho->token->params->simbolo = _null_;
	char* nome = no->filho->token->token;
	Node* temp = no->filho->irmao;
	ParamList* lista = malloc(sizeof(ParamList));
	ParamList* temp1 = lista;
	temp1->simbolo = _null_;
	while (temp != NULL){
		temp1->simbolo = temp->token->annotation;
		if (temp->irmao != NULL)
				temp1->next = malloc(sizeof(ParamList));
		temp1 = temp1->next;
		temp = temp->irmao;
	}
	//printf("FUNCAO A PESQUISAR: %s\n", nome);
	Elemento* search = searchMethod(tabela,nome,lista,1);
	ParamList* params = NULL;
				
	if (search != NULL){
		//printf("FUNCAO: %s\n", getTipoTabela(search->tType));
		params = search->tParams;
		if (params->simbolo != _null_) 							//ESTE IF-ELSE SERVE PARA DIFERENCIAR METODOS COM PARAMETROS
			no->filho->token->params = params;
		else
			no->filho->token->params->simbolo = _void_;			//COM METODOS SEM PARAMETROS (VOID)
		//no->token->params = params;
		//addAnnotation(no->filho, annotMethod(params));
		//no->token = malloc(sizeof(Info));
		addAnnotation(no, search->tType);
	}
	else{
		addAnnotation(no->filho, _undef_);
		//no->token = malloc(sizeof(Info));
		addAnnotation(no,_undef_);
	}
}

#endif
