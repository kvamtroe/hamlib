/*
 *  Hamlib Kenwood backend - TS2K description
 *  Copyright (c) 2000-2002 by Stephane Fillod
 *  Copyright (c) 2002-2003 by Dale E. Edmons
 *
 *		$Id: ts2k.c,v 1.1.2.2 2003-02-25 06:00:59 dedmons Exp $
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <hamlib/rig.h>
#include "ts2k/ts2k_menu.h"
#include "ts2k/ts2k.h"
#include "ts2k/ts2k_cmds.h"

ts2k_pm_t ts2k_pm[TS2K_PMSIZ];

ts2k_menu_t ts2k_menu[2*TS2K_PMSIZ];	// One set for each PM

const struct ts2k_priv_caps  ts2k_priv_caps  = {
	cmdtrm: EOM_KEN,
};

/*
 * ts2k rig capabilities.
 *
 * TODO: antenna caps
 *
 * part of infos comes from http://www.kenwood.net/
 */
const struct rig_caps ts2k_caps = {
	rig_model: RIG_MODEL_TS2K,
	model_name:"TS-2K",
	mfg_name: "Kenwood",
	version: "0.1.2",
	copyright: "LGPL",
	status: RIG_STATUS_ALPHA,
	rig_type: RIG_TYPE_TRANSCEIVER,
	ptt_type: RIG_PTT_RIG,
	dcd_type: RIG_DCD_RIG,
	port_type: RIG_PORT_SERIAL,
	serial_rate_min: 1200,
	serial_rate_max: 57600,
	serial_data_bits: 8,
	serial_stop_bits: 1,
	serial_parity: RIG_PARITY_NONE,
	serial_handshake: RIG_HANDSHAKE_NONE,
	write_delay: 0,
// set 1,20,4 and cured timeouts for "if...;" on ts2k_get_freq()	--Dale
	post_write_delay: 1,
	timeout: 30,
	retry: 4,
	has_get_func: TS2K_FUNC_ALL,
	has_set_func: TS2K_FUNC_ALL,
	has_get_level: TS2K_LEVEL_ALL,
	has_set_level: RIG_LEVEL_SET(TS2K_LEVEL_ALL),
	has_get_parm: TS2K_PARM_OP,
	has_set_parm: TS2K_PARM_OP,
	level_gran: {},                 /* FIXME: granularity */
	parm_gran: {},
	vfo_ops: TS2K_VFO_OP,
	scan_ops: TS2K_SCAN_OP,
	vfo_all: RIG_VFO_VALID,
	ctcss_list: ts2k_ctcss_list,
	dcs_list: ts2k_dcs_list,
	preamp:  { 20, RIG_DBLST_END, },	/* FIXME: real preamp? */
	attenuator:  { 20, RIG_DBLST_END, },
	max_rit: kHz(20),
	max_xit: kHz(20),
	max_ifshift: kHz(1),
	targetable_vfo: RIG_TARGETABLE_FREQ,
	transceive: RIG_TRN_RIG,
	bank_qty:  0,
	chan_desc_sz: 8,
	chan_list: {
		{ 0, 289, RIG_MTYPE_MEM, 0 },		/* regular memories */
		{ 290, 299, RIG_MTYPE_EDGE, 0 },	/* band tune limits (not scan-only) */
		{ 0, 9, RIG_MTYPE_MEMOPAD, 0 },		/* Quick Memories, Main+sub both saved:) */
		{ 0, 1, RIG_MTYPE_CALL, 0 },		/* each TX band has one call */
		{ 0, 9, RIG_MTYPE_SAT, 0 },		/* direct operation from these */
		RIG_CHAN_END,
	},
	rx_range_list1: {
		{kHz(300),MHz(60),TS2K_ALL_MODES,-1,-1,TS2K_MAINVFO},
		{MHz(144),MHz(146),TS2K_ALL_MODES,-1,-1,TS2K_MAINVFO},
		{MHz(430),MHz(440),TS2K_ALL_MODES,-1,-1,TS2K_MAINVFO},
		{MHz(144),MHz(146),TS2K_ALL_MODES,-1,-1,TS2K_SUBVFO},
		{MHz(430),MHz(440),TS2K_ALL_MODES,-1,-1,TS2K_SUBVFO},
		RIG_FRNG_END,
	}, /* rx range */
	tx_range_list1: {
		{kHz(1830),kHz(1850),TS2K_OTHER_TX_MODES,W(5),W(100),TS2K_MAINVFO},
		{kHz(1830),kHz(1850),TS2K_AM_TX_MODES,2000,25000,TS2K_MAINVFO},
		{kHz(3500),kHz(3800),TS2K_OTHER_TX_MODES,W(5),W(100),TS2K_MAINVFO},
		{kHz(3500),kHz(3800),TS2K_AM_TX_MODES,W(5),W(25),TS2K_MAINVFO},
		{MHz(7),kHz(7100),TS2K_OTHER_TX_MODES,W(5),W(100),TS2K_MAINVFO},
		{MHz(7),kHz(7100),TS2K_AM_TX_MODES,W(5),W(25),TS2K_MAINVFO},
		{MHz(10.1),MHz(10.15),TS2K_OTHER_TX_MODES,W(5),W(100),TS2K_MAINVFO},
		{MHz(10.1),MHz(10.15),TS2K_AM_TX_MODES,W(5),W(25),TS2K_MAINVFO},
		{MHz(14),kHz(14350),TS2K_OTHER_TX_MODES,W(5),W(100),TS2K_MAINVFO},
		{MHz(14),kHz(14350),TS2K_AM_TX_MODES,W(5),W(25),TS2K_MAINVFO},
		{kHz(18068),kHz(18168),TS2K_OTHER_TX_MODES,W(5),W(100),TS2K_MAINVFO},
		{kHz(18068),kHz(18168),TS2K_AM_TX_MODES,W(5),W(25),TS2K_MAINVFO},
		{MHz(21),kHz(21450),TS2K_OTHER_TX_MODES,W(5),W(100),TS2K_MAINVFO},
		{MHz(21),kHz(21450),TS2K_AM_TX_MODES,W(5),W(25),TS2K_MAINVFO},
		{kHz(24890),kHz(24990),TS2K_OTHER_TX_MODES,W(5),W(100),TS2K_MAINVFO},
		{kHz(24890),kHz(24990),TS2K_AM_TX_MODES,W(5),W(25),TS2K_MAINVFO},
		{MHz(28),kHz(29700),TS2K_OTHER_TX_MODES,W(5),W(100),TS2K_MAINVFO},
		{MHz(28),kHz(29700),TS2K_AM_TX_MODES,W(5),W(25),TS2K_MAINVFO},
		{MHz(50),MHz(50.2),TS2K_OTHER_TX_MODES,W(5),W(100),TS2K_MAINVFO},
		{MHz(50),MHz(50.2),TS2K_AM_TX_MODES,W(5),W(25),TS2K_MAINVFO},
		{MHz(144),MHz(146),TS2K_OTHER_TX_MODES,W(5),W(100),TS2K_MAINVFO},
		{MHz(144),MHz(146),TS2K_AM_TX_MODES,W(5),W(25),TS2K_MAINVFO},
		{MHz(430),MHz(440),TS2K_OTHER_TX_MODES,W(5),W(50),TS2K_MAINVFO},
		{MHz(430),MHz(440),TS2K_AM_TX_MODES,W(5),W(12.5),TS2K_MAINVFO},
		RIG_FRNG_END,
	}, /* tx range */
	rx_range_list2: {
		{kHz(300),MHz(60),TS2K_ALL_MODES,-1,-1,TS2K_MAINVFO},
		{MHz(142),MHz(152),TS2K_ALL_MODES,-1,-1,TS2K_MAINVFO},
		{MHz(420),MHz(450),TS2K_ALL_MODES,-1,-1,TS2K_MAINVFO},
		{MHz(118),MHz(174),TS2K_ALL_MODES,-1,-1,TS2K_SUBVFO},
		{MHz(220),MHz(512),TS2K_ALL_MODES,-1,-1,TS2K_SUBVFO},
		RIG_FRNG_END,
	}, /* rx range */
	tx_range_list2: {
		{kHz(1800),MHz(2),TS2K_OTHER_TX_MODES,W(5),W(100),TS2K_MAINVFO},
		{kHz(1800),MHz(2),TS2K_AM_TX_MODES,2000,25000,TS2K_MAINVFO},
		{kHz(3500),MHz(4),TS2K_OTHER_TX_MODES,W(5),W(100),TS2K_MAINVFO},
		{kHz(3500),MHz(4),TS2K_AM_TX_MODES,W(5),W(25),TS2K_MAINVFO},
		{MHz(7),kHz(7300),TS2K_OTHER_TX_MODES,W(5),W(100),TS2K_MAINVFO},
		{MHz(7),kHz(7300),TS2K_AM_TX_MODES,W(5),W(25),TS2K_MAINVFO},
		{MHz(10.1),MHz(10.15),TS2K_OTHER_TX_MODES,W(5),W(100),TS2K_MAINVFO},
		{MHz(10.1),MHz(10.15),TS2K_AM_TX_MODES,W(5),W(25),TS2K_MAINVFO},
		{MHz(14),kHz(14350),TS2K_OTHER_TX_MODES,W(5),W(100),TS2K_MAINVFO},
		{MHz(14),kHz(14350),TS2K_AM_TX_MODES,W(5),W(25),TS2K_MAINVFO},
		{kHz(18068),kHz(18168),TS2K_OTHER_TX_MODES,W(5),W(100),TS2K_MAINVFO},
		{kHz(18068),kHz(18168),TS2K_AM_TX_MODES,W(5),W(25),TS2K_MAINVFO},
		{MHz(21),kHz(21450),TS2K_OTHER_TX_MODES,W(5),W(100),TS2K_MAINVFO},
		{MHz(21),kHz(21450),TS2K_AM_TX_MODES,W(5),W(25),TS2K_MAINVFO},
		{kHz(24890),kHz(24990),TS2K_OTHER_TX_MODES,W(5),W(100),TS2K_MAINVFO},
		{kHz(24890),kHz(24990),TS2K_AM_TX_MODES,W(5),W(25),TS2K_MAINVFO},
		{MHz(28),kHz(29700),TS2K_OTHER_TX_MODES,W(5),W(100),TS2K_MAINVFO},
		{MHz(28),kHz(29700),TS2K_AM_TX_MODES,W(5),W(25),TS2K_MAINVFO},
		{MHz(50),MHz(54),TS2K_OTHER_TX_MODES,W(5),W(100),TS2K_MAINVFO},
		{MHz(50),MHz(54),TS2K_AM_TX_MODES,W(5),W(25),TS2K_MAINVFO},
		{MHz(144),MHz(148),TS2K_OTHER_TX_MODES,W(5),W(100),TS2K_MAINVFO},
		{MHz(144),MHz(148),TS2K_AM_TX_MODES,W(5),W(25),TS2K_MAINVFO},
		{MHz(430),MHz(450),TS2K_OTHER_TX_MODES,W(5),W(50),TS2K_MAINVFO},
		{MHz(430),MHz(450),TS2K_AM_TX_MODES,W(5),W(12.5),TS2K_MAINVFO},
		RIG_FRNG_END,
	}, /* tx range */
	tuning_steps: {
		{TS2K_ALL_MODES,50},
		{TS2K_ALL_MODES,100},
		{TS2K_ALL_MODES,kHz(1)},
		{TS2K_ALL_MODES,kHz(5)},
		{TS2K_ALL_MODES,kHz(9)},
		{TS2K_ALL_MODES,kHz(10)},
		{TS2K_ALL_MODES,12500},
		{TS2K_ALL_MODES,kHz(20)},
		{TS2K_ALL_MODES,kHz(25)},
		{TS2K_ALL_MODES,kHz(100)},
		{TS2K_ALL_MODES,MHz(1)},
		{TS2K_ALL_MODES,0},	/* any tuning step */
		RIG_TS_END,
	},
	/* mode/filter list, remember: order matters! */
	filters: {
		{RIG_MODE_SSB, kHz(2.2)},
		{RIG_MODE_CW, Hz(600)},
		{RIG_MODE_RTTY, Hz(1500)},
		{RIG_MODE_AM, kHz(6)},
		{RIG_MODE_FM|RIG_MODE_AM, kHz(12)},
		RIG_FLT_END,
	},
	priv: (void *)&ts2k_priv_caps,
/* ts2k */
// Temporary!  We might be able to use priv.  Until I sort that out, I just
// added a new stucture.  Locally nothing should be affected.

//	pm: (ts2k_pm_t *) &ts2k_pm,
	pm: NULL,
//	rig_init: ts2k_init,
	rig_open: ts2k_open,
	rig_close: ts2k_pm_close,
	set_tone: ts2k_set_tone,
	get_tone: ts2k_get_tone,
	set_ctcss: ts2k_set_ctcss,
	get_ctcss: ts2k_get_ctcss,
	get_dcd: ts2k_get_dcd,
	set_freq: ts2k_set_freq,
	get_freq: ts2k_get_freq,
	get_func: ts2k_get_func,
	set_func: ts2k_set_func,
	get_info: ts2k_get_info,
	get_level: ts2k_get_level,
	set_level: ts2k_set_level,
	get_mem: ts2k_get_mem,
	set_mem: ts2k_set_mem,
	get_mode: ts2k_get_mode,
	set_mode: ts2k_set_mode,
	get_powerstat: ts2k_get_powerstat,
	set_powerstat: ts2k_set_powerstat,
	get_ptt: ts2k_get_ptt,
	set_ptt: ts2k_set_ptt,
	reset: ts2k_reset,
	send_morse: ts2k_send_morse,
	get_trn: ts2k_get_trn,
	set_trn: ts2k_set_trn,
/*
*/
	set_vfo: ts2k_set_vfo,
	get_vfo: ts2k_get_vfo,
	vfo_op: ts2k_vfo_op,
/*
 * stuff I've written--kd7eni
 */
	scan:		ts2k_scan,
	get_channel:	ts2k_get_channel,
	set_channel:	ts2k_set_channel,
	get_dcs:	ts2k_get_dcs,
	set_dcs:	ts2k_set_dcs,
	get_parm:	ts2k_get_parm,
	set_parm:	ts2k_set_parm,
	get_rit:	ts2k_get_rit,
	set_rit:	ts2k_set_rit,
	get_rptr_offs:	ts2k_get_rptr_offs,
	set_rptr_offs:	ts2k_set_rptr_offs,
	get_rptr_shift:	ts2k_get_rptr_shift,
	set_rptr_shift:	ts2k_set_rptr_shift,
	get_split:	ts2k_get_split,
	set_split:	ts2k_set_split,
	get_split_freq:	ts2k_get_split_freq,
	set_split_freq:	ts2k_set_split_freq,
	get_split_mode:	ts2k_get_split_mode,
	set_split_mode:	ts2k_set_split_mode,
	get_ts:		ts2k_get_ts,
	set_ts:		ts2k_set_ts,
	get_xit:	ts2k_get_xit,
	set_xit:	ts2k_set_xit,
/* comming soon... */
//	get_tone_sql:	ts2k_get_tone_sql,
//	set_tone_sql:	ts2k_set_tone_sql,
//	decode_event:	ts2k_decode_event,	/* highest */
//	get_conf:	ts2k_get_conf,
//	set_conf:	ts2k_set_conf,
//	get_ant:	ts2k_get_ant,
//	set_ant:	ts2k_set_ant,
//	recv_dtmf:	ts2k_recv_dtmf,		/* possible? */
//	get_ctcss_sql:	ts2k_get_ctcss_sql,
//	set_ctcss_sql:	ts2k_set_ctcss_sql,
//	get_dcs_sql:	ts2k_get_dcs_sql,
//	set_dcs_sql:	ts2k_set_dcs_sql,
//	send_dtmf:	ts2k_send_dtmf,		/* lowest */
/* and never... */
//	set_bank:				/* not needed */
/*
 * end ts2k
 */
};

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <ctype.h>
#include <hamlib/rig.h>
#include "ts2k.h"

