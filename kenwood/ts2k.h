/*
 *  Hamlib TS2000 backend - main header
 *  Copyright (c) 2000-2002 by Stephane Fillod
 *
 *		$Id: ts2k.h,v 1.3.2.2 2002-07-10 20:28:01 dedmons Exp $
 *
 *   This library is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2 of
 *   the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Library General Public License for more details.
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this library; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

/*
 * created from kenwood.h	--Dale kd7eni
 */

#include <hamlib/rig.h>

#ifndef _TS2K_H
#define _TS2K_H

#define	_USEVFO

// imported from ts2000.h --Dale kd7eni
/*
 * 103 available DCS codes
 */
static const tone_t ts2k_dcs_list[] = {
	 23,  25,  26,  31,  32,  36,  43,  47,  51,  53,  54,
	 65,  71,  72,  73,  74, 114, 115, 116, 122, 125, 131,
	132, 134, 143, 145, 152, 155, 156, 162, 165, 172, 174,
	205, 212, 223, 225, 226, 243, 244, 245, 246, 251, 252,
	255, 261, 263, 265, 266, 271, 274, 306, 311, 315, 325,
	331, 332, 343, 346, 351, 356, 364, 365, 371, 411, 412,
	413, 423, 431, 432, 445, 446, 452, 454, 455, 462, 464,
	465, 466, 503, 506, 516, 523, 526, 532, 546, 565, 606,
	612, 624, 627, 631, 632, 654, 662, 664, 703, 712, 723,
	731, 732, 734, 743, 754,
	0
};

#define TS2K_PTT_ON_MAIN	8
#define TS2K_CTRL_ON_MAIN	4
#define TS2K_PTT_ON_SUB		2
#define TS2K_CTRL_ON_SUB	1

// Needed to prevent ts2k_transaction (kenwood_transaction) from
// expecting a reply from the ts2000 in cases where there is none.
#define NOREPLY	0
// some commands reply "?;" if rig currently in requested mode
// usually, there is NOREPLY when mode is changed.  This can be
// put in the acknowledge length to signify the error is no error.
#define BUGERR	-1

//-kd7eni

#define EOM_KEN ";"
#define EOM_TH "\r"

struct ts2k_priv_caps {
    /* read-only values */
    const char *cmdtrm;    /* Command termination chars (ken=';' or th='\r') */
    /* changable values */
        // nothing
};

#if 0  /* No private data for Kenwood backends. */
struct ts2k_priv_data {
    int dummy;  // placeholder for real entries.
};
#endif

extern int ts2k_init(RIG *rig);
extern int ts2k_cleanup(RIG *rig);


extern const int ts2k_ctcss_list[];

int ts2k_transaction(RIG *rig, const char *cmd, int cmd_len, char *data,
				size_t *data_len);
int ts2k_set_vfo(RIG *rig, vfo_t vfo);
int ts2k_get_vfo(RIG *rig, vfo_t *vfo);
int ts2k_set_freq(RIG *rig, vfo_t vfo, freq_t freq);
int ts2k_get_freq(RIG *rig, vfo_t vfo, freq_t *freq);
int ts2k_set_mode(RIG *rig, vfo_t vfo, rmode_t mode, pbwidth_t width);
int ts2k_get_mode(RIG *rig, vfo_t vfo, rmode_t *mode, pbwidth_t *width);
int ts2k_set_level(RIG *rig, vfo_t vfo, setting_t level, value_t val);
int ts2k_get_level(RIG *rig, vfo_t vfo, setting_t level, value_t *val);
int ts2k_set_func(RIG *rig, vfo_t vfo, setting_t func, int status);
int ts2k_get_func(RIG *rig, vfo_t vfo, setting_t func, int *status);
int ts2k_set_ctcss_tone(RIG *rig, vfo_t vfo, tone_t tone);
int ts2k_set_tone(RIG *rig, vfo_t vfo, tone_t tone);
int ts2k_get_ctcss_tone(RIG *rig, vfo_t vfo, tone_t *tone);
int ts2k_get_tone(RIG *rig, vfo_t vfo, tone_t *tone);
int ts2k_set_Tones(RIG *rig, vfo_t vfo, tone_t tone, const char ct);
int ts2k_get_Tones(RIG *rig, vfo_t vfo, tone_t *tone, const char *ct);
int ts2k_set_powerstat(RIG *rig, powerstat_t status);
int ts2k_get_powerstat(RIG *rig, powerstat_t *status);
int ts2k_reset(RIG *rig, reset_t reset);
int ts2k_send_morse(RIG *rig, vfo_t vfo, const char *msg);
int ts2k_get_ptt(RIG *rig, vfo_t vfo, ptt_t *ptt);
int ts2k_set_ptt(RIG *rig, vfo_t vfo, ptt_t ptt);
int ts2k_get_dcd(RIG *rig, vfo_t vfo, dcd_t *dcd);
int ts2k_vfo_op(RIG *rig, vfo_t vfo, vfo_op_t op);
int ts2k_set_mem(RIG *rig, vfo_t vfo, int ch);
int ts2k_get_mem(RIG *rig, vfo_t vfo, int *ch);
const char* ts2k_get_info(RIG *rig);

