#include "tabelhas.h"

char* getLLVMConstant(symbol type);

void generateLLVMFromAST(Node* raiz, Table* tabela){
	char* nome;
	Node* temp;
	Elemento* var;
	symbol type;
	if (raiz != NULL){
		switch (raiz->tipo) {
			case FieldDecl:
				temp = raiz->filho; //Entrar na declaracao de vars
				while(temp->irmao != NULL){ //avancar até à declaracao do token
					temp = temp->irmao;
				}
				var = checkFieldDecl(temp, tabela);
				if (var != NULL){
					printf("@%s = global ",var->token);
					printf("%s\n", getLLVMConstant(var->tType));
				}
				generateLLVMFromAST(raiz->irmao, tabela);
			break;

			case MethodHeader:
				type = getTipoInserirTabela(getTipo(raiz->filho->tipo));
				nome = raiz->filho->irmao->token->token;
				printf("define %s @%s(){\n", getLLVMConstant(type), nome);
				generateLLVMFromAST(raiz->filho, tabela);
				printf("}\n");
				generateLLVMFromAST(raiz->irmao, tabela);
			default:
				generateLLVMFromAST(raiz->filho, tabela);
				generateLLVMFromAST(raiz->irmao, tabela);
		}
	}
}

char* getLLVMConstant(symbol type){
	char* constant;
	if (type == _int_)
		constant = strdup("i32");
	else if (type == _Double_)
		constant = strdup("f64");
	else if (type == _boolean_)
		constant = strdup("i1");
	else if (type == _void_)
		constant = strdup("void");
	else
		constant = strdup("");
	return constant;
}