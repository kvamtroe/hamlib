/*
 * rigcmd.c	Copyright (C) 2002 by Dale E. Edmons.  All rights reserved.
 *
 *	license:	GNU
 */

/*
 * This file contains all functions called by scanner.l and parser.y to
 * implement semantic actions and other routines.
 */

#include "rigcmd.h"

// the following *only* included here!
#include "lex_rigs.h"
#include "lex_rotors.h"
#include "lex_hamlib.h"
#include "lex_keywords.h"

symtab_t *symbols=NULL;

/*********************** Parser Routines ******************************/

#ifdef _YY_BASE_TEST
main()
{
	yydebug = 1;
//	do {
		fprintf(stderr, "\nrigcmd> ");
		yyparse();
//	} while(yytext[0] != EOF);
}

void tst(char *s)
{
	fprintf(stderr, "\t<* :%i:%s *>\n", yylineno, s);
}
#endif

//void rig_yyerror(char *err)
void yyerror(char *err)
{
#ifndef _YY_BASE_TEST
	rig_debug(RIG_DEBUG_ERR, "cmd_parse: %s on line %i\n", err, yylineno);
#else
	fprintf(stderr, __FUNCTION__"> %s on line %i\n", err, yylineno);
#endif
}

void help(void)
{
#ifndef _YY_BASE_TEST
	rig_debug(RIG_DEBUG_ERR, "\n"__FUNCTION__">\n"
#else
	fprintf(stdout, "\n"__FUNCTION__">\n"
#endif
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

/*
 * Simple execution of binary operators.  Little type checking is
 *	performed.  Basically I do my best to turn everything into
 *	a long double.  If this cannot be done, an error or improper
 *	result will occur.
 */
int exec_unary_op(int op, int prefix, tree_t *left, tree_t *right)
{
	long double dright, dleft;
	long long lright, lleft;
	symtab_t *pright, *pleft;
	int is_long = 0;

	if(right->sym.type == SYM_A)
		pright = right->symptr;
	else
		pright = &right->sym;

	switch(pright->type) {
	case CHAR_T:	dright = (long double) pright->val.c; break;
	case DOUBLE_T:	dright = (long double) pright->val.d; break;
	case FLOAT_T:	dright = (long double) pright->val.d; break;
	case FREQ_T:	lright = (long long) pright->val.f; is_long = 1; break;
	case INT_T:	lright = (long long) pright->val.i; is_long = 1; break;
	case LDOUBLE_T:	dright = pright->val.D; break;
	case LLONG_T:	lright = (long long) pright->val.f; is_long = 1; break;
	case LONG_T:	dright = (long double) pright->val.i; break;
	case MODE_T:	dright = (long double) pright->val.m; break;
	case MODEL_T:	dright = (long double) pright->val.r; break;
	case VFO_T:	lright = (long long) pright->val.v; is_long = 1; break;

	/* conversion not possible */
	case STRING_T:
//	case SYM_A:
	case UNKNOWN_T:
//	case RIG_A:
	default:
		yyerror("right operand conversion not possible\n");
		//YYERROR;
		break;
	}

	s_cp(left, right);
	switch(op) {
	case '+':	break;
	case '-':
		if(is_long) {
			lleft = -lright;
			pleft->val.f = lleft;
			
		} else {
			dleft = -dright;
			pleft->val.D = dleft;
		}
		break;
	case RIG_TOKEN_PLUSPLUS:
	case RIG_TOKEN_MINUSMINUS:

	default:
		yyerror(__FUNCTION__"> unknown operator.\n");
		//YYERROR;
		break;
	}

	return RIG_OK;
}

/*
 * copy from right into left and also to result ($$)
 *	if result != NULL.   Not all cast operations
 *	are implemented but shouldn't be a problem.
 *
 *	This section is long and ugly but should be fast.
 */
int exec_assign_op(int op, tree_t *result, tree_t *left, tree_t *right)
{
	symtab_t *lft, *rt;
	int i;

	if(left == NULL || right == NULL) {
		yyerror(__FUNCTION__"> NULL pointer arg\n");
		return -RIG_EINVAL;
	}
/*	if(left->rig != NULL || right->rig != NULL) {
		yyerror(__FUNCTION__"> attempt made to overwrite RIG *\n");
		return -RIG_EINVAL;
	}
	if(left->func != NULL || right->func != NULL) {
		yyerror(__FUNCTION__"> attempt made to overwrite FUNC *\n");
		return -RIG_EINVAL;
	}
*/
	if(left->sym.type == SYM_A) lft = left->symptr;
	else lft = &left->sym;
	if(right->sym.type == SYM_A) rt = right->symptr;
	else rt = &right->sym;

	/*
	 * Each type must be handled and some checks performed
	 *	Each is being implemented as needed.
	 */
	i = 0;
	i |= lft->type == UNKNOWN_T;
	i |= lft->type == rt->type;
	i |= lft->type == 0;	// uninitialized!
	if( i ) { 
		lft->type = rt->type;
		switch(lft->type) {
		case INT_T:
			lft->val.i = rt->val.i;
			break;
		case FREQ_T:
			lft->val.f = rt->val.f;
			break;
		case VFO_T:
			lft->val.v = rt->val.v;
			break;
		case DOUBLE_T:
			lft->val.d = rt->val.d;
			break;
		case STRING_T:
			lft->val.s = rt->val.s;	// strdup() already been used.
			break;
		case MODE_T:
			lft->val.m = rt->val.m;
			break;
		case MODEL_T:
			lft->val.r = rt->val.r;
			break;
		case LDOUBLE_T:
			lft->val.D = rt->val.D;
			break;
		case CHAR_T:
			lft->val.c = rt->val.c;
			break;

//		case SYM_A:
//			yyerror(__FUNCTION__": Bug> Symbols cannot have SYM_A as type!\n");
//			return -RIG_EINVAL;
//			break;
		default:
			fprintf(stderr, __FUNCTION__"> lhs type = %i, rhs type = %i\n",
						lft->type, rt->type);
			yyerror(__FUNCTION__": LHS rule> type not handled\n");
			return -RIG_EINVAL;
			break;
		}
	} else if(rt->type == INT_T) {
		switch(lft->type) {
		case MODEL_T:
			lft->val.r = (rig_model_t) rt->val.i;
			break;
		case VFO_T:
			lft->val.v = (vfo_t) rt->val.i;
			break;
		case FREQ_T:
			lft->val.f = (freq_t) rt->val.i;
			break;
		case MODE_T:
			lft->val.m = (rmode_t) rt->val.i;
			break;
		case CHAR_T:
			lft->val.c = (char) rt->val.i;
			break;
		case DOUBLE_T:
			lft->val.d = (double) rt->val.i;
			break;
		case LDOUBLE_T:
			lft->val.D = (long double) rt->val.i;
			break;

		case STRING_T:
		default:
			fprintf(stderr, __FUNCTION__"> lhs type = %i, rhs type = %i\n",
						lft->type, rt->type);
			yyerror(__FUNCTION__" int> type not implemented\n");
			return -RIG_EINVAL;
			break;
		}
	} else if(rt->type == FREQ_T) {
		switch(lft->type) {
		case LDOUBLE_T:
			lft->val.D = (long double) rt->val.f;
			break;
		case DOUBLE_T:
			lft->val.d = (double) rt->val.f;
			break;
		case INT_T:
			lft->val.i = (int) rt->val.f;
			break;
		case VFO_T:
			lft->val.v = (vfo_t) rt->val.f;
			break;
		case MODE_T:
			lft->val.m = (rmode_t) rt->val.f;
			break;
		case MODEL_T:
			lft->val.r = (rig_model_t) rt->val.f;
			break;

		default:
			fprintf(stderr, __FUNCTION__"> lhs type = %i, rhs type = %i\n",
						lft->type, rt->type);
			yyerror(__FUNCTION__" freq> type not implemented\n");
			return -RIG_EINVAL;
			break;
		}
	} else if(rt->type == VFO_T) {
		switch(lft->type) {
		case INT_T:
			lft->val.i = (int) rt->val.v;
			break;
		case FREQ_T:
			lft->val.f = (freq_t) rt->val.v;
			break;
		case DOUBLE_T:
			lft->val.d = (double) rt->val.v;
			break;
		case LDOUBLE_T:
			lft->val.D = (long double) rt->val.v;
			break;
		case MODE_T:
			lft->val.m = (rmode_t) rt->val.v;
			break;
		case MODEL_T:
			lft->val.r = (rig_model_t) rt->val.v;
			break;

		default:
			fprintf(stderr, __FUNCTION__"> lhs type = %i, rhs type = %i\n",
						lft->type, rt->type);
			yyerror(__FUNCTION__" vfo> type not implemented\n");
			return -RIG_EINVAL;
			break;
		}
	} else if(rt->type == DOUBLE_T) {
		switch(lft->type) {
		case LDOUBLE_T:
			lft->val.D = (long double) rt->val.d;
			break;
		case FREQ_T:
			lft->val.f = (freq_t) rt->val.d;
			break;
		case INT_T:
			lft->val.i = (int) rt->val.d;
			break;
		case VFO_T:
			lft->val.v = (double) rt->val.d;
			break;
		case MODE_T:
			lft->val.m = (rmode_t) rt->val.d;
			break;
		case MODEL_T:
			lft->val.r = (rig_model_t) rt->val.d;
			break;

		default:
			fprintf(stderr, __FUNCTION__"> lhs type = %i, rhs type = %i\n",
						lft->type, rt->type);
			yyerror(__FUNCTION__" double> type not implemented\n");
			return -RIG_EINVAL;
			break;
		}
	} else if(rt->type == MODE_T) {
		switch(lft->type) {
		case INT_T:
			lft->val.i = (int) rt->val.m;
			break;
		case FREQ_T:
			lft->val.f = (freq_t) rt->val.m;
			break;
		case DOUBLE_T:
			lft->val.d = (double) rt->val.m;
			break;
		case LDOUBLE_T:
			lft->val.D = (long double) rt->val.m;
			break;
		case VFO_T:
			lft->val.v = (vfo_t) rt->val.m;
			break;
		case MODEL_T:
			lft->val.r = (rig_model_t) rt->val.m;
			break;

		default:
			fprintf(stderr, __FUNCTION__"> lhs type = %i, rhs type = %i\n",
						lft->type, rt->type);
			yyerror(__FUNCTION__" mode> type not implemented\n");
			return -RIG_EINVAL;
			break;
		}
	} else if(rt->type == MODEL_T) {
		switch(lft->type) {
		case FREQ_T:
			lft->val.f = (freq_t) rt->val.r;
			break;
		case INT_T:
			lft->val.m = (int) rt->val.r;
			break;

		case STRING_T:
		default:
			fprintf(stderr, __FUNCTION__"> lhs type = %i, rhs type = %i\n",
						lft->type, rt->type);
			yyerror(__FUNCTION__" model> type not implemented\n");
			return -RIG_EINVAL;
			break;
		}
	} else if(rt->type == STRING_T) {
		switch(lft->type) {
		case 0:
		case UNKNOWN_T:
		case STRING_T:
			fprintf(stderr, __FUNCTION__"> type \'%i\'\n", lft->type);
			lft->val.s = (char *) strdup(rt->val.s);
			break;

		default:
			fprintf(stderr, __FUNCTION__"> lhs type = %i, rhs type = %i\n",
						lft->type, rt->type);
			yyerror(__FUNCTION__" string> type not implemented\n");
			return -RIG_EINVAL;
			break;
		}
	} else {
		fprintf(stderr, __FUNCTION__": lhs type = %i, rhs type = %i\n",
					lft->type, rt->type);
		yyerror(__FUNCTION__": error: item not implemented\n");
		return -RIG_EINVAL;
	}

	return RIG_OK;
}

/*
 * Simple execution of binary operators.  Little type checking is
 *	performed.  Basically I do my best to turn everything into
 *	a long double.  If this cannot be done, an error or improper
 *	result will occur.
 */
int exec_binary_op(int op, tree_t *result, tree_t *left, tree_t *right)
{
	long double dresult, dright, dleft;
	long lresult, lright, lleft;
	int is_long = 0;	// 1 forces cast to long long
	symtab_t *pright, *pleft;

	if(right->sym.type == SYM_A)
		pright = right->symptr;
	else
		pright = &right->sym;

	if(left->sym.type == SYM_A)
		pleft = left->symptr;
	else
		pleft = &left->sym;

	switch(pright->type) {
	case CHAR_T:	dright = (long double) pright->val.c; break;
	case DOUBLE_T:	dright = (long double) pright->val.d; break;
	case FLOAT_T:	dright = (long double) pright->val.d; break;
	case FREQ_T:	dright = (long double) pright->val.f; break;
	case INT_T:	lright = (long) pright->val.i; is_long = 1; break;
	case LDOUBLE_T:	lright = pright->val.D; break;
	case LLONG_T:	dright = (long double) pright->val.f; break;
	case LONG_T:	lright = (long) pright->val.i; is_long = 1; break;
	case MODE_T:	dright = (long double) pright->val.m; break;
	case MODEL_T:	dright = (long double) pright->val.r; break;
	case VFO_T:	lright = (long) pright->val.v; is_long = 1; break;

	/* conversion not possible */
	case STRING_T:
	//case SYM_A:
	case UNKNOWN_T:
	//case RIG_A:
	default:
		yyerror("right operand conversion not performed\n");
		//YYERROR;
		break;
	}
	switch(pleft->type) {
	case CHAR_T:	dleft = (long double) pleft->val.c; break;
	case DOUBLE_T:	dleft = (long double) pleft->val.d; break;
	case FLOAT_T:	dleft = (long double) pleft->val.d; break;
	case FREQ_T:	dleft = (long double) pleft->val.f; break;
	case INT_T:	lleft = (long) pleft->val.i; is_long = 1; break;
	case LDOUBLE_T:	dleft = pleft->val.D; break;
	case LLONG_T:	dleft = (long double) pleft->val.f; break;
	case LONG_T:	lleft = (long) pleft->val.i; is_long = 1; break;
	case MODE_T:	dleft = (long double) pleft->val.m; break;
	case MODEL_T:	dleft = (long double) pleft->val.r; break;
	case VFO_T:	lleft = (long) pleft->val.v; is_long = 1; break;

	/* conversion not possible */
	//case STRING_T:
	//case SYM_A:
	case UNKNOWN_T:
	//case RIG_A:
	default:
		yyerror("left operand conversion not performed\n");
		//YYERROR;
		break;
	}

	switch(op) {
	case '+':
		if(is_long)
			lresult = lleft + lright;
		else
			dresult = dleft + dright;
		break;
	case '-':
		if(is_long)
			lresult = lleft - lright;
		else
			dresult = dleft - dright;
		break;
	case '*':
		if(is_long)
			lresult = lleft * lright;
		else
			dresult = dleft * dright;
		break;
	case '/':
		if(is_long)
			lresult = lleft / lright;
		else
			dresult = dleft / dright;
		break;
	case '|':
		if(is_long)
			lresult = lleft | lright;
		else
			yyerror("invalid type for logical operand\n");
		break;
	case '&':
		if(is_long)
			lresult = lleft & lright;
		else
			yyerror("invalid type for logical operand\n");
		break;
	case '=':
		s_cp(left, right);
		s_cp(result, right);
		break;

	default:
		yyerror(__FUNCTION__": unknown operator.\n");
		//YYERROR;
		break;
	}

	if(op != '=') {
		result->sym.val.D = dresult;
		result->sym.type = LDOUBLE_T;
	}

	return RIG_OK;
}

/*
 * copy one tree struct to another.  pointers are copied but are expected
 *	to be set properly by an external routine
 */
int s_cp( tree_t *dst, tree_t *src)
{
	symtab_t *sp, *dp;

	if(dst == NULL || src == NULL) {
		yyerror(__FUNCTION__"> NULL on src or dst pointer!\n");
		return -RIG_EINVAL;
	}

	if(src->sym.type == SYM_A)
		sp = src->symptr;
	else
		sp = &src->sym;

	dst->rig = src->rig;
	dst->symptr = NULL;
	dst->retval = src->retval;
	dst->chan = src->chan;
	dst->func = src->func;
	dst->sym.next = NULL;
	dst->sym.type = sp->type;
	switch(sp->type) {
	case INT_T:	dst->sym.val.i = sp->val.i; break;
	case CHAR_T:	dst->sym.val.c = sp->val.c; break;
	case FLOAT_T:	dst->sym.val.d = sp->val.d; break;
	case LONG_T:	dst->sym.val.i = sp->val.i; break;
	case VFO_T:	dst->sym.val.v = sp->val.v; break;
	case MODE_T:	dst->sym.val.m = sp->val.m; break;
	case MODEL_T:	dst->sym.val.r = sp->val.r; break;
	case STRING_T:	dst->sym.val.d = sp->val.d; break;
//	case RIG_A:	dst->sym.val.d = sp->val.d; break;
//	case SYM_A:
		yyerror("dereferenced symbol has SYM_A\n");
	//	YYERROR;
		break;

	case LLONG_T:
	case FREQ_T:
	case UNKNOWN_T:
	default:	dst->sym.val.f = sp->val.f; break;
	}

	return RIG_OK;
}

/*********************** Scanner Routines ******************************/

/*
 * Checks any remaining keywords and returns the appropriate info.
 *	I only do a straight linear search.
 */
int keyword(char * token)
{
	int i, j;
	char *p;
	struct lexvfo_s *lp;
	symtab_t *sp;

	fprintf(stderr, __FUNCTION__"> %s\n", token);
	// VFOs (often used)
	for(i=0; lex_vfos[i].name != NULL; i++) {
		//fprintf(stderr, "%s, ", lex_vfos[i].name);
		j = strcmp( lex_vfos[i].name, token); 
		if( j == 0 ) {
			fprintf(stderr, "\n"__FUNCTION__"> Found vfo!\n");
			yylval.vfo = lex_vfos[i].vfo;
			return RIG_LEX_VFO;
		}
	}
	// Nope, not a vfo_t

	// Functions (often used)
	for(i=0; lex_hamlib[i].name != NULL; i++) {
		j = strcmp( lex_hamlib[i].name, token); 
		//fprintf(stderr, "\'%s\', %i, %i ", &lex_hamlib[i].name[0], i, j);
		if( j == 0 ) {
			fprintf(stderr, "\n"__FUNCTION__"> Found func!\n");
			yylval.sym.name = (char *)lex_hamlib[i].name;
			return RIG_LEX_HAMLIB;
		}
	}
	// Nope, not a function name

	// Models (least used, so last!)
	for(i=0; lex_riglist[i].name != NULL; i++) {
		//fprintf(stderr, "%s, ", lex_riglist[i].name);
		for(j=0; lex_riglist[i].name[j] == token[j]; j++) {
			if((lex_riglist[i].name[j] == '\0')
			   && (token[j] == '\0')) {	// exact match only
				yylval.model = lex_riglist[i].model;
				return RIG_LEX_MODEL;
			}
		}
	}
	// Nope, not a model_id

#ifdef _YY_BASE_TEST
	fprintf(stderr, __FUNCTION__
		"> keyword not found.  Adding `%s\' to Symbol Table\n", yytext);
#endif

	// FIXME: The following causes a segment fault!
	fprintf(stderr, __FUNCTION__":lookup> looking up %s\n", token);
	sp = lookup(token);
	//fprintf(stderr, __FUNCTION__":lookup> checking result.\n");
	if(sp == NULL) {
		//fprintf(stderr, __FUNCTION__"> not found.  Adding %s\n", token);
		sp = addsym(token);
		if(sp == NULL) {
			fprintf(stderr, __FUNCTION__"> addsym() failed\n");
			return 0;
		}
		sp->type = UNKNOWN_T;
		yylval.tree = tree_init();	// non-NULL guaranteed!
		yylval.tree->symptr = sp;
		yylval.tree->sym.type = SYM_A;
		//fprintf(stderr, __FUNCTION__"> Added %s!\n", token);
	}
	fprintf(stderr, __FUNCTION__"> calling disp_tree()\n");
	disp_tree(yylval.tree);
	
	return RIG_TOKEN_IDENT;
}

/* The following symbol table routines are *not* studies
 * in optimized methods.  They (hopefully) get the job done
 * and no more.  Please feel free to make them faster!
 * (I currently don't expect more than a few entries anyway.)
 */

/*
 *	return NULL if name is found, otherwise
 *	point to insertion point.
 */
symtab_t * chkdup(char *name)
{
	symtab_t	*cur;
	symtab_t	*prev;
	int		match;

	prev = cur = symbols;	// must never be NULL

	while(cur != NULL) {
		match = strcmp(cur->name, name);
		if(match == 0)
			return NULL;
		if(match < 0) {
			prev = cur;
			cur = cur->next;
			continue;
		}
		if(match > 0) {	// found!
			return prev;
		}
	}
	return prev;	// end of list
}

symtab_t * lookup(char *name)
{
	symtab_t	*cur;
	symtab_t	*prev;
	int		match;

	//fprintf(stderr, __FUNCTION__"> %s\n", name);

	if(symbols == NULL) {
		//fprintf(stderr, __FUNCTION__"> Symbol table empty!\n");
		return NULL;
	}
	prev = cur = symbols;	// must never be NULL

	while(cur != NULL) {
		match = strcmp(cur->name, name);
		fprintf(stderr, __FUNCTION__"> comparing %s, %s, %i\n", \
						cur->name, name, match);
		if(match == 0)
			return cur;
		if(match < 0) {
			prev = cur;
			cur = cur->next;
			continue;
		}
		if(match > 0) {
			return NULL;
		}
	}
	return NULL;
}

/* add new symbol in alpha order */
symtab_t * addsym(char *name)
{
	symtab_t	*stmp;
	symtab_t	*snew;

	if(symbols == NULL) {
		symbols = syminit();
		symbols->name = (char *) strdup(name);

		return symbols;
	}

	stmp = chkdup(name);

	if(stmp == NULL) {
		fprintf(stderr, __FUNCTION__":error> symbol exists!\n");
		return NULL;	// it already exists
	}

	//fprintf(stderr, __FUNCTION__"> getting new symbol\n");
	snew = syminit();

	snew->next = stmp->next;
	stmp->next = snew;

	snew->name = (char *) strdup(name);
	snew->val.f = 0;
	snew->type = UNKNOWN_T;

	return snew;
}

symtab_t * delsym(char *name)
{
	symtab_t	*cur;
	symtab_t	*prev;
	symtab_t	*dom;
	int		match;

	prev = NULL;
	cur = symbols;	// must never be NULL

	if(cur == NULL) return NULL;

	while(cur != NULL) {
		match = strcmp(cur->name, name);
		if(match == 0) {
			dom = cur;
			if(prev != NULL) {
				prev->next = cur->next;
			}
			free(dom);

			return dom;	// do not use!!!!
		}
		if(match < 0) {
			prev = cur;
			cur = prev->next;
			continue;
		}
		if(match > 0) {
			return NULL;
		}
	}
	return NULL;	// something is wrong!
}

/* get one symbol and initialize it.  Return pointer. */
symtab_t * syminit()
{
	symtab_t *p;

	p = (symtab_t *) calloc(sizeof(symtab_t), 1);
	if(p == NULL) {
		fprintf(stderr, __FUNCTION__"> Couldn't create symbol!\n");
		fprintf(stderr, "  Exiting!\n");
		exit(1);
	}
	p->next = NULL;
	p->val.i = 0xCEEE + 0xDEE;
	p->name = NULL;
	p->type = UNKNOWN_T;

	//fprintf(stderr, __FUNCTION__"> Created new symbol!\n");

	return p;	// non-NULL must be guaranteed!
}

/* create and init new tree_t.  return pointer */
tree_t * tree_init(void)
{
	tree_t *t;

	t = calloc(sizeof(tree_t), 1);
	if(t == NULL) {
		fprintf(stderr, __FUNCTION__"> calloc failed.  exiting...\n");
		exit(1);
	}
	t->rig = NULL;
	t->symptr = NULL;
	t->chan = NULL;
	t->func = NULL;
	t->lattrib = 0;
	t->rattrib = 0;
	t->retval = 0;

	t->sym.name = NULL;
	t->sym.next = NULL;
	t->sym.type = UNKNOWN_A;
	t->sym.val.f = 0;

	return t;
}

void disp_tree(tree_t *d)
{
	symtab_t *t;
	char tmp[80], *name;

	if(d == NULL) {
		fprintf(stdout, __FUNCTION__"> received NULL tree_t pointer.  Bye!\n");
		return;
	}
	if(d->sym.type == SYM_A) {
		fprintf(stdout, __FUNCTION__"> SYM_A found.  Using pointer.\n");
		t = d->symptr;
	}
	else
		t = &d->sym;
	if(t == NULL) {
		fprintf(stdout, __FUNCTION__"> NULL symtab_t pointer.  Bye!\n");
		return;
	}
	disp_sym(t);

	return;
}

/*
 * formatted output of symbol to stdout
 */
void disp_sym(symtab_t *d)
{
	symtab_t *t;
	char tmp[80], *name;

	t = d;
	if(t == NULL) {
		fprintf(stdout, __FUNCTION__"> NULL symtab_t pointer.  Bye!\n");
		return;
	}

	fprintf(stdout, "\nName:\t%s\n", t->name);
	fprintf(stdout, "\ntype:\t");
	switch(t->type) {
	case INT_T:	sprintf(tmp, "INT_T, val.i = %i\n", t->val.i); break;
	case CHAR_T:	sprintf(tmp, "CHAR_T, val.c = %c\n", t->val.c); break;
	case FLOAT_T:	sprintf(tmp, "FLOAT_T, val.d = %g\n", t->val.d); break;
	case LONG_T:	sprintf(tmp, "LONG_T, val.i = %li\n", t->val.i); break;
#ifndef _YY_BASE_TEST
	case VFO_T:	sprintf(tmp, "VFO_T, val.v = %s\n", strvfo(t->val.v)); break;
#else
	case VFO_T:	sprintf(tmp, "VFO_T, val.v = %i\n", (t->val.v)); break;
#endif
	case MODE_T:	sprintf(tmp, "MODE_T, val.m = %i\n", t->val.m); break;
	case MODEL_T:	sprintf(tmp, "MODEL_T, val.r = %i\n", t->val.r); break;
	case STRING_T:	sprintf(tmp, "STRING_T, val.s = %s\n", t->val.s); break;
	//case RIG_A:	sprintf(tmp, "RIG_A\n"); break;
	//case SYM_A:	sprintf(tmp, "SYM_A\n"); break;
	case LLONG_T:	sprintf(tmp, "LLONG_T, val.f = %lli\n", t->val.f); break;
	case FREQ_T:	sprintf(tmp, "FREQ_T, val.f = %Lg Hz\n", (long double) t->val.f); break;
	case UNKNOWN_T:	sprintf(tmp, "UNKNOWN_T, val.f = %llx\n", t->val.f); break;

	default:
		sprintf(tmp, "Invalid Type!\n");
		break;
	}
	fprintf(stdout, "%s\n", tmp);

	return;
}

int s_sw(RIG *rig, vfo_t dst, vfo_t src)
{
	return -RIG_ENIMPL;
}


int s_cpy(RIG *rig, vfo_t dst, vfo_t src)
{
	int retval;
	channel_t ctmp;

	return -RIG_ENIMPL;	// don't use yet!

	if(dst == src) return RIG_OK;	// trivial

	// currently very broken since hamlib is broken too
	//retval = rig_save_channel(rig, src, &ctmp);
//	retval = rig_save_channel(rig, &ctmp);
	if(retval != RIG_OK) return retval;
	//return rig_restore_channel(rig, dst, &ctmp);
//	return rig_restore_channel(rig, &ctmp);
}

