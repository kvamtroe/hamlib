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
#include "../src/misc.c"

#define RIG_YY_MAXRIGS	10
#define YYERROR_VERBOSE

static RIG *rigs[RIG_YY_MAXRIGS];
static rig_model_t models[RIG_YY_MAXRIGS];
static char *ports[RIG_YY_MAXRIGS];
extern int ts2k_yylineno;

int setup(RIG *rig, rig_model_t model, char *port);
//int rig_setup(RIG *rig, rig_model_t model, char *port);
void help(void);
void ts2k_yyerror(char *);
int s_cpy(RIG *, vfo_t, vfo_t);
int s_sw(RIG *, vfo_t, vfo_t);

%}

%union {
	rig_model_t	model;
	RIG		*rig;
	vfo_t		vfo;
	freq_t		freq;
	char		*txt;
	int		val;
	float		fval;
	struct {
		RIG	*rig;
		vfo_t	vfo;
	} spec;
}

%left '+' '-'
%left '*' '/'
%right '='


%token	RIG_TOKEN_CLOSE
	RIG_TOKEN_EXIT
	RIG_TOKEN_FREQ
	RIG_TOKEN_HELP
	RIG_TOKEN_INIT
	RIG_TOKEN_OPEN
	RIG_TOKEN_SETUP
	RIG_TOKEN_AEQB
	RIG_TOKEN_BEQA
	RIG_TOKEN_AEQC
	RIG_TOKEN_CEQA
	RIG_TOKEN_BEQC
	RIG_TOKEN_CEQB
	RIG_TOKEN_ASWB
	RIG_TOKEN_ASWC

%token <freq>	

%token <fval>	RIG_TOKEN_FLOAT
		RIG_TOKEN_GHZ
		RIG_TOKEN_MHZ
		RIG_TOKEN_KHZ
		RIG_TOKEN_HZ

%token <model>	RIG_TOKEN_MODEL
		RIG_TOKEN_DUMMY
		RIG_TOKEN_TS2K
		RIG_TOKEN_TS2000

%token <rig>	

%token <spec>	

%token <txt>	RIG_TOKEN_STRING

%token <val>	RIG_TOKEN_INT
		RIG_TOKEN_PORT
		RIG_TOKEN_RIG

%token <vfo>	RIG_TOKEN_VFO
		RIG_LEX_CALLA
		RIG_LEX_CALLC
		RIG_LEX_MEMA
		RIG_LEX_MEMC 
		RIG_LEX_VFOA
		RIG_LEX_VFOB
		RIG_LEX_VFOC
		RIG_LEX_VFOAB
		RIG_LEX_VFOBA
		RIG_LEX_CROSS
		RIG_LEX_FAKE 
		RIG_LEX_MAIN
		RIG_LEX_RIT
		RIG_LEX_SAT
		RIG_LEX_SCAN
		RIG_LEX_SUB
		RIG_LEX_XIT
		RIG_LEX_CURR
		RIG_LEX_MEM
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
		vfo_item
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
			$$ = rig_set_freq($1.rig, $1.vfo, $3);
		}
	;

super_function:
		rig_id '.' RIG_TOKEN_AEQB	{ $$ = s_cpy(rigs[$1], RIG_VFO_A, RIG_VFO_B); }
	|	rig_id '.' RIG_TOKEN_BEQA	{ $$ = s_cpy(rigs[$1], RIG_VFO_B, RIG_VFO_A); }
	|	rig_id '.' RIG_TOKEN_AEQC	{ $$ = s_cpy(rigs[$1], RIG_VFO_A, RIG_VFO_C); }
	|	rig_id '.' RIG_TOKEN_CEQA	{ $$ = s_cpy(rigs[$1], RIG_VFO_C, RIG_VFO_A); }
	|	rig_id '.' RIG_TOKEN_BEQC	{ $$ = s_cpy(rigs[$1], RIG_VFO_B, RIG_VFO_C); }
	|	rig_id '.' RIG_TOKEN_CEQB	{ $$ = s_cpy(rigs[$1], RIG_VFO_C, RIG_VFO_B); }
	|	rig_id '.' RIG_TOKEN_ASWB	{ $$ = s_sw(rigs[$1], RIG_VFO_A, RIG_VFO_B); }
	|	rig_id '.' RIG_TOKEN_ASWC	{ $$ = s_sw(rigs[$1], RIG_VFO_A, RIG_VFO_C); }

freq_lhs:	rig_id '.' RIG_TOKEN_FREQ		{
			$$.rig = rigs[$1];
			$$.vfo = RIG_VFO_CURR;
		}
	|	rig_id '.' vfo_id '.' RIG_TOKEN_FREQ	{
			$$.rig = rigs[$1];
			$$.vfo = $3;
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
		};
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

model_type:	RIG_TOKEN_DUMMY		{ $$ = $1; }
	|	RIG_TOKEN_TS2K		{ $$ = $1; }
	|	RIG_TOKEN_TS2000	{ $$ = $1; }
	|	RIG_TOKEN_INT		{ $$ = (rig_model_t) $1; }
	;

vfo_type:	vfo_item		{ $$ = $1; }
	|	vfo_type '|' vfo_item	{ $$ = $1 | $3; }
	;

vfo_item:	RIG_LEX_CALLA
	|	RIG_LEX_CALLC
	|	RIG_LEX_MEMA
	|	RIG_LEX_MEMC 
	|	RIG_LEX_VFOA
	|	RIG_LEX_VFOB
	|	RIG_LEX_VFOC
	|	RIG_LEX_VFOAB
	|	RIG_LEX_VFOBA
	|	RIG_LEX_CROSS
	|	RIG_LEX_FAKE 
	|	RIG_LEX_MAIN
	|	RIG_LEX_RIT
	|	RIG_LEX_SAT
	|	RIG_LEX_SCAN
	|	RIG_LEX_SUB
	|	RIG_LEX_XIT
	|	RIG_LEX_CURR
	|	RIG_LEX_MEM
	|	RIG_LEX_VFO
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
			if($$ != RIG_OK)
				yyerror("cleanup() failed.\n");
		} else {
			yyerror("use open() first.");	// semantics.
			$$ = -RIG_EPROTO;
		}
	}
	;

%%

int s_cpy(RIG *rig, vfo_t dst, vfo_t src)
{
	int retval;
	channel_t ctmp;

	return -RIG_ENIMPL;	// don't use yet!

	if(dst == src) return RIG_OK;	// trivial

	// currently very broken since my hamlib isn't current
	//retval = rig_save_channel(rig, src, &ctmp);
	retval = rig_save_channel(rig, &ctmp);
	if(retval != RIG_OK) return retval;
	//return rig_restore_channel(rig, dst, &ctmp);
	return rig_restore_channel(rig, &ctmp);
}

int s_sw(RIG *rig, vfo_t dst, vfo_t src)
{
	return -RIG_ENIMPL;
}

void yyerror(char *err)
{
	//	fprintf(stderr, __FUNCTION__": %s\n", err);
	rig_debug(RIG_DEBUG_ERR, "cmd_parse: %s on line %i\n", err, ts2k_yylineno);
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
