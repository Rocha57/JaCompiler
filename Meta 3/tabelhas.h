#include "structures.h"

/*Isto vai ser a variavel que vai ser usada na main para os erros semanticos*/
extern int errosS;


/*
*
*
*	VERIFICAR TIPOS DOS NOS PARA OS ERROS
*
*
*/


void percorreAST(Node* raiz, Table* tabela){
	Table* tabelaMethod;
	//char* tipoVar;
	char* nome;
	Node* temp;
	Elemento* search;
	/*Elemento* assignTwo;
	Elemento* elExp1;
	Elemento* elExp2;
	Node* nodeAux;
	Node* exp1;
	Node* exp2;
	int tipoElem;*/
	if (raiz != NULL){
		//printf("%s\n", getTipo(raiz->tipo));
		switch (raiz->tipo) {

			case 	Program:
				tabela->idTable = strdup(raiz->filho->token->token);
				tabela->tableType = _Class_;
				tabela->parent = NULL;
				tabela->simbolo = NULL;
				//teste inserir um elemento, funciona
				//insertElement(createElement(raiz->filho->token->token,_Class_,_null_,_null_),tabela,NULL);
				//PROBLEMA  este nó não devia ser Null
				percorreAST(raiz->filho->irmao,tabela);
				break;
			
			case FieldDecl:
				temp = raiz->filho; //Entrar na declaracao de vars
				while(temp->irmao != NULL){ //avancar até à declaracao do token
					temp = temp->irmao;
				}
				//nodeAux = temp;
				insertElement(createElement(temp->token->token,NULL,getTipoInserirTabela(getTipo(raiz->filho->tipo)),_null_),tabela,NULL);

				percorreAST(raiz->irmao,tabela);
				break;

//FALTA ESTE
			case VarDecl:
				temp = raiz->filho; //Entrar na declaracao de vars
				while(temp->irmao != NULL){ //avancar até à declaracao do token
					temp = temp->irmao;
				}
				//nodeAux = temp;
				insertElement(createElement(temp->token->token,NULL,getTipoInserirTabela(getTipo(raiz->filho->tipo)),_null_),tabela,NULL);

				percorreAST(raiz->irmao,tabela);
				break;
			case 	MethodDecl:
				nome = raiz -> filho -> filho -> irmao -> token -> token;
				tabelaMethod = createTable(nome,_Method_,tabela, createSymbolList(raiz->filho->filho->irmao->irmao->filho));
				insertElement(createElement(nome,createSymbolList(raiz->filho->filho->irmao->irmao->filho),getTipoInserirTabela(getTipo(raiz->filho->filho->tipo)),_null_),tabela,tabelaMethod);

				//printTabela(tabelaMethod);
				percorreAST(raiz->irmao,tabela);
				percorreAST(raiz->filho,tabelaMethod);
				break;
			case 	MethodHeader:
				insertElement(createElement("return",NULL,getTipoInserirTabela(getTipo(raiz->filho->tipo)),_null_),tabela, NULL);
				percorreAST(raiz->filho->irmao->irmao,tabela);
				percorreAST(raiz->irmao,tabela);
			break;
			case 	MethodParams:
				percorreAST(raiz->filho,tabela);
				percorreAST(raiz->irmao,tabela);
			break;
			case 	ParamDecl: 
				nome = raiz -> filho -> irmao -> token -> token;
				//printf("%s\n", nome);
				insertElement(createElement(nome,NULL, getTipoInserirTabela(getTipo(raiz->filho->tipo)),_param_),tabela, NULL);
				percorreAST(raiz->irmao,tabela);
			break;
			case	MethodBody: 
				percorreAST(raiz->filho,tabela);
				percorreAST(raiz->irmao,tabela);
			break;
			case	Assign:
				percorreAST(raiz->filho,tabela);
				//raiz->token = malloc(sizeof(Info));
				addAnnotation(raiz, raiz->filho->token->annotation);
				percorreAST(raiz->irmao,tabela);
			break;
			case	Call:
				percorreAST(raiz->filho->irmao,tabela);
				checkCall(raiz, tabela);
				percorreAST(raiz->irmao,tabela);
			break;
			case 	ParseArgs:
				percorreAST(raiz->filho,tabela);
				//raiz->token = malloc(sizeof(Info));
				addAnnotation(raiz,_int_);
				percorreAST(raiz->irmao,tabela);
			break;
			case 	Or:
			case 	And:
				// VERIFICAR TIPOS DOS FILHOS DESTES MENS
				percorreAST(raiz->filho,tabela);
				//raiz->token = malloc(sizeof(Info));
				addAnnotation(raiz,_boolean_);
				percorreAST(raiz->irmao,tabela);
			break;
			case 	Eq: 
			case	Neq: 
			case  	Lt: 
			case  	Leq: 
			case	Geq: 
			case	Gt: 
				// VERIFICAR TIPOS DOS FILHOS DESTES MENS
				percorreAST(raiz->filho,tabela);
				//raiz->token = malloc(sizeof(Info));
				addAnnotation(raiz,_boolean_);
				percorreAST(raiz->irmao,tabela);
			break;
			case	Not: 
				//raiz->token = malloc(sizeof(Info));
				addAnnotation(raiz,_boolean_);
				percorreAST(raiz->filho,tabela);
				percorreAST(raiz->irmao,tabela);
			break;
			case	Minus: 
			case 	Plus: 
				percorreAST(raiz->filho,tabela);
				checkUnary(raiz);
				percorreAST(raiz->irmao,tabela);
			break;
			case	Length: 
				percorreAST(raiz->filho,tabela);
				//raiz->token = malloc(sizeof(Info));
				addAnnotation(raiz,_int_);
				percorreAST(raiz->irmao,tabela);
			break;
			case 	BoolLit:
				addAnnotation(raiz,_boolean_); 
				percorreAST(raiz->irmao,tabela);
			break;
			case	DecLit: 
				addAnnotation(raiz, _int_);
				percorreAST(raiz->irmao,tabela);
			break;
			case 	Id:
				nome = raiz->token->token;
				search = searchGlobalID(tabela, nome);
				if (search != NULL)
					addAnnotation(raiz, search->tType);
				else
					addAnnotation(raiz, _undef_);
				percorreAST(raiz->irmao,tabela);
			break;
			case	RealLit:
				addAnnotation(raiz,_Double_); 
				percorreAST(raiz->irmao,tabela);
			break;
			case 	StrLit: 
				addAnnotation(raiz,_String_);
				percorreAST(raiz->irmao,tabela);
			break;
			case	Mod:
			case 	Mul:
			case	Div: 
			case  	Add: 
			case	Sub:
				percorreAST(raiz->filho,tabela);
				checkArithmetic(raiz);
				percorreAST(raiz->irmao,tabela);
			break;
			case	Print:
			case  	Return:
			case	Bool: 
			case 	Double: 
			case	Void: 
			case	StringArray: 
			case 	Int: 
			case 	Null:
			case  	Block: 
			case 	DoWhile: 
			case	If: 
			case	While: 
			default:
				percorreAST(raiz->filho,tabela);
				percorreAST(raiz->irmao,tabela);
				break;
		}
	}
}
