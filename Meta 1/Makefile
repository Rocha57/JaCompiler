FILE_NAME = jac

all:
	@lex $(FILE_NAME).l
	@clang-3.8 -o $(FILE_NAME) lex.yy.c 
	@#zip $(FILE_NAME).zip $(FILE_NAME).l
	@echo "[INFO] Compiled"

clean:
	@rm lex.yy.c $(FILE_NAME) $(FILE_NAME).zip
	echo "[INFO] Cleaned"