int ts2k_get_trn(RIG *rig, int *trn);
int ts2k_set_trn(RIG *rig, int trn);

/*
 * functions I've written -- Dale KD7ENI
 */
int int_n(char *tmp, char *src, int cnt);
int ts2k_scan(RIG *rig, vfo_t vfo, scan_t scan, int ch);
int ts2k_scan_on(RIG *rig, char ch);
int ts2k_scan_off(RIG *rig);
int ts2k_set_basic(RIG *rig, vfo_t vfo);
int ts2k_get_channel(RIG *rig, channel_t *chan); 
int ts2k_set_channel(RIG *rig, channel_t *chan); 
int ts2k_get_ctrl(RIG *rig, char *dc_buf, int dc_len);
int ts2k_set_ctrl(RIG *rig, int ptt, int ctrl);
int ts2k_vfo_ctrl(RIG *rig, vfo_t vfo);
int ts2k_get_dcs_code(RIG *rig, vfo_t vfo, tone_t *tone); 
int ts2k_set_dcs_code(RIG *rig, vfo_t vfo, tone_t  tone); 
int ts2k_get_int(char *c, int i);
int ts2k_sat_off(RIG *rig, vfo_t vfo);
int ts2k_sat_on(RIG *rig, vfo_t vfo);
int ts2k_get_parm(RIG *rig, setting_t parm, value_t *val);
int ts2k_set_parm(RIG *rig, setting_t parm, value_t val);
int ts2k_get_rit(RIG *rig, vfo_t vfo, shortfreq_t *rit);
int ts2k_set_rit(RIG *rig, vfo_t vfo, shortfreq_t  rit);
int ts2k_get_rptr_offs(RIG *rig, vfo_t vfo, shortfreq_t *offs);
int ts2k_set_rptr_offs(RIG *rig, vfo_t vfo, shortfreq_t  offs);
int ts2k_get_rptr_shift(RIG *rig, vfo_t vfo, rptr_shift_t *shift);
int ts2k_set_rptr_shift(RIG *rig, vfo_t vfo, rptr_shift_t  shift);
int ts2k_get_split(RIG *rig, vfo_t vfo, split_t *split);
int ts2k_set_split(RIG *rig, vfo_t vfo, split_t  split);
int ts2k_get_split_freq(RIG *rig, vfo_t vfo, freq_t *tx_freq);
int ts2k_set_split_freq(RIG *rig, vfo_t vfo, freq_t  tx_freq);
int ts2k_get_split_mode(RIG *rig, vfo_t vfo, rmode_t *txmode, pbwidth_t *txwidth);
int ts2k_set_split_mode(RIG *rig, vfo_t vfo, rmode_t  txmode, pbwidth_t  txwidth); 
int ts2k_get_ts(RIG *rig, vfo_t vfo, shortfreq_t *ts);
int ts2k_set_ts(RIG *rig, vfo_t vfo, shortfreq_t ts);
int ts2k_get_xit(RIG *rig, vfo_t vfo, shortfreq_t *freq);
int ts2k_set_xit(RIG *rig, vfo_t vfo, shortfreq_t  freq);
/* Redsigned Functions */
int ts2k_uniq_GetVfo(RIG *rig, vfo_t *vfo);
int ts2k_uniq_GetCtrl(RIG *rig, vfo_t *vfo);
int ts2k_uniq_LockPanel(RIG *rig);
int ts2k_uniq_MemOff(RIG *rig, vfo_t);
int ts2k_uniq_MemOn(RIG *rig, vfo_t);
int ts2k_uniq_SatOff(RIG *rig);
int ts2k_uniq_SatOn(RIG *rig, vfo_t);
int ts2k_uniq_SendScan(RIG *rig, vfo_t, char sc);
int ts2k_uniq_SendVfo(RIG *rig, vfo_t, char v);
int ts2k_uniq_SetCtrl(RIG *rig, vfo_t vfo);
int ts2k_uniq_SetMajor(RIG *rig, vfo_t);
int ts2k_uniq_SetMinor(RIG *rig, vfo_t);
int ts2k_uniq_SetSat(RIG *rig, vfo_t);
int ts2k_uniq_SetScan(RIG *rig, vfo_t);
int ts2k_uniq_SetVfo(RIG *rig, vfo_t);
int ts2k_uniq_ScanOff(RIG *rig, vfo_t);
int ts2k_uniq_ScanOn(RIG *rig, vfo_t);
int ts2k_uniq_SplitVfo(RIG *rig, char rx, char tx);
int ts2k_uniq_SwitchVfo(RIG *rig, vfo_t);
int ts2k_uniq_UnlockPanel(RIG *rig);

