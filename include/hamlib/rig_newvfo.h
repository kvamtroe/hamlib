/*
 * Differences between branch_ts2k and HEAD	--Dale
 */

#ifdef _RIG_NEWVFO_PART_1

enum rptr_shift_e {
	RIG_RPT_SHIFT_NONE = 0,
	RIG_RPT_SHIFT_MINUS,
	RIG_RPT_SHIFT_PLUS,
	RIG_RPT_SHIFT_1750	// ts2000 E-type '='
};

# undef _RIG_NEWVFO_PART_1
#endif

#ifdef _RIG_NEWVFO_PART_2

/*
 * I've cleaned up the VFO definition to make it easier to change
 * when the MoonMelter is finally released.  Essentially, I've
 * done nothing.	--Dale	:)
 */

/*
 * Upper segment: "rig Major"
 * Lower segment: "VFO minor"
 *
 *	MSB		    LSB
 *	N	n+1 n	      0
 *	+-+-+-+-+-+-+-+-+-+-+-+
 *	|	   |	      |
 *	    Rig		VFO
 *	   Major       minor
 */
//typedef unsigned int vfo_t;
typedef unsigned long int vfo_t;

#define BIT(a)	( ((vfo_t) 1) << (a))
//#define BIT(a)	(1L << (a))

#define RIG_MINOR	6
/* M=Major, m=minor */
#define RIG_SET_VFO(M,m)	((vfo_t) ( ((M) << (RIG_MINOR+1)) | (m) ))
/* Note: prior definition exibited exponential growth in bit count */

#define RIG_VFO_CTRL	RIG_SET_VFO(0, BIT(0))
#define RIG_VFO_PTT	RIG_SET_VFO(0, BIT(1))

/* VFO Minor */
#define RIG_VFO1	RIG_SET_VFO(0, BIT(2))	// VFO_A
#define RIG_VFO2	RIG_SET_VFO(0, BIT(3))	// VFO_B
#define RIG_VFO3	RIG_SET_VFO(0, BIT(4))	// VFO_C
#define RIG_VFO4	RIG_SET_VFO(0, BIT(5))	// MEM
#define RIG_VFO5	RIG_SET_VFO(0, BIT(6))	// CALL
/*					   |
 * RIG_MINOR = n :== MAX >-----------------'
 */

/* Rig Major */
#define RIG_CTRL_MAIN	RIG_SET_VFO(BIT(0), 0)
#define RIG_CTRL_SUB	RIG_SET_VFO(BIT(1), 0)
#define RIG_CTRL_MEM	RIG_SET_VFO(BIT(2), 0)
#define RIG_CTRL_CALL	RIG_SET_VFO(BIT(3), 0)
#define RIG_CTRL_SPLIT	RIG_SET_VFO(BIT(4), 0)
#define RIG_CTRL_RIT	RIG_SET_VFO(BIT(5), 0)
#define RIG_CTRL_XIT	RIG_SET_VFO(BIT(6), 0)
#define RIG_CTRL_REV	RIG_SET_VFO(BIT(7), 0)
#define RIG_CTRL_SCAN	RIG_SET_VFO(BIT(8), 0)
#define RIG_CTRL_SAT	RIG_SET_VFO(BIT(9), 0)
#define RIG_CTRL_CROSS	RIG_SET_VFO(BIT(10), 0)
#define RIG_CTRL_FAKE	RIG_SET_VFO(BIT(11), 0)
/*					|
 * RIG_MAJOR = n :== MAX >--------------'
 */
#define RIG_MAJOR (RIG_MINOR + 11)

/* VFO stuff that may be handy. */
#define RIG_VFO_MASK	(RIG_VFO1 | RIG_VFO2 | RIG_VFO3 | RIG_VFO4 | RIG_VFO5)
#define RIG_CTRL_MASK	(RIG_CTRL_MAIN | RIG_CTRL_SUB | RIG_CTRL_MEM \
			| RIG_CTRL_CALL | RIG_CTRL_SPLIT | RIG_CTRL_SCAN \
			| RIG_CTRL_SAT | RIG_CTRL_REV)
#define RIG_VFO_VALID	(RIG_CTRL_MASK | RIG_VFO_MASK)
#define RIG_VFO_TEST(v)	(((v) & ~RIG_VFO_VALID) == 0)

/* The following are for compatibility with existing code! */

/* Standard VFO's for common use */
#define RIG_VFO_A	(RIG_CTRL_MAIN | RIG_VFO1)
#define RIG_VFO_B	(RIG_CTRL_MAIN | RIG_VFO2)
#define RIG_VFO_C	(RIG_CTRL_SUB  | RIG_VFO3)