#include <stdlib.h>
#include <stdio.h>		/* Standard input/output definitions */
#include <string.h>		/* String function definitions */
#include <unistd.h>		/* UNIX standard function definitions */
#include <fcntl.h>		/* File control definitions */
#include <errno.h>		/* Error number definitions */
#include <termios.h>		/* POSIX terminal control definitions */
#include <sys/ioctl.h>
#include <math.h>

#include <serial.h>
#include <misc.h>

/* Moved all const stuff to ts2k.h where it wants to be. */
#undef _ENABLE_PTT_CTRL

/**
 * kenwood_transaction
 * Assumes rig!=NULL rig->state!=NULL rig->caps!=NULL
 *
 * cmdstr - Command to be sent to the rig. Cmdstr can also be NULL, indicating
 *		that only a reply is needed (nothing will be send).
 * data - Buffer for reply string.  Can be NULL, indicating that no reply is
 *	   is needed and will return with RIG_OK after command was sent.
 * datasize - in: Size of buffer. It is the caller's responsibily to provide
 *		  a large enough buffer for all possible replies for a command.
 *		  out: location where to store number of bytes read.
 *
 * returns:
 *   RIG_OK  -  if no error occured.
 *   RIG_EIO  -  if an I/O error occured while sending/receiving data.
 *   RIG_ETIMEOUT  -  if timeout expires without any characters received.
 *   RIG_REJECTED  -  if a negative acknowledge was received or command not
 *				recognized by rig.
 */

