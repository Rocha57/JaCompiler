#include "structures.h"

/*Isto vai ser a variavel que vai ser usada na main para os erros semanticos*/
extern int errosS;



void percorreAST(Node* raiz, Table* tabela){
	Table* tabelaClass;
	Table* tabelaMethod;
	char* tipoVar;
	char* nome;
	Node* temp;
	Elemento* elementoDaPesquisa;
	Elemento* assignTwo;
	Elemento* elExp1;
	Elemento* elExp2;
	Node* nodeAux;
	Node* exp1;
	Node* exp2;
	int tipoElem;
	if (raiz != NULL){
		printf("%s\n",getTipo(raiz->tipo) );
		switch (raiz->tipo) {

			case 	Program:
				strcpy(nome, "Class");
				strcat(nome,raiz->filho->token);

				tabelaClass = createTable(nome,_Class_,tabela);
				/*insertElement(createElement("Class",_Class_,_null_,_null_),tabela,tabelaProgram);
*/

				percorreAST(raiz->filho->irmao,tabelaClass);
				break;
			case FieldDecl:
				break;
			case VarDecl:
				break;
			case 	MethodDecl:
				break;
			case 	MethodHeader: break;
			case 	MethodParams: break;
			case 	ParamDecl: break;
			case	MethodBody: break;
			case  Block: break;
			case 	DoWhile: break;
			case	If: break;
			case	Print: break;
			case  Return: break;
			case	While: break;
			case	Assign: break;
			case	Call: break;
			case 	ParseArgs: break;
			case 	Or: break;
			case 	And: break;
			case 	Eq: break;
			case	Neq: break;
			case  Lt: break;
			case  Leq: break;
			case	Geq: break;
			case  Add: break;
			case 	Mul: break;
			case	Div: break;
			case	Mod: break;
			case	Not: break;
			case	Minus: break;
			case 	Plus: break;
			case	Length: break;
			case	Bool: break;
			case 	BoolLit: break;
			case 	Double: break;
			case	DecLit: break;
			case 	Id: break;
			case 	Int: break;
			case	RealLit: break;
			case 	StrLit: break;
			case	Void: break;
			case 	Null: break;
			case	StringArray: break;
			case	Sub: break;
			case	Gt: break;
			default:
				percorreAST(raiz->filho,tabela);
				percorreAST(raiz->irmao,tabela);
				break;
		}


	}


}


