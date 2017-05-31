#include "tabelhas.h"

int temp_var = 1;
int parse = 0;
int strings = 0;
int prints = 0;
char* classe;

void loadId(Node* raiz, Table* tabela){
	Elemento* global;
	global = searchGlobalID(tabela, raiz->token->token);
	printf("\t%%%d = load %s, %s* ", temp_var, getLLVMConstant(raiz->token->annotation), getLLVMConstant(raiz->token->annotation));
	if (global == NULL)
		printf("%%%s\n", raiz->token->token);
	else
		printf("@%s\n", raiz->token->token);
	raiz->result = strdup("");
	sprintf(raiz->result,"%%%d", temp_var);
	temp_var++;
}

void assignVar(Node* raiz, Table* tabela){
	if (raiz->filho->irmao->tipo == Id)
		loadId(raiz->filho->irmao, tabela);
	Elemento* global;
	global = searchGlobalID(tabela, raiz->filho->token->token);
	if (raiz->filho->token->annotation == _Double_ && raiz->filho->irmao->token->annotation == _int_){
		printf("\t%%conv%d = sitofp i32 %s to double\n", temp_var, raiz->filho->irmao->result);
		printf("\tstore double %%conv%d, double* ", temp_var);
	}
	else
		printf("\tstore %s %s, %s* ", getLLVMConstant(raiz->filho->irmao->token->annotation), raiz->filho->irmao->result, getLLVMConstant(raiz->filho->token->annotation));
	if (global == NULL)
		printf("%%%s\n", raiz->filho->token->token);
	else
		printf("@%s\n", raiz->filho->token->token);
}



char* removeEscape(char* token){
	char* copia = (char*) malloc(sizeof(token)*2);
	char* src, *srcmaisum;
	memcpy(copia,token,strlen(token)+1);
	int len = strlen(token);
	srcmaisum = src = copia;
	int pos = 0;
	srcmaisum++;
	//printf("%d PRIINTNT: %c\t %s\n",pos, *src, src);
	while(*src != '\0'){ 
		//printf("0 %c\n", *src);
	    if (*src == '\\' && *srcmaisum == 'n'){
	    	//printf("bananq\n");
	    	src++;
	    	//printf("1 %c\n", *src);
	        *src = '0';
	        memmove(src+2, src+1, len - pos -1);
	        src++;
	        *src = 'A';
	        srcmaisum++;
	        srcmaisum++;
	    }
	    else if (*src == '\\' && *srcmaisum == 't'){
	    	//		printf("banan2\n");
	    	src++;
	    	//printf("2 %c\n", *src);
	        *src = '0';
	        memmove(src+2, src+1, len - pos -1);  
	        src++;
	        *src = '9';  
	        srcmaisum++;
	        srcmaisum++;

	    }
	    else if (*src == '\\' && *srcmaisum == 'r'){
	    	// 		printf("banan3\n");
	    	src++;
	    	//printf("3 %c\n", *src);
	        *src = '0';
	        memmove(src+2, src+1, len - pos -1);
	        src++;
	        *src = 'D';  
	        srcmaisum++;
	        srcmaisum++;

	    }
	    else if (*src == '\\' && *srcmaisum == 'f'){
	    	//		printf("bana4\n");
	    	src++;
	    	//printf("4 %c\n", *src);
	        *src = '0';
	        memmove(src+2, src+1, len - pos -1);
	        src++;
	        *src = '9'; 
	        srcmaisum++; 
	        srcmaisum++;

	    }
	    else if (*src == '\\' && *srcmaisum == '\\'){
	    	//		printf("banan5\n");
	    	src++;
	    	//printf("5 %c\n", *src);
	        *src = '5'; 
	        memmove(src+2, src+1, len - pos -1);
	        src++;
	        *src = 'C'; 
	        srcmaisum++;
	        srcmaisum++;

	    }
	    else if (*src == '\\' && *srcmaisum == '"'){
	    	//		printf("banan6\n");
	    	src++;
	    	//printf("6 %c\n", *src);	    	
	        *src = '2';  
	        memmove(src+2, src+1, len - pos -1);
	        src++;
	        *src = '2'; 
	        srcmaisum++;
	        srcmaisum++;
	    }
	    else{
		    pos++;  //count position in str
		    src++;  //move forward
		    srcmaisum++;
		}
	    //printf("%s\n", copia);
	}
	return copia;
}