/* FIXME: cmd_len appears to change and needs set every invocation? --Dale */

int
ts2k_transaction(RIG * rig, const char *cmdstr, int cmd_len,
		 char *data, size_t * datasize)
{
//       return kenwood_transaction(rig, cmdstr, cmd_len, data, datasize);

	struct rig_state *rs;
	int retval;
	const char *cmdtrm = EOM_KEN;	/* Default Command/Reply termination char */
	int retry_read = 2;
//       char *errtxt;

#define MAX_RETRY_READ 8

	rs = &rig->state;
	rs->hold_decode = 1;

	serial_flush(&rs->rigport);

	cmdtrm = cmd_trm(rig);

	if (cmdstr != NULL) {
		//       rig_debug(RIG_DEBUG_ERR, __FUNCTION__": 1) sending '%s'\n\n", cmdstr);
		retval = write_block(&rs->rigport, cmdstr, strlen(cmdstr));
		if (retval != RIG_OK) {
			ta_quit;
		}
#undef	TH_ADD_CMDTRM
#ifdef	TH_ADD_CMDTRM
		//       rig_debug(RIG_DEBUG_ERR, __FUNCTION__": 2) sending '%s'\n\n", cmdtrm);
		retval = write_block(&rs->rigport, cmdtrm, strlen(cmdtrm));
		if (retval != RIG_OK) {
			ta_quit;
		}
#endif
	}

/* FIXME: Everything below this line wants to be completely rewritten!!!! */

	if (data == NULL || datasize <= 0) {
		rig->state.hold_decode = 0;
		return RIG_OK;	/* don't want a reply */
	}

      transaction_read:
	/* FIXME : TS-2k gets alot of 'timedout' on read_string()! */
	//rig_debug(RIG_DEBUG_ERR, __FUNCTION__": 3a) reading %u bytes...\n", *datasize);
	retval =
	    read_string(&rs->rigport, data, *datasize, cmdtrm,
			strlen(cmdtrm));
	//rig_debug(RIG_DEBUG_ERR, __FUNCTION__": 3b) read '%s', retval=%u\n\n", data, retval);
	*datasize = retval;
	if (retval > 0) {
		//rig_debug(RIG_DEBUG_ERR, __FUNCTION__": 3b) read cmd '%s', retval=%u\n\n", data, retval);
		retval = RIG_OK;
		goto transaction_check;
	}

	/* Check that command termination is correct */
	if (!strchr(cmdtrm, data[strlen(data)])) {
		if (retry_read++ < MAX_RETRY_READ)
			goto transaction_read;
		rig_debug(RIG_DEBUG_ERR,
			  __FUNCTION__
			  ": Command is not correctly terminated '%s'\n",
			  data);
		retval = -RIG_EPROTO;
		ta_quit;
	}

	/* Errors */
	if (strlen(data) == 2 && data[0] == 'E') {
		switch (data[0]) {
		case 'E':
			rig_debug(RIG_DEBUG_ERR,
				  __FUNCTION__
				  ": Communication Error for '%s'\n",
				  cmdstr);
			break;

		case 'O':
			rig_debug(RIG_DEBUG_ERR,
				  __FUNCTION__
				  ": Communication Error for '%s'\n",
				  cmdstr);
			break;

		case '?':
			rig_debug(RIG_DEBUG_ERR,
				  __FUNCTION__
				  ": Communication Error for '%s'\n",
				  cmdstr);
			break;

		default:
			rig_debug(RIG_DEBUG_ERR,
				  __FUNCTION__ ": Hamlib Error for '%s'\n",
				  cmdstr);
			break;

		}
		retval = -RIG_ERJCTED;
		ta_quit;
	}
#undef	CONFIG_STRIP_CMDTRM
#ifdef	CONFIG_STRIP_CMDTRM
	if (strlen(data) > 0)
		data[strlen(data) - 1] = '\0';	/* not very elegant, but should work. */
	else
		data[0] = '\0';
#endif
	/*
	 * Check that received the correct reply. The first two characters
	 * should be the same as command.
	 */
      transaction_check:
	if (cmdstr && (toupper(data[0]) != toupper(cmdstr[0])
		       || toupper(data[1]) != toupper(cmdstr[1]))) {
		/*
		 * TODO: When RIG_TRN is enabled, we can pass the string
		 *        to the decoder for callback. That way we don't ignore
		 *        any commands.
		 */
		if (retry_read++ < MAX_RETRY_READ)
			goto transaction_read;

		rig_debug(RIG_DEBUG_ERR,
			  __FUNCTION__ ": Unexpected reply '%s'\n", data);
		retval = -RIG_EPROTO;
		{
			ta_quit;
		}
	}

	retval = RIG_OK;

//       transaction_quit:
	{
		ta_quit;
	}
}