/* end kd7eni functions */

extern const struct rig_caps thd7a_caps;
extern const struct rig_caps thf7a_caps;
extern const struct rig_caps thf7e_caps;
extern const struct rig_caps ts450s_caps;
extern const struct rig_caps ts50s_caps;
extern const struct rig_caps ts570d_caps;
extern const struct rig_caps ts570s_caps;
extern const struct rig_caps ts790_caps;
extern const struct rig_caps ts850_caps;
extern const struct rig_caps ts870s_caps;
extern const struct rig_caps ts950sdx_caps;
extern const struct rig_caps ts2000_caps;

extern BACKEND_EXPORT(int) initrigs_ts2k(void *be_handle);
extern BACKEND_EXPORT(rig_model_t) proberigs_ts2k(port_t *port);

/*
 * Some of the following imported from original ts2000.c	--Dale
 */

#define TS2000_ALL_MODES (RIG_MODE_AM|RIG_MODE_CW|RIG_MODE_SSB|RIG_MODE_FM| \
	RIG_MODE_RTTY)
#define TS2000_OTHER_TX_MODES (RIG_MODE_CW|RIG_MODE_SSB|RIG_MODE_FM| \
	RIG_MODE_RTTY)
#define TS2000_AM_TX_MODES (RIG_MODE_AM)

// the following might be cond. later

#define _NEW_VFO_H
#ifndef _NEW_VFO_H

// old / simple
# define TS2000_MAINVFO (RIG_VFO_A | RIG_VFO_B | RIG_VFO_MEM)
# define TS2000_SUBVFO (RIG_VFO_C)
# define TS2000_RIGVFO (0)
# define TS2000_FUNC_ALL (RIG_FUNC_TONE | RIG_FUNC_NB)
# define TS2000_PARM_OP (RIG_PARM_BEEP | RIG_PARM_BACKLIGHT)
# define TS2000_LEVEL_ALL (RIG_LEVEL_PREAMP | RIG_LEVEL_VOX | RIG_LEVEL_AF)
# define TS2000_SCAN_OP (RIG_SCAN_STOP | RIG_SCAN_MEM)

#else

// new
# define TS2000_FUNC_ALL ( RIG_FUNC_ALL & \
			~(RIG_FUNC_MN | RIG_FUNC_RNF | RIG_FUNC_VSC) ) 
# define TS2000_PARM_OP (RIG_PARM_ALL & ~(RIG_PARM_BAT | RIG_PARM_TIME))
# define TS2000_LEVEL_ALL (RIG_LEVEL_ALL & ~(RIG_LEVEL_APF))
# define TS2000_SCAN_OP (RIG_SCAN_ALL & ~(RIG_SCAN_DELTA))

// the following uses both Sub and Main for the Major mode
# define TS2000_MAINVFO (RIG_VFO_A | RIG_VFO_B | RIG_VFO_MEM_A \
			| RIG_VFO_CALL_A | RIG_VFO_AB | RIG_VFO_BA)
# define TS2000_SUBVFO (RIG_VFO_C | RIG_VFO_MEM_C | RIG_VFO_CALL_C)
# define TS2000_RIGVFO	RIG_CTRL_MASK 

#endif

