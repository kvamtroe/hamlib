%{
#include <hamlib/rig.h>
#include "ts2k.tab.h"
#include <string.h>

%}

%option noyywrap
%option yylineno

%%

[ \t]+	/* empty */;
	/* Keywords */
close	{ return RIG_TOKEN_CLOSE; }
exit	{ return RIG_TOKEN_EXIT; }
freq	{ return RIG_TOKEN_FREQ; }
help	{ return RIG_TOKEN_HELP; }
init	{ return RIG_TOKEN_INIT; }
open	{ return RIG_TOKEN_OPEN; }
model	{ ts2k_yylval.val = 0; return RIG_TOKEN_MODEL; }
port	{ ts2k_yylval.val = 0; return RIG_TOKEN_PORT; }
rig	{ ts2k_yylval.val = 0; return RIG_TOKEN_RIG; }
setup	{ return RIG_TOKEN_SETUP; }
vfo	{ return RIG_TOKEN_VFO; }

	/* Super functions available only via the parser! */
"a=b" |
"A=B"	{ return RIG_TOKEN_AEQB; }
"b=a" |
"B=A"	{ return RIG_TOKEN_BEQA; }
"a=c" |
"A=C"	{ return RIG_TOKEN_AEQC; }
"c=a" |
"C=A"	{ return RIG_TOKEN_CEQA; }
"b=c" |
"B=C"	{ return RIG_TOKEN_BEQC; }
"c=b" |
"C=B"	{ return RIG_TOKEN_CEQB; }
"a/b" |
"A/B"	{ return RIG_TOKEN_ASWB; }
"a/c" |
"A/C"	{ return RIG_TOKEN_ASWC; }

	/* constants for us lazy people */
GHz	{ ts2k_yylval.fval = 1e9; return RIG_TOKEN_GHZ; }
MHz	{ ts2k_yylval.fval = 1e6; return RIG_TOKEN_MHZ; }
kHz	{ ts2k_yylval.fval = 1e3; return RIG_TOKEN_KHZ; }
Hz	{ ts2k_yylval.fval = 1; return RIG_TOKEN_HZ; }

	/* models */
dummy	{ ts2k_yylval.model = RIG_MODEL_DUMMY; return RIG_TOKEN_DUMMY; }
ts2k	{ ts2k_yylval.model = RIG_MODEL_TS2K; return RIG_TOKEN_TS2K; }
ts2000	{ ts2k_yylval.model = RIG_MODEL_TS2000; return RIG_TOKEN_TS2000; }

	/* VFO constants */
	/* minors */
CALL_A	{ ts2k_yylval.vfo = RIG_VFO_CALL_A; return RIG_LEX_CALLA; }
CALL_C	{ ts2k_yylval.vfo = RIG_VFO_CALL_C; return RIG_LEX_CALLC; }
MEM_A	{ ts2k_yylval.vfo = RIG_VFO_MEM_A; return RIG_LEX_MEMA; }
MEM_C	{ ts2k_yylval.vfo = RIG_VFO_MEM_C; return RIG_LEX_MEMC; }
VFO_A	{ ts2k_yylval.vfo = RIG_VFO_A; return RIG_LEX_VFOA; }
VFO_B	{ ts2k_yylval.vfo = RIG_VFO_B; return RIG_LEX_VFOB; }
VFO_C	{ ts2k_yylval.vfo = RIG_VFO_C; return RIG_LEX_VFOC; }
VFO_AB	{ ts2k_yylval.vfo = RIG_VFO_AB; return RIG_LEX_VFOAB; }
VFO_BA	{ ts2k_yylval.vfo = RIG_VFO_BA; return RIG_LEX_VFOBA; }
	/* majors */
FAKE	{ ts2k_yylval.vfo = RIG_CTRL_FAKE; return RIG_LEX_FAKE; }
MAIN	{ ts2k_yylval.vfo = RIG_CTRL_MAIN; return RIG_LEX_MAIN; }
MEM	{ ts2k_yylval.vfo = RIG_CTRL_MEM; return RIG_LEX_MEM; }
RIT	{ ts2k_yylval.vfo = RIG_CTRL_RIT; return RIG_LEX_RIT; }
SAT	{ ts2k_yylval.vfo = RIG_CTRL_SAT; return RIG_LEX_SAT; }
SCAN	{ ts2k_yylval.vfo = RIG_CTRL_SCAN; return RIG_LEX_SCAN; }
SUB	{ ts2k_yylval.vfo = RIG_CTRL_SUB; return RIG_LEX_SUB; }
XIT	{ ts2k_yylval.vfo = RIG_CTRL_XIT; return RIG_LEX_XIT; }

\"[^"\n]*["\n]	{	/* we have to lop off the " on either side... :( */
		ts2k_yylval.txt = strdup(&yytext[1]);
		ts2k_yylval.txt[yyleng-2] = '\0';
		if(yytext[yyleng-2] == '\n');
		return RIG_TOKEN_STRING;
	}

[0-9]+	{ ts2k_yylval.val = atoi(yytext); return RIG_TOKEN_INT; }

"//".*\n	;

.	{ if(yytext[0] == '\n') ; return yytext[0]; }

%%

/* Notes:
	[:digit:]+ doesn't work!  I spent a couple hours debugging that one!
	There are two copies of the interpreter.  src/rc, and tests/rc.
		src/rc is non-interactive, tests/rc is interactive.  They
		want to be merged if possible.
	Very very very basic, but it works!
	C++ style comments "//" work in bison but *not* in flex!!
*/