/* kenwood_init
 *
 * Basically, it sets up *priv
 * REM: serial port is already open (rig->state.rigport.fd)
 */
int ts2k_init(RIG * rig)
{
	const struct rig_caps *caps;
	const struct ts2k_priv_caps *priv_caps;
	rig_debug(RIG_DEBUG_WARN, __FUNCTION__ ": called\n");
	if (!rig || !rig->caps)
		return -RIG_EINVAL;
	caps = rig->caps;
	if (!caps->priv)
		return -RIG_ECONF;
	priv_caps = (const struct ts2k_priv_caps *) caps->priv;

#if 0
	/* this section won't compile --Dale */
	priv = (struct ts2k_priv_data *)
	    malloc(sizeof(struct ts2k_priv_data));
	if (!priv) {
		/* whoops! memory shortage! */
		return -RIG_ENOMEM;
	}

	rig->state.priv = (void *) priv;
	/* Assign default values */
	priv->dummy = -1;	// placeholder for real entries.
#endif

	return RIG_OK;
}

/* kenwood_cleanup
 * the serial port is closed by the frontend
 */
int ts2k_cleanup(RIG * rig)
{
	rig_debug(RIG_DEBUG_TRACE, __FUNCTION__ ": called\n");
	if (!rig)
		return -RIG_EINVAL;
	if (rig->state.priv)
		free(rig->state.priv);
	rig->state.priv = NULL;
	return RIG_OK;
}

/*
 * ts2k_open()	Called just before rig.c does first rig_get_vfo().
 *	We must be able to talk to the rig so ts2k_pm_init() must
 *	be done here.
 *
 *	TODO:	Call a startup script to set things up the user's way.
 */
int ts2k_open(RIG *rig)
{
	return ts2k_pm_init(rig);
}

/***********************************************************
 * I've kept the above functions and deleted and rewritten
 *	everything from here down.  The new stuff will all
 *	use *only* the ts2k_g_??() and ts2k_s_??() routines
 *	for I/O.  They in turn call ts2k_transaction().  I
 *	kept only the function prototype.
 ***********************************************************/

/*****************************************************************************
	Added the following functions.
 	(C) Copyright 2002 by Dale E. Edmons.   All rights Reserved.
	License:	Identical to all other Hamlib code.
*****************************************************************************/

/*
 * status:	Working
 */
int ts2k_set_vfo(RIG * rig, vfo_t vfo)
{
	return ts2k_uniq_SetSimpleVfo(rig, vfo);
}

/*
 * status:	Simple Vfo ok, Complex not implemented
 */
int ts2k_get_vfo(RIG * rig, vfo_t * vfo)
{
	//int retval;

	/* Complex VFOs not yet implemented */
	// retval = ts2k_uniq_GetComplexVfo(rig, vfo);
	// CHKERR(retval);

	return ts2k_uniq_GetSimpleVfo(rig, vfo);
}

/*
 * Set Rx frequency.  Tx frequency unchanged.
 *
 * Note: The PLL is the device which implements the
 *	hardware VFO.  The rig saves and restores
 *	the value in the PLL when new modes are
 *	selected (e.g. Satelite).  We must place
 *	the rig in the same mode to ensure the rig's
 *	view of the frequency gets changed and not
 *	just the PLL.  For example:
 *		I wish to operate satelite and change
 *		the dnlink to freq = F1 using the
 *		"fb;" command.  I then turn on SAT
 *		mode.  The dnlink freq is *not* F1.
 *		The rig took the last SAT values out
 *		of memory an wrote them to the PLLs,
 *		an overwrote F1 with freq = F2.
 *		Thus, *just* writing to the correct
 *		PLL is not enough to ensure that we
 *		have stored F1 as the dnlink freq!
 *		However, I deselect SAT and the PLL
 *		now has freq F1 instead of F2.
 *	As a consequence, I do rig_set_vfo() before
 *	all operations that are not RIG_VFO_CURR and
 *	that apply to a single VFO rather than the
 *	entire rig.
 *
 *	Status:	working
 */
