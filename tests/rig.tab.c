
/*  A Bison parser, made from parser.y
 by  GNU Bison version 1.27
  */

#define YYBISON 1  /* Identify Bison output.  */

#define yyparse rig_yyparse
#define yylex rig_yylex
#define yyerror rig_yyerror
#define yylval rig_yylval
#define yychar rig_yychar
#define yydebug rig_yydebug
#define yynerrs rig_yynerrs
#define	RIG_TOKEN_CLOSE	257
#define	RIG_TOKEN_CALL	258
#define	RIG_TOKEN_CURR	259
#define	RIG_TOKEN_DEBUG	260
#define	RIG_TOKEN_EXIT	261
#define	RIG_TOKEN_FM	262
#define	RIG_TOKEN_FREQ	263
#define	RIG_TOKEN_HELP	264
#define	RIG_TOKEN_INIT	265
#define	RIG_TOKEN_MAIN	266
#define	RIG_TOKEN_MEM	267
#define	RIG_TOKEN_MODE	268
#define	RIG_TOKEN_MINUSMINUS	269
#define	RIG_TOKEN_OFFSET	270
#define	RIG_TOKEN_OPEN	271
#define	RIG_TOKEN_PLUSPLUS	272
#define	RIG_TOKEN_SETUP	273
#define	RIG_TOKEN_SHIFT	274
#define	RIG_TOKEN_SUB	275
#define	RIG_TOKEN_TONE	276
#define	RIG_TOKEN_AEQB	277
#define	RIG_TOKEN_BEQA	278
#define	RIG_TOKEN_AEQC	279
#define	RIG_TOKEN_CEQA	280
#define	RIG_TOKEN_BEQC	281
#define	RIG_TOKEN_CEQB	282
#define	RIG_TOKEN_ASWB	283
#define	RIG_TOKEN_ASWC	284
#define	RIG_TOKEN_FLOAT	285
#define	RIG_TOKEN_GHZ	286
#define	RIG_TOKEN_MHZ	287
#define	RIG_TOKEN_KHZ	288
#define	RIG_TOKEN_HZ	289
#define	RIG_TOKEN_MODEL	290
#define	RIG_LEX_MODEL	291
#define	RIG_TOKEN_STRING	292
#define	RIG_TOKEN_IDENTIFIER	293
#define	RIG_TOKEN_INT	294
#define	RIG_TOKEN_PORT	295
#define	RIG_TOKEN_RIG	296
#define	RIG_TOKEN_VFO	297
#define	RIG_LEX_VFO	298

#line 1 "parser.y"

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
#include "rigcmd.h"

#define RIG_YY_MAXRIGS	10
#define YYERROR_VERBOSE

static RIG *rigs[RIG_YY_MAXRIGS];
static rig_model_t models[RIG_YY_MAXRIGS];
static char *ports[RIG_YY_MAXRIGS];
static channel_t chans[RIG_YY_MAXRIGS];
static int default_idx = 0;	// Default index if none specified.
static vfo_t default_vfo = RIG_VFO_CURR;	// Default VFO

extern int yylineno;

//int setup(RIG *rig, rig_model_t model, char *port);
//int rig_setup(RIG *rig, rig_model_t model, char *port);
void help(void);
void yyerror(char *);
int s_cpy(RIG *, vfo_t, vfo_t);
int s_sw(RIG *, vfo_t, vfo_t);


#line 42 "parser.y"
typedef union {
	rig_model_t	model;
	RIG		*rig;
	channel_t	*chan;
	vfo_t		vfo;
	freq_t		freq;
	char		*txt;
	int		val;
	float		fval;
	spec_t		spec;
} YYSTYPE;
#ifndef YYDEBUG
#define YYDEBUG 1
#endif

#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		104
#define	YYFLAG		-32768
#define	YYNTBASE	58

