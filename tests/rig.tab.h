typedef union {
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
} YYSTYPE;
#define	RIG_TOKEN_CLOSE	257
#define	RIG_TOKEN_CALL	258
#define	RIG_TOKEN_CURR	259
#define	RIG_TOKEN_CTCSS	260
#define	RIG_TOKEN_DEBUG	261
#define	RIG_TOKEN_EXIT	262
#define	RIG_TOKEN_CW	263
#define	RIG_TOKEN_AM	264
#define	RIG_TOKEN_FM	265
#define	RIG_TOKEN_LSB	266
#define	RIG_TOKEN_USB	267
#define	RIG_TOKEN_RTTY	268
#define	RIG_TOKEN_FREQ	269
#define	RIG_TOKEN_HELP	270
#define	RIG_TOKEN_INIT	271
#define	RIG_TOKEN_MAIN	272
#define	RIG_TOKEN_MENU	273
#define	RIG_TOKEN_MEM	274
#define	RIG_TOKEN_MODE	275
#define	RIG_TOKEN_MINUSEQ	276
#define	RIG_TOKEN_MINUSMINUS	277
#define	RIG_TOKEN_OFFSET	278
#define	RIG_TOKEN_OPEN	279
#define	RIG_TOKEN_PLUSEQ	280
#define	RIG_TOKEN_PLUSPLUS	281
#define	RIG_TOKEN_PM	282
#define	RIG_TOKEN_PRINT	283
#define	RIG_TOKEN_RIGDEFAULT	284
#define	RIG_TOKEN_SETUP	285
#define	RIG_TOKEN_SHIFT	286
#define	RIG_TOKEN_SUB	287
#define	RIG_TOKEN_TONE	288
#define	RIG_TOKEN_BEQA	289
#define	RIG_TOKEN_CEQA	290
#define	RIG_TOKEN_AEQB	291
#define	RIG_TOKEN_CEQB	292
#define	RIG_TOKEN_AEQC	293
#define	RIG_TOKEN_BEQC	294
#define	RIG_TOKEN_AEQM	295
#define	RIG_TOKEN_BEQM	296
#define	RIG_TOKEN_CEQM	297
#define	RIG_TOKEN_MEQA	298
#define	RIG_TOKEN_MEQB	299
#define	RIG_TOKEN_MEQC	300
#define	RIG_TOKEN_ASWB	301
#define	RIG_TOKEN_ASWC	302
#define	RIG_TOKEN_BSWC	303
#define	RIG_TOKEN_ASWM	304
#define	RIG_TOKEN_BSWM	305
#define	RIG_TOKEN_CSWM	306
#define	RIG_LEX_HAMLIB	307
#define	RIG_TOKEN_FLOAT	308
#define	RIG_TOKEN_GHZ	309
#define	RIG_TOKEN_MHZ	310
#define	RIG_TOKEN_KHZ	311
#define	RIG_TOKEN_HZ	312
#define	RIG_TOKEN_MODEL	313
#define	RIG_LEX_MODEL	314
#define	RIG_TOKEN_STRING	315
#define	RIG_TOKEN_IDENT	316
#define	RIG_TOKEN_INT	317
#define	RIG_TOKEN_PORT	318
#define	RIG_TOKEN_RIG	319
#define	RIG_TOKEN_VFO	320
#define	RIG_LEX_VFO	321


extern YYSTYPE rig_yylval;