int ts2k_set_freq(RIG * rig, vfo_t vfo, freq_t freq)
{
	int retval, skip;
	vfo_t vorig, vcurr;
	TS2K_FA_T param;

	skip = (vfo & RIG_VFO_CURR);

	// Must be outside skip.  We need *real* VFO.
	retval = rig_get_vfo(rig, &vorig);
	CHKERR(retval);

	// Set correct VFO
	if( !skip ) {
		retval = rig_set_vfo(rig, vfo);
		CHKERR(retval);
		vcurr = vfo;
	} else {
		vcurr = vorig;	// I don't like changing caller's value
	}

	param.p1 = freq;	// Good starting place

	// Simplest VFOs
	switch(vcurr) {

	case RIG_SAT_UPLINK:
	case RIG_VFO_AB:
	case RIG_VFO_A:
		retval = ts2k_s_fa(rig, &param);
		break;

	case RIG_SAT_DNLINK:
	case RIG_VFO_BA:
	case RIG_VFO_B:
		retval = ts2k_s_fb(rig, &param);
		break;

	case RIG_VFO_C:
		retval = ts2k_s_fc(rig, &param);
		break;

	case RIG_CALL_C:
	case RIG_CALL_A:	// ugly hack!
		retval = ts2k_emul_SetCallFreq(rig, vcurr, freq);
		break;

	case RIG_MEM_A:
	case RIG_MEM_C:
	default:
		rig_debug(RIG_DEBUG_ERR, __FUNCTION__ 
			": Target not currently recognized, vfo = %s\n",
			strvfo(vcurr));
		return -RIG_ENIMPL;	// or, not available
		break;
	}

	if( !skip ) {
		retval = rig_set_vfo(rig, vorig);
		CHKERR(retval);
	}

	return RIG_OK;
}

/*
 *	status:	working
 */
int ts2k_get_freq(RIG *rig, vfo_t vfo, freq_t *freq)
{
	int retval, skip;
	vfo_t vorig;
	TS2K_IF_T param;

	skip = (vfo & RIG_VFO_CURR);

	// Must be outside skip.  We need *real* VFO.
	retval = rig_get_vfo(rig, &vorig);
	CHKERR(retval);

	// Set correct VFO
	if( !skip ) {
		retval = rig_set_vfo(rig, vfo);
		CHKERR(retval);
	}

	// Simplest VFOs
	switch(vfo) {
	case RIG_SAT_DNLINK:
		retval = ts2k_g_oi(rig, &param);
		break;

	default:
		retval = ts2k_g_if(rig, &param);
		break;
	}

	*freq = param.p1;

	if( !skip ) {
		retval = rig_set_vfo(rig, vorig);
		CHKERR(retval);
	}

	return RIG_OK;
}

/*
 * Preserves original vfo.  Width is only available for the
 *	entire transceiver, and not per VFO.  Use rig_passband_*().
 *
 *	status:	working
 */
int ts2k_set_mode(RIG * rig, vfo_t vfo, rmode_t mode, pbwidth_t width)
{
	int retval, skip;
	vfo_t vorig;
	TS2K_MD_T param;

	/*
 	 * Width is a transceiver parameter set by rig_passband_*()
	 *	functions and available for main only.  Rig applies
	 *	it to each identical mode (Main only) for all VFOs.
	 *	(My rig is E-type.  Manual says K-type can use narrow
	 *	on Sub.)
	 */

	skip = (vfo & RIG_VFO_CURR);

	// Set correct VFO
	if( !skip ) {
		retval = rig_get_vfo(rig, &vorig);
		CHKERR(retval);
		retval = rig_set_vfo(rig, vfo);
		CHKERR(retval);
	}

	if(width != RIG_PASSBAND_NORMAL) {
		rig_debug(RIG_DEBUG_WARN, __FUNCTION__
			": Passband not available.  "
			"Use rig_passsband_*()\n");
	}

	// Set Modes
	switch(mode) {

	case RIG_MODE_LSB:	param.p1 = 1; break;
	case RIG_MODE_USB:	param.p1 = 2; break;
	case RIG_MODE_CW:	param.p1 = 3; break;
	case RIG_MODE_FM:	param.p1 = 4; break;
	case RIG_MODE_AM:	param.p1 = 5; break;
	case RIG_MODE_RTTY:	param.p1 = 6; break;
	case RIG_MODE_CWR:	param.p1 = 7; break;

	case RIG_MODE_RTTYR:	param.p1 = 9; break;

	default:
		rig_debug(RIG_DEBUG_ERR, __FUNCTION__
			": Unknown mode.\n");
		return -RIG_ENAVAIL;
		break;
	}

	retval = ts2k_s_md(rig, &param);
	CHKERR(retval);

	if( !skip ) {
		retval = rig_set_vfo(rig, vorig);
		CHKERR(retval);
	}

	return RIG_OK;
}

/*
 *	status:	working
 */
int ts2k_get_mode(RIG * rig, vfo_t vfo, rmode_t * mode, pbwidth_t * width)
{
	int retval, skip;
	vfo_t vorig;
	union {
		TS2K_MD_T md;
		TS2K_FW_T fw;
	} param;

	skip = (vfo & RIG_VFO_CURR);

	// Set correct VFO
	if( !skip ) {
		retval = rig_get_vfo(rig, &vorig);
		CHKERR(retval);
		retval = rig_set_vfo(rig, vfo);
		CHKERR(retval);
	}

	retval = ts2k_g_md(rig, &param.md);
	CHKERR(retval);

	switch(param.md.p1) {
	case 1:	*mode = RIG_MODE_LSB; break;
	case 2:	*mode = RIG_MODE_USB; break;
	case 3:	*mode = RIG_MODE_CW; break;
	case 4:	*mode = RIG_MODE_FM; break;
	case 5:	*mode = RIG_MODE_AM; break;
	case 6:	*mode = RIG_MODE_RTTY; break;
	case 7:	*mode = RIG_MODE_CWR; break;

	case 9:	*mode = RIG_MODE_RTTYR; break;

	default:
		*mode = RIG_MODE_NONE;
		rig_debug(RIG_DEBUG_ERR, __FUNCTION__
			": Unknown mode = %i\n", param.md.p1);
		return -RIG_ENAVAIL;
		break;
	}

	*width = RIG_PASSBAND_NORMAL;	// FIXME:

	if( !skip ) {
		retval = rig_set_vfo(rig, vorig);
		CHKERR(retval);
	}

	return RIG_OK;
}

/*
 *	status:	working
 */
int ts2k_set_ptt(RIG * rig, vfo_t vfo, ptt_t ptt)
{
	int retval, skip;
	vfo_t vorig;

	skip = (vfo & RIG_VFO_CURR);

	if( !skip ) {
		/*
		 * Do NOT Restore original VFO with PTT enabled!
		 */
		retval = rig_get_vfo(rig, &vorig);
		CHKERR(retval);
		retval = rig_set_vfo(rig, vfo);
		CHKERR(retval);
	}

	switch(ptt) {
	case RIG_PTT_ON:
		retval = ts2k_s_tx(rig, NULL);	// Expect errors.  Rig Bug.
		skip |= 1;
		/* no CHKERR(retval) */
		break;

	case RIG_PTT_OFF:
		retval = ts2k_s_rx(rig, NULL);	// Expect errors.  Rig Bug.
		skip |= 0;
		/* no CHKERR(retval) */
		break;

	default:
		rig_debug(RIG_DEBUG_ERR, __FUNCTION__
			": Unknown ptt value = %i\n", ptt);
		return -RIG_ENAVAIL;
		break;
	}

	/*
	 * Do No Restore VFO with PTT enabled!
	 */
	if( !skip ) {
		retval = rig_set_vfo(rig, vorig);
		CHKERR(retval);
	}

	return RIG_OK;
}