#define YYTRANSLATE(x) ((unsigned)(x) <= 298 ? yytranslate[x] : 73)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,    52,
    53,     5,     3,    57,     4,    51,     6,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,    50,     2,
     7,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    54,     2,    55,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,    56,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     8,     9,    10,    11,
    12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
    22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
    32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
    42,    43,    44,    45,    46,    47,    48,    49
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     2,     5,     9,    11,    13,    15,    17,    19,    23,
    27,    31,    35,    39,    43,    47,    51,    55,    59,    63,
    67,    73,    76,    79,    82,    85,    87,    90,    93,    96,
    99,   101,   107,   115,   117,   122,   124,   129,   131,   136,
   138,   143,   145,   147,   149,   151,   153,   157,   162,   171,
   180,   186
};

static const short yyrhs[] = {    59,
     0,    60,    50,     0,    59,    60,    50,     0,    15,     0,
    12,     0,    72,     0,    61,     0,    62,     0,    65,     7,
    70,     0,    67,     7,    69,     0,    63,     7,    64,     0,
    66,    51,    28,     0,    66,    51,    29,     0,    66,    51,
    30,     0,    66,    51,    31,     0,    66,    51,    32,     0,
    66,    51,    33,     0,    66,    51,    34,     0,    66,    51,
    35,     0,    66,    51,    14,     0,    66,    51,    68,    51,
    14,     0,    45,    37,     0,    45,    38,     0,    45,    39,
     0,    45,    40,     0,    45,     0,    36,    37,     0,    36,
    38,     0,    36,    39,     0,    36,    40,     0,    36,     0,
    66,    51,    14,    52,    53,     0,    66,    51,    68,    51,
    14,    52,    53,     0,    41,     0,    41,    54,    45,    55,
     0,    47,     0,    47,    54,    45,    55,     0,    46,     0,
    46,    54,    45,    55,     0,    48,     0,    48,    52,    71,
    53,     0,    67,     0,    43,     0,    42,     0,    45,     0,
    49,     0,    71,    56,    49,     0,    11,    52,    45,    53,
     0,    66,    51,    22,    52,    65,    57,    69,    53,     0,
    22,    52,    66,    57,    65,    57,    69,    53,     0,    66,
    51,     8,    52,    53,     0,     8,    52,    66,    53,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
   137,   140,   141,   142,   143,   155,   157,   158,   161,   162,
   167,   172,   174,   175,   176,   177,   178,   179,   180,   182,
   186,   192,   193,   194,   195,   196,   197,   198,   199,   200,
   201,   203,   213,   225,   226,   229,   230,   233,   234,   237,
   238,   241,   242,   245,   246,   249,   250,   253,   256,   273,
   290,   305
};
#endif


#if YYDEBUG != 0 || defined (YYERROR_VERBOSE)

static const char * const yytname[] = {   "$","error","$undefined.","'+'","'-'",
"'*'","'/'","'='","RIG_TOKEN_CLOSE","RIG_TOKEN_CALL","RIG_TOKEN_CURR","RIG_TOKEN_DEBUG",
"RIG_TOKEN_EXIT","RIG_TOKEN_FM","RIG_TOKEN_FREQ","RIG_TOKEN_HELP","RIG_TOKEN_INIT",
"RIG_TOKEN_MAIN","RIG_TOKEN_MEM","RIG_TOKEN_MODE","RIG_TOKEN_MINUSMINUS","RIG_TOKEN_OFFSET",
"RIG_TOKEN_OPEN","RIG_TOKEN_PLUSPLUS","RIG_TOKEN_SETUP","RIG_TOKEN_SHIFT","RIG_TOKEN_SUB",
"RIG_TOKEN_TONE","RIG_TOKEN_AEQB","RIG_TOKEN_BEQA","RIG_TOKEN_AEQC","RIG_TOKEN_CEQA",
"RIG_TOKEN_BEQC","RIG_TOKEN_CEQB","RIG_TOKEN_ASWB","RIG_TOKEN_ASWC","RIG_TOKEN_FLOAT",
"RIG_TOKEN_GHZ","RIG_TOKEN_MHZ","RIG_TOKEN_KHZ","RIG_TOKEN_HZ","RIG_TOKEN_MODEL",
"RIG_LEX_MODEL","RIG_TOKEN_STRING","RIG_TOKEN_IDENTIFIER","RIG_TOKEN_INT","RIG_TOKEN_PORT",
"RIG_TOKEN_RIG","RIG_TOKEN_VFO","RIG_LEX_VFO","';'","'.'","'('","')'","'['",
"']'","'|'","','","top","rig_list","rig_cmd","assign_exp","super_function","freq_lhs",
"freq_val","model_id","rig_id","port_id","vfo_id","port_id_rhs","model_type",
"vfo_type","rig_function", NULL
};
#endif

