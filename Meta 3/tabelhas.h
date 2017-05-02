#include "structures.h"

/*Isto vai ser a variavel que vai ser usada na main para os erros semanticos*/
extern int errosS;



void percorreAST(Node* raiz, Table* tabela){
	Table* tabelaMethod;
	//char* tipoVar;
	char* nome;
	Node* temp;
	/*Elemento* elementoDaPesquisa;
	Elemento* assignTwo;
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
				//nome = raiz -> filho -> filho -> irmao -> token -> token;
				//tipoVar = getTipo(raiz->filho->filho->irmao->irmao->filho->filho->tipo);
				//printf("test:  %s\n",getTipo(raiz->filho ->filho->irmao->irmao->tipo));
				//printf("bananan  %s\n",getTipoTabela(getTipoInserirTabela(getTipo(raiz->filho->filho->tipo)) ));
				//getTipoInserirTabela(tipoVar);
				//quando criamos a tabela temos logo de inserir nó, caso contrário dá bode
				//printf("PARAMS: %s\n", getTipo(raiz->filho ->filho->irmao->irmao->filho->tipo));
				/*int i = 0;
				symbol symbols[5];
				temp = raiz->filho->filho->irmao->irmao->filho;
				while (temp != NULL){
					symbols[i] = getTipoInserirTabela(getTipo(temp->filho->tipo));
					i++;
					temp = temp->irmao;
				}
				while (i >0){
					printf("%d: %s\n", i-1, getTipoTabela(symbols[i-1]));
					i--;
				}*/
				nome = raiz -> filho -> filho -> irmao -> token -> token;
				tabelaMethod = createTable(nome,_Method_,tabela, createSymbolList(raiz->filho->filho->irmao->irmao->filho));
				insertElement(createElement(nome,createSymbolList(raiz->filho->filho->irmao->irmao->filho),getTipoInserirTabela(getTipo(raiz->filho->filho->tipo)),_null_),tabela,tabelaMethod);

				//printTabela(tabelaMethod);
				percorreAST(raiz->filho,tabelaMethod);
				percorreAST(raiz->irmao,tabela);
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
			case  Block: 
				percorreAST(raiz->filho,tabela);
				percorreAST(raiz->irmao,tabela);
			break;
			case 	DoWhile: 
				percorreAST(raiz->filho,tabela);
				percorreAST(raiz->irmao,tabela);
			break;
			case	If: 
				percorreAST(raiz->filho,tabela);
				percorreAST(raiz->irmao,tabela);
			break;
			case	Print: 
				percorreAST(raiz->filho,tabela);
				percorreAST(raiz->irmao,tabela);
			break;
			case  Return: 
				percorreAST(raiz->filho,tabela);
				percorreAST(raiz->irmao,tabela);
			break;
			case	While: 
				percorreAST(raiz->filho,tabela);
				percorreAST(raiz->irmao,tabela);
			break;
			case	Assign: 
				percorreAST(raiz->filho,tabela);
				percorreAST(raiz->irmao,tabela);
			break;
			case	Call: 
				percorreAST(raiz->filho,tabela);
				percorreAST(raiz->irmao,tabela);
			break;
			case 	ParseArgs: 
				percorreAST(raiz->filho,tabela);
				percorreAST(raiz->irmao,tabela);
			break;
			case 	Or: 
				percorreAST(raiz->filho,tabela);
				percorreAST(raiz->irmao,tabela);
			break;
			case 	And: 
				percorreAST(raiz->filho,tabela);
				percorreAST(raiz->irmao,tabela);
			break;
			case 	Eq: 
				percorreAST(raiz->filho,tabela);
				percorreAST(raiz->irmao,tabela);
			break;
			case	Neq: 
				percorreAST(raiz->filho,tabela);
				percorreAST(raiz->irmao,tabela);
			break;
			case  Lt: 
				percorreAST(raiz->filho,tabela);
				percorreAST(raiz->irmao,tabela);
			break;
			case  Leq: 
				percorreAST(raiz->filho,tabela);
				percorreAST(raiz->irmao,tabela);
			break;
			case	Geq: 
				percorreAST(raiz->filho,tabela);
				percorreAST(raiz->irmao,tabela);
			break;
			case  Add: 
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
				percorreAST(raiz->filho,tabela);
				percorreAST(raiz->irmao,tabela);
			break;
			case	Not: 
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
				percorreAST(raiz->filho,tabela);
				percorreAST(raiz->irmao,tabela);
			break;
			case	Bool: 
				percorreAST(raiz->filho,tabela);
				percorreAST(raiz->irmao,tabela);
			break;
			case 	BoolLit: 
				percorreAST(raiz->filho,tabela);
				percorreAST(raiz->irmao,tabela);
			break;
			case 	Double: 
				percorreAST(raiz->filho,tabela);
				percorreAST(raiz->irmao,tabela);
			break;
			case	DecLit: 
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
			case 	Int: 
				percorreAST(raiz->filho,tabela);
				percorreAST(raiz->irmao,tabela);
			break;
			case	RealLit: 
				percorreAST(raiz->filho,tabela);
				percorreAST(raiz->irmao,tabela);
			break;
			case 	StrLit: 
				percorreAST(raiz->filho,tabela);
				percorreAST(raiz->irmao,tabela);
			break;
			case	Void: 
				percorreAST(raiz->filho,tabela);
				percorreAST(raiz->irmao,tabela);
			break;
			case 	Null:
				percorreAST(raiz->filho,tabela);
				percorreAST(raiz->irmao,tabela);

			 break;
			case	StringArray: 
				percorreAST(raiz->filho,tabela);
				percorreAST(raiz->irmao,tabela);
			break;
			case	Sub: 
				percorreAST(raiz->filho,tabela);
				percorreAST(raiz->irmao,tabela);
			break;
			case	Gt: 
				percorreAST(raiz->filho,tabela);
				percorreAST(raiz->irmao,tabela);
			break;
			default:
				percorreAST(raiz->filho,tabela);
				percorreAST(raiz->irmao,tabela);
				break;
		}


	}
}
