#include "tabelhas.h"

int temp_var = 1;
int parse = 0;
int strings = 0;
int prints = 0;
char* classe;

void loadId(Node* raiz, symbol type){
	printf("\t%%%d = load %s, %s* %%%s\n", temp_var, getLLVMConstant(type), getLLVMConstant(type), raiz->token->token);
	raiz->result = strdup("");
	sprintf(raiz->result,"%%%d", temp_var);
	temp_var++;
}

void findStrLit(Node* raiz){
	char* aux;
	if (raiz != NULL){
		if (raiz->tipo == StrLit){
			printf("@.str%d = private unnamed_addr constant [%lu x i8]", strings, strlen(raiz->token->token));
			aux = ++raiz->token->token;
			aux[strlen(aux)-1] = 0;
			printf(" c\"%s\\0A\\00\"\n", aux);
			raiz->result = strdup("");
			sprintf(raiz->result,"@.str%d", strings);
			strings++;
		}

		findStrLit(raiz->filho);
		findStrLit(raiz->irmao);
	}
}

void generateLLVMFromAST(Node* raiz, Table* tabela){
	char* nome;
	symbol type;
	if (raiz != NULL){
		switch (raiz->tipo) {
			case Program:
				classe = raiz->filho->token->token;
				findStrLit(raiz);
				printf("@.strint = private unnamed_addr constant [4 x i8] c\"%%d\\0A\\00\"\n");
				printf("@.strdouble = private unnamed_addr constant [7 x i8] c\"%%.16E\\0A\\00\"\n");
				printf("@.strtrue = private unnamed_addr constant [6 x i8] c\"true\\0A\\00\"\n");
				printf("@.strfalse = private unnamed_addr constant [7 x i8] c\"false\\0A\\00\"\n");
				printf("\n");
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
				nome = raiz->filho->irmao->token->token;
				printf("define %s @%s.%s(", getLLVMConstant(type), classe, nome);
				generateLLVMFromAST(raiz->filho, tabela);
				printf("){\n");
				generateLLVMFromAST(raiz->irmao, tabela);
				if (type == _void_)
					printf("\tret void\n");
				printf("}\n\n");
				parse = 0;
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
				printf("\t%%%s = alloca ", nome);
				printf("%s\n", getLLVMConstant(type));
				generateLLVMFromAST(raiz->irmao, tabela);
			break;

			case Assign:
				generateLLVMFromAST(raiz->filho, tabela);
				nome = raiz->filho->token->token;
				type = raiz->filho->token->annotation;
				if (raiz->filho->irmao->tipo == Id)
					loadId(raiz->filho->irmao, type);
				printf("\tstore %s %s, %s* %%%s\n", getLLVMConstant(raiz->filho->irmao->token->annotation), raiz->filho->irmao->result, getLLVMConstant(type), nome);
				generateLLVMFromAST(raiz->irmao, tabela);
			break;

			case Add:
			case Sub:
			case Mul:
			case Div:
			case Mod:
				generateLLVMFromAST(raiz->filho, tabela);
				type = raiz->token->annotation;
				if (raiz->filho->tipo == Id)
					loadId(raiz->filho, type);
				
				if (raiz->filho->irmao->tipo == Id)
					loadId(raiz->filho->irmao, type);
				
				if (raiz->tipo == Add)
					printf("\t%%%d = add %s %s, %s\n",temp_var, getLLVMConstant(type), raiz->filho->result, raiz->filho->irmao->result);
				else if (raiz->tipo == Sub)
					printf("\t%%%d = sub %s %s, %s\n",temp_var, getLLVMConstant(type), raiz->filho->result, raiz->filho->irmao->result);
				else if (raiz->tipo == Mul)
					printf("\t%%%d = mul %s %s, %s\n",temp_var, getLLVMConstant(type), raiz->filho->result, raiz->filho->irmao->result);
				else if (raiz->tipo == Div)
					printf("\t%%%d = sdiv %s %s, %s\n",temp_var, getLLVMConstant(type), raiz->filho->result, raiz->filho->irmao->result);
				else if (raiz->tipo == Mod)
					printf("\t%%%d = srem %s %s, %s\n",temp_var, getLLVMConstant(type), raiz->filho->result, raiz->filho->irmao->result);
				raiz->result = strdup("");
				sprintf(raiz->result,"%%%d", temp_var);
				temp_var++;
				generateLLVMFromAST(raiz->irmao, tabela);
			break;

			case Print:
				generateLLVMFromAST(raiz->filho, tabela);
				type = raiz->filho->token->annotation;
				if (raiz->filho->tipo == Id)
					loadId(raiz->filho, type);
				if (raiz->filho->token->annotation == _int_)
					printf("\t%%call%d = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.strint, i32 0, i32 0), i32 %s)\n",prints, raiz->filho->result);
				else if (raiz->filho->token->annotation == _Double_)
					printf("\t%%call%d = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.strdouble, i32 0, i32 0), double %s)\n",prints, raiz->filho->result);
				else if (raiz->filho->token->annotation == _String_)
					printf("\t%%call%d = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([%lu x i8], [%lu x i8]* %s, i32 0, i32 0))\n",prints, strlen(raiz->filho->token->token)+2, strlen(raiz->filho->token->token)+2, raiz->filho->result);
				else if (raiz->filho->token->annotation == _boolean_){
					if (raiz->filho->)
				}
				prints++;
				generateLLVMFromAST(raiz->irmao, tabela);
			break;

			case Return:
				generateLLVMFromAST(raiz->filho, tabela);
				if (raiz->filho!= NULL)
					printf("\tret %s %s\n", getLLVMConstant(raiz->filho->token->annotation), raiz->filho->result);
				generateLLVMFromAST(raiz->irmao, tabela);
			break;

			case Plus:
			case Minus:
				generateLLVMFromAST(raiz->filho, tabela);
				raiz->result = strdup("");
				if (raiz->tipo == Minus)
					sprintf(raiz->result,"-%s", raiz->filho->result);
				else
					sprintf(raiz->result,"%s", raiz->filho->result);
				generateLLVMFromAST(raiz->irmao, tabela);
			break;

			case ParseArgs:
				generateLLVMFromAST(raiz->filho, tabela);
				nome = raiz->filho->token->token;
				if (parse == 0){
					printf("\t%%%s.c.addr = alloca i32\n\t%%%s.v.addr = alloca i8**\n", nome, nome);
					printf("\tstore i32 %%%s.c, i32* %%%s.c.addr\n\tstore i8** %%%s.v, i8*** %%%s.v.addr\n", nome,nome,nome,nome);
					parse++;
				}
				printf("\t%%%d = load i8**, i8*** %%%s.v.addr\n", temp_var, nome);
				printf("\t%%arrayid%d = getelementptr inbounds i8*, i8** %%%d, i64 %d\n", parse, temp_var, atoi(raiz->filho->irmao->result)+1);
				temp_var++;
				printf("\t%%%d = load i8*, i8** %%arrayid%d\n\t%%%d = call i32 @atoi(i8* %%%d)\n",temp_var,parse,temp_var+1,temp_var);
				temp_var++;
				parse++;
				raiz->result = strdup("");
				sprintf(raiz->result,"%%%d", temp_var);
				temp_var++;
				generateLLVMFromAST(raiz->irmao, tabela);
			break;

			case Length:
				generateLLVMFromAST(raiz->filho, tabela);
				nome = raiz->filho->token->token;
				if (parse == 0){
					printf("\t%%%s.c.addr = alloca i32\n\t%%%s.v.addr = alloca i8**\n", nome, nome);
					printf("\tstore i32 %%%s.c, i32* %%%s.c.addr\n\tstore i8** %%%s.v, i8*** %%%s.v.addr\n", nome,nome,nome,nome);
					parse++;
				}
				printf("\t%%%d = load i32, i32* %%%s.c.addr\n", temp_var, raiz->filho->token->token);
				temp_var++;
				printf("\t%%%d = sub i32 %%%d, 1\n", temp_var, temp_var-1);
				raiz->result = strdup("");
				sprintf(raiz->result,"%%%d", temp_var);
				temp_var++;
				generateLLVMFromAST(raiz->irmao, tabela);
			break;
			case DecLit:
			case RealLit:
			case BoolLit:
				raiz->result = raiz->token->token;

			default:
				generateLLVMFromAST(raiz->filho, tabela);
				generateLLVMFromAST(raiz->irmao, tabela);
		}
	}
}