static const short yyr1[] = {     0,
    58,    59,    59,    59,    59,    60,    60,    60,    61,    61,
    61,    62,    62,    62,    62,    62,    62,    62,    62,    63,
    63,    64,    64,    64,    64,    64,    64,    64,    64,    64,
    64,    64,    64,    65,    65,    66,    66,    67,    67,    68,
    68,    69,    69,    70,    70,    71,    71,    72,    72,    72,
    72,    72
};

static const short yyr2[] = {     0,
     1,     2,     3,     1,     1,     1,     1,     1,     3,     3,
     3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
     5,     2,     2,     2,     2,     1,     2,     2,     2,     2,
     1,     5,     7,     1,     4,     1,     4,     1,     4,     1,
     4,     1,     1,     1,     1,     1,     3,     4,     8,     8,
     5,     4
};

static const short yydefact[] = {     0,
     0,     0,     5,     4,     0,    34,    38,    36,     1,     0,
     7,     8,     0,     0,     0,     0,     6,     0,     0,     0,
     0,     0,     0,     0,     2,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     3,    31,    26,    11,     0,
    44,    45,     9,     0,    20,     0,    12,    13,    14,    15,
    16,    17,    18,    19,    40,     0,    43,    42,    10,    52,
    48,     0,    35,    39,    37,    27,    28,    29,    30,    22,
    23,    24,    25,     0,     0,     0,     0,     0,     0,     0,
     0,    51,     0,    46,     0,    21,     0,     0,     0,     0,
    41,     0,     0,    32,     0,     0,    47,    50,     0,    49,
    33,     0,     0,     0
};

static const short yydefgoto[] = {   102,
     9,    10,    11,    12,    13,    39,    14,    15,    58,    56,
    59,    43,    85,    17
};

static const short yypact[] = {    -4,
   -37,   -24,-32768,-32768,   -20,    -6,    -3,    -2,     8,   -16,
-32768,-32768,    39,    43,     2,    57,-32768,    18,    21,    18,
    22,    24,    25,    26,-32768,     0,   -40,    -8,   -34,    15,
    20,    14,    17,    27,    28,-32768,    19,    23,-32768,    30,
-32768,-32768,-32768,    32,-32768,    33,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,    34,    36,-32768,-32768,-32768,-32768,
-32768,    37,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,   -13,    35,    37,    31,    60,    38,    40,
    42,-32768,    41,-32768,   -43,-32768,   -34,    44,    61,   -34,
-32768,    45,    46,-32768,    48,    49,-32768,-32768,    50,-32768,
-32768,    77,    79,-32768
};

static const short yypgoto[] = {-32768,
-32768,    80,-32768,-32768,-32768,-32768,   -59,    13,    29,    16,
   -46,-32768,-32768,-32768
};


#define	YYLAST		103


static const short yytable[] = {    44,
    80,    41,    79,     1,    42,    45,     2,     3,    57,    91,
     4,     7,    92,    46,    18,     1,    83,     5,     2,    47,
    48,    49,    50,    51,    52,    53,    54,    19,    16,     5,
    30,    20,    32,    25,    55,    37,     6,    16,    40,    55,
    93,     7,     8,    96,    38,    26,     8,    21,     6,    27,
    22,    23,    28,     7,     8,    66,    67,    68,    69,    70,
    71,    72,    73,    29,     8,    31,    33,    60,    34,    35,
    62,    63,    61,    86,    95,    36,   103,     6,   104,    84,
    74,    64,    65,    75,    76,    77,    78,    82,    24,    81,
     0,    88,    89,    97,    87,     0,    94,    90,    98,    99,
     0,   100,   101
};

