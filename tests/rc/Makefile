
YYPREFIX=rig_yy
FILEPREFIX=rig

all:	header ${FILEPREFIX}.tab.c lex.${YYPREFIX}.c ../rigcmd.c

${FILEPREFIX}.tab.c:	parser.y rigcmd.h tree.h
	bison -d --defines -t -v -p ${YYPREFIX} -b ${FILEPREFIX} parser.y
	cp ${FILEPREFIX}.tab.[ch] ..

lex.${YYPREFIX}.c:	scanner.l rigcmd.h tree.h
	flex -I -P${YYPREFIX} scanner.l
	cp lex.${YYPREFIX}.c ..

# Same as parser.y except no code--just grammar
base:	base.y scanner.l rigcmd.h rigcmd.c tree.h lex_keywords.h lex_rigs.h
	bison -d -t -v base.y
	flex -I scanner.l
	gcc lex.yy.c base.tab.c rigcmd.c -o base -I../../include

header:	rigcmd.h lex_rigs.h tree.h rigcmd.c parser.y scanner.l lex_rotors.h
#../rigcmd.c:	rigcmd.h lex_rigs.h rigcmd.c tree.h
#../rigcmd.c:	rigcmd.h lex_rigs.h tree.h rigcmd.c parser.y scanner.l
	cp tree.h ../
	cp lex_*.h ../
	cp rigcmd.[ch] ../
	bin/mk_hdr libhamlib.so

clean:
	rm *tab.[ch] *.output lex.*.c ../*tab.[ch] ../lex* base ../tree.h
