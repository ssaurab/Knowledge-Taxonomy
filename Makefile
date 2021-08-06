a.out: y.tab.c lex.yy.c
	reset
	g++ lex.yy.c y.tab.c main.cpp -o program
	@echo "Run the program as ./program taxonomy.txt facts.txt conceptDefinitions.txt"
	./program taxonomy.txt facts.txt conceptDefinitions.txt
y.tab.c: a2.y 
	yacc -d a2.y

lex.yy.c: a2.l y.tab.h
	lex a2.l

clean:
	@rm lex.yy.c y.tab.h y.tab.c program

