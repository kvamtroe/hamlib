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


extern YYSTYPE rig_yylval;
#define yylval rig_yylval
#define yylineno rig_yylineno
#define yyparse rig_yyparse
