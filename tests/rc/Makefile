
all:	ts2k.tab.c lex.ts2k_yy.c

ts2k.tab.c:	parser.y 
	bison -d -t -v -p ts2k_yy -b ts2k parser.y
	cp ts2k.tab.[ch] ..

lex.ts2k_yy.c:	scanner.l
	flex -I -Pts2k_yy  scanner.l
	cp lex.ts2k_yy.c ..

clean:
	rm *tab.[ch] *.output lex.*.c
