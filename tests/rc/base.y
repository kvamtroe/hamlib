%{
/*
 * parser.y	(C) Copyright 2002 by Dale E. Edmons.  All rights reserved.
 *
 *	license:	GNU
 */

/*
 * Notes:	This parser and scanner are being written "on the fly".
 *	There is no design spec.  I'm using a c/c++ type format as a
 *	general rule.  Anybody is welcome to design and write something
 *	better.  For now, this seems to be working out well.	--Dale
 */

#include <hamlib/rig.h>
#include <string.h>
#include <stdlib.h>
#include "../src/misc.h"
#include "../rigcmd.h"

#define RIG_YY_MAXRIGS	10
#define YYERROR_VERBOSE
#define _YY_BASE_TEST 1

#define YYDEBUG 1
extern char *yytext;

static RIG *rigs[RIG_YY_MAXRIGS];
static rig_model_t models[RIG_YY_MAXRIGS];
static char *ports[RIG_YY_MAXRIGS];
static channel_t chans[RIG_YY_MAXRIGS];
static int default_idx = 0;	// Default index if none specified.
static vfo_t default_vfo = RIG_VFO_CURR;	// Default VFO

extern int yylineno;

int setup(RIG *rig, rig_model_t model, char *port);
//int rig_setup(RIG *rig, rig_model_t model, char *port);
void help(void);
void yyerror(char *);
int s_cpy(RIG *, vfo_t, vfo_t);
int s_sw(RIG *, vfo_t, vfo_t);

spec_t specs[RIG_YY_MAXRIGS];

%}

%union {
	rig_model_t	model;
	freq_t		freq;
	vfo_t		vfo;
	char		*txt;
	short int	sval;
	long int	val;
	float		fval;
	spec_t		spec;
};

%left	','
%right	'='
%left	'|'
%left	'^'
%left	'&'
%left	'+' '-'
%left	'*' '/'
%right	'~' RIG_YY_UMINUS RIG_TOKEN_PLUSPLUS RIG_TOKEN_MINUSMINUS
%left	'[' ']' '{' '}'
%left	'.' '(' ')'


%token <spec>
	RIG_LEX_MODEL
	RIG_TOKEN_CLOSE
	RIG_TOKEN_CTCSS
	RIG_TOKEN_DCS
	RIG_TOKEN_DEBUG
	RIG_TOKEN_EXIT
	RIG_TOKEN_FLOAT
	RIG_TOKEN_FM
	RIG_TOKEN_FREQ
	RIG_TOKEN_HELP
	RIG_TOKEN_IDENTIFIER
	RIG_TOKEN_INIT
	RIG_TOKEN_INT
	RIG_TOKEN_OPEN
	RIG_TOKEN_MINUSMINUS
	RIG_TOKEN_TONE
	RIG_TOKEN_MODE
	RIG_TOKEN_MODEL
	RIG_TOKEN_OFFSET
	RIG_TOKEN_PORT
	RIG_TOKEN_PLUSPLUS
	RIG_TOKEN_RIG
	RIG_TOKEN_SETUP
	RIG_TOKEN_SHIFT
	RIG_TOKEN_STEP
	RIG_TOKEN_STRING
	RIG_TOKEN_TONE
	RIG_TOKEN_AEQB
	RIG_TOKEN_BEQA
	RIG_TOKEN_AEQC
	RIG_TOKEN_CEQA
	RIG_TOKEN_BEQC
	RIG_TOKEN_CEQB
	RIG_TOKEN_ASWB
	RIG_TOKEN_ASWC
	RIG_TOKEN_GHZ
	RIG_TOKEN_MHZ
	RIG_TOKEN_KHZ
	RIG_TOKEN_HZ
	RIG_LEX_VFO
	RIG_TOKEN_CALL
	RIG_TOKEN_CURR
	RIG_TOKEN_MAIN
	RIG_TOKEN_MEM
	RIG_TOKEN_SCAN
	RIG_TOKEN_SUB
	RIG_TOKEN_VFO

%token <vfo>

%type <spec>
	top
	error
	rig_list
	rig_cmd
	rig_rhs
	rig_lhs
	rig_prefix
	rig_parms
	rig_expr
	rig_function
	parm_expr
	chan_prefix	
	chan
	expr
	idx_expr
	vfo_expr

%type <val>
	

%%

top:
		rig_list
	;

rig_list:
		rig_cmd ';'
	|	rig_list rig_cmd ';'
	|	error ';'
	;

rig_cmd:
		rig_rhs
	|	rig_lhs '=' rig_cmd
	|	rig_lhs '=' rig_expr
	;

rig_lhs:
		rig_function
	|	rig_prefix rig_function
	;

rig_rhs:
		rig_function '(' ')'			/* get function */
	|	rig_prefix rig_function '(' ')'		/* rig.vfo.func() */
	|	rig_function '(' rig_parms ')'			/* set function */
	|	rig_prefix rig_function '(' rig_parms ')'	/* rig.vfo.func() */
	;

rig_expr:
		parm_expr
	;

rig_parms:
		parm_expr
	|	rig_parms ',' parm_expr
	;

