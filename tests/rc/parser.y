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

#define YYERROR_VERBOSE 1

static RIG *rigs[RIG_YY_MAXRIGS];
static rig_model_t models[RIG_YY_MAXRIGS];
static char *ports[RIG_YY_MAXRIGS];
static channel_t chans[RIG_YY_MAXRIGS];
static int default_idx = 0;	// Default index if none specified.
static vfo_t default_vfo = RIG_VFO_CURR;	// Default VFO

extern int rig_yylineno;

%}

%union {
	channel_t	*spec;
/*	spec_t		spec; */	// FIXME:
	tree_t		*tree;
	rig_model_t	model;
	RIG		*rig;
	channel_t	*chan;
	symtab_t	sym;
	vfo_t		vfo;
	freq_t		freq;
	char		*txt;
	int		val;
	float		fval;
}

%right '='
%left '+' '-'
%left '*' '/'


%token	RIG_TOKEN_CLOSE
	RIG_TOKEN_CALL
	RIG_TOKEN_CURR
	RIG_TOKEN_CTCSS
	RIG_TOKEN_DEBUG
	RIG_TOKEN_EXIT
	RIG_TOKEN_CW
	RIG_TOKEN_AM
	RIG_TOKEN_FM
	RIG_TOKEN_LSB
	RIG_TOKEN_USB
	RIG_TOKEN_RTTY
	RIG_TOKEN_FREQ
	RIG_TOKEN_HELP
	RIG_TOKEN_INIT
	RIG_TOKEN_MAIN
	RIG_TOKEN_MENU
	RIG_TOKEN_MEM
	RIG_TOKEN_MODE
	RIG_TOKEN_MINUSEQ
	RIG_TOKEN_MINUSMINUS
	RIG_TOKEN_OFFSET
	RIG_TOKEN_OPEN
	RIG_TOKEN_PLUSEQ
	RIG_TOKEN_PLUSPLUS
	RIG_TOKEN_PM
	RIG_TOKEN_PRINT
	RIG_TOKEN_RIGDEFAULT
	RIG_TOKEN_SETUP
	RIG_TOKEN_SHIFT
	RIG_TOKEN_SUB
	RIG_TOKEN_TONE
	RIG_TOKEN_BEQA
	RIG_TOKEN_CEQA
	RIG_TOKEN_AEQB
	RIG_TOKEN_CEQB
	RIG_TOKEN_AEQC
	RIG_TOKEN_BEQC
	RIG_TOKEN_AEQM
	RIG_TOKEN_BEQM
	RIG_TOKEN_CEQM
	RIG_TOKEN_MEQA
	RIG_TOKEN_MEQB
	RIG_TOKEN_MEQC
	RIG_TOKEN_ASWB
	RIG_TOKEN_ASWC
	RIG_TOKEN_BSWC
	RIG_TOKEN_ASWM
	RIG_TOKEN_BSWM
	RIG_TOKEN_CSWM
	RIG_LEX_HAMLIB

%token <freq>	

%token <fval>	RIG_TOKEN_FLOAT
		RIG_TOKEN_GHZ
		RIG_TOKEN_MHZ
		RIG_TOKEN_KHZ
		RIG_TOKEN_HZ

%token <model>	RIG_TOKEN_MODEL
		RIG_LEX_MODEL

%token <rig>	

%token <spec>	

%token <txt>	RIG_TOKEN_STRING
		RIG_TOKEN_IDENT

%token <val>	RIG_TOKEN_INT
		RIG_TOKEN_PORT
		RIG_TOKEN_RIG

%token <vfo>	RIG_TOKEN_VFO
		RIG_LEX_VFO

%type <freq>	freq_val

%type <model>	model_type

%type <rig>	

%type <spec>	freq_lhs

%type <txt>	port_id_rhs

%type <val>	assign_exp
		model_id
		port_id
		rig_cmd
		rig_function
		rig_id
		rig_list
		super_function

%type <vfo>	vfo_id
		vfo_type

%%

top:	rig_list	{ if($1 == RIG_OK) YYACCEPT; else YYABORT; }
	;

rig_list:	rig_cmd ';'		{ $$ = $1; }
	|	rig_list rig_cmd ';'	{ $$ = $2; }
	|	RIG_TOKEN_HELP 		{ help(); $$ = RIG_OK; } 
	|	RIG_TOKEN_EXIT 		{
			int i;
			for( i=0; i<RIG_YY_MAXRIGS; i++) {
				if(rigs[i] != NULL) {
					rig_close(rigs[i]);
					free(rigs[i]);
				}
			}
			exit(0);
		}
	;

