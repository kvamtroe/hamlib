%{
#include <hamlib/rig.h>

//#define _YY_BASE_TEST
#undef _YY_BASE_TEST

#ifdef _YY_BASE_TEST
# include "rigcmd.h"
# include "base.tab.h"
#else
# include "rigcmd.h"
# include "rig.tab.h"
#endif

#include "lex_rigs.h"
#include <string.h>

struct {
	const char *name;
	vfo_t vfo;
} lex_vfos[] = {
	{ "A", RIG_VFO_A}, { "B", RIG_VFO_B}, { "C", RIG_VFO_C},
	{ "VFOA", RIG_VFO_A}, { "vfoa", RIG_VFO_A}, { "VFO_A", RIG_VFO_A},
	{ "VFOB", RIG_VFO_B}, { "vfob", RIG_VFO_B}, { "VFO_B", RIG_VFO_B},
	{ "VFOC", RIG_VFO_C}, { "vfoc", RIG_VFO_C}, { "VFO_C", RIG_VFO_C},
	{ "VFOAB", RIG_VFO_AB}, { "vfoab", RIG_VFO_AB}, { "VFO_AB", RIG_VFO_AB},
	{ "VFOBA", RIG_VFO_BA}, { "vfoba", RIG_VFO_BA}, { "VFO_BA", RIG_VFO_BA},
	{ "MEMA", RIG_VFO_MEM_A}, { "mema", RIG_VFO_MEM_A}, { "MEM_A", RIG_VFO_MEM_A},
	{ "MEMC", RIG_VFO_MEM_C}, { "memc", RIG_VFO_MEM_C}, { "MEM_C", RIG_VFO_MEM_C},
	{ "CALLA", RIG_VFO_CALL_A}, { "calla", RIG_VFO_CALL_A}, { "CALL_A", RIG_VFO_CALL_A},
	{ "CALLC", RIG_VFO_CALL_C}, { "callc", RIG_VFO_CALL_C}, { "CALL_C", RIG_VFO_CALL_C},

	{ "VFO1", RIG_VFO1},
	{ "VFO2", RIG_VFO2},
	{ "VFO3", RIG_VFO3},
	{ "VFO4", RIG_VFO4},
	{ "VFO5", RIG_VFO5},

	{ "PTT", RIG_VFO_PTT},
	{ "CTRL", RIG_VFO_CTRL},

	{ "FAKE", RIG_CTRL_FAKE},
	{ "MAIN", RIG_CTRL_MAIN}, { "Main", RIG_CTRL_MAIN},
	{ "SUB", RIG_CTRL_SUB}, { "Sub", RIG_CTRL_SUB},
	{ "MEM", RIG_CTRL_MEM},
	{ "CALL", RIG_CTRL_CALL},
	{ "SPLIT", RIG_CTRL_SPLIT},
	{ "REV", RIG_CTRL_REV},
	{ "RIT", RIG_CTRL_RIT},
	{ "SAT", RIG_CTRL_SAT},
	{ "SCAN", RIG_CTRL_SCAN},
	{ "XIT", RIG_CTRL_XIT},
	{ "CROSS", RIG_CTRL_CROSS},

	{ NULL, 0 }
};

%}

%option noyywrap
%option yylineno
%option always-interactive

%%

[ \t]+	/* empty */;
	/* Keywords */
call	{ return RIG_TOKEN_CALL; }
close	{ return RIG_TOKEN_CLOSE; }
curr	{ return RIG_TOKEN_CURR; }
debug	{ return RIG_TOKEN_DEBUG; }
exit	{ return RIG_TOKEN_EXIT; }
freq	{ return RIG_TOKEN_FREQ; }
help	{ return RIG_TOKEN_HELP; }
init	{ return RIG_TOKEN_INIT; }
open	{ return RIG_TOKEN_OPEN; }
offset	{ return RIG_TOKEN_OFFSET; }
main	{ return RIG_TOKEN_MAIN; }
mem	{ return RIG_TOKEN_MEM; }
mode	{ yylval.val = 0; return RIG_TOKEN_MODE; }
model	{ yylval.val = 0; return RIG_TOKEN_MODEL; }
port	{ yylval.val = 0; return RIG_TOKEN_PORT; }
rig	{ yylval.val = 0; return RIG_TOKEN_RIG; }
setup	{ return RIG_TOKEN_SETUP; }
shift	{ return RIG_TOKEN_SHIFT; }
sub	{ return RIG_TOKEN_SUB; }
tone	{ return RIG_TOKEN_TONE; }
vfo	{ return RIG_TOKEN_VFO; }
FM	{ return RIG_TOKEN_FM; }

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

"++"	{ return RIG_TOKEN_PLUSPLUS; }
"--"	{ return RIG_TOKEN_MINUSMINUS; }

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

/*
 * Checks any remaining keywords and returns the appropriate info.
 *	I only do a straight linear search.
 */
int keyword(char * token)
{
	int i, j;

	//rig_debug(RIG_DEBUG_VERBOSE, __FUNCTION__": %s\n", token);
	fprintf(stdout, __FUNCTION__": %s\n", token);
	// VFOs (often used)
	for(i=0; lex_vfos[i].name != NULL; i++) {
		for(j=0; lex_vfos[i].name[j] == token[j]; j++) {
			if((lex_vfos[i].name[j] == '\0')
			   && (token[j] == '\0')) {	// exact match only
				yylval.vfo = lex_vfos[i].vfo;
				return RIG_LEX_VFO;
			}
		}
	}
	// Nope, not a vfo_id

	// Models (least used, so last!)
	for(i=0; lex_riglist[i].name != NULL; i++) {
		for(j=0; lex_riglist[i].name[j] == token[j]; j++) {
			if((lex_riglist[i].name[j] == '\0')
			   && (token[j] == '\0')) {	// exact match only
				yylval.model = lex_riglist[i].model;
				return RIG_LEX_MODEL;
			}
		}
	}
	// Nope, not a model_id

	yylval.txt = strdup(token);
	return RIG_TOKEN_IDENTIFIER;
}
