%{
//#include <hamlib/rig.h>

#include "rigcmd.h"

%}

%option noyywrap
%option yylineno
%option always-interactive

%%

[ \t]+	/* empty */;
	/* Keywords */
am |
AM	{ return RIG_TOKEN_AM; }
call	{ return RIG_TOKEN_CALL; }
ctcss	{ return RIG_TOKEN_CTCSS; }
curr	{ return RIG_TOKEN_CURR; }
cw |
CW	{ return RIG_TOKEN_CW; }
exit	{ return RIG_TOKEN_EXIT; }
fm |
FM	{ return RIG_TOKEN_FM; }
fsk |
FSK |
RTTY	{ return RIG_TOKEN_RTTY; }
help	{ return RIG_TOKEN_HELP; }
init	{ return RIG_TOKEN_INIT; }
lsb |
LSB	{ return RIG_TOKEN_LSB; }
main	{ return RIG_TOKEN_MAIN; }
mem	{ return RIG_TOKEN_MEM; }
menu	{ return RIG_TOKEN_MENU; }
model	{ yylval.val = 0; return RIG_TOKEN_MODEL; }
offset	{ return RIG_TOKEN_OFFSET; }
port	{ yylval.val = 0; return RIG_TOKEN_PORT; }
pm	{ return RIG_TOKEN_PM; }
print	{ return RIG_TOKEN_PRINT; }
rig	{ yylval.val = 0; return RIG_TOKEN_RIG; }
setup	{ return RIG_TOKEN_SETUP; }
shift	{ return RIG_TOKEN_SHIFT; }
sub	{ return RIG_TOKEN_SUB; }
tone	{ return RIG_TOKEN_TONE; }
usb |
USB	{ return RIG_TOKEN_USB; }
rig_default		{ return RIG_TOKEN_RIGDEFAULT; }

	/* Super functions available only via the parser! */
"a>b" |
"b<a" |
"b=a" |
"A>B" |
"B<A" |
"B=A"	{ return RIG_TOKEN_BEQA; }	// A copied to B 
"a>c" |
"c<a" |
"c=a" |
"A>C" |
"C<A" |
"C=A"	{ return RIG_TOKEN_CEQA; }	// A copied to C
"b>a" |
"a<b" |
"a=b" |
"B>A" |
"A<B" |
"A=B"	{ return RIG_TOKEN_AEQB; }	// B copied to A
"b>c" |
"c<b" |
"c=b" |
"B>C" |
"C<B" |
"C=B"	{ return RIG_TOKEN_CEQB; }	// B copied to C
"c>a" |
"a<c" |
"a=c" |
"c>A" |
"A<C" |
"A=C"	{ return RIG_TOKEN_AEQC; }	// C copied to A
"b>a" |
"a<b" |
"a=b" |
"B>A" |
"A<B" |
"A=B"	{ return RIG_TOKEN_BEQC; }	// C copied to B
"a>m" |
"m<a" |
"m=a" |
"A>M" |
"M<A" |
"M=A"	{ return RIG_TOKEN_MEQA; }	// A copied to M
"b>m" |
"m<b" |
"b=m" |
"B>M" |
"M<B" |
"M=B"	{ return RIG_TOKEN_MEQB; }	// B copied to M
"c>m" |
"m<c" |
"m=c" |
"C>M" |
"M<C" |
"M=C"	{ return RIG_TOKEN_MEQC; }	// C copied to M
"m>a" |
"a<m" |
"m=a" |
"m>A" |
"A<M" |
"M=A"	{ return RIG_TOKEN_AEQM; }	// M copied to A
"m>b" |
"b<m" |
"b=m" |
"M>B" |
"B<M" |
"B=M"	{ return RIG_TOKEN_BEQM; }	// M copied to B
"m>c" |
"c<m" |
"c=m" |
"M>C" |
"C<M" |
"C=M"	{ return RIG_TOKEN_CEQM; }	// M copied to C
"a/b" |
"b/a" |
"A/B" |
"B/A"	{ return RIG_TOKEN_ASWB; }	// A->tmp, B->A, tmp->B
"a/c" |
"c/a" |
"A/C" |
"C/A"	{ return RIG_TOKEN_ASWC; }	// A->tmp, C->A, tmp->C
"b/c" |
"c/b" |
"B/C" |
"C/B"	{ return RIG_TOKEN_BSWC; }	// B->tmp, C->B, tmp->C
"a/m" |
"m/a" |
"A/M" |
"M/A"	{ return RIG_TOKEN_ASWM; }	// A->tmp, M->A, tmp->M
"b/m" |
"m/b" |
"B/M" |
"M/B"	{ return RIG_TOKEN_BSWM; }	// B->tmp, M->B, tmp->M
"c/m" |
"m/c" |
"C/M" |
"M/C"	{ return RIG_TOKEN_CSWM; }	// C->tmp, M->C, tmp->M

"++"	{ return RIG_TOKEN_PLUSPLUS; }
"--"	{ return RIG_TOKEN_MINUSMINUS; }
"-="	{ return RIG_TOKEN_MINUSEQ; }
"+="	{ return RIG_TOKEN_PLUSEQ; }

	/* constants for us lazy people */
GHz	{ yylval.fval = 1e9; return RIG_TOKEN_GHZ; }
MHz	{ yylval.fval = 1e6; return RIG_TOKEN_MHZ; }
kHz	{ yylval.fval = 1e3; return RIG_TOKEN_KHZ; }
Hz	{ yylval.fval = 1e0; return RIG_TOKEN_HZ; }

	/* models (only a few are lowercase;  the rest are a table */
dummy	{ yylval.model = RIG_MODEL_DUMMY; return RIG_LEX_MODEL; }
ts2k	{ yylval.model = RIG_MODEL_TS2K; return RIG_LEX_MODEL; }
rpcrig  |
mmelter |
moonmelter	{ yylval.model = RIG_MODEL_RPC; return RIG_LEX_MODEL; }


\"[^"\n]*["\n]	{	/* we have to lop off the " on either side... :( */
		yylval.txt = strdup(&yytext[1]);
		yylval.txt[yyleng-2] = '\0';
		if(yytext[yyleng-2] == '\n');
		return RIG_TOKEN_STRING;
	}

[+-]?[0-9]*\.[0-9]+([eE]?[0-9]+)? |
[+-]?[0-9]+\.[0-9]*([eE]?[0-9]+)? |
[+-]?[0-9]+[eE][0-9]+ 	{ yylval.fval = atof(yytext); return RIG_TOKEN_FLOAT; }

[0-9]+	{ yylval.val = atoi(yytext); return RIG_TOKEN_INT; }

[a-zA-Z_]+[a-zA-Z_0-9]*	{ return keyword(yytext); }

"//".*\n	;


.	{ return yytext[0]; }


%%

/* Notes:
	[:digit:]+ doesn't work!  I spent a couple hours debugging that one!
	There are two copies of the interpreter.  src/rc, and tests/rc.
		src/rc is non-interactive, tests/rc is interactive.  They
		want to be merged if possible.
	Very very very basic, but it works!
	C++ style comments "//" work in bison but *not* in flex!!
*/

