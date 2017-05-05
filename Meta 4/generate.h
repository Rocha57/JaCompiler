#include "tabelhas.h"

void generateLLVMFromAST(Node* raiz, Table* tabela){
	char* nome;
	Node* temp;
	Elemento* var;
	symbol type;
	if (raiz != NULL){
		switch (raiz->tipo) {
			case FieldDecl:
				type = getTipoInserirTabela(getTipo(raiz->filho->tipo)); //Entrar na declaracao de vars
				nome = raiz->filho->irmao->token->token;
				printf("@%s = global ",nome);
				printf("%s\n", getLLVMConstant(type));
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