#define TS2000_VFO_ALL (TS2000_RIGVFO | TS2000_MAINVFO | TS2000_SUBVFO)
// FIXME: Shouldn't this be part of rig_caps?!

#define TS2000_VFO_OP (RIG_OP_UP | RIG_OP_DOWN)

/*
 * 103 available DCS codes
 */
static const tone_t ts2000_dcs_list[] = {
       23,  25,  26,  31,   32,  36,  43,  47,       51,  53,
  54,  65,  71,  72,  73,   74, 114, 115, 116, 122, 125, 131,
  132, 134, 143, 145, 152, 155, 156, 162, 165, 172, 174, 205,
  212, 223, 225, 226, 243, 244, 245, 246, 251, 252, 255, 261,
  263, 265, 266, 271, 274, 306, 311, 315, 325, 331, 332, 343,
  346, 351, 356, 364, 365, 371, 411, 412, 413, 423, 431, 432,
  445, 446, 452, 454, 455, 462, 464, 465, 466, 503, 506, 516,
  523, 526, 532, 546, 565, 606, 612, 624, 627, 631, 632, 654,
  662, 664, 703, 712, 723, 731, 732, 734, 743, 754,
  0,
};

/*
 * modes in use by the "MD" command
 */
#define MD_NONE	'0'
#define MD_LSB	'1'
#define MD_USB	'2'
#define MD_CW	'3'
#define MD_FM	'4'
#define MD_AM	'5'
#define MD_FSK	'6'
#define MD_CWR	'7'
#define MD_FSKR	'9'


// Added the following two lists --Dale, kd7eni
// FIXME: RIG_MODE_[FSKR|CWR] undefined in rig.h
static const int ts2k_mode_list[] = {
	RIG_MODE_NONE, RIG_MODE_LSB, RIG_MODE_USB, RIG_MODE_CW,
	RIG_MODE_FM, RIG_MODE_AM, RIG_MODE_RTTY, RIG_MODE_CW,
	RIG_MODE_RTTY
};

static long int ts2k_steps[2][10] = {
	{1000, 2500, 5000, 10000, 0, 0, 0, 0, 0, 0},	// ssb, cw, fsk
	{5000, 6250, 10000, 12500, 15000, 20000,
	 25000, 30000, 50000, 100000}	// am/fm
};


struct ts2k_id {
	rig_model_t model;
	int id;
};

struct ts2k_id_string {
	rig_model_t model;
	const char *id;
};


#define UNKNOWN_ID -1

/*
 * Identification number as returned by "ID;"
 * Please, if the model number of your rig is listed as UNKNOWN_ID,
 * send the value to <fillods@users.sourceforge.net> for inclusion. Thanks --SF
 *
 * TODO: sort this list with most frequent rigs first.
 */
static const struct ts2k_id ts2k_id_list[] = {
	{RIG_MODEL_R5000, 5},
	{RIG_MODEL_TS870S, 15},
	{RIG_MODEL_TS570D, 17},
	{RIG_MODEL_TS570S, 18},
	{RIG_MODEL_TS2000, 19},	/* correct --kd7eni */
	{RIG_MODEL_NONE, UNKNOWN_ID},	/* end marker */
};

static const struct ts2k_id_string ts2k_id_string_list[] = {
	{RIG_MODEL_THD7A, "TH-D7"},
	{RIG_MODEL_THD7AG, "TH-D7G"},
	{RIG_MODEL_THF6A, "TH-F6"},
	{RIG_MODEL_THF7E, "TH-F7"},
	{RIG_MODEL_NONE, NULL},	/* end marker */
};


/*
 * 38 CTCSS sub-audible tones  (17500 invalid for ctcss --kd7eni)
  */
static const int ts2k_ctcss_list[] = {
	670, 719, 744, 770, 797, 825, 854, 885, 915, 948,
	974, 1000, 1035, 1072, 1109, 1148, 1188, 1230, 1273, 1318,
	1365, 1413, 1462, 1514, 1567, 1622, 1679, 1738, 1799, 1862,
	1928, 2035, 2107, 2181, 2257, 2336, 2418, 2503,	// 17500,
	/* Note: 17500 is not available as ctcss, only tone. --kd7eni */
	0,
};

#define cmd_trm(rig) ((struct ts2k_priv_caps *)(rig)->caps->priv)->cmdtrm
#define ta_quit	rs->hold_decode = 0; return retval

#endif /* _TS2000_H */