/* More standard VFO's but these are new! */
// How is it possible for rig_get_vfo() to tell use we're split otherwise?
#define RIG_VFO_AB	((RIG_CTRL_SPLIT | RIG_CTRL_MAIN) | (RIG_VFO1 | RIG_VFO2))
#define RIG_VFO_BA	((RIG_CTRL_REV | RIG_CTRL_SPLIT | RIG_CTRL_MAIN) \
						 | (RIG_VFO1 | RIG_VFO2))
#define RIG_SAT_DNLINK	(RIG_CTRL_SAT | RIG_VFO_A)
#define RIG_SAT_UPLINK	(RIG_CTRL_SAT | RIG_VFO_B)

/* memories (except temp) */
#define RIG_VFO_MEM_A	((RIG_CTRL_MEM | RIG_CTRL_MAIN) | RIG_VFO4)
#define RIG_VFO_MEM_C	((RIG_CTRL_MEM | RIG_CTRL_SUB)  | RIG_VFO4)
#define RIG_VFO_CALL_A	((RIG_CTRL_MEM | RIG_CTRL_MAIN) | RIG_VFO5)
#define RIG_VFO_CALL_C	((RIG_CTRL_MEM | RIG_CTRL_SUB)  | RIG_VFO5)

/* Standard control's for common use */
#define RIG_VFO_MEM	(RIG_CTRL_MEM | RIG_VFO_VFO)
#define RIG_VFO_MAIN	RIG_CTRL_MAIN
#define RIG_VFO_SUB	RIG_CTRL_SUB

/* pseudo targets */
#define RIG_VFO_CURR	(RIG_CTRL_FAKE | RIG_VFO1)
#define RIG_VFO_VFO	(RIG_CTRL_FAKE | RIG_VFO2)

/* Hopefully, to following will be dropped */
#define RIG_VFO_NONE	(RIG_CTRL_FAKE | RIG_VFO3)
#define RIG_VFO_ALL	(RIG_CTRL_FAKE | RIG_VFO4)
/*
 * Ahhh.  Now I can live happy and die free! --Dale
 */

//#define RIG_MODE_WFM   	(1<<6)	/* after all, Wide FM is a mode on its own */
#define RIG_MODE_WIDE   	(1<<7)	/* after all, Wide FM is a mode on its own */
#define RIG_MODE_NARROW   	(1<<8)	/* after all, Wide FM is a mode on its own */

# undef _RIG_NEWVFO_PART_2
#endif

#ifdef _RIG_NEWVFO_PART_3

struct channel {
	int channel_num;
	int bank_num;
	vfo_t vfo;
	int ant;
	freq_t freq;
	rmode_t mode;
	pbwidth_t width;
	vfo_t tx_vfo;	// This may not stay 
	freq_t tx_freq;
	rmode_t tx_mode;
	pbwidth_t tx_width;
	split_t split;
	rptr_shift_t rptr_shift;
	shortfreq_t rptr_offs;
	shortfreq_t tuning_step;
	shortfreq_t rit;
	shortfreq_t xit;
	setting_t funcs;
	value_t levels[RIG_SETTING_MAX];
	tone_t tone;		/* added --Dale kd7eni */
	tone_t tone_sql;	/* tone => tx, ctcss => rx */
	tone_t ctcss_tone;
	tone_t ctcss;
	tone_t ctcss_sql;
	tone_t dcs;
	tone_t dcs_code;
	tone_t dcs_sql;
	scan_t scan;
	int scan_group;		/* added, I use this often --kd7eni */
	int flags;		/* RIG_CHFLAGS... */
	char channel_desc[MAXCHANDESC];
	struct ext_list *ext_levels;	/* none for ts2000 --Dale */
};

# undef _RIG_NEWVFO_PART_3
#endif

#ifdef _RIG_NEWVFO_PART_4

struct rig_caps {
	rig_model_t rig_model;
	const char *model_name;
	const char *mfg_name;
	const char *version;
	const char *copyright;
	enum rig_status_e status;

	int rig_type;
	enum ptt_type_e ptt_type;
	enum dcd_type_e dcd_type;
	enum rig_port_e port_type;

	int serial_rate_min;
	int serial_rate_max;
	int serial_data_bits;
	int serial_stop_bits;
	enum serial_parity_e serial_parity;
	enum serial_handshake_e serial_handshake;

	int write_delay;
	int post_write_delay;
	int timeout;
	int retry;

	setting_t has_get_func;
	setting_t has_set_func;
	setting_t has_get_level;
	setting_t has_set_level;
	setting_t has_get_parm;
	setting_t has_set_parm;

	int level_gran[RIG_SETTING_MAX];
	int parm_gran[RIG_SETTING_MAX];

	const struct confparms *extparms;
	const struct confparms *extlevels;

	const tone_t *ctcss_list;
	const tone_t *dcs_list;

	int preamp[MAXDBLSTSIZ];
	int attenuator[MAXDBLSTSIZ];