static const short yycheck[] = {     8,
    14,    42,    62,     8,    45,    14,    11,    12,    43,    53,
    15,    46,    56,    22,    52,     8,    76,    22,    11,    28,
    29,    30,    31,    32,    33,    34,    35,    52,     0,    22,
    18,    52,    20,    50,    48,    36,    41,     9,    26,    48,
    87,    46,    47,    90,    45,     7,    47,    54,    41,     7,
    54,    54,    51,    46,    47,    37,    38,    39,    40,    37,
    38,    39,    40,     7,    47,    45,    45,    53,    45,    45,
    57,    55,    53,    14,    14,    50,     0,    41,     0,    49,
    51,    55,    55,    52,    52,    52,    51,    53,     9,    74,
    -1,    52,    51,    49,    57,    -1,    53,    57,    53,    52,
    -1,    53,    53
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "/usr/share/bison.simple"
/* This file comes from bison-1.27.  */

/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

#ifndef YYSTACK_USE_ALLOCA
#ifdef alloca
#define YYSTACK_USE_ALLOCA
#else /* alloca not defined */
#ifdef __GNUC__
#define YYSTACK_USE_ALLOCA
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi) || (defined (__sun) && defined (__i386))
#define YYSTACK_USE_ALLOCA
#include <alloca.h>
#else /* not sparc */
/* We think this test detects Watcom and Microsoft C.  */
/* This used to test MSDOS, but that is a bad idea
   since that symbol is in the user namespace.  */
#if (defined (_MSDOS) || defined (_MSDOS_)) && !defined (__TURBOC__)
#if 0 /* No need for malloc.h, which pollutes the namespace;
	 instead, just don't use alloca.  */
#include <malloc.h>
#endif
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
/* I don't know what this was needed for, but it pollutes the namespace.
   So I turned it off.   rms, 2 May 1997.  */
/* #include <malloc.h>  */
 #pragma alloca
#define YYSTACK_USE_ALLOCA
#else /* not MSDOS, or __TURBOC__, or _AIX */
#if 0
#ifdef __hpux /* haible@ilog.fr says this works for HPUX 9.05 and up,
		 and on HPUX 10.  Eventually we can turn this on.  */
#define YYSTACK_USE_ALLOCA
#define alloca __builtin_alloca
#endif /* __hpux */
#endif
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc */
#endif /* not GNU C */
#endif /* alloca not defined */
#endif /* YYSTACK_USE_ALLOCA not defined */

