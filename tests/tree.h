/*
 * rigcmd.h	Copyright (C) 2002 by Dale E. Edmons.  All rights reserved.
 *
 *	license: GNU
 */

#ifndef _RIGTREE_H
#define _RIGTREE_H

/* non-trivial tokens and rules */
enum attrib_e {
	CHAN_A=1,
	FUNC_A,
	RIG_A,
	SYM_A,
	UNKNOWN_A
};
typedef enum attrib_e attrib_t;

enum types_e {
	CHAR_T=UNKNOWN_A,
	DOUBLE_T,
	FLOAT_T,
	FREQ_T,
	INT_T,
	LDOUBLE_T,
	LLONG_T,
	LONG_T,
	MODE_T,
	MODEL_T,
	STRING_T,
	VFO_T,
	FUNC_T,
	IDENTIFIER_T,
	UNKNOWN_T
};
typedef enum types_e type_t;

struct symtab_s {
	char		*name;
	struct symtab_s	*next;
	type_t		type;
	union val {
		int		i;
		freq_t		f;
		vfo_t		v;
		double		d;
		rmode_t		m;
		rig_model_t	r;
		char		*s;
		long double	D;
		char		c;
		channel_t	*chan;
		void		(*func)();
	} val;
};
typedef struct symtab_s symtab_t;

struct tree_s {
	RIG		*rig;
	symtab_t	*symptr;	// FIXME: remove this someday
	void		(*func)();	// FIXME: remove this someday
	channel_t	*chan;	// FIXME: remove this someday
	symtab_t	*lsymptr;
	symtab_t	*rsymptr;
	attrib_t	lattrib;
	attrib_t	rattrib;
	int		retval;
	symtab_t	sym;	// FIXME: remove this someday
};
typedef struct tree_s tree_t;

/************************ Parser Stuff ****************************/

int s_cpy(RIG *, vfo_t, vfo_t);
int s_cp(tree_t *, tree_t *);
int s_sw(RIG *, vfo_t, vfo_t);
int s_cp(tree_t *, tree_t *);

/************************ Scanner Stuff ****************************/

// links to scanner
extern symtab_t *symbols;	// linked list

void disp_sym(symtab_t *);
void disp_tree(tree_t *);
tree_t * tree_init(void);

#endif // _RIGTREE_H
// end tree.h
