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
#define _YY_RIG_PARSER
#include "rigcmd.h"

#define YYERROR_VERBOSE

#define YYDEBUG 1
extern char *yytext;

static RIG *rigs[RIG_YY_MAXRIGS];
static rig_model_t models[RIG_YY_MAXRIGS];
static char *ports[RIG_YY_MAXRIGS];
static channel_t chans[RIG_YY_MAXRIGS];
static int default_idx = 0;	// Default index if none specified.
static vfo_t default_vfo = RIG_VFO_CURR;	// Default VFO

tree_t trees[RIG_YY_MAXRIGS];

%}

%union {
	rig_model_t	model;
	freq_t		freq;
	vfo_t		vfo;
	char		*txt;
	short int	sval;
	long int	val;
	float		fval;
	tree_t		*tree;
	symtab_t	sym;
};

%left	',' '.'
%right	'=' RIG_TOKEN_PLUSEQ RIG_TOKEN_MINUSEQ RIG_TOKEN_OREQ RIG_TOKEN_ANDEQ 
%left	'|'
%left	'^'
%left	'&'
%left	'+' '-'
%left	'*' '/'
%right	'~' RIG_TOKEN_UMINUS RIG_TOKEN_PLUSPLUS RIG_TOKEN_MINUSMINUS
//%left	'{' '}' '(' ')' '[' ']' '.'
%left	'{' '}' '(' ')' '[' ']'

%token <tree>
	RIG_TOKEN_AM
	RIG_TOKEN_CW
	RIG_TOKEN_FM
	RIG_TOKEN_LSB
	RIG_TOKEN_USB
	RIG_TOKEN_RTTY
	RIG_TOKEN_CALL
	RIG_TOKEN_CHAN
	RIG_TOKEN_CLOSE
	RIG_TOKEN_CTCSS
	RIG_TOKEN_CURR
	RIG_TOKEN_DCS
	RIG_TOKEN_DEBUG
	RIG_TOKEN_DNLINK
	RIG_TOKEN_EXIT
	RIG_TOKEN_FLOAT
	RIG_TOKEN_FM
	RIG_TOKEN_FUNC
	RIG_TOKEN_HELP
	RIG_TOKEN_IDENT
	RIG_TOKEN_INIT
	RIG_TOKEN_INT
	RIG_LEX_MODEL
	RIG_LEX_HAMLIB
	RIG_LEX_VFO
	RIG_TOKEN_MEM
	RIG_TOKEN_MENU
	RIG_TOKEN_MAIN
	RIG_TOKEN_MINUSMINUS
	RIG_TOKEN_MODEL
	RIG_TOKEN_OFFSET
	RIG_TOKEN_PORT
	RIG_TOKEN_PRINT
	RIG_TOKEN_PLUSPLUS
	RIG_TOKEN_PM
	RIG_TOKEN_RIGDEFAULT
	RIG_TOKEN_SAT
	RIG_TOKEN_SCAN
	RIG_TOKEN_SETUP
	RIG_TOKEN_SHIFT
	RIG_TOKEN_SPLIT
	RIG_TOKEN_STEP
	RIG_TOKEN_STRING
	RIG_TOKEN_SUB
	RIG_TOKEN_TONE
	RIG_TOKEN_UPLINK
	RIG_TOKEN_VFO
	RIG_TOKEN_RIG
	RIG_TOKEN_DO	/* c/c++ style keywords */
	RIG_TOKEN_ELSE
	RIG_TOKEN_FOR
	RIG_TOKEN_IF
	RIG_TOKEN_WHILE
	RIG_TOKEN_BEQA	/* button type functions */
	RIG_TOKEN_CEQA
	RIG_TOKEN_MEQA
	RIG_TOKEN_AEQB
	RIG_TOKEN_CEQB
	RIG_TOKEN_MEQB
	RIG_TOKEN_AEQC
	RIG_TOKEN_BEQC
	RIG_TOKEN_MEQC
	RIG_TOKEN_AEQM
	RIG_TOKEN_BEQM
	RIG_TOKEN_CEQM
	RIG_TOKEN_ASWB
	RIG_TOKEN_ASWC
	RIG_TOKEN_BSWC
	RIG_TOKEN_ASWM
	RIG_TOKEN_BSWM
	RIG_TOKEN_CSWM
	RIG_TOKEN_GHZ	/* hertz multipliers */
	RIG_TOKEN_MHZ
	RIG_TOKEN_KHZ
	RIG_TOKEN_HZ