rig_cmd:
		rig_function
	|	assign_exp
	|	super_function
	;

assign_exp:	model_id '=' model_type	{ models[$1] = $3; $$ = RIG_OK; }
	|	port_id '=' port_id_rhs	{
			//if( $1 != NULL) free($1);
			ports[$1] = $3;
			$$ = RIG_OK;
		}
	|	freq_lhs '=' freq_val	{
			//$$ = rig_set_freq($1.rig, $1.vfo, $3);
//			$$ = rig_set_freq($1->rig, $1->vfo, $3);	//FIXME:
		}
	;
//-verified

super_function:
		rig_id '.' RIG_TOKEN_AEQB	{ $$ = s_cpy(rigs[$1], RIG_VFO_A, RIG_VFO_B); }
	|	rig_id '.' RIG_TOKEN_AEQC	{ $$ = s_cpy(rigs[$1], RIG_VFO_A, RIG_VFO_C); }
	|	rig_id '.' RIG_TOKEN_BEQA	{ $$ = s_cpy(rigs[$1], RIG_VFO_B, RIG_VFO_A); }
	|	rig_id '.' RIG_TOKEN_BEQC	{ $$ = s_cpy(rigs[$1], RIG_VFO_B, RIG_VFO_C); }
	|	rig_id '.' RIG_TOKEN_CEQA	{ $$ = s_cpy(rigs[$1], RIG_VFO_C, RIG_VFO_A); }
	|	rig_id '.' RIG_TOKEN_CEQB	{ $$ = s_cpy(rigs[$1], RIG_VFO_C, RIG_VFO_B); }
	|	rig_id '.' RIG_TOKEN_ASWB	{ $$ = s_sw(rigs[$1], RIG_VFO_A, RIG_VFO_B); }
	|	rig_id '.' RIG_TOKEN_ASWC	{ $$ = s_sw(rigs[$1], RIG_VFO_A, RIG_VFO_C); }
	;

freq_lhs:	rig_id '.' RIG_TOKEN_FREQ		{
		//	$$.rig = rigs[$1];
		//	$$.vfo = default_vfo;
		}
	|	rig_id '.' vfo_id '.' RIG_TOKEN_FREQ	{
		//	$$.rig = rigs[$1];
		//	$$.vfo = $3;
		}
	;

freq_val:	RIG_TOKEN_INT RIG_TOKEN_GHZ	{ $$ = $1 * $2; }
	|	RIG_TOKEN_INT RIG_TOKEN_MHZ	{ $$ = $1 * $2; }
	|	RIG_TOKEN_INT RIG_TOKEN_KHZ	{ $$ = $1 * $2; }
	|	RIG_TOKEN_INT RIG_TOKEN_HZ	{ $$ = $1; }
	|	RIG_TOKEN_INT			{ $$ = $1; }
	|	RIG_TOKEN_FLOAT RIG_TOKEN_GHZ	{ $$ = (freq_t)($1 * $2); }
	|	RIG_TOKEN_FLOAT RIG_TOKEN_MHZ	{ $$ = (freq_t)($1 * $2); }
	|	RIG_TOKEN_FLOAT RIG_TOKEN_KHZ	{ $$ = (freq_t)($1 * $2); }
	|	RIG_TOKEN_FLOAT RIG_TOKEN_HZ	{ $$ = (freq_t)$1; }
	|	RIG_TOKEN_FLOAT			{ $$ = (freq_t)$1; }
	;
freq_val:	rig_id '.' RIG_TOKEN_FREQ '(' ')'		{
			int retval;
			freq_t ftmp;
			retval = rig_get_freq(rigs[$1], RIG_VFO_CURR, &ftmp);
			if(retval != RIG_OK) {
				yyerror("get_freq(): error occured");
				YYERROR;
			}
			$$ = ftmp;
		}
	;
freq_val:	rig_id '.' vfo_id '.' RIG_TOKEN_FREQ '(' ')'	{
			int retval;
			freq_t ftmp;
			retval = rig_get_freq(rigs[$1], $3, &ftmp);
			if(retval != RIG_OK) {
				yyerror("get_freq(): error occured");
				YYERROR;
			}
			$$ = ftmp;
		}
	;

