/*
 * rigcmd.h	Copyright (C) 2002 by Dale E. Edmons.  All rights reserved.
 *
 *	license: GNU
 */

struct struc_s {
	RIG		*rig;
	vfo_t		vfo;
	channel_t	*chan;
	void		(*func)();
};

typedef struct struc_s spec_t;

#define yylineno rig_yylineno
extern int yylineno;

int keyword(char * token);