void findStrLit(Node* raiz){
	char* aux, *final;
	if (raiz != NULL){
		if (raiz->tipo == StrLit){
			printf("@.str%d = private unnamed_addr constant [%lu x i8]", strings, strlen(raiz->token->token));
			aux = ++raiz->token->token;
			aux[strlen(aux)-1] = 0;
			final = removeEscape(aux);
			printf(" c\"%s\\0A\\00\"\n", final);
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
	char *src, *dest, *aux, *begin = NULL;
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
				printf("%s ", getLLVMConstant(type));
				if (type == _Double_)
					printf("0.0\n");
				else
					printf("0\n");
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
				type = raiz->filho->irmao->token->annotation;
				assignVar(raiz, tabela);
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
					loadId(raiz->filho, tabela);
				
				if (raiz->filho->irmao->tipo == Id)
					loadId(raiz->filho->irmao, tabela);
				
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
					loadId(raiz->filho, tabela);
				if (raiz->filho->token->annotation == _int_)
					printf("\t%%call%d = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.strint, i32 0, i32 0), i32 %s)\n",prints, raiz->filho->result);
				else if (raiz->filho->token->annotation == _Double_)
					printf("\t%%call%d = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.strdouble, i32 0, i32 0), double %s)\n",prints, raiz->filho->result);
				else if (raiz->filho->token->annotation == _String_)
					printf("\t%%call%d = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([%lu x i8], [%lu x i8]* %s, i32 0, i32 0))\n",prints, strlen(raiz->filho->token->token)+2, strlen(raiz->filho->token->token)+2, raiz->filho->result);
				else if (raiz->filho->token->annotation == _boolean_){
					printf("\t%%ifcond%d = icmp eq i1 %s, 1\n\tbr i1 %%ifcond%d , label %%then%d , label %%else%d\n", prints, raiz->filho->result, prints, prints, prints);
					printf("then%d:\n", prints);
					printf("\t%%call%d = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.strtrue, i32 0, i32 0))\n",prints);
					prints++;
					printf("\tbr label %%ifcont%d\n", prints);
					printf("else%d:\n", prints-1);
					printf("\t%%call%d = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.strfalse, i32 0, i32 0))\n",prints);
					printf("\tbr label %%ifcont%d\n", prints);
					printf("ifcont%d:\n", prints);
				}
				prints++;
				generateLLVMFromAST(raiz->irmao, tabela);
			break;

			case Return:
				generateLLVMFromAST(raiz->filho, tabela);
				if (raiz->filho!= NULL){
					type = raiz->filho->token->annotation;
					if (raiz->filho->tipo == Id)
						loadId(raiz->filho, tabela);
					printf("\tret %s %s\n", getLLVMConstant(raiz->filho->token->annotation), raiz->filho->result);
				}
				generateLLVMFromAST(raiz->irmao, tabela);
			break;

			case Plus:
			case Minus:
				generateLLVMFromAST(raiz->filho, tabela);
				raiz->result = strdup("");
				if (raiz->filho->tipo == DecLit || raiz->filho->tipo == RealLit){
					if (raiz->tipo == Minus)
						sprintf(raiz->result,"-%s", raiz->filho->result);
					else
						sprintf(raiz->result,"%s", raiz->filho->result);
				}
				else if (raiz->filho->tipo == Id){
					loadId(raiz->filho, tabela);
					if (raiz->tipo == Minus){
						printf("\t%%sub%d = sub nsw i32 0, %s\n", temp_var, raiz->filho->result);
						sprintf(raiz->result, "%%sub%d", temp_var);
					}
					else
						sprintf(raiz->result, "%s", raiz->filho->result);
				}
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
			//eliminar _ dos numeros
				src = dest = raiz->token->token;    // both pointers point to the first char of input
				while(*src != '\0'){ 
				    if (*src != '_'){
				        *dest = *src;  // copy the char at source to destination
				        dest++;        // increment destination pointer
				    }
				    src++;             // increment source pointer
				}
				*dest = '\0';
				raiz->result = raiz->token->token;
				generateLLVMFromAST(raiz->filho, tabela);
				generateLLVMFromAST(raiz->irmao, tabela);
				break;
			case RealLit:
				src = aux = dest = raiz->token->token;    // both pointers point to the first char of input
				while(*src != '\0'){ 
				    if (*src != '_'){
				        *dest = *src;  // copy the char at source to destination
				        dest++;        // increment destination pointer
				    }
				    src++;             // increment source pointer
				}
				*dest = '\0';
				//tratar do .3 para 0.3
				if(aux[0] =='.'){
					begin = (char*) malloc(strlen(dest)+1);
					begin[0]='0';
					strcat(begin, aux);
				}
				else{
					*begin=*dest;
				}
				
				raiz->result = begin;
				generateLLVMFromAST(raiz->filho, tabela);
				generateLLVMFromAST(raiz->irmao, tabela);
				break;
			
			case BoolLit:
				raiz->result = raiz->token->token;

			default:
				generateLLVMFromAST(raiz->filho, tabela);
				generateLLVMFromAST(raiz->irmao, tabela);
		}
	}
}