	shortfreq_t max_rit;
	shortfreq_t max_xit;
	shortfreq_t max_ifshift;

	ann_t announces;
	vfo_op_t vfo_ops;
	scan_t scan_ops;
	int targetable_vfo;
	int transceive;

	int bank_qty;
	int chan_desc_sz;

	chan_t chan_list[CHANLSTSIZ];

	freq_range_t rx_range_list1[FRQRANGESIZ];	/* ITU region 1 */
	freq_range_t tx_range_list1[FRQRANGESIZ];
	freq_range_t rx_range_list2[FRQRANGESIZ];	/* ITU region 2 */
	freq_range_t tx_range_list2[FRQRANGESIZ];
	struct tuning_step_list tuning_steps[TSLSTSIZ];

	struct filter_list filters[FLTLSTSIZ];	/* mode/filter table, at -6dB */

	const struct confparams *cfgparams;
	const rig_ptr_t priv;

// Temporary local experimental modification.  Not for general use.
	/* PM has pointer for menus.  Will grow as development progresses. */
	const rig_ptr_t pm;	// ts2k PM.  eventually we'll have to do this.
	vfo_t vfo_all;		/* Describe which vfo_t bits apply --Dale */


	/*
	 * Rig Admin API
	 *
	 */

	int (*rig_init) (RIG * rig);
	int (*rig_cleanup) (RIG * rig);
	int (*rig_open) (RIG * rig);
	int (*rig_close) (RIG * rig);

	/*
	 *  General API commands, from most primitive to least.. :()
	 *  List Set/Get functions pairs
	 */

	int (*set_freq) (RIG * rig, vfo_t vfo, freq_t freq);
	int (*get_freq) (RIG * rig, vfo_t vfo, freq_t * freq);

	int (*set_mode) (RIG * rig, vfo_t vfo, rmode_t mode,
			 pbwidth_t width);
	int (*get_mode) (RIG * rig, vfo_t vfo, rmode_t * mode,
			 pbwidth_t * width);

	int (*set_vfo) (RIG * rig, vfo_t vfo);
	int (*get_vfo) (RIG * rig, vfo_t * vfo);

	int (*set_ptt) (RIG * rig, vfo_t vfo, ptt_t ptt);
	int (*get_ptt) (RIG * rig, vfo_t vfo, ptt_t * ptt);
	int (*get_dcd) (RIG * rig, vfo_t vfo, dcd_t * dcd);

	int (*set_rptr_shift) (RIG * rig, vfo_t vfo,
			       rptr_shift_t rptr_shift);
	int (*get_rptr_shift) (RIG * rig, vfo_t vfo,
			       rptr_shift_t * rptr_shift);

	int (*set_rptr_offs) (RIG * rig, vfo_t vfo, shortfreq_t offs);
	int (*get_rptr_offs) (RIG * rig, vfo_t vfo, shortfreq_t * offs);

	int (*set_split_freq) (RIG * rig, vfo_t vfo, freq_t tx_freq);
	int (*get_split_freq) (RIG * rig, vfo_t vfo, freq_t * tx_freq);
	int (*set_split_mode) (RIG * rig, vfo_t vfo, rmode_t tx_mode,
			       pbwidth_t tx_width);
	int (*get_split_mode) (RIG * rig, vfo_t vfo, rmode_t * tx_mode,
			       pbwidth_t * tx_width);
	int (*set_split) (RIG * rig, vfo_t vfo, split_t split);
	int (*get_split) (RIG * rig, vfo_t vfo, split_t * split);

	int (*set_rit) (RIG * rig, vfo_t vfo, shortfreq_t rit);
	int (*get_rit) (RIG * rig, vfo_t vfo, shortfreq_t * rit);
	int (*set_xit) (RIG * rig, vfo_t vfo, shortfreq_t xit);
	int (*get_xit) (RIG * rig, vfo_t vfo, shortfreq_t * xit);

	int (*set_ts) (RIG * rig, vfo_t vfo, shortfreq_t ts);
	int (*get_ts) (RIG * rig, vfo_t vfo, shortfreq_t * ts);

// renamed --Dale
	int (*set_dcs_code) (RIG * rig, vfo_t vfo, tone_t tone);
	int (*get_dcs_code) (RIG * rig, vfo_t vfo, tone_t * tone);
	int (*set_ctcss_tone) (RIG * rig, vfo_t vfo, tone_t tone);
	int (*get_ctcss_tone) (RIG * rig, vfo_t vfo, tone_t * tone);

	int (*set_dcs) (RIG * rig, vfo_t vfo, tone_t tone);
	int (*get_dcs) (RIG * rig, vfo_t vfo, tone_t * tone);
	int (*set_tone) (RIG * rig, vfo_t vfo, tone_t tone);
	int (*get_tone) (RIG * rig, vfo_t vfo, tone_t * tone);
	int (*set_ctcss) (RIG * rig, vfo_t vfo, tone_t tone);
	int (*get_ctcss) (RIG * rig, vfo_t vfo, tone_t * tone);

