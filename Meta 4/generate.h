#include "tabelhas.h"

int temp_var = 0;

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
				printf("define %s @%s(", getLLVMConstant(type), nome);
				generateLLVMFromAST(raiz->filho, tabela);
				printf("){\n");
				generateLLVMFromAST(raiz->irmao, tabela);
				printf("}\n");
				temp_var = 1;
				break;

			case ParamDecl:
				type = getTipoInserirTabela(getTipo(raiz->filho->tipo));
				nome = raiz->filho->irmao->token->token;
				printf("%s %%%s", getLLVMConstant(type), nome);
				if (raiz->irmao!=NULL)
					printf(",");
				generateLLVMFromAST(raiz->irmao, tabela);
				break;
			
			case VarDecl:
				type = getTipoInserirTabela(getTipo(raiz->filho->tipo));
				nome = raiz->filho->irmao->token->token;
				printf("%%%s = alloca ", nome);
				printf("%s\n", getLLVMConstant(type));
				generateLLVMFromAST(raiz->irmao, tabela);
				break;

			case Assign:
				generateLLVMFromAST(raiz->filho, tabela);
				nome = raiz->filho->token->token;
				type = raiz->filho->token->annotation;
				printf("store %s %s, %s* %%%s\n", getLLVMConstant(raiz->filho->irmao->token->annotation), raiz->filho->irmao->token->token, getLLVMConstant(type), nome);
				generateLLVMFromAST(raiz->irmao, tabela);
			break;

			case Add:
				generateLLVMFromAST(raiz->filho, tabela);
				type = raiz->token->annotation;
				if (raiz->filho->tipo == Id){
					printf("%%%d = load %s, %s* %%%s\n", temp_var, getLLVMConstant(type), getLLVMConstant(raiz->filho->token->annotation), raiz->filho->token->token);
					raiz->filho->token->token = strdup("");
					sprintf(raiz->filho->token->token,"%%%d", temp_var);
					temp_var++;
				}
				if (raiz->filho->irmao->tipo == Id){
					printf("%%%d = load %s, %s* %%%s\n", temp_var, getLLVMConstant(type), getLLVMConstant(raiz->filho->irmao->token->annotation), raiz->filho->irmao->token->token);
					raiz->filho->irmao->token->token = strdup("");
					sprintf(raiz->filho->irmao->token->token,"%%%d", temp_var);
					temp_var++;
				}
				printf("%%%d = add %s %s, %s\n",temp_var, getLLVMConstant(type), raiz->filho->token->token, raiz->filho->irmao->token->token);
				raiz->token->token = strdup("");
				sprintf(raiz->token->token,"%d", temp_var);
				temp_var++;
				generateLLVMFromAST(raiz->irmao, tabela);
			break;
			default:
				generateLLVMFromAST(raiz->filho, tabela);
				generateLLVMFromAST(raiz->irmao, tabela);
		}
	}
}