model_id:	RIG_TOKEN_MODEL				{ $$ = $1; }
	|	RIG_TOKEN_MODEL '[' RIG_TOKEN_INT ']'	{ $$ = $3; }
	;

rig_id:		RIG_TOKEN_RIG				{ $$ = $1; }
	|	RIG_TOKEN_RIG '[' RIG_TOKEN_INT ']'	{ $$ = $3; }
	;

port_id:	RIG_TOKEN_PORT				{ $$ = $1; }
	|	RIG_TOKEN_PORT '[' RIG_TOKEN_INT ']'	{ $$ = $3; }
	;

vfo_id:		RIG_TOKEN_VFO			{ $$ = RIG_VFO_CURR; }
	|	RIG_TOKEN_VFO '(' vfo_type ')'	{ $$ = $3; }
	;

port_id_rhs:	port_id			{ $$ = ports[$1]; }
	|	RIG_TOKEN_STRING	{ $$ = $1; }
	;

model_type:	RIG_LEX_MODEL		{ $$ = $1; }
	|	RIG_TOKEN_INT		{ $$ = (rig_model_t) $1; }
	;

vfo_type:	RIG_LEX_VFO			{ $$ = $1; }
	|	vfo_type '|' RIG_LEX_VFO	{ $$ = $1 | $3; }
	;

rig_function:
	RIG_TOKEN_DEBUG '(' RIG_TOKEN_INT ')' {	rig_set_debug( $3 ); $$ = RIG_OK; }
	;
rig_function:
	rig_id '.' RIG_TOKEN_OPEN '(' model_id ',' port_id_rhs ')' {
		if( rigs[$1] == NULL) {
			/*$$ = rig_setup(&rigs[$3], models[$5], $7);*/
			rigs[$1] = rig_init(models[$5]);
			strncpy(rigs[$1]->state.rigport.pathname, $7, FILPATHLEN);
			$$ = rig_open(rigs[$1]);
			rig_debug(RIG_DEBUG_VERBOSE,
			   "rig_function: open rigs[%i]=%x, model[%i] = %i, port=%s\n",
				$1, rigs[$1], $5, models[$5], $7);
			if($$ != RIG_OK || rigs[$1] == NULL)
				yyerror("open() failed.\n");
		} else {
			yyerror("use close() first!");	// semantics.
			$$ = -RIG_EPROTO;
		}
	};
rig_function:
	RIG_TOKEN_OPEN '(' rig_id ',' model_id ',' port_id_rhs ')' {
		if( rigs[$3] == NULL) {
			/*$$ = rig_setup(&rigs[$3], models[$5], $7);*/
			rigs[$3] = rig_init(models[$5]);
			strncpy(rigs[$3]->state.rigport.pathname, $7, FILPATHLEN);
			$$ = rig_open(rigs[$3]);
			rig_debug(RIG_DEBUG_VERBOSE,
			   "rig_function: open rigs[%i]=%x, model[%i] = %i, port=%s\n",
				$3, rigs[$3], $5, models[$5], $7);
			if($$ != RIG_OK || rigs[$3] == NULL)
				yyerror("open() failed.\n");
		} else {
			yyerror("use close() first!");	// semantics.
			$$ = -RIG_EPROTO;
		}
	};
rig_function:
	rig_id '.' RIG_TOKEN_CLOSE '(' ')' {
		if(rigs[$1] != NULL) {
			$$ = rig_close(rigs[$1]);
			if($$ != RIG_OK)
				yyerror("close() failed.\n");
			$$ = rig_cleanup(rigs[$1]);
			rigs[$1] = NULL;
			if($$ != RIG_OK)
				yyerror("cleanup() failed.\n");
		} else {
			yyerror("use open() first.");	// semantics.
			$$ = -RIG_EPROTO;
		}
	};
rig_function:
	RIG_TOKEN_CLOSE '(' rig_id ')' {
		if(rigs[$3] != NULL) {
			$$ = rig_close(rigs[$3]);
			if($$ != RIG_OK)
				yyerror("close() failed.\n");
			$$ = rig_cleanup(rigs[$3]);
			rigs[$3] = NULL;
			if($$ != RIG_OK)
				yyerror("cleanup() failed.\n");
		} else {
			yyerror("use open() first.");	// semantics.
			$$ = -RIG_EPROTO;
		}
	}
	;

%%

