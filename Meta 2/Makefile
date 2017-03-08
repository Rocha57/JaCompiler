FILE_NAME = jac

all:
	@lex $(FILE_NAME).l
	@yacc -d -v $(FILE_NAME).y
	@clang-3.8 -o $(FILE_NAME) lex.yy.c y.tab.c -ll -ly 
	@zip $(FILE_NAME).zip $(FILE_NAME).l $(FILE_NAME).y
	@echo "[INFO] Compiled"

clean:
	@rm y.output y.tab.c y.tab.h lex.yy.c $(FILE_NAME) $(FILE_NAME).zip
	echo "[INFO] Cleaned"