/*
 * Manual incorrectly states "tx;" will query.  We
 *	must use "if;" instead.  "tx;" transmits!!!
 *	(Or causes an error if already set.  Geez!)
 *
 *	Status:	working!
 */
int ts2k_get_ptt(RIG * rig, vfo_t vfo, ptt_t * ptt)
{
	int retval, skip;
	vfo_t vorig;
	TS2K_IF_T param;

	skip = (vfo & RIG_VFO_CURR) || (vfo & RIG_MEM_CURR);

	if( !skip ) {
		retval = rig_get_vfo(rig, &vorig);
		CHKERR(retval);
	}

	retval = ts2k_g_if(rig, &param);
	CHKERR(retval);

	switch(param.p8) {

	case 0:	*ptt = RIG_PTT_OFF; break;
	case 1:	*ptt = RIG_PTT_ON; break;

	default:
		return -RIG_EINTERNAL;	// or rig
	}

	if( !skip ) {
		retval = rig_set_vfo(rig, vorig);
		CHKERR(retval);
	}

	return RIG_OK;
}

/*
 */
int ts2k_set_trn(RIG * rig, int trn)
{
	return -RIG_ENIMPL;
}

/*
 */
int ts2k_get_trn(RIG * rig, int *trn)
{
	return -RIG_ENIMPL;
}

/*
 */
int ts2k_set_powerstat(RIG * rig, powerstat_t status)
{
	return -RIG_ENIMPL;
}

/*
 */
int ts2k_get_powerstat(RIG * rig, powerstat_t * status)
{
	return -RIG_ENIMPL;
}

/*
 */
int ts2k_reset(RIG * rig, reset_t reset)
{
	return -RIG_ENIMPL;
}

/*
 */
int ts2k_send_morse(RIG * rig, vfo_t vfo, const char *msg)
{
	return -RIG_ENIMPL;
}

/*
 */
int ts2k_vfo_op(RIG * rig, vfo_t vfo, vfo_op_t op)
{
	return -RIG_ENIMPL;
}

/*
 *	status: working
 */
int ts2k_set_mem(RIG * rig, vfo_t vfo, int ch)
{
	int retval, skip;
	vfo_t vorig;
	TS2K_MC_T param;

	skip = (vfo & RIG_VFO_CURR);

	if( !skip ) {
		retval = rig_get_vfo(rig, &vorig);
		CHKERR(retval);
	}

	switch(vfo) {

	/* Note: Main/Sub memory pointers are always valid.
	 *	Memory mode needn't be enabled! */
	case RIG_MEM_A:
	case RIG_MEM_C:
		retval = rig_set_vfo(rig, vfo);
		/* no break */

	case RIG_MEM_CURR:
	case RIG_VFO_CURR:	// Compatibility with existing code
		param.p1 = ch;
		retval = ts2k_s_mc(rig, &param);
		CHKERR(retval);
		break;

	case RIG_MEMO:	// scratchpad or temp memories
		return -RIG_ENIMPL;
		break;

	case RIG_SAT_MEM:
		return -RIG_ENIMPL;
		break;
	
	default:
		rig_debug(RIG_DEBUG_ERR, __FUNCTION__
			": Unknown mem = %s\n", strvfo(vfo));
		return -RIG_ENAVAIL;
		break;
	}

	if( !skip ) {
		retval = rig_set_vfo(rig, vorig);
		CHKERR(retval);
	}

	return RIG_OK;
}

/*
 *	status:	working
 */
int ts2k_get_mem(RIG * rig, vfo_t vfo, int *ch)
{
	int retval, skip;
	vfo_t vorig;
	TS2K_MC_T param;

	skip = (vfo & RIG_VFO_CURR) || (vfo & RIG_MEM_CURR);

	if( !skip ) {
		retval = rig_get_vfo(rig, &vorig);
		CHKERR(retval);
	}

	switch(vfo) {
	case RIG_MEM_A:
	case RIG_MEM_C:
		retval = rig_set_vfo(rig, vfo);
		/* no break */

	case RIG_MEM_CURR:	// Like RIG_VFO_CURR but for mem
	case RIG_VFO_CURR:	// compatibility with existing code
		retval = ts2k_g_mc(rig, &param);
		CHKERR(retval);
		*ch = param.p1;
		break;

	case RIG_MEMO:	// scratchpad or temp memories
		return -RIG_ENIMPL;
		break;

	case RIG_SAT_MEM:
		return -RIG_ENIMPL;
		break;
	
	default:
		rig_debug(RIG_DEBUG_ERR, __FUNCTION__
			": Unknown mem = %s\n", strvfo(vfo));
		return -RIG_ENAVAIL;
		break;
	}

	if( !skip ) {
		retval = rig_set_vfo(rig, vorig);
		CHKERR(retval);
	}

	return RIG_OK;
}

/*
 */
const char *ts2k_get_info(RIG * rig)
{
	return NULL;
}

/*
 */
rig_model_t probe_ts2k(port_t * port)
{
	return -RIG_ENIMPL;
}

int ts2k_get_rit(RIG * rig, vfo_t vfo, shortfreq_t * rit)
{
	return -RIG_ENIMPL;
}

/*
 */
int ts2k_get_dcd(RIG * rig, vfo_t vfo, dcd_t * dcd)
{
	return -RIG_ENIMPL;
}

/*
*/
int ts2k_get_xit(RIG * rig, vfo_t vfo, shortfreq_t * rit)
{
	return ts2k_get_rit(rig, vfo, rit);
}

int ts2k_set_rit(RIG * rig, vfo_t vfo, shortfreq_t rit)
{
	return -RIG_ENIMPL;
}

int ts2k_set_xit(RIG * rig, vfo_t vfo, shortfreq_t rit)
{
	return ts2k_set_rit(rig, vfo, rit);
}

int ts2k_get_ts(RIG * rig, vfo_t vfo, shortfreq_t * ts)
{
	return -RIG_ENIMPL;
}

/*
 */
int ts2k_set_ts(RIG * rig, vfo_t vfo, shortfreq_t ts)
{
	return -RIG_ENIMPL;
}

/*
 */
int ts2k_set_ctcss(RIG * rig, vfo_t vfo, tone_t tone)
{
	return -RIG_ENIMPL;
}

/*
 */
int ts2k_get_ctcss(RIG * rig, vfo_t vfo, tone_t * tone)
{
	return -RIG_ENIMPL;
}

int ts2k_set_tone(RIG * rig, vfo_t vfo, tone_t tone)
{
	return -RIG_ENIMPL;
}

