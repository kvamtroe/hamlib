/*
 * lex_keywords.h	Copyright 2003 by Dale E. Edmons.  All Rights Reserved.
 *
 *	license: GNU Public License
 */

/*
 * Note: This was removed from scanner.l.  For reasons unknown to
 *	this author, the #define's wouldn't create proper most of
 *	the time.  It worked immediately after removing it!
 */

#include "rigcmd.h"

// debugging
#define _YY_BASE_TEST

#ifndef _YY_BASE_TEST
	// typical rig_get_func(), rig_set_func()
#	define ADDFUNC(f)		{ # f , rig_get_ ## f , rig_set_ ## f }
	/*
	 * renamed rig_get_func(), rig_set_func()
	 *	this is used if hamlib name conflicts with grammar
	 */
#	define ALIASFUNC(__N, __F)	{ #__N , rig_get_ ## __F , rig_set_ ## __F }
	// non-typical rig???()
#	define ADDSINGLEFUNC(__F)	{ #__F , rig_ ## __F , rig_ ## __F }
	// parser-uniq func()
#	define UNIQFUNC(__F)	{ # __F , rig_get_uniq_ ## __F , rig_set_uniq_ ## __F }
	// Everything else must be added manually
#else
#	define ADDFUNC(_F_)		{ #_F_ , NULL, NULL}
#	define ALIASFUNC(_A_, _F_)		{ #_A_ , NULL, NULL}
#	define ADDSINGLEFUNC(_F_)		{ #_F_ , NULL, NULL}
#	define UNIQFUNC(_F_)		{ #_F_ , NULL, NULL}
	int basetest(int a)
	{
		return RIG_OK;
	}
#endif

lexfunc_t lex_func[] = {
	ADDFUNC(ant),
	ADDFUNC(bank),
	ADDFUNC(channel),
	ADDFUNC(conf),
	ADDFUNC(ctcss_sql),
	ADDFUNC(ctcss_tone),
	ADDFUNC(ext_level),
	ADDFUNC(ext_parm),
	ADDFUNC(dcs_code),
	ADDFUNC(dcs_sql),
	ADDFUNC(freq),
	ADDFUNC(level),
	ADDFUNC(mode),
	ADDFUNC(parm),
	ADDFUNC(powerstat),
	ADDFUNC(ptt),
	ADDFUNC(rit),
	ADDFUNC(rptr_offs),
	ADDFUNC(rptr_shift),
	ADDFUNC(split),
	ADDFUNC(split_freq),
	ADDFUNC(split_mode),
	ADDFUNC(tone),
	ADDFUNC(trn),
	ADDFUNC(ts),
	ADDFUNC(vfo),
	ADDFUNC(xit),
	ADDSINGLEFUNC(open),
	ADDSINGLEFUNC(cleanup),
	ADDSINGLEFUNC(debug),
	ADDSINGLEFUNC(passband_normal),
	ADDSINGLEFUNC(passband_narrow),
	ADDSINGLEFUNC(passband_wide),
	ADDSINGLEFUNC(power2mW),
	ADDSINGLEFUNC(mW2power),
	ADDSINGLEFUNC(reset),
	ADDSINGLEFUNC(scan),
	ADDSINGLEFUNC(has_scan),
	ADDSINGLEFUNC(get_range),
	ADDSINGLEFUNC(setting2idx),
	ALIASFUNC(mem_idx, mem),
	ALIASFUNC(parmfunc, func),
	ALIASFUNC(chan, channel),
	ADDSINGLEFUNC(generic_retr_extl),
	UNIQFUNC(repeater),
	ADDFUNC(vfo_callback),
	ADDFUNC(vfo_op),
	ADDFUNC(dcd),
	ADDFUNC(dcd_callback),
	ADDFUNC(freq_callback),
	ADDFUNC(mode_callback),
	ADDFUNC(ptt_callback),
	ADDFUNC(resolution),
#ifndef _YY_BASE_TEST
	{"dtmf", rig_recv_dtmf, rig_send_dtmf},
	{"morse", (void *)0, rig_send_morse},
	{"channel", generic_save_channel, generic_restore_channel},
#else
	{"basetest", basetest, basetest},
#endif
	{ NULL, NULL, NULL}
};

// end debugging
#undef _YY_BASE_TEST

lexvfo_t lex_vfos[] = {
//	{ "A" , RIG_VFO_A}, { "B" , RIG_VFO_B}, { "C" , RIG_VFO_C},
	{ "VFOA", RIG_VFO_A}, { "vfoa", RIG_VFO_A}, { "VFO_A", RIG_VFO_A},
	{ "VFOB", RIG_VFO_B}, { "vfob", RIG_VFO_B}, { "VFO_B", RIG_VFO_B},
	{ "VFOC", RIG_VFO_C}, { "vfoc", RIG_VFO_C}, { "VFO_C", RIG_VFO_C},
	{ "VFOAB", RIG_VFO_AB}, { "vfoab", RIG_VFO_AB}, { "VFO_AB", RIG_VFO_AB},
	{ "VFOBA", RIG_VFO_BA}, { "vfoba", RIG_VFO_BA}, { "VFO_BA", RIG_VFO_BA},
	{ "MEMA", RIG_MEM_A}, { "mema", RIG_MEM_A}, { "MEM_A", RIG_MEM_A},
	{ "MEMC", RIG_MEM_C}, { "memc", RIG_MEM_C}, { "MEM_C", RIG_MEM_C},
	{ "CALLA", RIG_CALL_A}, { "calla", RIG_CALL_A}, { "CALL_A", RIG_CALL_A},
	{ "CALLC", RIG_CALL_C}, { "callc", RIG_CALL_C}, { "CALL_C", RIG_CALL_C},

	{ "VFO1", RIG_VFO1},
	{ "VFO2", RIG_VFO2},
	{ "VFO3", RIG_VFO3},
	{ "VFO4", RIG_VFO4},
	{ "VFO5", RIG_VFO5},

	{ "PTT", RIG_VFO_PTT},
	{ "CTRL", RIG_VFO_CTRL},

	// The following all are uppercase.  Lowercase is a TOKEN!
	{ "FAKE", RIG_CTRL_FAKE},
	{ "MAIN", RIG_CTRL_MAIN}, { "Main", RIG_CTRL_MAIN},
	{ "SUB", RIG_CTRL_SUB}, { "Sub", RIG_CTRL_SUB},
	{ "MEM", RIG_CTRL_MEM},
	{ "CALL", RIG_CTRL_CALL},
	{ "SPLIT", RIG_CTRL_SPLIT},
	{ "REV", RIG_CTRL_REV},
	{ "RIT", RIG_CTRL_RIT},
	{ "SAT", RIG_CTRL_SAT},
	{ "SCAN", RIG_CTRL_SCAN},
	{ "XIT", RIG_CTRL_XIT},
	{ "CROSS", RIG_CTRL_CROSS},

	{ NULL, 0}
};
