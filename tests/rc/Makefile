
YYPREFIX=rig_yy
FILEPREFIX=rig

all:	${FILEPREFIX}.tab.c lex.${YYPREFIX}.c

${FILEPREFIX}.tab.c:	parser.y 
	bison -d --defines -t -v -p ${YYPREFIX} -b ${FILEPREFIX} parser.y
	echo "#define yylval "${YYPREFIX}"lval" >> ${FILEPREFIX}.tab.h
	echo "#define yylineno "${YYPREFIX}"lineno" >> ${FILEPREFIX}.tab.h
	echo "#define yyparse "${YYPREFIX}"parse" >> ${FILEPREFIX}.tab.h
	cp ${FILEPREFIX}.tab.[ch] ..

lex.${YYPREFIX}.c:	scanner.l
	flex -I -P${YYPREFIX} scanner.l
	cp lex.${YYPREFIX}.c ..

# Same as parser.y except no code--just grammar
base:	base.y scanner.l
	bison -d -t -v base.y
	flex -I scanner.l
	gcc lex.yy.c base.tab.c -o base -I../../include

header:
	cp lex_rigs.h ../
	cp rigcmd.h ../

clean:
	rm *tab.[ch] *.output lex.*.c ../*tab.[ch] ../lex* base