#ifdef YYSTACK_USE_ALLOCA
#define YYSTACK_ALLOC alloca
#else
#define YYSTACK_ALLOC malloc
#endif

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	goto yyacceptlab
#define YYABORT 	goto yyabortlab
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    { yychar = (token), yylval = (value);			\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, &yylloc, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval, &yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	yychar;			/*  the lookahead symbol		*/
YYSTYPE	yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int yynerrs;			/*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
int yydebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/* Define __yy_memcpy.  Note that the size argument
   should be passed with type unsigned int, because that is what the non-GCC
   definitions require.  With GCC, __builtin_memcpy takes an arg
   of type size_t, but it can handle unsigned int.  */

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __yy_memcpy(TO,FROM,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (to, from, count)
     char *to;
     char *from;
     unsigned int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (char *to, char *from, unsigned int count)
{
  register char *t = to;
  register char *f = from;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif

#line 216 "/usr/share/bison.simple"

/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
#ifdef __cplusplus
#define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#else /* not __cplusplus */
#define YYPARSE_PARAM_ARG YYPARSE_PARAM
#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
#endif /* not __cplusplus */
#else /* not YYPARSE_PARAM */
#define YYPARSE_PARAM_ARG
#define YYPARSE_PARAM_DECL
#endif /* not YYPARSE_PARAM */

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
#ifdef YYPARSE_PARAM
int yyparse (void *);
#else
int yyparse (void);
#endif
#endif

int
yyparse(YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;
  int yyfree_stacks = 0;

#ifdef YYPURE
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int yylen;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Starting parse\n");
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror("parser stack overflow");
	  if (yyfree_stacks)
	    {
	      free (yyss);
	      free (yyvs);
#ifdef YYLSP_NEEDED
	      free (yyls);
#endif
	    }
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
#ifndef YYSTACK_USE_ALLOCA
      yyfree_stacks = 1;
#endif
      yyss = (short *) YYSTACK_ALLOC (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss, (char *)yyss1,
		   size * (unsigned int) sizeof (*yyssp));
      yyvs = (YYSTYPE *) YYSTACK_ALLOC (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs, (char *)yyvs1,
		   size * (unsigned int) sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) YYSTACK_ALLOC (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls, (char *)yyls1,
		   size * (unsigned int) sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  goto yybackup;
 yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Reading a token: ");
#endif
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
      if (yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 1:
#line 137 "parser.y"
{ if(yyvsp[0].val == RIG_OK) YYACCEPT; else YYABORT; ;
    break;}
case 2:
#line 140 "parser.y"
{ yyval.val = yyvsp[-1].val; ;
    break;}
case 3:
#line 141 "parser.y"
{ yyval.val = yyvsp[-1].val; ;
    break;}
case 4:
#line 142 "parser.y"
{ help(); yyval.val = RIG_OK; ;
    break;}
case 5:
#line 143 "parser.y"
{
			int i;
			for( i=0; i<RIG_YY_MAXRIGS; i++) {
				if(rigs[i] != NULL) {
					rig_close(rigs[i]);
					free(rigs[i]);
				}
			}
			exit(0);
		;
    break;}
case 9:
#line 161 "parser.y"
{ models[yyvsp[-2].val] = yyvsp[0].model; yyval.val = RIG_OK; ;
    break;}
case 10:
#line 162 "parser.y"
{
			//if( $1 != NULL) free($1);
			ports[yyvsp[-2].val] = yyvsp[0].txt;
			yyval.val = RIG_OK;
		;
    break;}
case 11:
#line 167 "parser.y"
{
			yyval.val = rig_set_freq(yyvsp[-2].spec.rig, yyvsp[-2].spec.vfo, yyvsp[0].freq);
		;
    break;}
case 12:
#line 173 "parser.y"
{ yyval.val = s_cpy(rigs[yyvsp[-2].val], RIG_VFO_A, RIG_VFO_B); ;
    break;}
case 13:
#line 174 "parser.y"
{ yyval.val = s_cpy(rigs[yyvsp[-2].val], RIG_VFO_B, RIG_VFO_A); ;
    break;}
case 14:
#line 175 "parser.y"
{ yyval.val = s_cpy(rigs[yyvsp[-2].val], RIG_VFO_A, RIG_VFO_C); ;
    break;}
case 15:
#line 176 "parser.y"
{ yyval.val = s_cpy(rigs[yyvsp[-2].val], RIG_VFO_C, RIG_VFO_A); ;
    break;}
case 16:
#line 177 "parser.y"
{ yyval.val = s_cpy(rigs[yyvsp[-2].val], RIG_VFO_B, RIG_VFO_C); ;
    break;}
case 17:
#line 178 "parser.y"
{ yyval.val = s_cpy(rigs[yyvsp[-2].val], RIG_VFO_C, RIG_VFO_B); ;
    break;}
case 18:
#line 179 "parser.y"
{ yyval.val = s_sw(rigs[yyvsp[-2].val], RIG_VFO_A, RIG_VFO_B); ;
    break;}
case 19:
#line 180 "parser.y"
{ yyval.val = s_sw(rigs[yyvsp[-2].val], RIG_VFO_A, RIG_VFO_C); ;
    break;}
case 20:
#line 182 "parser.y"
{
			yyval.spec.rig = rigs[yyvsp[-2].val];
			yyval.spec.vfo = default_vfo;
		;
    break;}
case 21:
#line 186 "parser.y"
{
			yyval.spec.rig = rigs[yyvsp[-4].val];
			yyval.spec.vfo = yyvsp[-2].vfo;
		;
    break;}
case 22:
#line 192 "parser.y"
{ yyval.freq = yyvsp[-1].val * yyvsp[0].fval; ;
    break;}
case 23:
#line 193 "parser.y"
{ yyval.freq = yyvsp[-1].val * yyvsp[0].fval; ;
    break;}
case 24:
#line 194 "parser.y"
{ yyval.freq = yyvsp[-1].val * yyvsp[0].fval; ;
    break;}
case 25:
#line 195 "parser.y"
{ yyval.freq = yyvsp[-1].val; ;
    break;}
case 26:
#line 196 "parser.y"
{ yyval.freq = yyvsp[0].val; ;
    break;}
case 27:
#line 197 "parser.y"
{ yyval.freq = (freq_t)(yyvsp[-1].fval * yyvsp[0].fval); ;
    break;}
case 28:
#line 198 "parser.y"
{ yyval.freq = (freq_t)(yyvsp[-1].fval * yyvsp[0].fval); ;
    break;}
case 29:
#line 199 "parser.y"
{ yyval.freq = (freq_t)(yyvsp[-1].fval * yyvsp[0].fval); ;
    break;}
case 30:
#line 200 "parser.y"
{ yyval.freq = (freq_t)yyvsp[-1].fval; ;
    break;}
case 31:
#line 201 "parser.y"
{ yyval.freq = (freq_t)yyvsp[0].fval; ;
    break;}
case 32:
#line 203 "parser.y"
{
			int retval;
			freq_t ftmp;
			retval = rig_get_freq(rigs[yyvsp[-4].val], RIG_VFO_CURR, &ftmp);
			if(retval != RIG_OK) {
				yyerror("get_freq(): error occured");
				YYERROR;
			}
			yyval.freq = ftmp;
		;
    break;}
case 33:
#line 213 "parser.y"
{
			int retval;
			freq_t ftmp;
			retval = rig_get_freq(rigs[yyvsp[-6].val], yyvsp[-4].vfo, &ftmp);
			if(retval != RIG_OK) {
				yyerror("get_freq(): error occured");
				YYERROR;
			}
			yyval.freq = ftmp;
		;
    break;}
case 34:
#line 225 "parser.y"
{ yyval.val = yyvsp[0].model; ;
    break;}
case 35:
#line 226 "parser.y"
{ yyval.val = yyvsp[-1].val; ;
    break;}
case 36:
#line 229 "parser.y"
{ yyval.val = yyvsp[0].val; ;
    break;}
case 37:
#line 230 "parser.y"
{ yyval.val = yyvsp[-1].val; ;
    break;}
case 38:
#line 233 "parser.y"
{ yyval.val = yyvsp[0].val; ;
    break;}
case 39:
#line 234 "parser.y"
{ yyval.val = yyvsp[-1].val; ;
    break;}
case 40:
#line 237 "parser.y"
{ yyval.vfo = RIG_VFO_CURR; ;
    break;}
case 41:
#line 238 "parser.y"
{ yyval.vfo = yyvsp[-1].vfo; ;
    break;}
case 42:
#line 241 "parser.y"
{ yyval.txt = ports[yyvsp[0].val]; ;
    break;}
case 43:
#line 242 "parser.y"
{ yyval.txt = yyvsp[0].txt; ;
    break;}
case 44:
#line 245 "parser.y"
{ yyval.model = yyvsp[0].model; ;
    break;}
case 45:
#line 246 "parser.y"
{ yyval.model = (rig_model_t) yyvsp[0].val; ;
    break;}
case 46:
#line 249 "parser.y"
{ yyval.vfo = yyvsp[0].vfo; ;
    break;}
case 47:
#line 250 "parser.y"
{ yyval.vfo = yyvsp[-2].vfo | yyvsp[0].vfo; ;
    break;}
case 48:
#line 254 "parser.y"
{	rig_set_debug( yyvsp[-1].val ); yyval.val = RIG_OK; ;
    break;}
case 49:
#line 257 "parser.y"
{
		if( rigs[yyvsp[-7].val] == NULL) {
			/*$$ = rig_setup(&rigs[$3], models[$5], $7);*/
			rigs[yyvsp[-7].val] = rig_init(models[yyvsp[-3].val]);
			strncpy(rigs[yyvsp[-7].val]->state.rigport.pathname, yyvsp[-1].txt, FILPATHLEN);
			yyval.val = rig_open(rigs[yyvsp[-7].val]);
			rig_debug(RIG_DEBUG_VERBOSE,
			   "rig_function: open rigs[%i]=%x, model[%i] = %i, port=%s\n",
				yyvsp[-7].val, rigs[yyvsp[-7].val], yyvsp[-3].val, models[yyvsp[-3].val], yyvsp[-1].txt);
			if(yyval.val != RIG_OK || rigs[yyvsp[-7].val] == NULL)
				yyerror("open() failed.\n");
		} else {
			yyerror("use close() first!");	// semantics.
			yyval.val = -RIG_EPROTO;
		}
	;
    break;}
case 50:
#line 274 "parser.y"
{
		if( rigs[yyvsp[-5].val] == NULL) {
			/*$$ = rig_setup(&rigs[$3], models[$5], $7);*/
			rigs[yyvsp[-5].val] = rig_init(models[yyvsp[-3].val]);
			strncpy(rigs[yyvsp[-5].val]->state.rigport.pathname, yyvsp[-1].txt, FILPATHLEN);
			yyval.val = rig_open(rigs[yyvsp[-5].val]);
			rig_debug(RIG_DEBUG_VERBOSE,
			   "rig_function: open rigs[%i]=%x, model[%i] = %i, port=%s\n",
				yyvsp[-5].val, rigs[yyvsp[-5].val], yyvsp[-3].val, models[yyvsp[-3].val], yyvsp[-1].txt);
			if(yyval.val != RIG_OK || rigs[yyvsp[-5].val] == NULL)
				yyerror("open() failed.\n");
		} else {
			yyerror("use close() first!");	// semantics.
			yyval.val = -RIG_EPROTO;
		}
	;
    break;}
case 51:
#line 291 "parser.y"
{
		if(rigs[yyvsp[-4].val] != NULL) {
			yyval.val = rig_close(rigs[yyvsp[-4].val]);
			if(yyval.val != RIG_OK)
				yyerror("close() failed.\n");
			yyval.val = rig_cleanup(rigs[yyvsp[-4].val]);
			rigs[yyvsp[-4].val] = NULL;
			if(yyval.val != RIG_OK)
				yyerror("cleanup() failed.\n");
		} else {
			yyerror("use open() first.");	// semantics.
			yyval.val = -RIG_EPROTO;
		}
	;
    break;}
case 52:
#line 306 "parser.y"
{
		if(rigs[yyvsp[-1].val] != NULL) {
			yyval.val = rig_close(rigs[yyvsp[-1].val]);
			if(yyval.val != RIG_OK)
				yyerror("close() failed.\n");
			yyval.val = rig_cleanup(rigs[yyvsp[-1].val]);
			rigs[yyvsp[-1].val] = NULL;
			if(yyval.val != RIG_OK)
				yyerror("cleanup() failed.\n");
		} else {
			yyerror("use open() first.");	// semantics.
			yyval.val = -RIG_EPROTO;
		}
	;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */
#line 542 "/usr/share/bison.simple"

  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      yyerror(msg);
	      free(msg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror("parse error");
    }

  goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

yyerrhandle:

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;

 yyacceptlab:
  /* YYACCEPT comes here.  */
  if (yyfree_stacks)
    {
      free (yyss);
      free (yyvs);
#ifdef YYLSP_NEEDED
      free (yyls);
#endif
    }
  return 0;

 yyabortlab:
  /* YYABORT comes here.  */
  if (yyfree_stacks)
    {
      free (yyss);
      free (yyvs);
#ifdef YYLSP_NEEDED
      free (yyls);
#endif
    }
  return 1;
}
#line 322 "parser.y"


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
	rig_debug(RIG_DEBUG_ERR, "cmd_parse: %s on line %i\n", err, yylineno);
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