/*a meta 3 vai ser isto, em principio mais facil, mas isto
void percorreAST(Node* raiz,Table* tabela){
	Table* tabelaProgram;
	Table* tabelaFunc;
	char* tipoVar;
	char* nome;
	Node* temp;
	Elemento* elementoDaPesquisa;
	Elemento* assignTwo;
	Elemento* elExp1;
	Elemento* elExp2;
	Node* nodeAux;
	Node* exp1;
	Node* exp2;
	int tipoElem;

	if(raiz != NULL){
		if(errosS>0) return;
		//printf("%s\n",getTipo(raiz->tipo) ); //Print de debug para saber onde ele anda a passar,impresso antes da tabela

		switch(raiz->tipo){
			case tag_Program:
				tabelaProgram = createTable("program",_program_,tabela);
				insertElement(createElement("program",_program_,_null_,_null_),tabela,tabelaProgram);

				percorreAST(raiz->filho->irmao,tabelaProgram);
				break;

			case tag_VarDecl:
				temp = raiz->filho; //Entrar na declaracao de vars

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
						errosS +=1;
						printf("Line %d, col %d: Symbol %s already defined\n",temp->token->linha,temp->token->coluna,temp->token->token);
						return; //Already defined
					}
					//Verificar se este tipo existe

					if(elementoDaPesquisa == NULL){
						printf("Line %d, col %d: Symbol %s not defined\n",nodeAux->token->linha,nodeAux->token->coluna,nodeAux->token->token );
						errosS+=1;
						return;
					}

					if(elementoDaPesquisa->tType != _type_){ //Tipo nao declarado
						printf("Line %d, col %d: Type identifier expected\n",nodeAux->token->linha,nodeAux->token->coluna);
						errosS +=1;
						return; //Type expected
					}


					//printf("%s\n",getTipoTabela(elementoDaPesquisa->tValue) );
					if(elementoDaPesquisa->tValue != _null_)
						insertElement(createElement(temp->token->token,elementoDaPesquisa->tValue,_null_,_null_),tabela,NULL);
					temp = temp->irmao;
				}
				percorreAST(raiz->filho,tabela);
				percorreAST(raiz->irmao,tabela);
				break;

			case tag_FuncDef:
				temp= raiz->filho;
				nome = temp->token->token; //Get function ID

				//Perceber se tabela já existe
				elementoDaPesquisa = searchGlobalID(tabela,nome);
				if(elementoDaPesquisa == NULL)tabelaFunc = NULL;
				else tabelaFunc = elementoDaPesquisa->table;
				if(tabelaFunc != NULL){ //Mostra erro

				}

				//Criar ponteiro para a tabela desta funcao
				tabelaFunc = createTable(nome,_function_,tabela);
				insertElement(createElement(nome,_function_,_null_,_null_),tabela,tabelaFunc);

				//Sacar tipo da funcao
				temp = temp->irmao->irmao; //Saltar funcParams e buscar proximo ID
				tipoVar = temp->token->token; //Temos tipo da funcao
				nodeAux = temp;

				//Verificar se este tipo existe
				elementoDaPesquisa = searchGlobalID(tabela,tipoVar);

				insertElement(createElement(nome,elementoDaPesquisa->tValue,_return_,_null_),tabelaFunc,NULL);

				percorreAST(raiz->filho->irmao,tabelaFunc);//Go to Params/Var_Params

				if(errosS >0)return;
				if(elementoDaPesquisa == NULL){ //Tipo nao declarado
					printf("Line %d, col %d: Type identifier expected\n",temp->token->linha,temp->token->coluna);
					errosS +=1;
					return; //Type expected
				}
				if(elementoDaPesquisa->tType != _type_){ //Tipo nao declarado
					printf("Line %d, col %d: Type identifier expected\n",nodeAux->token->linha,nodeAux->token->coluna);
					errosS +=1;
					return; //Type expected
				}

				percorreAST(raiz->irmao,tabela); //Keep going in AST
				break;

			case  tag_FuncDecl:
				//printf("tou em %s com tabela\n",raiz->filho->token->token );
				//Get id funcao
				temp = raiz->filho;
				nome = temp->token->token; //Nome da funcao

				//Perceber se tabela já existe
				elementoDaPesquisa = searchGlobalID(tabela,nome);
				if(elementoDaPesquisa == NULL)tabelaFunc = NULL;
				else tabelaFunc = elementoDaPesquisa->table;
				if(tabelaFunc != NULL){ //Mostra erro

				}

				//Criar tabela da info disponivel da funcao
				tabelaFunc = createTable(nome,_function_,tabela);
				insertElement(createElement(nome,_function_,_null_,_null_),tabela,tabelaFunc);

				//Sacar tipo da funcao
				while(temp->irmao != NULL){temp = temp->irmao;}
				tipoVar = temp->token->token; //Temos tipo da funcao
				nodeAux = temp;

				//Verificar se este tipo existe
				elementoDaPesquisa = searchGlobalID(tabela,tipoVar);

				insertElement(createElement(nome,elementoDaPesquisa->tValue,_return_,_null_),tabelaFunc,NULL);

				percorreAST(raiz->filho->irmao,tabelaFunc);//Go to Params/Var_Params

				if(errosS >0)return;
				if(elementoDaPesquisa == NULL){ //Tipo nao declarado
					printf("Line %d, col %d: Type identifier expected\n",temp->token->linha,temp->token->coluna);
					errosS +=1;
					return; //Type expected
				}
				if(elementoDaPesquisa->tType != _type_){ //Tipo nao declarado
					printf("Line %d, col %d: Type identifier expected\n",nodeAux->token->linha,nodeAux->token->coluna);
					errosS +=1;
					return; //Type expected
				}

				percorreAST(raiz->irmao,tabela); //Keep going in AST
				break;

			case tag_FuncDef2:
				//Get id_tabela
				temp = raiz->filho;
				nome = temp->token->token;

				//Sacar tabela correspondente
				elementoDaPesquisa = searchGlobalID(tabela,nome);
				if(elementoDaPesquisa == NULL)tabelaFunc = NULL;
				else tabelaFunc = elementoDaPesquisa->table;

				if(tabelaFunc == NULL){ //Do something, table does not exist
					printf("Line %d, col %d: Function identifier expected\n", temp->token->linha,temp->token->coluna);
					return;
				}

				percorreAST(temp->irmao,tabelaFunc);
				percorreAST(raiz->irmao,tabela);

				break;

			case tag_Params:
				temp = raiz->filho;
				//Sacar tipo dos params
				for(temp=raiz->filho;temp->irmao!= NULL;temp=temp->irmao);
				tipoVar = temp->token->token; //Temos tipo da var
				nodeAux = temp;

				elementoDaPesquisa = searchGlobalID(tabela,tipoVar);//Verificar se tipo existe

				temp = raiz->filho;
				while(temp->irmao != NULL){ //Até chegar ao nó que contem o tipo é tudo argumentos
					if(searchLocal(tabela,temp->token->token) != NULL){ //Ver se já existe nesta scope
						errosS +=1;
						printf("Line %d, col %d: Symbol %s already defined\n",temp->token->linha,temp->token->coluna,temp->token->token);
						return; //Already defined
					}

					//Verificar se este tipo existe
					if(elementoDaPesquisa == NULL){ //Tipo nao declarado
						printf("Line %d, col %d: Type identifier expected\n",temp->token->linha,temp->token->coluna);
						errosS +=1;
						return; //Type expected
					}
					if(elementoDaPesquisa->tType != _type_){ //Tipo nao declarado
						printf("Line %d, col %d: Type identifier expected\n",nodeAux->token->linha,nodeAux->token->coluna);
						errosS +=1;
						return; //Type expected
					}

					insertElement(createElement(temp->token->token,elementoDaPesquisa->tValue,_param_,_null_),tabela,NULL);
					temp = temp->irmao;
				}

				percorreAST(raiz->irmao,tabela);
				break;

			case tag_VarParams: //Copy paste do acima(quase)

				temp = raiz->filho;
				//Sacar tipo dos params
				for(temp=raiz->filho;temp->irmao!= NULL;temp=temp->irmao);
				tipoVar = temp->token->token; //Temos tipo da var
				nodeAux = temp;

				elementoDaPesquisa = searchGlobalID(tabela,tipoVar);//Verificar se tipo existe

				temp = raiz->filho;
				while(temp->irmao != NULL){ //Até chegar ao nó que contem o tipo é tudo argumentos
					if(searchLocal(tabela,temp->token->token) != NULL){ //Ver se já existe nesta scope
						errosS +=1;
						printf("Line %d, col %d: Symbol %s already defined\n",temp->token->linha,temp->token->coluna,temp->token->token);
						return; //Already defined
					}

					//Verificar se este tipo existe
					if(elementoDaPesquisa == NULL){ //Tipo nao declarado
						printf("Line %d, col %d: Type identifier expected\n",temp->token->linha,temp->token->coluna);
						errosS +=1;
						return; //Type expected
					}

					if(elementoDaPesquisa->tType != _type_){ //Tipo nao declarado
						printf("Line %d, col %d: Type identifier expected\n",nodeAux->token->linha,nodeAux->token->coluna);
						errosS +=1;
						return; //Type expected
					}

					insertElement(createElement(temp->token->token,elementoDaPesquisa->tValue,_varparam_,_null_),tabela,NULL);
					temp = temp->irmao;
				}
				percorreAST(raiz->irmao,tabela);
				break;

//---------------------------STATMENTS e EXPR -------------------------------------------------------------------------------------------
			case tag_Assign:
				temp = raiz->filho;
				//Check if ID has been declared
				nome = raiz->filho->token->token;

				elementoDaPesquisa = searchGlobalID(tabela,nome);

				if(elementoDaPesquisa == NULL){ //Var not declared
					printf("Line %d, col %d: Symbol %s not defined\n",raiz->filho->token->linha,raiz->filho->token->coluna,nome);
					errosS +=1;
					raiz->type = _erro_;
					return;
				}
				if(elementoDaPesquisa->tType == _function_ || elementoDaPesquisa->tType == _program_ || elementoDaPesquisa->tFlag == _constant_){
					printf("Line %d, col %d: Variable identifier expected\n",raiz->filho->token->linha,raiz->filho->token->coluna );
					errosS+=1;
					raiz->type = _erro_;
					return;
				}

				if(elementoDaPesquisa->tType == _function_){
					elementoDaPesquisa = elementoDaPesquisa->table->simbolo;
				}

				tipoElem = elementoDaPesquisa->tType;

				//Check if 2nd part is Var or intlit/realit

				percorreAST(temp->irmao,tabela);
				if(temp->irmao->type == _erro_) {
					raiz->type = _erro_;
					return;
				}
				if(tipoElem == _integer_ && temp->irmao->type == _boolean_){
					printf("Line %d, col %d: Incompatible type in assigment to %s (got %s, expected %s)\n",temp->token->linha,temp->token->coluna,temp->token->token,getTipoTabela(temp->irmao->type),getTipoTabela(tipoElem) );
					errosS+=1;
					raiz->type = _erro_;
					return;
				}
				if(tipoElem == _integer_ && temp->irmao->type == _real_){
					printf("Line %d, col %d: Incompatible type in assigment to %s (got %s, expected %s)\n",temp->token->linha,temp->token->coluna,temp->token->token,getTipoTabela(temp->irmao->type),getTipoTabela(tipoElem) );
					errosS+=1;
					return;
				}
				if(tipoElem == _boolean_ && temp->irmao->type != _boolean_){
					printf("Line %d, col %d: Incompatible type in assigment to %s (got %s, expected %s)\n",temp->token->linha,temp->token->coluna,temp->token->token,getTipoTabela(temp->irmao->type),getTipoTabela(tipoElem) );
					errosS+=1;
					return;
				}
				if(tipoElem == _real_ && temp->irmao->type == _boolean_){
					printf("Line %d, col %d: Incompatible type in assigment to %s (got %s, expected %s)\n",temp->irmao->token->linha,temp->irmao->token->coluna,temp->irmao->token->token,getTipoTabela(temp->irmao->type),getTipoTabela(tipoElem) );
					errosS+=1;
					return;
				}


				percorreAST(raiz->irmao,tabela);
				break;

			case tag_Sub:
			case tag_Add:
			case tag_Mul:
				percorreAST(raiz->filho,tabela); //preencher o no raiz->filho com info da op de baixo
				if(raiz->filho->type == _erro_){
					raiz->type = _erro_;
					return;
				}

				percorreAST(raiz->filho->irmao,tabela);
				if(raiz->filho->irmao->type == _erro_) {
					raiz->type = _erro_;
					return;
				}

				exp1 = raiz->filho; //Primeiro termo é o 1º filho, mesmo que seja 1 expressao
				exp2 = raiz->filho->irmao; //Segundo termo

				//printf("op:%s, e1:%s | e2:%s\n",raiz->token->token,getTipoTabela(exp1->type),getTipoTabela(exp2->type) );

				if(exp1->type != _integer_ && exp1->type != _real_){
					printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n",raiz->token->linha,raiz->token->coluna,raiz->token->token,getTipoTabela(exp1->type),getTipoTabela(exp2->type) );
					errosS +=1;
					raiz->type = _erro_;
					return;
				}

				if(exp2->type != _integer_ && exp2->type != _real_){
					printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n",raiz->token->linha,raiz->token->coluna,raiz->token->token,getTipoTabela(exp1->type),getTipoTabela(exp2->type) );
					errosS +=1;
					raiz->type = _erro_;
					return;
				}

				if(exp1->type == _integer_ && exp2->type == _integer_){ //Se sao os 2 ints, resultado é int
					raiz->type = _integer_;
				}else{
					raiz->type = _real_;
				}

				break;

			case tag_Plus:
			case tag_Minus:
				percorreAST(raiz->filho,tabela); //preencher o no raiz->filho com info da op de baixo
				if(raiz->filho->type == _erro_) {
					raiz->type = _erro_;
					return;
				}

				exp1 = raiz->filho; //Primeiro termo é o 1º filho, mesmo que seja 1 expressao

				if(exp1->type != _integer_ && exp1->type != _real_){
					printf("Line %d, col %d: Operator %s cannot be applied to type %s\n",raiz->token->linha,raiz->token->coluna,raiz->token->token,getTipoTabela(exp1->type) );
					errosS +=1;
					raiz->type = _erro_;
					return;
				}else if(exp1->type == _integer_){
					raiz->type = _integer_;
				}else{
					raiz->type = _real_;
				}

				break;


			case tag_Div:
			case tag_Mod:

				percorreAST(raiz->filho,tabela); //preencher o no raiz->filho com info da op de baixo
				if(raiz->filho->type == _erro_) {
					raiz->type = _erro_;
					return;
				}

				percorreAST(raiz->filho->irmao,tabela);
				if(raiz->filho->irmao->type == _erro_) {
					raiz->type = _erro_;
					return;
				}

				exp1 = raiz->filho; //Primeiro termo é o 1º filho, mesmo que seja 1 expressao
				exp2 = raiz->filho->irmao; //Segundo termo

				//printf("%d,%d,%s\n",raiz->token->linha,raiz->token->coluna,raiz->token->token );

				if(exp1->type == _integer_ && exp2->type == _integer_){
					raiz->type = _integer_;
				}else{
					printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n",raiz->token->linha,raiz->token->coluna,raiz->token->token,getTipoTabela(exp1->type),getTipoTabela(exp2->type) );
					errosS +=1;
					raiz->type = _erro_;
					return;
				}
				break;

			case tag_RealDiv:

				percorreAST(raiz->filho,tabela); //preencher o no raiz->filho com info da op de baixo
				if(raiz->filho->type == _erro_) {
					raiz->type = _erro_;
					return;
				}

				percorreAST(raiz->filho->irmao,tabela);
				if(raiz->filho->irmao->type == _erro_) {
					raiz->type = _erro_;
					return;
				}

				exp1 = raiz->filho; //Primeiro termo é o 1º filho, mesmo que seja 1 expressao
				exp2 = raiz->filho->irmao; //Segundo termo

				if((exp1->type == _integer_ || exp1->type == _real_) && (exp2->type == _integer_ || exp2->type == _real_)){
					raiz->type = _real_;
				}else{
					printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n",raiz->token->linha,raiz->token->coluna,raiz->token->token,getTipoTabela(exp1->type),getTipoTabela(exp2->type) );
					errosS +=1;
					raiz->type = _erro_;
					return;
				}


				break;


			case tag_Eq:
			case tag_Geq:
			case tag_Gt:
			case tag_Leq:
			case tag_Lt:
			case tag_Neq:
				percorreAST(raiz->filho,tabela); //preencher o no raiz->filho com info da op de baixo
				if(raiz->filho->type == _erro_) {
					raiz->type = _erro_;
					return;
				}

				percorreAST(raiz->filho->irmao,tabela);
				if(raiz->filho->irmao->type == _erro_) {
					raiz->type = _erro_;
					return;
				}

				exp1 = raiz->filho; //Primeiro termo é o 1º filho, mesmo que seja 1 expressao
				exp2 = raiz->filho->irmao; //Segundo termo

				if(exp1->type == _boolean_ && exp2->type == _boolean_){
					raiz->type = _boolean_;
				}
				else if( (exp1->type == _integer_ || exp1->type == _real_) && (exp2->type == _integer_ || exp2->type == _real_)  ){
					raiz->type = _boolean_;
				}else{
					printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n",raiz->token->linha,raiz->token->coluna,raiz->token->token,getTipoTabela(exp1->type),getTipoTabela(exp2->type) );
					errosS +=1;
					raiz->type = _erro_;
					return;
				}
			break;

			case tag_And:
			case tag_Or:
				percorreAST(raiz->filho,tabela); //preencher o no raiz->filho com info da op de baixo
				if(raiz->filho->type == _erro_) {
					raiz->type = _erro_;
					return;
				}

				percorreAST(raiz->filho->irmao,tabela);
				if(raiz->filho->irmao->type == _erro_) {
					raiz->type = _erro_;
					return;
				}

				exp1 = raiz->filho; //Primeiro termo é o 1º filho, mesmo que seja 1 expressao
				exp2 = raiz->filho->irmao; //Segundo termo

				if(exp1->type == _boolean_ && exp2->type == _boolean_){
					raiz->type = _boolean_;
				}
				else{
					printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n",raiz->token->linha,raiz->token->coluna,raiz->token->token,getTipoTabela(exp1->type),getTipoTabela(exp2->type) );
					errosS +=1;
					raiz->type = _erro_;
					return;
				}
			break;

			case tag_Not:
				percorreAST(raiz->filho,tabela); //preencher o no raiz->filho com info da op de baixo
				if(raiz->filho->type == _erro_) {
					raiz->type = _erro_;
					return;
				}

				exp1 = raiz->filho; //Primeiro termo é o 1º filho, mesmo que seja 1 expressao

				if(exp1->type == _boolean_){
					raiz->type = _boolean_;
				}else{
					printf("Line %d, col %d: Operator %s cannot be applied to type %s\n",raiz->token->linha,raiz->token->coluna,raiz->token->token,getTipoTabela(exp1->type) );
					errosS +=1;
					raiz->type = _erro_;
					return;
				}
			break;

			case tag_Repeat:
				percorreAST(raiz->filho,tabela);
				if(raiz->filho->type == _erro_){
					raiz->type = _erro_;
					return;
				}

				temp = raiz->filho->irmao;

				if(temp->type == _boolean_){
					raiz->type = _boolean_;
				}else{
					printf("Line %d, col %d: Incompatible type in repeat-until statement (got %s, expected _boolean_)\n",temp->token->linha,temp->token->coluna,getTipoTabela(temp->type));
					errosS +=1;
					raiz->type = _erro_;
					return;
				}


				percorreAST(raiz->irmao,tabela);
				break;

			case tag_WriteLn:
				percorreAST(raiz->filho,tabela);
				if(raiz->filho != NULL){
					if(raiz->filho->type == _erro_){
						raiz->type = _erro_;
						return;
					}
				}
				temp = raiz->filho;

				while(temp != NULL){
					if(!(temp->type == _integer_ || temp->type == _boolean_ || temp->type == _real_)){
						printf("Line %d, col %d: Cannot write values of type %s\n",temp->token->linha,temp->token->coluna,getTipoTabela(temp->type));
						errosS +=1;
						raiz->type = _erro_;
						return;
					}

					temp = temp->irmao;
				}


				percorreAST(raiz->irmao,tabela);
				break;


			case tag_Id:
				elementoDaPesquisa = searchGlobalID(tabela,raiz->token->token);
				if(elementoDaPesquisa == NULL){
					printf("Line %d, col %d: Symbol %s not defined\n",raiz->token->linha,raiz->token->coluna,raiz->token->token);
					errosS +=1;
					raiz->type = _erro_;
					return;
				}

				raiz->type = elementoDaPesquisa->tType;
				percorreAST(raiz->filho,tabela);
				percorreAST(raiz->irmao,tabela);
				break;

			case tag_ValParam:

				percorreAST(raiz->filho,tabela);
				exp1 = raiz->filho;

				if(exp1->type != _integer_){
					printf("Line %d, col %d: Incompatible type in val-paramstr statement (got %s, expected _integer_)\n",exp1->token->linha,exp1->token->coluna,getTipoTabela(exp1->type) );
					errosS+=1;
					raiz->type = _erro_;
					return;
				}

				exp2 = raiz->filho->irmao;
				elementoDaPesquisa = searchGlobalID(tabela,exp2->token->token);
				if(exp2 == NULL || exp2->type == _function_ || exp2->type == _program_ || exp2->type == _constant_){
					printf("Line %d, col %d: Variable identifier expected\n",exp2->token->linha,exp2->token->coluna );
					errosS+=1;
					raiz->type = _erro_;
					return;
				}

				if(exp2->type != _integer_){
					printf("Line %d, col %d: Incompatible type in val-paramstr statement (got %s, expected _integer_)\n",exp1->token->linha,exp1->token->coluna,getTipoTabela(exp1->type) );
					errosS+=1;
					raiz->type = _erro_;
					return;
				}



				break;

			default:
				percorreAST(raiz->filho,tabela);
				percorreAST(raiz->irmao,tabela);
		}
	}
}
*/