int ts2k_get_tone(RIG * rig, vfo_t vfo, tone_t * tone)
{
	return -RIG_ENIMPL;
}

/*
 */
int ts2k_get_dcs(RIG * rig, vfo_t vfo, tone_t * tone)
{
	return -RIG_ENIMPL;
}

/*
 */
int ts2k_set_dcs(RIG * rig, vfo_t vfo, tone_t tone)
{
	return -RIG_ENIMPL;
}

/* Rig truncates in kHz(50) steps, so we don't. */
int ts2k_get_rptr_offs(RIG * rig, vfo_t vfo, shortfreq_t * rptr_offs)
{
	return -RIG_ENIMPL;
}

/* Rig truncates in kHz(50) steps, so we don't. */
int ts2k_set_rptr_offs(RIG * rig, vfo_t vfo, shortfreq_t rptr_offs)
{
	return -RIG_ENIMPL;
}

/*
 */
int ts2k_get_rptr_shift(RIG * rig, vfo_t vfo, rptr_shift_t * rptr_shift)
{
	return -RIG_ENIMPL;
}

/*
 */
int ts2k_set_rptr_shift(RIG * rig, vfo_t vfo, rptr_shift_t rptr_shift)
{
	return -RIG_ENIMPL;
}

int ts2k_get_split(RIG * rig, vfo_t vfo, split_t * split)
{
	return -RIG_ENIMPL;
}

int ts2k_set_split(RIG * rig, vfo_t vfo, split_t split)
{
	return -RIG_ENIMPL;
}

int ts2k_get_split_freq(RIG * rig, vfo_t vfo, freq_t * tx_freq)
{
	return -RIG_ENIMPL;
}

int ts2k_set_split_freq(RIG * rig, vfo_t vfo, freq_t tx_freq)
{
	return -RIG_ENIMPL;
}

/*
 */
int ts2k_set_split_mode(RIG * rig,
			vfo_t vfo, rmode_t txmode, pbwidth_t txwidth)
{
	return -RIG_ENIMPL;
}

int ts2k_get_split_mode(RIG * rig,
			vfo_t vfo, rmode_t * txmode, pbwidth_t * txwidth)
{
	return -RIG_ENIMPL;
}

/*
 */
int ts2k_get_channel(RIG * rig, channel_t * chan)
{
	return -RIG_ENIMPL;
}

/*
 */
int ts2k_set_channel(RIG * rig, channel_t * chan)
{
	return -RIG_ENIMPL;
}

/*
 */
int ts2k_scan(RIG * rig, vfo_t vfo, scan_t scan, int ch)
{
	return -RIG_ENIMPL;
}

int ts2k_set_level(RIG * rig, vfo_t vfo, setting_t level, value_t val)
{
	return -RIG_ENIMPL;
}

/*
 */
int ts2k_get_level(RIG * rig, vfo_t vfo, setting_t level, value_t * val)
{
	return -RIG_ENIMPL;
}

/*
 */
int ts2k_set_func(RIG * rig, vfo_t vfo, setting_t func, int status)
{
	return -RIG_ENIMPL;
}

/*
 */
int ts2k_get_func(RIG * rig, vfo_t vfo, setting_t func, int *status)
{
	return -RIG_ENIMPL;
}

int ts2k_get_parm(RIG * rig, setting_t parm, value_t * val)
{
	return -RIG_ENIMPL;
}

int ts2k_set_parm(RIG * rig, setting_t parm, value_t val)
{
	return -RIG_ENIMPL;
}

/*
 * ts2k_save_channel() (rig_save_channel() doesn't call us! don't write either)
 *
int ts2k_save_channel(RIG *rig, chan_t *ch)
{
	
}
int ts2k_restore_channel(){}
 This part awaiting Hamlib changes
 */

/* The following are "so-to-be" functions:

int ts2k_init_pm(){}	// done
int ts2k_init_menu(){}	// done
int ts2k_menu_build(){}
int ts2k_menu_parse(){}
int ts2k_menu_list(){}	// done

// The rest is anybody's guess...  --Dale

 * end TODO list
 */

/*
 * Uniq Functions for the TS-2000 are below.
 */

/*
 * PTT/CTRL are assumed to match.  CTRL defines VFO.
 *	Eventually we need to provide the user an option!
 *
 *	status: working
 */
int ts2k_uniq_GetSimpleVfo(RIG *rig, vfo_t *vfo)
{
	int retval, rx, t;
	vfo_t vtmp;
	union {
		TS2K_FR_T rx;
		TS2K_FT_T tx;
		TS2K_DC_T dc;
	} param;

	vtmp = 0;

	// Get Main/Sub
	retval = ts2k_uniq_GetTransceiver(rig, &vtmp);

	// Get Rx vfo
	retval = ts2k_g_fr(rig, &param.rx);
	rx = param.rx.p1;
	CHKERR(retval);

	t = (vtmp & RIG_CTRL_MAIN);

	switch(rx) {
	case 0:		// First vfo
		vtmp |= (t)? RIG_VFO_A: RIG_VFO_C;
		break;

	case 1:		// Second vfo
		vtmp |= (t)? RIG_VFO_B: 0;
		break;

	case 2:		// Mem
		vtmp |= (t)? RIG_MEM_A: RIG_MEM_C;
		break;

	case 3:		// Call
		vtmp |= (t)? RIG_CALL_A: RIG_CALL_C;
		break;

		default:
			rig_debug(RIG_DEBUG_ERR, __FUNCTION__ \
				": Rx not yet recognized.\n");
			break;
	}

	// Get Tx vfo
	retval = ts2k_g_ft(rig, &param.tx);
	CHKERR(retval);

	if( rx != param.tx.p1) {	// We're in Split
		vtmp |= RIG_CTRL_SPLIT;

		switch(param.tx.p1) {
		case 0:
			vtmp |= RIG_VFO_A;
			if(rx==1)
				vtmp |= RIG_CTRL_REV;	// RIG_VFO_BA
			// FIXME: else do mem/vfo split here
			break;

		case 1:
			vtmp |= RIG_VFO_B;
			break;

		case 2:
			vtmp |= RIG_CTRL_MEM;	// Guess this'll work...
			break;

		default:
			rig_debug(RIG_DEBUG_ERR, __FUNCTION__ \
				": Split not yet recognized.\n");
			break;
		}
	}
	*vfo = vtmp;

	return RIG_OK;
}

/*
 * Sets only RIG_VFO_[ABC],
 *		RIG_MEM_[ABC],
 *		RIG_CALL_[AC], no questions asked.
 *
 *	No error checking (e.g. testing for Complex VFOs)
 *
 *	Status:	Sets applicable VFOs correctly!
 */