parm_expr:
		RIG_LEX_VFO
	|	RIG_LEX_MODEL
	|	RIG_TOKEN_RIG
	|	RIG_TOKEN_RIG '[' idx_expr ']'
	|	RIG_TOKEN_MODEL
	|	RIG_TOKEN_MODEL '[' idx_expr ']'
	|	RIG_TOKEN_PORT
	|	RIG_TOKEN_PORT '[' idx_expr ']'
	|	RIG_TOKEN_FLOAT RIG_TOKEN_GHZ
	|	RIG_TOKEN_FLOAT RIG_TOKEN_MHZ
	|	RIG_TOKEN_FLOAT RIG_TOKEN_KHZ
	|	RIG_TOKEN_FLOAT RIG_TOKEN_HZ
	|	RIG_TOKEN_FLOAT
	|	RIG_TOKEN_INT
	|	RIG_TOKEN_INT RIG_TOKEN_GHZ
	|	RIG_TOKEN_INT RIG_TOKEN_MHZ
	|	RIG_TOKEN_INT RIG_TOKEN_KHZ
	|	RIG_TOKEN_INT RIG_TOKEN_HZ
	|	RIG_TOKEN_STRING
	;
		

rig_function:
		RIG_TOKEN_FREQ
	|	RIG_TOKEN_MODE
	|	RIG_TOKEN_SCAN
	|	RIG_TOKEN_STEP
	|	RIG_TOKEN_CTCSS
	|	RIG_TOKEN_TONE
	|	RIG_TOKEN_DCS
	|	RIG_TOKEN_MODEL
	|	RIG_TOKEN_MODEL '[' idx_expr ']'
	|	RIG_TOKEN_PORT
	|	RIG_TOKEN_PORT '[' idx_expr ']'
	|	RIG_TOKEN_OPEN
	|	RIG_TOKEN_CLOSE
	;

rig_prefix:
		RIG_TOKEN_RIG  '.'
	|	RIG_TOKEN_RIG  '[' idx_expr ']' '.'
	|	RIG_TOKEN_RIG  '[' idx_expr ']' '.' chan_prefix
	|	chan_prefix
	|	RIG_TOKEN_RIG  '.' chan_prefix
	|	RIG_TOKEN_RIG  '{' rig_list '}'	/* Apply rig_prefix to all rig_list */
	|	RIG_TOKEN_RIG  '[' idx_expr ']' '{' rig_list '}'
	|	RIG_TOKEN_RIG  '[' idx_expr ']' '.' chan_prefix '{' rig_list '}'
	|	chan_prefix '{' rig_list '}'
	;

chan_prefix:
		chan '.'
	|	chan_prefix '.' chan '.'
	;

chan:
		RIG_TOKEN_CALL
	|	RIG_TOKEN_MAIN
	|	RIG_TOKEN_CURR
	|	RIG_TOKEN_MEM
	|	RIG_TOKEN_SUB
	|	RIG_TOKEN_CALL '(' vfo_expr ')'
	|	RIG_TOKEN_MEM '(' vfo_expr ')'	/* set MEMA or MEMC */
	|	RIG_TOKEN_MEM '(' idx_expr ')'	/* set channel # */
	|	RIG_TOKEN_VFO '(' vfo_expr ')'
	;

vfo_expr:
		RIG_LEX_VFO
	|	vfo_expr '|' RIG_LEX_VFO
	|	'~' vfo_expr		{ $$ = $2; }
	|	'(' vfo_expr ')'	{ $$ = $2; }
	;

idx_expr:
		expr
	|	'+' expr	%prec RIG_YY_UMINUS	{ $<val>$ = $<val>2; }
	|	'-' expr	%prec RIG_YY_UMINUS	{ $<val>$ = -$<val>2; }
	|	idx_expr '+' expr
	|	idx_expr '-' expr
	|	idx_expr '*' expr
	|	idx_expr '/' expr
	|	expr RIG_TOKEN_PLUSPLUS
	|	expr RIG_TOKEN_MINUSMINUS
	;

expr:
		RIG_TOKEN_INT
	;
%%

main()
{
	yydebug = 1;
//	do {
		fprintf(stderr, "\nrigcmd> ");
		yyparse();
//	} while(yytext[0] != EOF);
}

void yyerror(char *err)
{
	//rig_debug(RIG_DEBUG_ERR, "cmd_parse: %s on line %i\n", err, yylineno);
	fprintf(stderr, __FUNCTION__": %s on line %i\n", err, yylineno);
}

void help(void)
{
	//rig_debug(RIG_DEBUG_ERR, "\n"__FUNCTION__":\n"
	fprintf(stdout, "\n"__FUNCTION__":\n"
		"Functions:	close, help, open\n"
		"Arrays:	model[[%i]], port[[%i]], rig[[%i]]\n"
		"Examples:\n\tport = \"/dev/ttyS1\";\n"
		"\tmodel = ts2k;\n"
		"\trig.open(model, port);\n"
		"\tport[2] = \"/dev/ttyS0\";\n"
		"\tmodel[2] = ts2k;\n"
		"\topen(rig[2], model[2], port[2]);\n"
		"\tclose(rig[0]);\t// rig == rig[0]\n"
		"\tclose(rig[2]);\t// This is a comment\n",
		RIG_YY_MAXRIGS, RIG_YY_MAXRIGS, RIG_YY_MAXRIGS
	);
	return;
}
