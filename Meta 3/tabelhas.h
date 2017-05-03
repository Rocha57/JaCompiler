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
	ParamList* params;
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

				percorreAST(raiz->filho,tabela);
				percorreAST(raiz->irmao,tabela);
				break;
				/*temp = raiz->filho; //Entrar na declaracao de vars

				while(temp->irmao != NULL){ //avancar até à declaracao do tipo
					temp = temp->irmao;
				}
				tipoVar = temp->token->token; //Temos tipo da var
				nodeAux = temp;

				//Meter todos os ids excepto o ultimo na tabela atual
				//printf("Type found:%s\n",getTipoTabela(elementoDaPesquisa->tValue));
				temp = raiz->filho;

				elementoDaPesquisa = searchGlobalID(tabela,tipoVar);//Pesquisar se tipo existe

				while(temp->irmao != NULL){
					if(searchLocal(tabela,temp->token->token) != NULL){ //Ver se já existe nesta scope
						//errosS +=1;
						printf("Line %d, col %d: Symbol %s already defined\n",temp->token->linha,temp->token->coluna,temp->token->token);
						return; //Already defined
					}
					//Verificar se este tipo existe

					if(elementoDaPesquisa == NULL){
						printf("Line %d, col %d: Symbol %s not defined\n",nodeAux->token->linha,nodeAux->token->coluna,nodeAux->token->token );
						//errosS+=1;
						return;
					}

					if(elementoDaPesquisa->tType != _type_){ //Tipo nao declarado
						printf("Line %d, col %d: Type identifier expected\n",nodeAux->token->linha,nodeAux->token->coluna);
						//errosS +=1;
						return; //Type expected
					}


					//printf("%s\n",getTipoTabela(elementoDaPesquisa->tValue) );
					if(elementoDaPesquisa->tValue != _null_)
						insertElement(createElement(temp->token->token,NULL,elementoDaPesquisa->tValue,_null_),tabela,NULL);
					temp = temp->irmao;
				}
				percorreAST(raiz->filho,tabela);
				percorreAST(raiz->irmao,tabela);*/
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
				percorreAST(raiz->filho,tabela);
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
				percorreAST(raiz->filho,tabela);
				percorreAST(raiz->irmao,tabela);
			break;
			case	MethodBody: 
				percorreAST(raiz->filho,tabela);
				percorreAST(raiz->irmao,tabela);
			break;
			case	Assign:
				nome = raiz->filho->token->token;
				search = searchGlobalID(tabela, nome);
				if (search != NULL)
					addAnnotation(raiz->filho, getTipoTabela(search->tType));
				raiz->token = malloc(sizeof(Info));
				if (search != NULL)
					addAnnotation(raiz, getTipoTabela(search->tType));
				percorreAST(raiz->filho,tabela);
				percorreAST(raiz->irmao,tabela);
			break;
			case	Call:
				nome = raiz->filho->token->token;
				search = searchMethod(tabela, nome);
				params = search->tParams;
				
				addAnnotation(raiz->filho, annotMethod(params));
				raiz->token = malloc(sizeof(Info));
				addAnnotation(raiz, getTipoTabela(search->tType));
				temp = raiz->filho->irmao;
				while (temp != NULL){
					if (temp->tipo == Id){
						nome = temp->token->token;
						search = searchGlobalID(tabela, nome);
						addAnnotation(temp, getTipoTabela(search->tType));
					}
					temp = temp->irmao;
				}
				percorreAST(raiz->filho,tabela);
				percorreAST(raiz->irmao,tabela);
			break;
			case 	ParseArgs:
				raiz->token = malloc(sizeof(Info));
				addAnnotation(raiz,"int");
				nome = raiz->filho->token->token;
				search = searchGlobalID(tabela, nome);
				addAnnotation(raiz->filho, getTipoTabela(search->tType));
				percorreAST(raiz->filho,tabela);
				percorreAST(raiz->irmao,tabela);
			break;
			case 	Or:
			case 	And:
				// VERIFICAR TIPOS DOS FILHOS DESTES MENS
				raiz->token = malloc(sizeof(Info));
				addAnnotation(raiz,"boolean");
				percorreAST(raiz->filho,tabela);
				percorreAST(raiz->irmao,tabela);
			break;

			case 	Eq: 
			case	Neq: 
			case  	Lt: 
			case  	Leq: 
			case	Geq: 
			case	Gt: 
				// VERIFICAR TIPOS DOS FILHOS DESTES MENS
				raiz->token = malloc(sizeof(Info));
				addAnnotation(raiz,"boolean");
				temp = raiz->filho;
				while (temp != NULL){
					if (temp->tipo == Id){
						nome = temp->token->token;
						search = searchGlobalID(tabela, nome);
						if (search != NULL){
							printf("%s\n",search->token );
							addAnnotation(temp, getTipoTabela(search->tType));
						}
					}
					temp = temp->irmao;
				}
				percorreAST(raiz->filho,tabela);
				percorreAST(raiz->irmao,tabela);
			break;
			case 	Mul: 
				percorreAST(raiz->filho,tabela);
				percorreAST(raiz->irmao,tabela);
			break;
			case	Div: 
				percorreAST(raiz->filho,tabela);
				percorreAST(raiz->irmao,tabela);
			break;
			case	Mod:
				raiz->token = malloc(sizeof(Info));
				addAnnotation(raiz,"int");
				percorreAST(raiz->filho,tabela);
				percorreAST(raiz->irmao,tabela);
			break;
			case	Not: 
				raiz->token = malloc(sizeof(Info));
				addAnnotation(raiz,"boolean");
				percorreAST(raiz->filho,tabela);
				percorreAST(raiz->irmao,tabela);
			break;
			case	Minus: 
				percorreAST(raiz->filho,tabela);
				percorreAST(raiz->irmao,tabela);
			break;
			case 	Plus: 
				percorreAST(raiz->filho,tabela);
				percorreAST(raiz->irmao,tabela);
			break;
			case	Length: 
				raiz->token = malloc(sizeof(Info));
				addAnnotation(raiz,"int");
				nome = raiz->filho->token->token;
				search = searchGlobalID(tabela, nome);
				addAnnotation(raiz->filho, getTipoTabela(search->tType));
				percorreAST(raiz->filho,tabela);
				percorreAST(raiz->irmao,tabela);
			break;
			case 	BoolLit:
				addAnnotation(raiz,"boolean"); 
				percorreAST(raiz->filho,tabela);
				percorreAST(raiz->irmao,tabela);
			break;
			case	DecLit: 
				addAnnotation(raiz,"int");
				percorreAST(raiz->filho,tabela);
				percorreAST(raiz->irmao,tabela);
			break;
			case 	Id:
			/*
			elementoDaPesquisa = searchGlobalID(tabela,raiz->token->token);
			if(elementoDaPesquisa == NULL){
				printf("Line %d, col %d: Symbol %s not defined\n",raiz->token->linha,raiz->token->coluna,raiz->token->token);
				raiz->type = _erro_;
				return;
			}

			raiz->type = elementoDaPesquisa->tType;
			percorreAST(raiz->filho,tabela);
			percorreAST(raiz->irmao,tabela);*/
				percorreAST(raiz->filho,tabela);
				percorreAST(raiz->irmao,tabela);
			break;
			case	RealLit:
				addAnnotation(raiz,"double"); 
				percorreAST(raiz->filho,tabela);
				percorreAST(raiz->irmao,tabela);
			break;
			case 	StrLit: 
				addAnnotation(raiz,"String");
				percorreAST(raiz->filho,tabela);
				percorreAST(raiz->irmao,tabela);
			break;
			case  	Add: 
			case	Sub:
				temp = raiz->filho;
				while (temp != NULL){
					if (temp->tipo == Id){
						nome = temp->token->token;
						search = searchGlobalID(tabela, nome);
						if (search != NULL)
							addAnnotation(temp, getTipoTabela(search->tType));
					}
					temp = temp->irmao;
				}
				raiz->token = malloc(sizeof(Info));
				if (search != NULL)
					addAnnotation(raiz, getTipoTabela(search->tType));
				percorreAST(raiz->filho,tabela);
				percorreAST(raiz->irmao,tabela);
			break;
			case	Print:
				if (raiz->filho->tipo == Id){
					nome = raiz->filho->token->token;
					search = searchGlobalID(tabela, nome);
					if (search != NULL)
						addAnnotation(raiz->filho, getTipoTabela(search->tType));
				} 
				percorreAST(raiz->filho,tabela);
				percorreAST(raiz->irmao,tabela);
			break;
			case  	Return:
				if (raiz->filho != NULL){
					nome = raiz->filho->token->token;
					search = searchGlobalID(tabela, nome);
					if (search != NULL)
						addAnnotation(raiz->filho, getTipoTabela(search->tType));
				}
				percorreAST(raiz->filho,tabela);
				percorreAST(raiz->irmao,tabela);
				break; 
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