int ts2k_uniq_SetSimpleVfo(RIG *rig, vfo_t vfo)
{
	vfo_t vtmp;
	int retval, skip;
	union {
		TS2K_FR_T rx;
		TS2K_FT_T tx;
	} param;

	vtmp = skip = 0;

	// Set Main/Sub
	switch(vfo) {
	case RIG_VFO_C:
	case RIG_MEM_C:
	case RIG_CALL_C:
	case RIG_CTRL_SUB:
		vtmp = RIG_CTRL_SUB;
		break;

	case RIG_VFO_A:
	case RIG_VFO_B:
	case RIG_MEM_A:
	case RIG_CALL_A:
	case RIG_VFO_AB:
	case RIG_VFO_BA:
	case RIG_CTRL_MAIN:
		vtmp = RIG_CTRL_MAIN;
		break;

	default:
		skip = 1;	// Force action to not be taken.
		break;
	}

	if(!skip) {
		retval = ts2k_uniq_SetTransceiver(rig, vtmp);
		CHKERR(retval);
		skip = 0;
	}

	// Set Rx (always first, since rig sets Tx to match)
	switch(vfo) {
	case RIG_VFO_A:
	case RIG_VFO_AB:
	case RIG_VFO_C:		// looks strange, but it's correct
		param.rx.p1 = 0;
		break;

	case RIG_VFO_B:
	case RIG_VFO_BA:
		param.rx.p1 = 1;
		break;

	case RIG_MEM_A:
	case RIG_MEM_C:
	//case RIG_VFO_M_A:	// mem/vfo
	//case RIG_VFO_M_B:	// mem/vfo
		param.rx.p1 = 2;
		break;

	case RIG_CALL_A:
	case RIG_CALL_C:
		param.rx.p1 = 3;
		break;

	default:
		skip = 1;	// Force action to not be taken.
		break;
	}

	if(!skip) {
		retval = ts2k_s_fr(rig, &param.rx);
		CHKERR(retval);
		skip = 0;
	}

	// Set Tx if different
	if(vfo & RIG_CTRL_SPLIT) {
		switch(vfo) {
		case RIG_VFO_AB:
		//case RIG_VFO_M_A:	// mem/vfo
			param.tx.p1 = 1;
			break;

		case RIG_VFO_BA:
		//case RIG_VFO_M_B:	// mem/vfo
			param.tx.p1 = 0;
			break;

		// Mem/Vfo split goes here if we ever do it
		// and Menu 6A is On.
		/*case RIG_VFO_A_M:	// mem/vfo
		case RIG_VFO_B_M:
			param.tx.p1 = 2;
			break;
		*/
		default:
			skip = 1;	// Force action to not be taken.
			break;
		}

		if(!skip) {
			retval = ts2k_s_ft(rig, &param.tx);
			CHKERR(retval);
			//skip = 0;	// not needed here
		}
	}

	return RIG_OK;
}

/*
 * Sets either Main or Sub.  PTT/CTRL match!
 *
 *	status:	working
 */
int ts2k_uniq_SetTransceiver(RIG *rig, vfo_t vfo)
{
	TS2K_DC_T param;

	param.p1 = param.p2 = (vfo & RIG_CTRL_MAIN)? 0: 1;

	return ts2k_s_dc(rig, &param);
}

/*
 * This routine determines whether the Main or Sub
 *	transceiver is selected (CTRL).
 *
 *	Status:	Working.
 */
int ts2k_uniq_GetTransceiver(RIG *rig, vfo_t *vfo)
{
	int retval;
	vfo_t vtmp;
	TS2K_DC_T param;

	vtmp = 0;

	// Get Main/Sub
	retval = ts2k_g_dc(rig, &param);
	CHKERR(retval);

	/*
	 * The following is inadequate.  But, since we have only one VFO passed
	 *	to us, we deal with it as best we can.
	 */
	if(param.p2 == 0) {
		vtmp = RIG_CTRL_MAIN;		// CTRL on Main
		//vtmp = RIG_CTRL_MAIN | RIG_VFO_CTRL;		// CTRL on Main
		//vtmp |= (param.p1 == 0)? RIG_VFO_PTT: 0;	// PTT on Main
	} else if(param.p2 == 1) {
		vtmp = RIG_CTRL_SUB;		// CTRL on Sub
		//vtmp = RIG_CTRL_SUB | RIG_VFO_CTRL;		// CTRL on Sub
		//vtmp |= (param.p1 == 1)? RIG_VFO_PTT: 0;	// PTT on Sub
	} else {
		rig_debug(RIG_DEBUG_ERR, __FUNCTION__ \
			": Transceiver %i not yet recognized.\n", param.p2);
		//return -RIG_EINTERNAL;	// Debugging required
	}
	*vfo = vtmp;

	return RIG_OK;
}

/*
 * FIXME: simple ascii to integer converter--expensive!
 *	Prevents trashing original string, otherwise
 *	just drop a (char)0 where you need it.
 */
//int ts2k_get_int(char *src, int cnt)
long int int_n(char *src, int cnt)
{
//	static char buf[20];
	char buf[20];	// enough for 1e20 - 1
	strncpy(buf, src, cnt);
	buf[cnt] = (char) 0;
	return atol(buf);
//	return atoi(buf);
}

/****************************************
	Emulation Routines
 ****************************************/

/*
 * Emulate direct access to Call freqs
 *	Side effects: original modes destroyed
 *
 *	status:	working	
 */
int ts2k_emul_SetCallFreq(RIG *rig, vfo_t vfo, freq_t freq)
{
	int retval;
	vfo_t vtmp, vorig, vorig2;
	freq_t ftmp;

	// Save current VFO
	retval = rig_get_vfo(rig, &vorig);
	CHKERR(retval);

	// Switch to correct Transceiver
	vtmp = (vfo & (RIG_CTRL_MAIN | RIG_CTRL_SUB));
	retval = ts2k_uniq_SetTransceiver(rig, vtmp);

	// Save current VFO in case its a different transceiver 
	retval = rig_get_vfo(rig, &vorig2);
	CHKERR(retval);

	vtmp = vfo & (RIG_VFO_A | RIG_VFO_C);

	// Set either VFOA or VFOC
	retval = rig_set_vfo(rig, vtmp);
	CHKERR(retval);

	// Save current frequency in PLL
	retval = ts2k_get_freq(rig, vtmp, &ftmp);
	CHKERR(retval);
	retval = ts2k_set_freq(rig, vtmp, freq); 
	CHKERR(retval);

	// Now, actually write to CALL_n
	retval = ts2k_s_ci(rig, NULL);	// Non-standard.
	CHKERR(retval);

	// restore orig freq to PLL
	retval = ts2k_set_freq(rig, vtmp, ftmp);
	CHKERR(retval);

	// restore this transceiver vfo
	retval = rig_set_vfo(rig, vorig2);
	CHKERR(retval);

	// restore starting transceiver vfo
	retval = rig_set_vfo(rig, vorig);
	CHKERR(retval);

	return RIG_OK;
}

// End
