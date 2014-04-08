GRAMMAR = parser.y

CFLAGS = -I. -funsigned-char -g -DYYDEBUG 	
YFLAGS = -v -d

mcc: y.tab.o lex.yy.o tree.o driver.o symtab.o
	gcc $(CFLAGS) -o mcc y.tab.o driver.o lex.yy.o tree.o symtab.o -ll

y.tab.o: y.tab.c y.tab.h 
	gcc $(CFLAGS) -c y.tab.c 

y.tab.c: $(GRAMMAR)
	yacc $(YFLAGS) $(GRAMMAR)

lex.yy.o: lex.yy.c y.tab.h 
	gcc $(CFLAGS) -c lex.yy.c

lex.yy.c: scanner.l
	lex scanner.l

driver.o: driver.c
	gcc $(CFLAGS) -c driver.c

tree.o: tree.c tree.h
	gcc $(CFLAGS) -c tree.c

symtab.o: symtab.c symtab.h
	gcc $(CFLAGS) -c symtab.c

clean:
	rm -f y.tab.* y.output lex.yy.* *.o *~ mcc     