%token <vfo>

%type <tree>
	top
	rig_list
	rig_cmd
	expr
//	rig_id
//	vfo_id
	assign_lhs

%type <val>

%%

top:
		rig_list			{ tst("top: rig_list"); }
	;

rig_list:
		rig_cmd ';'			{ tst("rig_list: rig_cmd ;"); }
	|	rig_list rig_cmd ';'		{ tst("rig_list: rig_cmd ;"); }
	|	error ';'			{ tst("rig_list: error ;"); yyerror("Geez...!\n"); }
	;

rig_cmd:
		expr				{ tst("rig_cmd: expr"); }
	|	RIG_TOKEN_PRINT	identifier	{ tst("rig_cmd: RIG_TOKEN_PRINT;"); }
	|	RIG_TOKEN_HELP			{ tst("rig_cmd: RIG_TOKEN_HELP ;"); help(); }
	|	RIG_TOKEN_EXIT			{ tst("rig_cmd: RIG_TOKEN_EXIT ;"); exit(0); }
	;

expr:
		arith_expr			{ tst("expr: arith_expr"); }
//	|	expr ',' arith_expr		{ tst("expr: expr , arith_expr"); }
	;

unop:
		'-'	{}
	;

unary_expr:
		primary					{ tst("unary_expr: primary"); }
//	|	unop unary_expr %prec RIG_TOKEN_UMINUS	{ tst("unary_expr: - expr"); }
	;

arith_expr:
		unary_expr			{ tst("arith_expr: unary_expr"); }
	|	arith_expr '+' arith_expr	{ tst("arith_expr: expr + expr"); }
	|	arith_expr '*' arith_expr	{ tst("expr: expr * expr"); }
	|	assign_lhs '=' arith_expr	{ tst("expr: assign_lhs = arith_expr"); }
	;

primary:
		constant			{ tst("expr: constant"); }
	|	identifier			{ tst("expr: constant"); }
	|	'(' expr ')'			{ tst("unary_expr: ( expr )"); }
	;

constant:
		RIG_TOKEN_INT			{ tst("constants: RIG_TOKEN_INT"); }
	|	RIG_TOKEN_INT RIG_TOKEN_MHZ	{ tst("constants: RIG_TOKEN_INT RIG_TOKEN_MHZ"); }
	;

identifier:
		RIG_TOKEN_IDENT		{ tst("identifier: RIG_TOKEN_IDENT"); }
	|	RIG_TOKEN_MODEL		{ tst("identifier: RIG_TOKEN_MODEL"); }
	|	RIG_TOKEN_PORT		{ tst("identifier: RIG_TOKEN_PORT"); }
	|	RIG_TOKEN_RIG		{ tst("identifier: RIG_TOKEN_RIG"); }
	|	RIG_TOKEN_MODEL '[' RIG_TOKEN_INT ']'
					{ tst("identifier: RIG_TOKEN_MODEL [ RIG_TOKEN_INT ]"); }
	|	RIG_TOKEN_PORT '[' RIG_TOKEN_INT ']'
					{ tst("identifier: RIG_TOKEN_PORT [ RIG_TOKEN_INT ]"); }
	|	RIG_TOKEN_RIG '[' RIG_TOKEN_INT ']'
					{ tst("identifier: RIG_TOKEN_RIG [ RIG_TOKEN_INT ]"); }
	;

assign_lhs:
		identifier		{ tst("assign_lhs: identifier ="); }
	;

%%

/*
 * See rigcmd.c for all functions used in the parser.
 */

// end base.y
