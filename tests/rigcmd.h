/*
 * rigcmd.h	Copyright (C) 2002 by Dale E. Edmons.  All rights reserved.
 *
 *	license: GNU
 */

#ifndef _RIGCMD_H
#define _RIGCMD_H

//#define _YY_BASE_TEST 1
#undef _YY_BASE_TEST

#define RIG_YY_MAXRIGS 10
#include <hamlib/rig.h>
#include <hamlib/riglist.h>
#include <hamlib/rotlist.h>
#include <string.h>
#include <stdlib.h>
#include "tree.h"

#ifdef _YY_BASE_TEST
#	ifndef _YY_RIG_PARSER
#		include "base.tab.h"
#	endif
#	include "../../src/misc.h"
#else
#	ifndef _YY_RIG_PARSER
#		include "rig.tab.h"
#	endif
#	include "../src/misc.h"
#endif


#ifndef _YY_BASE_TEST
#	define yylineno	rig_yylineno
#	define yylval	rig_yylval
#	define yyerror	rig_yyerror
#	define yyparse	rig_yyparse
#	define yylex	rig_yylex
#endif

/************************ Parser Stuff ****************************/

extern int yydebug;

void help(void);
int setup(RIG *, rig_model_t, char *);
int exec_binary_op(int, tree_t *, tree_t *, tree_t *);
int exec_assign_op(int, tree_t *, tree_t *, tree_t *);
int exec_unary_op(int, int, tree_t *, tree_t *);
void yyerror(char *);
int yyparse(void);	// has optional (void *) argument!

/************************ Scanner Stuff ****************************/

// links to scanner
int yylex(void);
extern int yylineno;
extern char *yytext;

struct lexriglist_s {
	const char	*name;
	rig_model_t	model;
};

struct lexvfo_s {
	const char	*name;
	vfo_t		vfo;
};

struct lexfunc_s {
	const char	*name;
	void		*func;	// get
	void		*sfunc;	// set
};

typedef struct lexriglist_s lexriglist_t;
typedef struct lexvfo_s lexvfo_t;
typedef struct lexfunc_s lexfunc_t;

int keyword(char * token);
symtab_t *lookup(char *name);
symtab_t *addsym(char *name);
symtab_t *delsym(char *name);
symtab_t *syminit();

#endif // _RIGCMD_H
// end rigcmd.h
