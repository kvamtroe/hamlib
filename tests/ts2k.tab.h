typedef union {
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
} YYSTYPE;
#define	RIG_TOKEN_CLOSE	257
#define	RIG_TOKEN_EXIT	258
#define	RIG_TOKEN_FREQ	259
#define	RIG_TOKEN_HELP	260
#define	RIG_TOKEN_INIT	261
#define	RIG_TOKEN_OPEN	262
#define	RIG_TOKEN_SETUP	263
#define	RIG_TOKEN_AEQB	264
#define	RIG_TOKEN_BEQA	265
#define	RIG_TOKEN_AEQC	266
#define	RIG_TOKEN_CEQA	267
#define	RIG_TOKEN_BEQC	268
#define	RIG_TOKEN_CEQB	269
#define	RIG_TOKEN_ASWB	270
#define	RIG_TOKEN_ASWC	271
#define	RIG_TOKEN_FLOAT	272
#define	RIG_TOKEN_GHZ	273
#define	RIG_TOKEN_MHZ	274
#define	RIG_TOKEN_KHZ	275
#define	RIG_TOKEN_HZ	276
#define	RIG_TOKEN_MODEL	277
#define	RIG_TOKEN_DUMMY	278
#define	RIG_TOKEN_TS2K	279
#define	RIG_TOKEN_TS2000	280
#define	RIG_TOKEN_STRING	281
#define	RIG_TOKEN_INT	282
#define	RIG_TOKEN_PORT	283
#define	RIG_TOKEN_RIG	284
#define	RIG_TOKEN_VFO	285
#define	RIG_LEX_CALLA	286
#define	RIG_LEX_CALLC	287
#define	RIG_LEX_MEMA	288
#define	RIG_LEX_MEMC	289
#define	RIG_LEX_VFOA	290
#define	RIG_LEX_VFOB	291
#define	RIG_LEX_VFOC	292
#define	RIG_LEX_VFOAB	293
#define	RIG_LEX_VFOBA	294
#define	RIG_LEX_CROSS	295
#define	RIG_LEX_FAKE	296
#define	RIG_LEX_MAIN	297
#define	RIG_LEX_RIT	298
#define	RIG_LEX_SAT	299
#define	RIG_LEX_SCAN	300
#define	RIG_LEX_SUB	301
#define	RIG_LEX_XIT	302
#define	RIG_LEX_CURR	303
#define	RIG_LEX_MEM	304
#define	RIG_LEX_VFO	305


extern YYSTYPE ts2k_yylval;