	int (*set_dcs_sql) (RIG * rig, vfo_t vfo, tone_t tone);
	int (*get_dcs_sql) (RIG * rig, vfo_t vfo, tone_t * tone);
	int (*set_tone_sql) (RIG * rig, vfo_t vfo, tone_t tone);
	int (*get_tone_sql) (RIG * rig, vfo_t vfo, tone_t * tone);
	int (*set_ctcss_sql) (RIG * rig, vfo_t vfo, tone_t tone);
	int (*get_ctcss_sql) (RIG * rig, vfo_t vfo, tone_t * tone);

	/*
	 * It'd be nice to have a power2mW and mW2power functions
	 * that could tell at what power (watts) the rig is running.
	 * Unfortunately, on most rigs, the formula is not the same
	 * on all bands/modes. Have to work this out.. --SF
	 */
	int (*power2mW) (RIG * rig, unsigned int *mwpower, float power,
			 freq_t freq, rmode_t mode);
	int (*mW2power) (RIG * rig, float *power, unsigned int mwpower,
			 freq_t freq, rmode_t mode);

	int (*set_powerstat) (RIG * rig, powerstat_t status);
	int (*get_powerstat) (RIG * rig, powerstat_t * status);
	int (*reset) (RIG * rig, reset_t reset);

	int (*set_ant) (RIG * rig, vfo_t vfo, ant_t ant);
	int (*get_ant) (RIG * rig, vfo_t vfo, ant_t * ant);

	int (*set_level) (RIG * rig, vfo_t vfo, setting_t level,
			  value_t val);
	int (*get_level) (RIG * rig, vfo_t vfo, setting_t level,
			  value_t * val);

	int (*set_func) (RIG * rig, vfo_t vfo, setting_t func, int status);
	int (*get_func) (RIG * rig, vfo_t vfo, setting_t func,
			 int *status);

	int (*set_parm) (RIG * rig, setting_t parm, value_t val);
	int (*get_parm) (RIG * rig, setting_t parm, value_t * val);

	int (*set_ext_level) (RIG * rig, vfo_t vfo, token_t token, value_t val);
	int (*get_ext_level) (RIG * rig, vfo_t vfo, token_t token, value_t * val);
	int (*set_ext_parm) (RIG * rig, token_t token, value_t val);
	int (*get_ext_parm) (RIG * rig, token_t token, value_t * val);

	int (*set_conf) (RIG * rig, token_t token, const char *val);
	int (*get_conf) (RIG * rig, token_t token, char *val);

	int (*send_dtmf) (RIG * rig, vfo_t vfo, const char *digits);
	int (*recv_dtmf) (RIG * rig, vfo_t vfo, char *digits, int *length);
	int (*send_morse) (RIG * rig, vfo_t vfo, const char *msg);

	int (*set_bank) (RIG * rig, vfo_t vfo, int bank);
	int (*set_mem) (RIG * rig, vfo_t vfo, int ch);
	int (*get_mem) (RIG * rig, vfo_t vfo, int *ch);
	int (*vfo_op) (RIG * rig, vfo_t vfo, vfo_op_t op);
	int (*scan) (RIG * rig, vfo_t vfo, scan_t scan, int ch);

	int (*set_trn) (RIG * rig, int trn);
	int (*get_trn) (RIG * rig, int *trn);


	int (*decode_event) (RIG * rig);

/*
 * Convenience Functions 
 */

	int (*set_channel) (RIG * rig, channel_t * chan);
	int (*get_channel) (RIG * rig, channel_t * chan);

	/* get firmware info, etc. */
	const char *(*get_info) (RIG * rig);

	/* more to come... */
};

# undef _RIG_NEWVFO_PART_4
#endif

#ifdef _RIG_NEWVFO_PART_5

/* --------------- API function prototypes -----------------*/

extern HAMLIB_EXPORT(int)
rig_set_tone HAMLIB_PARAMS((RIG * rig, vfo_t vfo, tone_t tone));
extern HAMLIB_EXPORT(int)
rig_get_tone HAMLIB_PARAMS((RIG * rig, vfo_t vfo, tone_t * tone));

extern HAMLIB_EXPORT(int)
rig_set_tone_sql HAMLIB_PARAMS((RIG * rig, vfo_t vfo, tone_t tone));
extern HAMLIB_EXPORT(int)
rig_get_tone_sql HAMLIB_PARAMS((RIG * rig, vfo_t vfo, tone_t * tone));

# undef _RIG_NEWVFO_PART_5
#endif

// end rig_newvfo.h
