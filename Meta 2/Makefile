FILE_NAME = jac

all:
	@lex $(FILE_NAME).l
	@yacc -d $(FILE_NAME).y
	@clang-3.8 -o $(FILE_NAME) lex.yy.c y.tab.c -ll -ly 
	@zip $(FILE_NAME).zip $(FILE_NAME).l $(FILE_NAME).y
	@echo "[INFO] Compiled"

clean:
	@rm y.tab.c lex.yy.c $(FILE_NAME) $(FILE_NAME).zip
	echo "[INFO] Cleaned"