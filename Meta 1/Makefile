FILE_NAME = jac

all:
	@lex $(FILE_NAME).l
	@clang-3.8 -o $(FILE_NAME) lex.yy.c 
	@zip $(FILE_NAME).zip $(FILE_NAME).l
	@echo "[INFO] Compiled"
	@echo "[INFO] Testing example"
	@./$(FILE_NAME) -l < testestrings.txt

clean:
	@rm lex.yy.c $(FILE_NAME) $(FILE_NAME).zip
	echo "[INFO] Cleaned"