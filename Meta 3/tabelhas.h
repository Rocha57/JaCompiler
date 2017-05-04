#include "structures.h"

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
	ParamList* params;
	unsigned long input_number;
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
				percorreAST(raiz->filho->irmao,tabela);
				break;
			
			case FieldDecl:
				temp = raiz->filho; //Entrar na declaracao de vars
				while(temp->irmao != NULL){ //avancar até à declaracao do token
					temp = temp->irmao;
				}
				if (checkFieldDecl(temp, tabela) == NULL)
					insertElement(createElement(temp->token->token,NULL,getTipoInserirTabela(getTipo(raiz->filho->tipo)),_null_),tabela,NULL);
				else
					printf("Line %d, col %d: Symbol %s already defined\n",temp->token->linha, temp->token->coluna, temp->token->token);

				percorreAST(raiz->irmao,tabela);
				break;
			case 	MethodDecl:
				//tabelaMethod = NULL;
				temp = raiz->filho->filho->irmao;
				nome = raiz -> filho -> filho -> irmao -> token -> token;
				params = createSymbolList(raiz->filho->filho->irmao->irmao->filho);
				if (checkMethodDecl(nome, params, tabela) == NULL){
					tabelaMethod = createTable(nome,_Method_,tabela, params);
					insertElement(createElement(nome,params,getTipoInserirTabela(getTipo(raiz->filho->filho->tipo)),_null_),tabela,tabelaMethod);
					insertElement(createElement("return",NULL,getTipoInserirTabela(getTipo(raiz->filho->filho->tipo)),_null_),tabelaMethod, NULL);
					percorreAST(raiz->filho,tabelaMethod);
				}
				else{
					tabelaMethod = createTable(nome,_erro_,tabela, params);
					percorreAST(raiz->filho,tabelaMethod);
					printf("Line %d, col %d: Symbol %s already defined\n",temp->token->linha, temp->token->coluna, temp->token->token);
				}

				//printTabela(tabelaMethod);
				//if (tabelaMethod != NULL)
				percorreAST(raiz->irmao,tabela);
				break;
			case 	MethodHeader:
				percorreAST(raiz->filho->irmao->irmao,tabela);
				percorreAST(raiz->irmao,tabela);
			break;
			case 	MethodParams:
				percorreAST(raiz->filho,tabela);
				percorreAST(raiz->irmao,tabela);
			break;
			case 	ParamDecl: 
				temp = raiz -> filho -> irmao;
				nome = raiz -> filho -> irmao -> token -> token;

				if(checkParamDecl(tabela,temp) == NULL){
					insertElement(createElement(nome,NULL, getTipoInserirTabela(getTipo(raiz->filho->tipo)),_param_),tabela, NULL);
				}
				else{
					printf("Line %d, col %d: Symbol %s already defined\n",temp->token->linha, temp->token->coluna, temp->token->token);
				}
				//printf("%s\n", nome);
				percorreAST(raiz->irmao,tabela);
			break;
			case	MethodBody: 
				percorreAST(raiz->filho,tabela);
				percorreAST(raiz->irmao,tabela);
			break;
			case VarDecl:
				temp = raiz->filho; //Entrar na declaracao de vars
				while(temp->irmao != NULL){ //avancar até à declaracao do token
					temp = temp->irmao;
				}

				if(checkVarDecl(tabela,temp) == NULL){
					insertElement(createElement(temp->token->token,NULL,getTipoInserirTabela(getTipo(raiz->filho->tipo)),_null_),tabela,NULL);					
				}
				else{
					printf("Line %d, col %d: Symbol %s already defined\n",temp->token->linha, temp->token->coluna, temp->token->token);
				}
				
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
			  	input_number = strtol(raiz->token->token,NULL,10);
				if(2147483647 < input_number){
					printf("Number %s out of bounds\n", raiz->token->token);
					addAnnotation(raiz, _undef_);
				}
				else{
					addAnnotation(raiz, _int_);
				}
				
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
