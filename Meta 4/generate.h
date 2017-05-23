#include "tabelhas.h"

int temp_var = 1;
char* classe;
symbol retorno;

void generateLLVMFromAST(Node* raiz, Table* tabela){
	char* nome;
	Node* temp;
	Elemento* var;
	symbol type;
	if (raiz != NULL){
		switch (raiz->tipo) {
			case Program:
				classe = raiz->filho->token->token;
				printf("@.strint = private unnamed_addr constant [4 x i8] c\"%%d\\0A\\00\"\n");
				printf("@.strdouble = private unnamed_addr constant [7 x i8] c\"%%.16E\\0A\\00\"\n\n");
				generateLLVMFromAST(raiz->filho, tabela);
				printf("declare i32 @atoi(i8*)\ndeclare i32 @printf(i8*, ...)\ndefine i32 @main(i32 %%argc, i8** %%argv){\n\t%%retval = alloca i32\n\t%%argc.addr = alloca i32\n\t%%argv.addr = alloca i8**\n\tstore i32 0, i32* %%retval\n\tstore i32 %%argc, i32* %%argc.addr\n\tstore i8** %%argv, i8*** %%argv.addr\n\t%%1 = load i32, i32* %%argc.addr\n\t%%2 = load i8**, i8*** %%argv.addr\n\tcall void @%s.main(i32 %%1, i8** %%2)\n\tret i32 0\n}\n",classe);
			break;

			case FieldDecl:
				type = getTipoInserirTabela(getTipo(raiz->filho->tipo)); //Entrar na declaracao de vars
				nome = raiz->filho->irmao->token->token;
				printf("@%s = global ",nome);
				printf("%s\n", getLLVMConstant(type));
				generateLLVMFromAST(raiz->irmao, tabela);
			break;

			case MethodHeader:
				type = getTipoInserirTabela(getTipo(raiz->filho->tipo));
				retorno = type;
				nome = raiz->filho->irmao->token->token;
				printf("define %s @%s.%s(", getLLVMConstant(type), classe, nome);
				generateLLVMFromAST(raiz->filho, tabela);
				printf("){\n");
				generateLLVMFromAST(raiz->irmao, tabela);
				if (type == _void_)
					printf("ret void\n");
				printf("}\n\n");
				temp_var = 1;
				break;

			case ParamDecl:
				type = getTipoInserirTabela(getTipo(raiz->filho->tipo));
				nome = raiz->filho->irmao->token->token;
				if (type != _StringArray_)
					printf("%s %%%s", getLLVMConstant(type), nome);
				else
					printf("i32 %%%s.c, i8** %%%s.v", nome, nome);
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
				if (raiz->filho->irmao->tipo == DecLit || raiz->filho->irmao->tipo == StrLit || raiz->filho->irmao->tipo == RealLit || raiz->filho->irmao->tipo == BoolLit)
					printf("store %s %s, %s* %%%s\n", getLLVMConstant(raiz->filho->irmao->token->annotation), raiz->filho->irmao->token->token, getLLVMConstant(type), nome);
				else
					printf("store %s %s, %s* %%%s\n", getLLVMConstant(raiz->filho->irmao->token->annotation), raiz->filho->irmao->result, getLLVMConstant(type), nome);
				generateLLVMFromAST(raiz->irmao, tabela);
			break;

			case Add:
				generateLLVMFromAST(raiz->filho, tabela);
				type = raiz->token->annotation;
				if (raiz->filho->tipo == Id){
					printf("%%%d = load %s, %s* %%%s\n", temp_var, getLLVMConstant(type), getLLVMConstant(raiz->filho->token->annotation), raiz->filho->token->token);
					raiz->filho->result = strdup("");
					sprintf(raiz->filho->result,"%%%d", temp_var);
					temp_var++;
				}
				if (raiz->filho->irmao->tipo == Id){
					printf("%%%d = load %s, %s* %%%s\n", temp_var, getLLVMConstant(type), getLLVMConstant(raiz->filho->irmao->token->annotation), raiz->filho->irmao->token->token);
					raiz->filho->irmao->result = strdup("");
					sprintf(raiz->filho->irmao->result,"%%%d", temp_var);
					temp_var++;
				}
				printf("%%%d = add %s %s, %s\n",temp_var, getLLVMConstant(type), raiz->filho->result, raiz->filho->irmao->result);
				raiz->result = strdup("");
				sprintf(raiz->result,"%%%d", temp_var);
				temp_var++;
				generateLLVMFromAST(raiz->irmao, tabela);
			break;

			case Print:
			if (raiz->filho->tipo == DecLit){
				printf("%%call = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.strint, i32 0, i32 0), i32 %s)\n", raiz->filho->token->token);
			}
			else if (raiz->filho->tipo == RealLit){
				printf("%%call = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.strdouble, i32 0, i32 0), double %s)\n", raiz->filho->token->token);
			}
			default:
				generateLLVMFromAST(raiz->filho, tabela);
				generateLLVMFromAST(raiz->irmao, tabela);
		}
	}
}