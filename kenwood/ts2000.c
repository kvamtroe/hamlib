/*
 *  Hamlib Kenwood backend - TS2000 description
 *  Copyright (c) 2000-2002 by Stephane Fillod
 *
 *		$Id: ts2000.c,v 1.9.2.4 2003-02-25 03:58:50 dedmons Exp $
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

#include <ctype.h>
#include <hamlib/rig.h>
//#include "kenwood.h"
#include "ts2000.h"
#include "ts2k/ts2k_cmds.h"
//#include "ts2k/ts2k_menu.h"

const struct ts2000_priv_caps  ts2000_priv_caps  = {
		cmdtrm: EOM_KEN,
};

/*
 * ts2000 rig capabilities.
 *
 * TODO: antenna caps
 *
 * part of infos comes from http://www.kenwood.net/
 */
const struct rig_caps ts2000_caps = {
rig_model: RIG_MODEL_TS2000,
model_name:"TS-2000",
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
// set 1,20,4 and cured timeouts for "if...;" on ts2000_get_freq()	--Dale
post_write_delay: 1,
timeout: 15,
retry: 4,

has_get_func: TS2000_FUNC_ALL,
has_set_func: TS2000_FUNC_ALL,
has_get_level: TS2000_LEVEL_ALL,
has_set_level: RIG_LEVEL_SET(TS2000_LEVEL_ALL),
has_get_parm: TS2000_PARM_OP,
has_set_parm: TS2000_PARM_OP,
level_gran: {},                 /* FIXME: granularity */
parm_gran: {},
vfo_ops: TS2000_VFO_OP,
scan_ops: TS2000_SCAN_OP,
ctcss_list: ts2000_ctcss_list,
dcs_list: ts2000_dcs_list,
preamp:  { 20, RIG_DBLST_END, },	/* FIXME: real preamp? */
attenuator:  { 20, RIG_DBLST_END, },
max_rit: kHz(20),
max_xit: kHz(20),
max_ifshift: kHz(1),
targetable_vfo: RIG_TARGETABLE_FREQ,
transceive: RIG_TRN_RIG,
bank_qty:  0,
chan_desc_sz: 8,

/* set up the memories.  See also, rig.h --D.E. kd7eni */

/* The following are suggested 'modes' and when the following may
 *  be accessed:
 *
 *	MTYPE		MSTATE		Description
 *
 *	MEM		M_MEM		main, sub
 *	EDGE		M_MEM		main, sub (vhf/uhf)
 *	MEMOPAD		M_VFO		e.g. main&&sub in vfo (both!)
 *	CALL		M_ANY		at least VFO and MEM (others?)
 *	SAT		M_SAT		only (uses both main+sub)
 *	PCT		M_PCT		when P.C.T. enabled on sub+tnc
 *	MENU		M_MOST		rig does it if it feels like it :)
 *	SETUP		M_UNKNOWN	twilight zone stuff...
 */
chan_list: {
	{ 0, 289, RIG_MTYPE_MEM, 0 },		/* regular memories */
	/* Note: each memory is receive+transmit an RX != TX is split memory. */
	{ 290, 299, RIG_MTYPE_EDGE, 0 },	/* band tune limits (not scan-only) */
	{ 0, 9, RIG_MTYPE_MEMOPAD, 0 },		/* Quick Memories, Main+sub both saved:) */
	{ 0, 1, RIG_MTYPE_CALL, 0 },		/* each TX band has one call */
	{ 0, 9, RIG_MTYPE_SAT, 0 },		/* direct operation from these */
//	{ 0, 9, RIG_MTYPE_PCT, 0 },		/* packet clusters buffered as
//						   they come in */
//	{ 0, 1, RIG_MTYPE_MENU, 0 },		/* There are two menus, A/B. I
//						   set one for HF, one for VHF/UHF*/
//	{ 0, 5, RIG_MTYPE_SETUP, 0 },		/* See: "pm;" command.  ;) */
	/* This seems to be undocumented and not accesible to the front panel.
	   When operated it seems to be an independently settable menu. Thus,
	   more than just A/B are available.  I don't know if the memopad
	   quick memories are involved but the regular MEM ones are *NOT*
	   duplicated.  The manual only says: 0=PM off, 1-5=channel 1-5.
	   Kenwood calls this "Programmable Memory".  I haven't used this
	   in some time but 0-5 seems more appropriate than 1-5.  I'll
	   investigate more after hamlib-1.1.3 (gnurig's target release). */

/*	{ 0, , RIG_MTYPE_, 0 },*/
	RIG_CHAN_END,
   },
rx_range_list1: {
	{kHz(300),MHz(60),TS2000_ALL_MODES,-1,-1,TS2000_MAINVFO},
	{MHz(144),MHz(146),TS2000_ALL_MODES,-1,-1,TS2000_MAINVFO},
	{MHz(430),MHz(440),TS2000_ALL_MODES,-1,-1,TS2000_MAINVFO},
	{MHz(144),MHz(146),TS2000_ALL_MODES,-1,-1,TS2000_SUBVFO},
	{MHz(430),MHz(440),TS2000_ALL_MODES,-1,-1,TS2000_SUBVFO},
	RIG_FRNG_END,
  }, /* rx range */
tx_range_list1: {
    {kHz(1830),kHz(1850),TS2000_OTHER_TX_MODES,W(5),W(100),TS2000_MAINVFO},
    {kHz(1830),kHz(1850),TS2000_AM_TX_MODES,2000,25000,TS2000_MAINVFO},
    {kHz(3500),kHz(3800),TS2000_OTHER_TX_MODES,W(5),W(100),TS2000_MAINVFO},
    {kHz(3500),kHz(3800),TS2000_AM_TX_MODES,W(5),W(25),TS2000_MAINVFO},
    {MHz(7),kHz(7100),TS2000_OTHER_TX_MODES,W(5),W(100),TS2000_MAINVFO},
    {MHz(7),kHz(7100),TS2000_AM_TX_MODES,W(5),W(25),TS2000_MAINVFO},
    {MHz(10.1),MHz(10.15),TS2000_OTHER_TX_MODES,W(5),W(100),TS2000_MAINVFO},
    {MHz(10.1),MHz(10.15),TS2000_AM_TX_MODES,W(5),W(25),TS2000_MAINVFO},
    {MHz(14),kHz(14350),TS2000_OTHER_TX_MODES,W(5),W(100),TS2000_MAINVFO},
    {MHz(14),kHz(14350),TS2000_AM_TX_MODES,W(5),W(25),TS2000_MAINVFO},
    {kHz(18068),kHz(18168),TS2000_OTHER_TX_MODES,W(5),W(100),TS2000_MAINVFO},
    {kHz(18068),kHz(18168),TS2000_AM_TX_MODES,W(5),W(25),TS2000_MAINVFO},
    {MHz(21),kHz(21450),TS2000_OTHER_TX_MODES,W(5),W(100),TS2000_MAINVFO},
    {MHz(21),kHz(21450),TS2000_AM_TX_MODES,W(5),W(25),TS2000_MAINVFO},
    {kHz(24890),kHz(24990),TS2000_OTHER_TX_MODES,W(5),W(100),TS2000_MAINVFO},
    {kHz(24890),kHz(24990),TS2000_AM_TX_MODES,W(5),W(25),TS2000_MAINVFO},
    {MHz(28),kHz(29700),TS2000_OTHER_TX_MODES,W(5),W(100),TS2000_MAINVFO},
    {MHz(28),kHz(29700),TS2000_AM_TX_MODES,W(5),W(25),TS2000_MAINVFO},
    {MHz(50),MHz(50.2),TS2000_OTHER_TX_MODES,W(5),W(100),TS2000_MAINVFO},
    {MHz(50),MHz(50.2),TS2000_AM_TX_MODES,W(5),W(25),TS2000_MAINVFO},
    {MHz(144),MHz(146),TS2000_OTHER_TX_MODES,W(5),W(100),TS2000_MAINVFO},
    {MHz(144),MHz(146),TS2000_AM_TX_MODES,W(5),W(25),TS2000_MAINVFO},
    {MHz(430),MHz(440),TS2000_OTHER_TX_MODES,W(5),W(50),TS2000_MAINVFO},
    {MHz(430),MHz(440),TS2000_AM_TX_MODES,W(5),W(12.5),TS2000_MAINVFO},
	RIG_FRNG_END,
  }, /* tx range */

rx_range_list2: {
	{kHz(300),MHz(60),TS2000_ALL_MODES,-1,-1,TS2000_MAINVFO},
	{MHz(142),MHz(152),TS2000_ALL_MODES,-1,-1,TS2000_MAINVFO},
	{MHz(420),MHz(450),TS2000_ALL_MODES,-1,-1,TS2000_MAINVFO},
	{MHz(118),MHz(174),TS2000_ALL_MODES,-1,-1,TS2000_SUBVFO},
	{MHz(220),MHz(512),TS2000_ALL_MODES,-1,-1,TS2000_SUBVFO},
	RIG_FRNG_END,
  }, /* rx range */
tx_range_list2: {
    {kHz(1800),MHz(2),TS2000_OTHER_TX_MODES,W(5),W(100),TS2000_MAINVFO},
    {kHz(1800),MHz(2),TS2000_AM_TX_MODES,2000,25000,TS2000_MAINVFO},
    {kHz(3500),MHz(4),TS2000_OTHER_TX_MODES,W(5),W(100),TS2000_MAINVFO},
    {kHz(3500),MHz(4),TS2000_AM_TX_MODES,W(5),W(25),TS2000_MAINVFO},
    {MHz(7),kHz(7300),TS2000_OTHER_TX_MODES,W(5),W(100),TS2000_MAINVFO},
    {MHz(7),kHz(7300),TS2000_AM_TX_MODES,W(5),W(25),TS2000_MAINVFO},
    {MHz(10.1),MHz(10.15),TS2000_OTHER_TX_MODES,W(5),W(100),TS2000_MAINVFO},
    {MHz(10.1),MHz(10.15),TS2000_AM_TX_MODES,W(5),W(25),TS2000_MAINVFO},
    {MHz(14),kHz(14350),TS2000_OTHER_TX_MODES,W(5),W(100),TS2000_MAINVFO},
    {MHz(14),kHz(14350),TS2000_AM_TX_MODES,W(5),W(25),TS2000_MAINVFO},
    {kHz(18068),kHz(18168),TS2000_OTHER_TX_MODES,W(5),W(100),TS2000_MAINVFO},
    {kHz(18068),kHz(18168),TS2000_AM_TX_MODES,W(5),W(25),TS2000_MAINVFO},
    {MHz(21),kHz(21450),TS2000_OTHER_TX_MODES,W(5),W(100),TS2000_MAINVFO},
    {MHz(21),kHz(21450),TS2000_AM_TX_MODES,W(5),W(25),TS2000_MAINVFO},
    {kHz(24890),kHz(24990),TS2000_OTHER_TX_MODES,W(5),W(100),TS2000_MAINVFO},
    {kHz(24890),kHz(24990),TS2000_AM_TX_MODES,W(5),W(25),TS2000_MAINVFO},
    {MHz(28),kHz(29700),TS2000_OTHER_TX_MODES,W(5),W(100),TS2000_MAINVFO},
    {MHz(28),kHz(29700),TS2000_AM_TX_MODES,W(5),W(25),TS2000_MAINVFO},
    {MHz(50),MHz(54),TS2000_OTHER_TX_MODES,W(5),W(100),TS2000_MAINVFO},
    {MHz(50),MHz(54),TS2000_AM_TX_MODES,W(5),W(25),TS2000_MAINVFO},
    {MHz(144),MHz(148),TS2000_OTHER_TX_MODES,W(5),W(100),TS2000_MAINVFO},
    {MHz(144),MHz(148),TS2000_AM_TX_MODES,W(5),W(25),TS2000_MAINVFO},
    {MHz(430),MHz(450),TS2000_OTHER_TX_MODES,W(5),W(50),TS2000_MAINVFO},
    {MHz(430),MHz(450),TS2000_AM_TX_MODES,W(5),W(12.5),TS2000_MAINVFO},
	RIG_FRNG_END,
  }, /* tx range */
tuning_steps: {
	 {TS2000_ALL_MODES,50},
	 {TS2000_ALL_MODES,100},
	 {TS2000_ALL_MODES,kHz(1)},
	 {TS2000_ALL_MODES,kHz(5)},
	 {TS2000_ALL_MODES,kHz(9)},
	 {TS2000_ALL_MODES,kHz(10)},
	 {TS2000_ALL_MODES,12500},
	 {TS2000_ALL_MODES,kHz(20)},
	 {TS2000_ALL_MODES,kHz(25)},
	 {TS2000_ALL_MODES,kHz(100)},
	 {TS2000_ALL_MODES,MHz(1)},
	 {TS2000_ALL_MODES,0},	/* any tuning step */
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
priv: (void *)&ts2000_priv_caps,

pm: NULL,
vfo_all: RIG_VFO_VALID,

/* ts2000 */
//set_tone: ts2000_set_tone,
//get_tone: ts2000_get_tone,
set_ctcss: ts2000_set_ctcss,
get_ctcss: ts2000_get_ctcss,
get_dcd: ts2000_get_dcd,
set_freq: ts2000_set_freq,
get_freq: ts2000_get_freq,
get_func: ts2000_get_func,
set_func: ts2000_set_func,
get_info: ts2000_get_info,
get_level: ts2000_get_level,
set_level: ts2000_set_level,
get_mem: ts2000_get_mem,
set_mem: ts2000_set_mem,
get_mode: ts2000_get_mode,
set_mode: ts2000_set_mode,
get_powerstat: ts2000_get_powerstat,
set_powerstat: ts2000_set_powerstat,
get_ptt: ts2000_get_ptt,
set_ptt: ts2000_set_ptt,
reset: ts2000_reset,
send_morse: ts2000_send_morse,
get_trn: ts2000_get_trn,
set_trn: ts2000_set_trn,
/*
*/
set_vfo: ts2000_set_vfo,
get_vfo: ts2000_get_vfo,
vfo_op: ts2000_vfo_op,
/*
 * stuff I've written--kd7eni
 */

scan:		ts2000_scan,
get_channel:	ts2000_get_channel,
set_channel:	ts2000_set_channel,
get_dcs:	ts2000_get_dcs,
set_dcs:	ts2000_set_dcs,
get_parm:	ts2000_get_parm,
set_parm:	ts2000_set_parm,
get_rit:	ts2000_get_rit,
set_rit:	ts2000_set_rit,
get_rptr_offs:	ts2000_get_rptr_offs,
set_rptr_offs:	ts2000_set_rptr_offs,
get_rptr_shift:	ts2000_get_rptr_shift,
set_rptr_shift:	ts2000_set_rptr_shift,
get_split:	ts2000_get_split,
set_split:	ts2000_set_split,
get_split_freq:	ts2000_get_split_freq,
set_split_freq:	ts2000_set_split_freq,
get_split_mode:	ts2000_get_split_mode,
set_split_mode:	ts2000_set_split_mode,
get_ts:		ts2000_get_ts,
set_ts:		ts2000_set_ts,
get_xit:	ts2000_get_xit,
set_xit:	ts2000_set_xit,

/* comming soon... */
//get_tone_sql:	ts2000_get_tone_sql,
//set_tone_sql:	ts2000_set_tone_sql,
//decode_event:	ts2000_decode_event,	/* highest */
//get_conf:	ts2000_get_conf,
//set_conf:	ts2000_set_conf,
//get_ant:	ts2000_get_ant,
//set_ant:	ts2000_set_ant,
//recv_dtmf:	ts2000_recv_dtmf,		/* possible? */
//get_ctcss_sql:	ts2000_get_ctcss_sql,
//set_ctcss_sql:	ts2000_set_ctcss_sql,
//get_dcs_sql:	ts2000_get_dcs_sql,
//set_dcs_sql:	ts2000_set_dcs_sql,
//send_dtmf:	ts2000_send_dtmf,		/* lowest */

/* and never... */
//set_bank:				/* not needed */
/*
  end ts2000
 */
};

/*
 * Function definitions below
 */


/*
 *  Hamlib Kenwood backend - TS2000 description
 *  Copyright (c) 2000-2002 by Stephane Fillod
 * (C) Copyright 2002 by Dale E. Edmons (KD7ENI)
 *
 *		$Id: ts2000.c,v 1.9.2.4 2003-02-25 03:58:50 dedmons Exp $
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
 * This code is has been substantially altered from the original
 * author's.  Also, many new functions have been added and are
 * (C) Copyrighted 2002 by Dale E. Edmons (KD7ENI).  The license
 * is unchanged, and fitness disclaimers still apply.  The file
 * kenwwood.c was the original source.		--Dale
 */

/*
 * I just read in kenwood.c and will modify the functions here.  This way,
 * kenwood functions that actually work on other rigs won't be broken by
 * my hacks.  Anything that works for all can be sent back over to kenwood.c
 *
 * Note: due to my compulsive laziness, I often abbreviate Kenwood TS-2000
 *	as simply ts2k, especially for code!
 *
 * Dale kd7eni
 */

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

/* Moved all const stuff to ts2000.h where it wants to be. */
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
ts2000_transaction(RIG * rig, const char *cmdstr, int cmd_len,
		 char *data, size_t * datasize)
{
//       return kenwood_transaction(rig, cmdstr, cmd_len, data, datasize);

	struct rig_state *rs;
	int retval;
	const char *cmdtrm = EOM_KEN;	/* Default Command/Reply termination char */
	int retry_read = 0;
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
	/* FIXME : TS-2000 gets alot of 'timedout' on read_string()! */
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

/*
 * kenwood_set_vfo
 * Assumes rig!=NULL
 *
 *	status:		VFOA, VFOB, VFOC, Main, Sub,
 *			MEMA, MEMC, CALLA, CALLC 
 *			VFO_AB, VFO_BA, ...
 *		They all work!		--Dale
 */
int ts2000_set_vfo(RIG * rig, vfo_t vfo)
{
	rig_debug(RIG_DEBUG_VERBOSE, __FUNCTION__
		  ": vfo = %s\n", strvfo(vfo));

// Tesing new function without replacing old stuff!
	return ts2000_uniq_SwitchVfo(rig, vfo);
}

/*
 * ts2000_get_vfo
 *
 *	status:	works perfect for all modes!  --Dale
 *		Completely rewritten.
 *		TS2000 only
 */
int ts2000_get_vfo(RIG * rig, vfo_t * vfo)
{
// the following is the new vfo function
	return ts2000_uniq_GetVfo(rig, vfo);
}

/*
 * kenwood_set_freq
 * Assumes rig!=NULL
 *
 *	status:	correctly sets FA, FB, FC	--Dale
 *		Cannot handle anything but VFOA-C.  Broken!
 *		Being rewritten.
 */
int ts2000_set_freq(RIG * rig, vfo_t vfo, freq_t freq)
{
	unsigned char freqbuf[16];
	int freq_len, ack_len = 0, retval, spcl, ptt_ctrl, scan;
	char vfo_letter;
	TS2K_FA_T fp;

	if (vfo == RIG_VFO_CURR) {
		retval = rig_get_vfo(rig, &vfo);
		if (retval != RIG_OK)
			return retval;
	}

	if(vfo == RIG_VFO_VFO) {
		retval = ts2000_set_basic(rig, RIG_VFO_VFO);
	}

	// Set parameter to cmd function
	fp.p1 = freq;

	// Save PTT/CTRL bits as an int
	ptt_ctrl = (int) (vfo & (RIG_VFO_PTT | RIG_VFO_CTRL));
	// Mask out PTT/CTRL for easier handling
	vfo = vfo & ~(RIG_VFO_PTT | RIG_VFO_CTRL);

	/* Special Cases */
	spcl = (vfo & RIG_CTRL_SAT);
	spcl |= (vfo & RIG_CTRL_MEM);

	/* VFO scan is just like a vfo except it changes! */
	scan = (vfo & RIG_CTRL_SCAN)
		& !(vfo & RIG_CTRL_MEM) & !(vfo & RIG_CTRL_SAT);

	if(!spcl || scan) {
		/* We just mask scan and blaze forward */
		vfo = vfo & ~RIG_CTRL_SCAN;

		switch (vfo) {
		case RIG_VFO_A:
//		case RIG_VFO_AB:
//			vfo_letter = 'a';
			retval = ts2k_s_fa(rig, &fp);
			break;
		case RIG_VFO_B:
//		case RIG_VFO_BA:
			retval = ts2k_s_fb(rig, &fp);
//			vfo_letter = 'b';
			break;
		case RIG_VFO_C:
			retval = ts2k_s_fc(rig, &fp);
//			vfo_letter = 'c';
			break;

		/* Any other VFO minor is caught here */
		default:
			rig_debug(RIG_DEBUG_VERBOSE, __FUNCTION__
				  ": unsupported VFO = %s\n", strvfo(vfo));
			return -RIG_EINVAL;
		}
//		freq_len = sprintf(freqbuf, "f%c%011u;", vfo_letter, (unsigned) freq);

//		ack_len = 14;
//		retval = ts2000_transaction(rig, freqbuf, freq_len, NULL, NULL);
	} else {
		if(vfo & RIG_CTRL_SAT) {	// Sat before MEM!
		/* Note: The memory BUG below doesn't apply in SAT mode.  It
		 *	seems when they got here it wasn't Monday or Friday!
		 */
		
		} else if(vfo & RIG_CTRL_MEM) {
		/* TS-2000 BUG!  You know that great BIG frequency tuning knob
		 *	on the front of the rig?  It seems Kenwood forgot to
		 *	include the ability to turn this knob.  "bu;", isn't 
		 *	it; "ru;" isn't it; "ch0;" isn't it.  There is not a
		 *	bloody command to control the knob!  The direct entry
		 *	panel is simulated by "fa;", "fb;", "fc;" but we can't
		 *	turn the knob.  The only real problem this creates is
		 *	when doing temporary frequency changes in memory mode.
		 *	Thus, we disallow it completely, even though you can
		 *	do it on the front panel!  There is no "fa;" equivalent
		 *	for temporary memory changes so none are allowed!
		 *
		 *	Sat must be checked first because it may be in mem mode
		 *	also.
		 */
			retval = -RIG_ENAVAIL;	// But, should be!
		}
		retval = -RIG_EINTERNAL;	// Something bad is wrong.
	}

	return retval;
}

/*
 * kenwood_get_freq
 * Assumes rig!=NULL, freq!=NULL
 *
 *	status:	100% rewritten	(except comment, func name)
 */
int ts2000_get_freq(RIG * rig, vfo_t vfo, freq_t * freq)
{
	char tmp[20], ack[40];
	int retval, acklen, getvfo;
	vfo_t vtmp;
//      const struct rig_caps *caps;

	/*
	 * new version: handles more cases
	 */

//      caps = rig->caps;

	rig_debug(RIG_DEBUG_VERBOSE, __FUNCTION__
		  ": checking rig's vfo = 0x%X, mask = 0x%X\n", vfo,
		  RIG_VFO_TEST(vfo));
//      if( RIG_VFO_TEST(vfo) || (vfo & ~*caps->vfo_all))       // check caps?
	if (!RIG_VFO_TEST(vfo))
		return -RIG_EINVAL;

	getvfo = (RIG_VFO_CURR == vfo) \
	    | (RIG_VFO_VFO == vfo) | (RIG_VFO_MEM == vfo);

	if (getvfo) {
		rig_debug(RIG_DEBUG_VERBOSE, __FUNCTION__ ": getting exact vfo\n");
		retval = rig_get_vfo(rig, &vtmp);
		if (retval != RIG_OK) {
			rig_debug(RIG_DEBUG_ERR, __FUNCTION__
				": error getting rig's vfo\n");
			return retval;
		}
		vfo = vtmp;	// change the Arg on the Stack
		// removing previous line is a wicked bug to fix
	}
	rig_debug(RIG_DEBUG_VERBOSE, __FUNCTION__ ": vfo = %s\n", strvfo(vfo));
	rig_debug(RIG_DEBUG_VERBOSE, __FUNCTION__ ": vtmp = %s\n", strvfo(vtmp));

// FIXME:  The following hack removes the CTRL/PTT bits 'cause nothing's written for 'em
	vfo = vfo & ~(RIG_VFO_CTRL | RIG_VFO_PTT);
	vtmp = vtmp & ~(RIG_VFO_CTRL | RIG_VFO_PTT);

// TODO: How do we handle scans?  Right now we abort
	rig_debug(RIG_DEBUG_VERBOSE, __FUNCTION__
		": checking scan... (vfo & RIG_CTRL_SCAN) = %0x\n",
			vfo & RIG_CTRL_SCAN);
	if(vfo & RIG_CTRL_SCAN)
		return -RIG_EINVAL;	// Can't hit a moving target yet!

	rig_debug(RIG_DEBUG_VERBOSE, __FUNCTION__ ": setting tmp vfo\n");
	retval = rig_set_vfo(rig, vfo);
	if (retval != RIG_OK)
		return retval;

	/* the following switch() uses default:, others are error or special */

	switch (vfo) {
		/* Direct mem access not allowed since we don't have the channel# */
	case RIG_CTRL_MEM:
		retval = -RIG_EINVAL;	// VFO is valid, so caller is wrong!
		break;

		/* since we temporarily change vfo most everything works here */
	default:
		acklen = 40;
		retval = ts2000_transaction(rig, "if;", 3, ack, &acklen);
		if (retval == RIG_OK)
			*freq = (int) int_n(tmp, &ack[2], 11);
		else
			return retval;
		break;
	}

	/* restore original VFO */

	rig_debug(RIG_DEBUG_VERBOSE, __FUNCTION__ ": setting orig vfo\n");

	rig_set_vfo(rig, vtmp);	// Error not detected here!

	return retval;		// but previous errors are here!
}

/*
 * kenwood_set_mode
 * Assumes rig!=NULL
 */
int ts2000_set_mode(RIG * rig, vfo_t vfo, rmode_t mode, pbwidth_t width)
{
	unsigned char mdbuf[16];
	int mdbuf_len, ack_len = 0, kmode, retval;

	switch (mode) {
	case RIG_MODE_CW:
		kmode = MD_CW;
		break;
	case RIG_MODE_USB:
		kmode = MD_USB;
		break;
	case RIG_MODE_LSB:
		kmode = MD_LSB;
		break;
	case RIG_MODE_FM:
		kmode = MD_FM;
		break;
	case RIG_MODE_AM:
		kmode = MD_AM;
		break;
	case RIG_MODE_RTTY:
		kmode = MD_FSK;
		break;
	default:
		rig_debug(RIG_DEBUG_ERR, "ts2000_set_mode: "
			  "unsupported mode %u\n", mode);
		return -RIG_EINVAL;
	}
	mdbuf_len = sprintf(mdbuf, "MD%c;", kmode);
	ack_len = 16;
	rig_debug(RIG_DEBUG_ERR, "ts2000_set_mode: sending %s\n", mdbuf);
	retval = ts2000_transaction(rig, mdbuf, mdbuf_len, NULL, NULL);

	return retval;
}

/*
 * kenwood_get_mode
 * Assumes rig!=NULL, mode!=NULL
 */
int ts2000_get_mode(RIG * rig, vfo_t vfo, rmode_t * mode, pbwidth_t * width)
{
	unsigned char modebuf[50];
	int mode_len, retval;


	mode_len = 50;
	retval = ts2000_transaction(rig, "MD;", 3, modebuf, &mode_len);
	if (retval != RIG_OK)
		return retval;

	if (mode_len != 4 || modebuf[1] != 'D') {
		rig_debug(RIG_DEBUG_ERR, __FUNCTION__
			  ": unexpected answer, len=%u\n", mode_len);
		return -RIG_ERJCTED;
	}

	*width = RIG_PASSBAND_NORMAL;	/* FIXME */
	switch (modebuf[2]) {
	case MD_CW:
		*mode = RIG_MODE_CW;
		break;
	case MD_USB:
		*mode = RIG_MODE_USB;
		break;
	case MD_LSB:
		*mode = RIG_MODE_LSB;
		break;
	case MD_FM:
		*mode = RIG_MODE_FM;
		break;
	case MD_AM:
		*mode = RIG_MODE_AM;
		break;
	case MD_FSK:
		*mode = RIG_MODE_RTTY;
		break;
#ifdef RIG_MODE_CWR
	case MD_CWR:
		*mode = RIG_MODE_CWR;
		break;
#endif
#ifdef RIG_MODE_RTTYR
	case MD_FSKR:
		*mode = RIG_MODE_RTTY;
		break;
#endif
	case MD_NONE:
		*mode = RIG_MODE_NONE;
		break;
	default:
		rig_debug(RIG_DEBUG_ERR, __FUNCTION__ ": "
			  "unsupported mode '%c'\n", modebuf[2]);
		return -RIG_EINVAL;
	}

	return RIG_OK;
}

/* added a whole buch of stuff --Dale */
int ts2000_set_level(RIG * rig, vfo_t vfo, setting_t level, value_t val)
{
	unsigned char levelbuf[16], ackbuf[16], ctrl;
	char dc[10], m, s;
	int dc_len;
	int level_len, ack_len = 0, retval, reply;
	int ts2000_val;

	if (RIG_LEVEL_IS_FLOAT(level))
		ts2000_val = val.f * 255;
	else
		ts2000_val = val.i;

	if (vfo == RIG_VFO_CURR) {
		if (rig_get_vfo(rig, &vfo) != RIG_OK)
			return -RIG_EINVAL;
	}
/* Several commands need to know status of CTRL */

	dc_len = 10;
	if (ts2000_get_ctrl(rig, dc, (int) 10) != RIG_OK)
		return -RIG_EINVAL;

	m = dc[2];
	s = dc[3];		// save values

	ctrl = '0';		// Assume Main
	if (vfo & RIG_CTRL_SUB)	// Change only if sub (bitwise, not logical)
		ctrl = '1';

	// assume no replies
	reply = 0;
	switch (level) {
	case RIG_LEVEL_RFPOWER:
		level_len = sprintf(levelbuf, "pc%03d;", ts2000_val);
		break;

	case RIG_LEVEL_AF:
		level_len = sprintf(levelbuf, "ag%c%03u;", ctrl, ts2000_val);
		break;

	case RIG_LEVEL_RF:
		level_len = sprintf(levelbuf, "rg%03d;", ts2000_val);
		break;

	case RIG_LEVEL_SQL:	// fixed: uses main/sub --Dale
		level_len = sprintf(levelbuf, "sq%c%03u;", ctrl, ts2000_val);
		break;


		/* I added the following levels --Dale */
	case RIG_LEVEL_PREAMP:
		level_len =
		    sprintf(levelbuf, "pa%c;",
			    (ts2000_val == 0) ? '0' : '1');
		break;

	case RIG_LEVEL_ATT:
		if (ts2000_val < 1 || ts2000_val > 2)	// only 1 or 2, 0=read
			return -RIG_EINVAL;
		level_len = sprintf(levelbuf, "an%01u;", ts2000_val);
		break;

	case RIG_LEVEL_NR:
		if (ts2000_val < 0 || ts2000_val > 2)	// only 1 or 2, 0=read
			return -RIG_EINVAL;
		level_len = sprintf(levelbuf, "nr%01u;", ts2000_val);
		break;

		/* FIXME: FM mic gain is low/mid/high; cmd="ex0410000n;" 0=low --Dale */
	case RIG_LEVEL_MICGAIN:
		level_len =
		    sprintf(levelbuf, "mg%03u;",
			    (ts2000_val > 100) ? 100 : ts2000_val);
		break;

		/* no rig error on invalid values */
	case RIG_LEVEL_KEYSPD:
		if (ts2000_val < 10 || ts2000_val > 60)	// only 1 or 2, 0=read
			return -RIG_EINVAL;
		level_len = sprintf(levelbuf, "ks%03u;", ts2000_val);
		break;

	case RIG_LEVEL_NOTCHF:	// actually, this is autonotch--same thing?
		level_len =
		    sprintf(levelbuf, "al%03u;",
			    (ts2000_val > 4) ? 4 : ts2000_val);
		break;

	case RIG_LEVEL_AGC:
		level_len =
		    sprintf(levelbuf, "gt%03u;",
			    (ts2000_val > 20) ? 20 : ts2000_val);
		break;

	case RIG_LEVEL_BKINDL:	// 0-1000ms in 50ms steps; this'll be good 'nuff.
		level_len =
		    sprintf(levelbuf, "sd%04u;",
			    (int) ((float) ts2000_val * 1000.0 / 255.0));
		break;

	case RIG_LEVEL_VOXGAIN:
		level_len =
		    sprintf(levelbuf, "vg%03u;",
			    (ts2000_val > 9) ? 9 : ts2000_val);
// alternate
//                  level_len = sprintf(levelbuf, "vg%03u;",(int)((float) ts2000_val*9.0/255.0));
		break;

//       case RIG_LEVEL_VOX:    // header file declares these the same
	case RIG_LEVEL_VOXDELAY:
		level_len =
		    sprintf(levelbuf, "vg%03u;",
			    (int) ((float) ts2000_val * 3000.0 / 255.0));
		break;
// vox on/off
//                  level_len = sprintf(levelbuf, "vx%c;", (ts2000_val==0)? '0' : '1');
//                  break;

		/* Check unsupported just so we can complain if one is found.  :) */
	case RIG_LEVEL_APF:

		/* readonly */
	case RIG_LEVEL_SWR:
	case RIG_LEVEL_ALC:
	case RIG_LEVEL_STRENGTH:
		return -RIG_EINTERNAL;	// and complain loud!

		/* not currently implemented */
	case RIG_LEVEL_METER:	// readonly + needs rechecked.
	case RIG_LEVEL_PBT_IN:	// nnn
	case RIG_LEVEL_PBT_OUT:	// NNN; "plnnnNNN;"
	case RIG_LEVEL_IF:	// hmmmm....
	case RIG_LEVEL_CWPITCH:	// "ex0310000n;" and a fair bit of programming...
	case RIG_LEVEL_COMP:	// available?, processor's not the same is it?
	case RIG_LEVEL_BALANCE:	// AF R/L balance?  See "ex01600002;" for similar(?)
	case RIG_LEVEL_SQLSTAT:	// need more info; avail?
		return -RIG_ENIMPL;

	default:
		rig_debug(RIG_DEBUG_ERR,
			  "Unsupported set_level %u", level);
		return -RIG_EINVAL;
	}
	// The following might make a nifty macro         --Dale
	ack_len = reply ? 16 : 0;
	retval = ts2000_transaction(rig, levelbuf, level_len,
				  reply ? ackbuf : NULL,
				  reply ? &ack_len : NULL);

	if (retval != RIG_OK)
		return retval;

	// restore CTRL (not currently needed)
//      retval = ts2000_set_ctrl(dc);
//      if(retval != RIG_OK)
//              return retval;


	return retval;
}

/* 
 * assumes f!=NULL
 */
static int
get_ts2000_level(RIG * rig, const char *cmd, int cmd_len, float *f)
{
	unsigned char lvlbuf[50];
	int lvl_len, retval;
	int lvl;

	lvl_len = 50;
	retval = ts2000_transaction(rig, cmd, cmd_len, lvlbuf, &lvl_len);
	if (retval != RIG_OK)
		return retval;

	if (lvl_len != 6) {
		rig_debug(RIG_DEBUG_ERR, __FUNCTION__
			  ": wrong answer len=%u\n", lvl_len);
		return -RIG_ERJCTED;
	}

	/*
	 * 000..255
	 */
	sscanf(lvlbuf + 2, "%u", &lvl);
	*f = (float) lvl / 255.0;

	return RIG_OK;
};


/*
 * kenwood_get_level
 * Assumes rig!=NULL, val!=NULL
 */
int ts2000_get_level(RIG * rig, vfo_t vfo, setting_t level, value_t * val)
{
	unsigned char lvlbuf[50];
	int lvl_len, retval;
	int lvl;
	int i;

	lvl_len = 50;
	/* Optimize:
	 *   sort the switch cases with the most frequent first
	 */
	switch (level) {
	case RIG_LEVEL_STRENGTH:
		// fixme: "sm0;" VFO must be specified!  --kd7eni
		retval = ts2000_transaction(rig, "SM;", 3, lvlbuf, &lvl_len);
		if (retval != RIG_OK)
			return retval;

		if (lvl_len != 7 || lvlbuf[1] != 'M') {
			rig_debug(RIG_DEBUG_ERR, __FUNCTION__ ": "
				  "wrong answer len=%u\n", lvl_len);
			return -RIG_ERJCTED;
		}

		/*
		 * Frontend expects:
		 *    -54 = S0
		 *                  0 = S9
		 */
		sscanf(lvlbuf + 2, "%u", &val->i);
		val->i = (val->i * 4) - 54;
		break;

	case RIG_LEVEL_SQLSTAT:
		return -RIG_ENIMPL;	/* get_dcd ? */

	case RIG_LEVEL_PREAMP:
		return -RIG_ENIMPL;

	case RIG_LEVEL_ATT:
		retval = ts2000_transaction(rig, "RA;", 3, lvlbuf, &lvl_len);
		if (retval != RIG_OK)
			return retval;

		if (lvl_len != 5) {
			rig_debug(RIG_DEBUG_ERR, __FUNCTION__ ": "
				  "unexpected answer len=%u\n", lvl_len);
			return -RIG_ERJCTED;
		}

		sscanf(lvlbuf + 2, "%u", &lvl);
		if (lvl == 0) {
			val->i = 0;
		} else {
			for (i = 0; i < lvl && i < MAXDBLSTSIZ; i++)
				if (rig->state.attenuator[i] == 0) {
					rig_debug(RIG_DEBUG_ERR,
						  __FUNCTION__ ": "
						  "unexpected att level %u\n",
						  lvl);
					return -RIG_EPROTO;
				}
			if (i != lvl)
				return -RIG_EINTERNAL;
			val->i = rig->state.attenuator[i - 1];
		}
		break;
	case RIG_LEVEL_RFPOWER:
		return get_ts2000_level(rig, "PC;", 3, &val->f);

	case RIG_LEVEL_AF:
		return get_ts2000_level(rig, "AG;", 3, &val->f);

	case RIG_LEVEL_RF:
		return get_ts2000_level(rig, "RG;", 3, &val->f);

	case RIG_LEVEL_SQL:
		return get_ts2000_level(rig, "SQ;", 3, &val->f);

	case RIG_LEVEL_MICGAIN:
		return get_ts2000_level(rig, "MG;", 3, &val->f);

	case RIG_LEVEL_AGC:
		return get_ts2000_level(rig, "GT;", 3, &val->f);

	case RIG_LEVEL_IF:
	case RIG_LEVEL_APF:
	case RIG_LEVEL_NR:
	case RIG_LEVEL_PBT_IN:
	case RIG_LEVEL_PBT_OUT:
	case RIG_LEVEL_CWPITCH:
	case RIG_LEVEL_KEYSPD:
	case RIG_LEVEL_NOTCHF:
	case RIG_LEVEL_COMP:
	case RIG_LEVEL_BKINDL:
	case RIG_LEVEL_BALANCE:
		return -RIG_ENIMPL;

	default:
		rig_debug(RIG_DEBUG_ERR,
			  "Unsupported get_level %u", level);
		return -RIG_EINVAL;
	}

	return RIG_OK;
}

/*
 *	status:	originally very broken
 *		corrected use of status
 */
int ts2000_set_func(RIG * rig, vfo_t vfo, setting_t func, int status)
{
	unsigned char fctbuf[16], ackbuf[16];
	int fct_len, ack_len = 16;

	/* Optimize:
	 *   sort the switch cases with the most frequent first
	 */
	switch (func) {
	case RIG_FUNC_NB:
		fct_len =
		    sprintf(fctbuf, "NB%c;", (status == 0) ? '0' : '1');

		return ts2000_transaction(rig, fctbuf, fct_len, NULL, NULL);

	case RIG_FUNC_ABM:
		fct_len =
		    sprintf(fctbuf, "AM%c;", (status == 0) ? '0' : '1');
		return ts2000_transaction(rig, fctbuf, fct_len, NULL, NULL);

	case RIG_FUNC_COMP:
		fct_len =
		    sprintf(fctbuf, "PR%c;", (status == 0) ? '0' : '1');
		return ts2000_transaction(rig, fctbuf, fct_len, NULL, NULL);

	case RIG_FUNC_TONE:
		fct_len =
		    sprintf(fctbuf, "TO%c;", (status == 0) ? '0' : '1');
		return ts2000_transaction(rig, fctbuf, fct_len, NULL, NULL);

	case RIG_FUNC_TSQL:
		// fixme: see ts2000.doc and follow the proceedure!  --kdeni
		// rigbug!
		fct_len =
		    sprintf(fctbuf, "CT%c;",
			    (status == RIG_FUNC_TSQL) ? '0' : '1');
		return ts2000_transaction(rig, fctbuf, fct_len,
					ackbuf, &ack_len);
	case RIG_FUNC_VOX:
		fct_len =
		    sprintf(fctbuf, "VX%c;", (status == 0) ? '0' : '1');
		return ts2000_transaction(rig, fctbuf, fct_len, NULL, NULL);
	case RIG_FUNC_NR:
		fct_len =
		    sprintf(fctbuf, "NR%c;", (status == 0) ? '0' : '1');
		return ts2000_transaction(rig, fctbuf, fct_len, NULL, NULL);
	case RIG_FUNC_BC:
		fct_len =
		    sprintf(fctbuf, "BC%c;", (status == 0) ? '0' : '1');
		return ts2000_transaction(rig, fctbuf, fct_len, NULL, NULL);
	case RIG_FUNC_ANF:
		fct_len =
		    sprintf(fctbuf, "NT%c;", (status == 0) ? '0' : '1');
		return ts2000_transaction(rig, fctbuf, fct_len, NULL, NULL);
	case RIG_FUNC_LOCK:
		fct_len =
		    sprintf(fctbuf, "LK%c0;", (status == 0) ? '0' : '1');
		return ts2000_transaction(rig, fctbuf, fct_len, NULL, NULL);
	default:
		rig_debug(RIG_DEBUG_ERR, "Unsupported set_func %#x", func);
		return -RIG_EINVAL;
	}
	return RIG_OK;
}


/* 
 * assumes status!=NULL
 * works for any 'format 1' command
 */
static int
get_ts2000_func(RIG * rig, const char *cmd, int cmd_len, int *status)
{
	unsigned char fctbuf[50];
	int fct_len, retval;
	fct_len = 50;
	retval = ts2000_transaction(rig, cmd, cmd_len, fctbuf, &fct_len);
	if (retval != RIG_OK)
		return retval;
	if (fct_len != 4) {
		rig_debug(RIG_DEBUG_ERR,
			  __FUNCTION__ ": wrong answer len=%u\n", fct_len);
		return -RIG_ERJCTED;
	}

	*status = fctbuf[2] == '0' ? 0 : 1;
	return RIG_OK;
};
/*
 * kenwood_get_func
 * Assumes rig!=NULL, val!=NULL
 */
int ts2000_get_func(RIG * rig, vfo_t vfo, setting_t func, int *status)
{
	unsigned char fctbuf[50];
	int fct_len, retval;
	fct_len = 50;
	/* Optimize:
	 *   sort the switch cases with the most frequent first
	 */
	switch (func) {
	case RIG_FUNC_FAGC:
		retval = ts2000_transaction(rig, "GT;", 3, fctbuf, &fct_len);
		if (retval != RIG_OK)
			return retval;
		if (fct_len != 6) {
			rig_debug(RIG_DEBUG_ERR,
				  __FUNCTION__ ": "
				  "wrong answer len=%u\n", fct_len);
			return -RIG_ERJCTED;
		}

		*status = fctbuf[4] != '4' ? 1 : 0;
		break;
	case RIG_FUNC_NB:
		return get_ts2000_func(rig, "NB;", 3, status);
		break;
	case RIG_FUNC_ABM:
		return get_ts2000_func(rig, "AM;", 3, status);
		break;
	case RIG_FUNC_COMP:
		return get_ts2000_func(rig, "PR;", 3, status);
		break;
	case RIG_FUNC_TONE:
		return get_ts2000_func(rig, "TO;", 3, status);
		break;
	case RIG_FUNC_TSQL:
		return get_ts2000_func(rig, "CT;", 3, status);
		break;
	case RIG_FUNC_VOX:
		return get_ts2000_func(rig, "VX;", 3, status);
		break;
	case RIG_FUNC_NR:
		return get_ts2000_func(rig, "NR;", 3, status);
		break;
		/* FIXME on TS2000 */
	case RIG_FUNC_BC:
		return get_ts2000_func(rig, "BC;", 3, status);
		break;
	case RIG_FUNC_ANF:
		return get_ts2000_func(rig, "NT;", 3, status);
		break;
	case RIG_FUNC_LOCK:
		return get_ts2000_func(rig, "LK;", 3, status);
		break;
	default:
		rig_debug(RIG_DEBUG_ERR, "Unsupported get_func %#x", func);
		return -RIG_EINVAL;
		break;
	}

	return RIG_OK;
}

/*
 * ts2000_set_ctcss()
 * Assumes rig!=NULL, rig->caps->ctcss_list != NULL
 *
 * Warning! This is untested stuff! May work at least on TS-870S
 * 	Please owners report to me <fillods@users.sourceforge.net>, thanks. --SF
 *
 * TODO: TS-2000 uses CN/CT
 *	ex057 menu is AutoPower off for TS-2000	--kd7eni
 */
int ts2000_set_ctcss(RIG * rig, vfo_t vfo, tone_t tone)
{
	return ts2000_set_Tones(rig, vfo, tone, (char) 'c');
}

int ts2000_set_tone(RIG * rig, vfo_t vfo, tone_t tone)
{
	return ts2000_set_Tones(rig, vfo, tone, (char) 't');
}

int ts2000_set_Tones(RIG * rig, vfo_t vfo, tone_t tone, const char ct)
{
	const struct rig_caps *caps;
	unsigned char tonebuf[16];
	int tone_len, ack_len = 0;
	int i;
	caps = rig->caps;
/* TODO: replace 200 by something like RIGTONEMAX */
	for (i = 0; caps->ctcss_list[i] != 0 && i < 38; i++) {
		if ((caps->ctcss_list[i] >= tone)
		    && (caps->ctcss_list[i - 1] < tone))	// at least get close
			break;
	}
	if (caps->ctcss_list[i - 1] == tone)
		i--;
	if (caps->ctcss_list[i] > tone)
		return -RIG_EINVAL;
	tone_len = sprintf(tonebuf, "%cn%02u;", ct, i + 1);
	ack_len = 16;
	return ts2000_transaction(rig, tonebuf, tone_len, NULL, NULL);
	rig_debug(RIG_DEBUG_ERR, __FUNCTION__ ": sent %s", tonebuf);
}

/*
 * ts2000_get_ctcss
 * Assumes rig!=NULL, rig->state.priv!=NULL
 */
int ts2000_get_ctcss(RIG * rig, vfo_t vfo, tone_t * tone)
{
	return ts2000_get_Tones(rig, vfo, tone, "cn;");
}

int ts2000_get_tone(RIG * rig, vfo_t vfo, tone_t * tone)
{
	return ts2000_get_Tones(rig, vfo, tone, "tn;");
}

int ts2000_get_Tones(RIG * rig, vfo_t vfo, tone_t * tone, const char *ct)
{
	const struct rig_caps *caps;
	unsigned char tonebuf[10];
	int tone_len, i, retval;
	unsigned int tone_idx;
	caps = rig->caps;
	tone_len = 10;
	retval = ts2000_transaction(rig, ct, 3, tonebuf, &tone_len);
	if (retval != RIG_OK)
		return retval;
	if (tone_len != 5) {
		rig_debug(RIG_DEBUG_ERR,
			  __FUNCTION__ ": unexpected reply "
			  "'%s', len=%u\n", tonebuf, tone_len);
		return -RIG_ERJCTED;
	}

	sscanf(tonebuf + 2, "%u", (int *) &tone_idx);
	if (tone_idx == 0) {
		rig_debug(RIG_DEBUG_ERR,
			  __FUNCTION__ ": Unexpected Tone "
			  "no (%04d)\n", tone_idx);
		return -RIG_EPROTO;
	}

	/* check this tone exists. That's better than nothing. */
	for (i = 0; i < tone_idx; i++) {
		if (caps->ctcss_list[i] == 0) {
			rig_debug(RIG_DEBUG_ERR,
				  __FUNCTION__ ": Tone NG "
				  "(%04d)\n", tone_idx);
			return -RIG_EPROTO;
		}
	}
	*tone = caps->ctcss_list[tone_idx - 1];
	return RIG_OK;
}

/*
 * kenwood_get_ptt
 * Assumes rig!=NULL, ptt!=NULL
 */
int ts2000_get_ptt(RIG * rig, vfo_t vfo, ptt_t * ptt)
{
	unsigned char infobuf[50];
	int info_len, retval;
	info_len = 50;
	retval = ts2000_transaction(rig, "IF;", 3, infobuf, &info_len);
	if (retval != RIG_OK)
		return retval;
	if (info_len != 38 || infobuf[1] != 'F') {
		rig_debug(RIG_DEBUG_ERR,
			  __FUNCTION__
			  ": wrong answer len=%u\n", info_len);
		return -RIG_ERJCTED;
	}

	*ptt = infobuf[28] == '0' ? RIG_PTT_OFF : RIG_PTT_ON;
	return RIG_OK;
}

/*
 * kenwood_set_ptt
 * Assumes rig!=NULL
 */
int ts2000_set_ptt(RIG * rig, vfo_t vfo, ptt_t ptt)
{
	return ts2000_transaction(rig,
				ptt ==
				RIG_PTT_ON ? "TX;" : "RX;", 3, NULL, NULL);}
/*
 * kenwood_get_dcd
 * Assumes rig!=NULL, dcd!=NULL
 */
int ts2000_get_dcd(RIG * rig, vfo_t vfo, dcd_t * dcd)
{
	unsigned char busybuf[50];
	int busy_len, retval;
	busy_len = 50;
	retval = ts2000_transaction(rig, "BY;", 3, busybuf, &busy_len);
	if (retval != RIG_OK)
		return retval;
	if (busy_len != 4) {
		rig_debug(RIG_DEBUG_ERR,
			  __FUNCTION__
			  ": wrong answer len=%u\n", busy_len);
		return -RIG_ERJCTED;
	}

	*dcd = (busybuf[2] == 0x01) ? RIG_DCD_ON : RIG_DCD_OFF;
	return RIG_OK;
}

/*
 * kenwood_set_trn
 * Assumes rig!=NULL
 */
int ts2000_set_trn(RIG * rig, int trn)
{
	unsigned char trnbuf[16];
	int trn_len;
	/* changed to TS-2000 --D.E. kd7eni */
	trn_len = sprintf(trnbuf, "AI%c;", trn == RIG_TRN_RIG ? '2' : '0');
	return ts2000_transaction(rig, trnbuf, trn_len, NULL, NULL);
	// No reply on "ai2;"--how quaint!
}
/*
 * kenwood_get_trn
 * Assumes rig!=NULL, trn!=NULL
 */
int ts2000_get_trn(RIG * rig, int *trn)
{
	unsigned char trnbuf[50];
	int trn_len, retval;
	trn_len = 50;
	retval = ts2000_transaction(rig, "AI;", 3, trnbuf, &trn_len);
	if (retval != RIG_OK)
		return retval;
	if (trn_len != 4) {
		rig_debug(RIG_DEBUG_ERR,
			  __FUNCTION__ ": wrong answer"
			  "len=%u\n", trn_len);
		return -RIG_ERJCTED;
	}
	*trn = trnbuf[2] != '0' ? RIG_TRN_RIG : RIG_TRN_OFF;
	return RIG_OK;
}

/*
 * kenwood_set_powerstat
 * Assumes rig!=NULL
 */
int ts2000_set_powerstat(RIG * rig, powerstat_t status)
{
	unsigned char pwrbuf[16], ackbuf[16];
	int pwr_len, ack_len = 16;
	pwr_len =
	    sprintf(pwrbuf, "PS%c;", status == RIG_POWER_ON ? '1' : '0');
	return ts2000_transaction(rig, pwrbuf, pwr_len, ackbuf, &ack_len);
}
/*
 * kenwood_get_powerstat
 * Assumes rig!=NULL, trn!=NULL
 */
int ts2000_get_powerstat(RIG * rig, powerstat_t * status)
{
	unsigned char pwrbuf[50];
	int pwr_len = 50, retval;
	// No reply when powered off, 1=power on.  Geez!
	retval = ts2000_transaction(rig, "PS;", 3, pwrbuf, &pwr_len);
	if (retval != RIG_OK)
		return retval;
	if (pwr_len != 4) {
		rig_debug(RIG_DEBUG_ERR,
			  __FUNCTION__ ": wrong answer "
			  "len=%u\n", pwr_len);
		return -RIG_ERJCTED;
	}
	*status = pwrbuf[2] == '0' ? RIG_POWER_OFF : RIG_POWER_ON;
	return RIG_OK;
}

/*
 * kenwood_reset
 * Assumes rig!=NULL
 */
int ts2000_reset(RIG * rig, reset_t reset)
{
	unsigned char rstbuf[16], ackbuf[16];
	int rst_len, ack_len = 16;
	char rst;
	switch (reset) {
	case RIG_RESET_VFO:
		rst = '1';
		break;
	case RIG_RESET_MASTER:
		rst = '2';
		break;
	default:
		rig_debug(RIG_DEBUG_ERR,
			  "ts2000_reset: unsupported reset %u\n", reset);
		return -RIG_EINVAL;
	}
	rst_len = sprintf(rstbuf, "SR%c;", rst);
	// Largely untested!    ;)       --kd7eni
	return ts2000_transaction(rig, rstbuf, rst_len, ackbuf, &ack_len);
}

/*
 * kenwood_send_morse
 * Assumes rig!=NULL
 */
int ts2000_send_morse(RIG * rig, vfo_t vfo, const char *msg)
{
	unsigned char morsebuf[30], ackbuf[16];
	int morse_len, ack_len = 0;
	int msg_len, buff_len, retval;
	const char *p;
	p = msg;
	msg_len = strlen(msg);
	while (msg_len > 0) {
		/*
		 * TODO: check with "KY;" if char buffer is available.
		 *                  if not, sleep.
		 */
		buff_len = msg_len > 24 ? 24 : msg_len;
		strcpy(morsebuf, "KY ");
		strncat(morsebuf, p, buff_len);
		strcat(morsebuf, ";");
		morse_len = 4 + buff_len;
		ack_len = 16;
		retval =
		    ts2000_transaction(rig, morsebuf, morse_len,
				     ackbuf, &ack_len);
		if (retval != RIG_OK)
			return retval;
		msg_len -= buff_len;
		p += buff_len;
	}
	return RIG_OK;
}

/*
 * kenwood_vfo_op
 * Assumes rig!=NULL
 */
int ts2000_vfo_op(RIG * rig, vfo_t vfo, vfo_op_t op)
{
	unsigned char *cmd;
	int ack_len = 0;
	switch (op) {
	case RIG_OP_UP:
		cmd = "UP;";
		break;
	case RIG_OP_DOWN:
		cmd = "DN;";
		break;
	case RIG_OP_BAND_UP:
		cmd = "BD;";
		break;
	case RIG_OP_BAND_DOWN:
		cmd = "BU;";
		break;
	default:
		rig_debug(RIG_DEBUG_ERR,
			  "ts2000_vfo_op: unsupported op %#x\n", op);
		return -RIG_EINVAL;
	}
	ack_len = 16;
	return ts2000_transaction(rig, cmd, 3, NULL, NULL);
}

/*
 * kenwood_set_mem
 * Assumes rig!=NULL
 */
int ts2000_set_mem(RIG * rig, vfo_t vfo, int ch)
{
	unsigned char membuf[16];
	int mem_len;
	/*
	 * "MCbmm;"
	 * where b is the bank number, mm the memory number.
	 * b can be a space ( *not*! manual wrong. --kd7eni
	 */
	mem_len = sprintf(membuf, "MC%03d;", ch);
	return ts2000_transaction(rig, membuf, mem_len, NULL, NULL);
}
/*
 * kenwood_get_mem
 * Assumes rig!=NULL
 */
int ts2000_get_mem(RIG * rig, vfo_t vfo,
		     int *ch)
{
	unsigned char membuf[50];
	int retval, mem_len;
	/*
	 * "MCbmm;"
	 * where b is the bank number, mm the memory number.
	 * b can be a space (*not* --kd7eni)
	 */
	mem_len = 10;
	retval = ts2000_transaction(rig, "MC;", 3, membuf, &mem_len);
	if (retval != RIG_OK)
		return retval;
	if (mem_len != 6) {
		rig_debug(RIG_DEBUG_ERR,
			  __FUNCTION__ ": wrong answer "
			  "len=%u\n", mem_len);
		return -RIG_ERJCTED;
	}
	membuf[5] = '\0';
	*ch = atoi(membuf + 2);
	return RIG_OK;
}

/*
 * kenwood_get_info
 * supposed to work only for TS2000...
 * Assumes rig!=NULL
 */
const char *ts2000_get_info(RIG * rig)
{
	unsigned char firmbuf[50];
	int firm_len, retval;
	firm_len = 50;
	retval = ts2000_transaction(rig, "TY;", 3, firmbuf, &firm_len);
	if (retval != RIG_OK)
		return NULL;
	if (firm_len != 6) {
		rig_debug(RIG_DEBUG_ERR,
			  __FUNCTION__
			  ": wrong answer len=%u\n", firm_len);
		return NULL;
	}

	switch (firmbuf[4]) {
	case '0':
		return "Firmware: Overseas type";
	case '1':
		return "Firmware: Japanese 100W type";
	case '2':
		return "Firmware: Japanese 20W type";
	default:
		return "Firmware: unknown";
	}
}

#define IDBUFSZ 16

/*
 * probe_kenwood
 */
rig_model_t probe_ts2000(port_t * port)
{
	unsigned char idbuf[IDBUFSZ];
	int id_len, i, k_id;
	int retval;
	if (!port)
		return RIG_MODEL_NONE;
	port->write_delay = port->post_write_delay = 0;
	port->timeout = 50;
	port->retry = 1;
	retval = serial_open(port);
	if (retval != RIG_OK)
		return RIG_MODEL_NONE;
	retval = write_block(port, "ID;", 3);
	id_len = read_string(port, idbuf, IDBUFSZ, EOM_KEN EOM_TH, 2);
	close(port->fd);
	if (retval != RIG_OK)
		return RIG_MODEL_NONE;
	/* 
	 * reply should be something like 'IDxxx;'
	 */
	if (id_len != 5 || id_len != 6) {
		idbuf[7] = '\0';
		rig_debug(RIG_DEBUG_VERBOSE,
			  "probe_ts2000: protocol error,"
			  " expected %u, received %u: %s\n", 6,
			  id_len, idbuf);
		return RIG_MODEL_NONE;
	}


	/* first, try ID string */
	for (i = 0; ts2000_id_string_list[i].model != RIG_MODEL_NONE; i++) {
		if (!strncmp(ts2000_id_string_list[i].id, idbuf + 2, 16)) {
			rig_debug(RIG_DEBUG_VERBOSE,
				  "probe_ts2000: " "found %s\n", idbuf + 2);
			return ts2000_id_string_list[i].model;
		}
	}

	/* then, try ID numbers */

	k_id = atoi(idbuf + 2);
	for (i = 0; ts2000_id_list[i].model != RIG_MODEL_NONE; i++) {
		if (ts2000_id_list[i].id == k_id) {
			rig_debug(RIG_DEBUG_VERBOSE,
				  "probe_ts2000: " "found %03d\n", k_id);
			return ts2000_id_list[i].model;
		}
	}
	/*
	 * not found in known table.... 
	 * update ts2000_id_list[]!
	 */
	rig_debug(RIG_DEBUG_WARN,
		  "probe_ts2000: found unknown device "
		  "with ID %03d, please report to Hamlib "
		  "developers.\n", k_id);
	return RIG_MODEL_NONE;
}

/* kenwood_init
 *
 * Basically, it sets up *priv
 * REM: serial port is already open (rig->state.rigport.fd)
 */
int ts2000_init(RIG * rig)
{
	const struct rig_caps *caps;
	const struct ts2000_priv_caps *priv_caps;
	rig_debug(RIG_DEBUG_TRACE, __FUNCTION__ ": called\n");
	if (!rig || !rig->caps)
		return -RIG_EINVAL;
	caps = rig->caps;
	if (!caps->priv)
		return -RIG_ECONF;
	priv_caps = (const struct ts2000_priv_caps *) caps->priv;
#if 0				/* No private data for Kenwood backends */
	priv = (struct ts2000_priv_data *)
	    malloc(sizeof(struct ts2000_priv_data));
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
int ts2000_cleanup(RIG * rig)
{
	rig_debug(RIG_DEBUG_TRACE, __FUNCTION__ ": called\n");
	if (!rig)
		return -RIG_EINVAL;
	if (rig->state.priv)
		free(rig->state.priv);
	rig->state.priv = NULL;
	return RIG_OK;
}

/*****************************************************************************
	Added the following functions.
 	(C) Copyright 2002 by Dale E. Edmons.   All rights Reserved.
	License:	Identical to all other Hamlib code.
*****************************************************************************/

#define CHKERR(c)	if((c) != RIG_OK) return c
#define STUFF(c)	int retval, acklen=(c); char ack[c]
//#define STUFF(c)       static int retval, acklen=(c); static char ack[c]

/*
 * ts2000_get_ctrl() ts2000 transceiver check.  Tests and returns the value of the current
 *	PTT/CTRL (using "dc;") for main and sub transceivers.  The settings are:
 *
 *	    PTT	__    __ CTRL	'0' = main; '1' = sub
 *		   \ /
 *		"dc00;"	PTT && CTRL both on main
 *		"dc01;"	PTT on main; CTRL on sub 
 *		"dc10;"	PTT on sub; CTRL both on main
 *		"dc11;"	PTT && CTRL both on sub
 *
 *	status:	obsolete, use ts2000_uniq_GetCtrl()
 */
int ts2000_get_ctrl(RIG * rig, char *dc_buf, int dc_len)	// use this when static removed.
{
	int retval;

	rig_debug(RIG_DEBUG_VERBOSE, __FUNCTION__": getting PTT/CTRL bytes\n");

	retval = ts2000_transaction(rig, "dc;", 3, dc_buf, &dc_len);
	if (retval != RIG_OK) {
		rig_debug(RIG_DEBUG_VERBOSE,
			  __FUNCTION__
			  ": error: retval=%u, dc_buf=%s\n",
			  retval, dc_buf); return retval;
	}
	rig_debug(RIG_DEBUG_VERBOSE, __FUNCTION__
			": returning %u PTT/CTRL bytes\n", dc_len);

	return RIG_OK;
}

/* NOTE: PTT/CTRL enable is set only if ptt != 0 (or ctrl) */
int ts2000_set_ctrl(RIG * rig, int ptt, int ctrl)
{
	int retval, acklen;
	char buf[20];
	retval = ts2000_get_ctrl(rig, buf, (int) 20);
	if (retval != RIG_OK) {
		rig_debug(RIG_DEBUG_VERBOSE, __FUNCTION__ \
			  ": returned NULL!\n");
		return -RIG_EINVAL;
	}
	rig_debug(RIG_DEBUG_VERBOSE,
		  __FUNCTION__ ": curr='%s',"
		  "ptt=%u, ctrl=%u\n", buf, ptt, ctrl);
	if (ptt != 0)
		buf[2] = (TS2000_PTT_ON_SUB == ptt) ? '1' : '0';
	if (ctrl != 0)
		buf[3] = (TS2000_CTRL_ON_SUB == ctrl) ? '1' : '0';
	buf[4] = ';';
	buf[5] = '\0';		// just for printing debug
	acklen = 10;
	retval = ts2000_transaction(rig, buf, 5, NULL, NULL);
	CHKERR(retval);
	return RIG_OK;
}

/*
 * FIXME: simple ascii to integer converter--expensive!
 *	Prevents trashing original string, otherwise
 *	just drop a (char)0 where you need it.
 */
int ts2000_get_int(char *src, int cnt)
{
	static char buf[20];
	strncpy(buf, src, cnt);
	buf[cnt] = (char) 0;
	return atoi(buf);
}

int ts2000_get_rit(RIG * rig, vfo_t vfo, shortfreq_t * rit)
{
	int retval, acklen;
	char ack[40];
	char ctrl[20];
	retval = ts2000_get_ctrl(rig, ctrl, (int) 20);
	if (ctrl != RIG_OK)	// do we have valid "dc;" ?
		return -RIG_EINVAL;	// don't know proper errors yet!
	// sub shows main's rit!  We return 0 if subreceiver.
	if (ctrl[3] == '1') {
		*rit = 0;
		return RIG_OK;	// not really, but it's paid for...
	}
	retval = ts2000_transaction(rig, "if;", 3, ack, &acklen);
	CHKERR(retval);
	ack[23] = (char) 0;
	*rit = atoi(&ack[17]);
	return RIG_OK;
}

/*
 * ts2000_get_xit()  We just call ts2000_get_rit()
 * On the ts2000, they're the same.  The rig
 * acts this way.
*/
int ts2000_get_xit(RIG * rig, vfo_t vfo, shortfreq_t * rit)
{
	return ts2000_get_rit(rig, vfo, rit);
}

int ts2000_set_rit(RIG * rig, vfo_t vfo, shortfreq_t rit)
{
	char buf[40], c;
	int retval, i, len;
	// Clear current rit/xit.
	retval = ts2000_transaction(rig, "rc;", 3, NULL, NULL);
	CHKERR(retval);
	// Execute up/down request.
	if (rit > 0) {
		c = 'u';
		i = rit;
	} else {
		c = 'd';
		i = -rit;
	}

	len = sprintf(buf, "r%c%6d;", c, i);
	return ts2000_transaction(rig, buf, len, NULL, NULL);
}

int ts2000_set_xit(RIG * rig, vfo_t vfo, shortfreq_t rit)
{
	return ts2000_set_rit(rig, vfo, rit);
}

int ts2000_get_ts(RIG * rig, vfo_t vfo, shortfreq_t * ts)
{
	STUFF(10);
	int m, s;
	retval = ts2000_transaction(rig, "ST;", 5, ack, &acklen);
	CHKERR(retval);
	ack[4] = '\0';
	s = atoi(&ack[2]);
	rig_debug(RIG_DEBUG_VERBOSE,
		  __FUNCTION__ ": received: '%s', %u\n", ack, s);
	retval = ts2000_transaction(rig, "MD;", 5, ack, &acklen);
	CHKERR(retval);
	// fm or am mode selects 1
	m = (ack[2] == '4' || ack[2] == '5') ? 1 : 0;
	rig_debug(RIG_DEBUG_VERBOSE,
		  __FUNCTION__ ": received: '%s', %u\n", ack, m);
	*ts = ts2000_steps[m][s];
	return RIG_OK;
}

// FIXME: should get nearest, not easiest.  :)
/*
 *	status:	working.  fixed timeout.	--Dale
 */
int ts2000_set_ts(RIG * rig, vfo_t vfo, shortfreq_t ts)
{
	STUFF(10);
	char st[10];
	int m, s, k;
	long int diff[2];
	retval = ts2000_transaction(rig, "md;", 5, ack, &acklen);
	CHKERR(retval);
	// fm or am mode selects 1
	m = (ack[2] == '4' || ack[2] == '5') ? 1 : 0;
	// fm or am selects 10 step freq. else 4.
	k = (m) ? 10 : 4;
	s = 0;
	if (ts < ts2000_steps[m][0]) {
		k = s;
	}

	if (ts > ts2000_steps[m][9]) {
		s = 9;
	}

	for (s = 1; s < k; s++) {
		if (ts2000_steps[m][s - 1] <= ts && ts <= ts2000_steps[m][s]) {
			diff[0] = ts - ts2000_steps[m][s - 1];
			diff[1] = ts2000_steps[m][s] - ts;
			if (diff[0] > diff[1]) {	// closer to [s]
				break;
			} else {	// closer to [s-1]
				s--;
				break;
			}
		}
	}
	// m is tmp now
	m = sprintf(st, "st0%1u;", s);
	// no reply!
	retval = ts2000_transaction(rig, st, m, NULL, NULL);
	CHKERR(retval);
	return RIG_OK;
}

/* Rig truncates in kHz(50) steps, so we don't. */
int ts2000_get_rptr_offs(RIG * rig, vfo_t vfo, shortfreq_t * rptr_offs)
{
	STUFF(20);
	retval = ts2000_transaction(rig, "of;", 3, ack, &acklen);
	CHKERR(retval);
	if (ack[0] != 'O' || ack[1] != 'F')
		return -RIG_EINVAL;
	if (acklen != 12)
		return -RIG_EINVAL;
	ack[11] = '\0';
	*rptr_offs = atoi(&ack[2]);
	return RIG_OK;
}

/* Rig truncates in kHz(50) steps, so we don't. */
int ts2000_set_rptr_offs(RIG * rig, vfo_t vfo, shortfreq_t rptr_offs)
{
	int retval, i;
	char buf[20];
	i = sprintf(buf, "of%09u;", (unsigned int) rptr_offs);
	retval = ts2000_transaction(rig, buf, i, NULL, NULL);
	CHKERR(retval);
	return RIG_OK;
}

/*
 *	status:	working, leaves vfo intact.
 */
int ts2000_get_rptr_shift(RIG * rig, vfo_t vfo, rptr_shift_t * rptr_shift)
{
	STUFF(20);
	vfo_t vfo_tmp;
	// FIXME: I don't know if I should change back to currVFO, but I do.
	retval = rig_get_vfo(rig, &vfo_tmp);
	CHKERR(retval);
	retval = ts2000_transaction(rig, "os;", 3, ack, &acklen);
	CHKERR(retval);
	retval = ts2000_vfo_ctrl(rig, vfo);
	if (ack[0] != 'O' || ack[1] != 'S')
		return -RIG_EINVAL;
	//if(acklen != 4) return -RIG_EINVAL;
	switch (ack[2]) {
	case '0':
		*rptr_shift = RIG_RPT_SHIFT_NONE;
		break;
	case '1':
		*rptr_shift = RIG_RPT_SHIFT_MINUS;
		break;
	case '2':
		*rptr_shift = RIG_RPT_SHIFT_PLUS;
		break;

// TODO: FIXME: Help!  I didn't make this up, really!
//	case '3':
//		*rptr_shift = RIG_RPT_SHIFT_1750;	// "They're comming to take me away--ha, ha!"
//		break;

	default:
		return -RIG_EINVAL;
		break;
	}

	// FIXME: I don't know if I should change back to currVFO, but I do.
	retval = ts2000_set_vfo(rig, vfo_tmp);
	CHKERR(retval);
	return RIG_OK;
}

/*
 * ts2000_set_rptr_shift()
 *
 *	status:		doesn't check VFO yet	--Dale
 */
int ts2000_set_rptr_shift(RIG * rig, vfo_t vfo, rptr_shift_t rptr_shift)
{
	STUFF(10);
	char c;
	switch (rptr_shift) {
	case RIG_RPT_SHIFT_NONE:
		c = '0';
		break;
	case RIG_RPT_SHIFT_PLUS:
		c = '1';
		break;
	case RIG_RPT_SHIFT_MINUS:
		c = '2';
		break;
	case RIG_RPT_SHIFT_1750:	// FIXME: invalid for mine! (non-Etype)
		c = '3';
		break;
	default:
		return -RIG_EINVAL;
		break;
	}
	acklen = sprintf(ack, "os%c;", c);
	retval = ts2000_transaction(rig, ack, 4, NULL, NULL);
	CHKERR(retval);
	return RIG_OK;
}

int ts2000_get_split(RIG * rig, vfo_t vfo, split_t * split)
{
	STUFF(10);
	char ack2[10];
	int ack2len = 10;
	retval = ts2000_transaction(rig, "fr;", 3, ack, &acklen);
	CHKERR(retval);
	retval = ts2000_transaction(rig, "ft;", 3, ack2, &ack2len);
	CHKERR(retval);
	if (ack[2] != ack2[2])
		*split = RIG_SPLIT_ON;
	else
		*split = RIG_SPLIT_OFF;
	return RIG_OK;
}

/* no VFO check, and no mem.  Yet. */
int ts2000_set_split(RIG * rig, vfo_t vfo, split_t split)
{
	STUFF(10);
	char ack2[10];
	int ack2len = 10;
	retval = ts2000_transaction(rig, "fr;", 3, ack, &acklen);
	CHKERR(retval);
	retval = ts2000_transaction(rig, "ft;", 3, ack2, &ack2len);
	CHKERR(retval);
	if (split == RIG_SPLIT_ON) {	// RX/TX on different vfo's
		if (ack[2] == '0')
			ack2[2] = '1';
		else if (ack[2] == '1')
			ack2[2] = '0';
		/* FIXME: mem split.  mem/vfo split must enable menu 6a
		 * with the "ex...;" or mem/vfo won't work.  A split
		 * memory operates simply by storing a memory with the
		 * freq with RX != TX then recalling it.  Don't know if
		 * if two separate memories can be used (e.g. 100/101)
		 */
		else
			return -RIG_EINVAL;
	} else {		// RX/TX on same vfo (or mem etc...)
		ack2[2] = ack[2];
	}

	// now, just send back the rig's strings :)
	retval = ts2000_transaction(rig, ack, acklen, NULL, NULL);
	CHKERR(retval);
	retval = ts2000_transaction(rig, ack2, ack2len, NULL, NULL);
	CHKERR(retval);
	return RIG_OK;
}

int ts2000_get_split_freq(RIG * rig, vfo_t vfo, freq_t * tx_freq)
{
	// FIXME : This makes too many assumptions--I'll fix it later...
	return ts2000_get_freq(rig, vfo, tx_freq);
}

int ts2000_set_split_freq(RIG * rig, vfo_t vfo, freq_t tx_freq)
{
	// FIXME : This makes too many assumptions--I'll fix it later...
	return ts2000_set_freq(rig, vfo, tx_freq);
}

/* ts2000_get_channel()
 *
 *	status:	working!  reading unset memory is an error.  this
 *		is the rig.  vfo_t is not needed for regular memory
 *		and we don't check it now.  when we start to access
 *		other memory we'll split into multiple functions
 *		each dedicated to the unique type (e.g. quick/tmp).
 *
 *		memory range not checked.
 *
 *	Note: the manual erroneously states bank should be ' ' if
 *		memory is <100.  The "correct" way is to *never*
 *		send a ' ' or you'll get a "?;" response.  Always
 *		set memory with (e.g.) "mc1020;".	--kd7eni
 */
int ts2000_get_channel(RIG * rig, channel_t * chan)
{
	char mrtxt[2][60], mrcmd[15], ack[60], tmp[20];
	int i, check, retval, mrtxt_len, mrcmd_len, ack_len, curr_mem;
	vfo_t curr_vfo;
	vfo_t vfo;
	if (chan == NULL)
		return -RIG_EINVAL;
#undef _USEVFO
#ifdef _USEVFO
	vfo = chan->vfo;
	// check the memory bit
	if (!(vfo & RIG_CTRL_MEM) && 0) {	// disabled
		rig_debug(RIG_DEBUG_ERR, __FUNCTION__
			  ": Non Memory VFO='%u', vfo=%u\n",
			  chan->vfo, vfo); return -RIG_EINVAL;	// not a channel!
	}
#endif
// get needed info if rig's mem pointers used
	rig_debug(RIG_DEBUG_ERR, __FUNCTION__ ": using rig's ptr\n");
	retval = rig_get_vfo(rig, &curr_vfo);
	CHKERR(retval);
	retval = rig_get_mem(rig, curr_vfo, &curr_mem);
	CHKERR(retval);
	mrtxt_len = ack_len = 60;
	mrcmd_len = 15;
// send request for both rx mem and tx mem
	for (i = 0; i < 2; i++) {	// 0=rx; 1=tx

		mrcmd_len =
		    sprintf(mrcmd, "mr%01d%03u;", i,
			    chan->channel_num); ack_len = 60;	// must be reset inside loop!
		retval =
		    ts2000_transaction(rig, mrcmd, mrcmd_len, ack,
				     &ack_len); CHKERR(retval);
		rig_debug(RIG_DEBUG_ERR,
			  __FUNCTION__ ": read \n\t'%s'\n", ack); ack[50] = '\0';	// May be too far, but helps.
		// watch out.  |= and != look the same!
		// Perform checks on data.
		check = (ack[0] != 'M');
		check |= (ack[1] != 'R');
		check |= (ack[2] != ((i == 0) ? '0' : '1'));
		check |= (chan->channel_num != int_n(tmp, &ack[3], 3));
		if (check) {
			rig_debug(RIG_DEBUG_ERR,
				  __FUNCTION__ ":check failed!\n");
			return -RIG_EINVAL;	// correct error type?
		}
		// all is well, save string
		strncpy(&mrtxt[i][0], ack, 52);
	}
	// FIXME: handle mem split
	// final check on data. (if RX!=TX mem split, or limit!)
	if (strncmp(&mrtxt[0][3], &mrtxt[1][3], 41 - 3)
	    != 0) {
		rig_debug(RIG_DEBUG_ERR,
			  "\n" __FUNCTION__
			  ": MEM split and band limits not yet supported!\n");
		return -RIG_ENIMPL;	// FIXME: sending proper error?
	}
	// FIXME: 1) Since chan is not an array, we fudge and only do TX!
	//                  even if split!!!!!!!!
	// FIXME: 2) we only handle regular memories, not everything
	// FIXME: 3) I store only data ts2000 actually saves in memory
	//              some values are the ts2000 value, not the hamlib value!

	// (keep same order as channel struct to ease debugging!)
//       chan->channel_num = ;   // already set?

	chan->flags = 0;
// The following may be used to indicate we're reading limits (290-299).
// At any rate, it's currently unused. 
	chan->bank_num = 0;	// I merge the two--do not use! --Dale
	/* chan->channel_num set by caller! */
	chan->vfo = RIG_VFO_MEM;
	chan->ant = 0;		// NIMPL
	chan->flags |= (mrtxt[0][18] = '0')? RIG_CHFLAG_SKIP : 0;
	chan->freq = int_n(tmp, &mrtxt[0][06], 11);
	chan->mode = ts2000_mode_list[int_n(tmp, &mrtxt[0][17], 1)];
	chan->width = 0;	// NIMPL
	chan->tx_freq = int_n(tmp, &mrtxt[1][06], 11);
	chan->tx_mode = ts2000_mode_list[int_n(tmp, &mrtxt[1][17], 1)];
	chan->tx_width = 0;	// NIMPL
	i = chan->freq != chan->tx_freq;
	i |= chan->mode != chan->tx_mode;
	if( i )
		chan->split = RIG_SPLIT_ON;
	else
		chan->split = RIG_SPLIT_OFF;
	chan->rptr_shift = int_n(tmp, &mrtxt[0][28], 1);
	chan->rptr_offs = int_n(tmp, &mrtxt[0][29], 9);
	if (chan->mode == RIG_MODE_AM || chan->mode == RIG_MODE_FM)
		i = 1;
	else
		i = 0;
	chan->tuning_step = ts2000_steps[i][int_n(tmp, &mrtxt[0][38], 2)];
	chan->rit = 0;		// n/a for memory
	chan->xit = 0;		// n/a for memory
	chan->funcs = 0;	// n/a for memory
	for (i = 0; i < RIG_SETTING_MAX; i++) {
		// the following shamelessly stolen from rigctl.c  --Dale
		setting_t level = rig_idx2setting(i);	// now, I understand
		if (RIG_LEVEL_IS_FLOAT(level))
			chan->levels[i].f = 0.0;	// I'd figured this out.
		else
			chan->levels[i].i = 0;
	}
	/* ctcss and tone use the same list except for 1750 */
	chan->tone = ts2000_ctcss_list[int_n(tmp, &mrtxt[0][20], 2) - 1];
	i = int_n(tmp, &mrtxt[1][19], 1);
	chan->tone_sql = (i == 1) ? 1 : 0;	// FIXME: What value is ON?
	chan->ctcss =
	    ts2000_ctcss_list[int_n(tmp, &mrtxt[0][22], 2) - 1];
	/* i still valid */
	chan->ctcss_sql = (i == 2) ? 1 : 0;	// FIXME: What value is ON?
	chan->dcs = ts2000_dcs_list[int_n(tmp, &mrtxt[0][24], 3) - 1];
	/* i still valid */
	chan->dcs_sql = (i == 3) ? 1 : 0;
	chan->scan = RIG_SCAN_NONE;	// n/a for memory read  
	chan->scan_group = int_n(tmp, &mrtxt[0][40], 1);
	// FIXME : The following may have trailing garbage
	strncpy(chan->channel_desc, &mrtxt[0][41], 8);
	chan->channel_desc[8] = '\0';
#ifdef _USEVFO
// if curr mem is changed at top, this'll restore it
	if ((vfo == RIG_MEM_A || vfo == RIG_MEM_C)) {
		retval = rig_set_vfo(rig, curr_vfo);
		CHKERR(retval);
	}

	rig_debug(RIG_DEBUG_ERR,
		  __FUNCTION__ ": restored mem=%i\n", curr_mem);
	retval = ts2000_set_mem(rig, curr_vfo, curr_mem);
	CHKERR(retval);
#endif
	return RIG_OK;
}

/*
 * ts2000_set_channel()
 */
int ts2000_set_channel(RIG * rig, channel_t * chan)
{
	char mrtxt[2][60], ack[60];
	int retval, i, j, mr_len[2], ack_len;
	// the following are the actual memory data to be written.
	unsigned int p1,	// RX/TX (bool)
	 p2p3,			// this is not a bug--I combine bank/channel
	 p4,			// freq
	 p5,			// mode
	 p6,			// lockout
	 p7,			// tone, ctcss, dcs
	 p8,			// tone #
	 p9,			// CTCSS #
	 p10,			// DCS code
	 p11,			// revers status
	 p12,			// repeater shift
	 p13,			// offset freq
	 p14,			// step size
	 p15;			// memory group (0-9)
	char *p16;		// 8 char + 1 null byte
	ack_len = 10;
	/*
	 * Write everthing in order.
	 */
	/* FIXME: we are required to have RX/TX match */
	if (chan->freq != chan->tx_freq)
		return -RIG_EINVAL;	// should be 'unimplemented'
	for (i = 0; i < 2; i++) {
		p1 = (unsigned int) i;	// 0=RX, 1=TX
		p2p3 = (unsigned int) chan->channel_num;	// we'll compare 'em later
		if (i == 0)
			p4 = (unsigned int) chan->freq;
		else
			p4 = (unsigned int) chan->tx_freq;
		for (j = 0;
		     j < (sizeof(ts2000_mode_list) / sizeof(int)); j++) {
			if (chan->mode == ts2000_mode_list[j])
				break;
		}
		p5 = (unsigned int) j;	// FIXME: either not found, or last!
		p6 = (unsigned int) (chan->flags == RIG_CHFLAG_SKIP)? 1 : 0 ;
		p7 = 0;		// FIXME: to lazy to sort this out right now
		p8 = 0;		//       "        "       "       "       "
		p9 = 0;		//       "        "       "       "       "
		p10 = 0;	//      "         "       "       "       "
		p11 = 0;	//      "         "       "       "       "
		p12 = 0;	//      "         "       "       "       "
		p13 = 0;	//      "         "       "       "       "
		p14 = 0;	//      "         "       "       "       "
		p15 = (unsigned int) chan->scan_group;
		p16 = (char *) &(chan->channel_desc[0]);
		mr_len[i] = sprintf(&(mrtxt[i][0]),
				    "mr%1u%3u%11u%1u%1u%1u%2u%2u%3u%1u%1u%9u%2u%1u%s;",
				    p1, p2p3, p4, p5, p6, p7,
				    p8, p9, p10, p11, p12, p13, p14, p15, p16);	// yikes!
		retval =
		    ts2000_transaction(rig, &mrtxt[i][0], mr_len[i],
				     ack, &ack_len);
		CHKERR(retval);
		// FIXME: now readback the string and make sure it worked!
	}

	return retval;
}

/*
 * ts2000_vfo_ctrl()	set PTT/CTRL based on VFO arg
 *
 *	(Taken from my revision of ts2000_set_vfo())
 *
 *	status:		VFOA, VFOB, VFOC, Main, Sub,
 *			MEMA, MEMC, CALLA, CALLC 
 *			VFO_AB, VFO_BA, ...
 *		They all work!		--Dale
 */
int ts2000_vfo_ctrl(RIG * rig, vfo_t vfo)
{
	int ptt, ctrl;
	int retval;
	ptt = ctrl = 0;
	// Main/Sub Active Transceiver
	switch (vfo) {
	case RIG_VFO_A:
	case RIG_VFO_B:
	case RIG_VFO_AB:	// split
	case RIG_VFO_BA:
	case RIG_CTRL_SAT:	// Should be PTT on main CTRL on sub (?)
	case RIG_VFO_MAIN:
	case RIG_MEM_A:
	case RIG_CALL_A:
		ctrl = TS2000_CTRL_ON_MAIN;	// FIXME : these are independent!
		ptt = TS2000_PTT_ON_MAIN;
		break;
	case RIG_VFO_C:
	case RIG_VFO_SUB:
	case RIG_MEM_C:
	case RIG_CALL_C:
		ctrl = TS2000_CTRL_ON_SUB;
		ptt = TS2000_PTT_ON_SUB;
		break;
	default:
		break;
	}

	// set PTT/CTRL
	retval = ts2000_set_ctrl(rig, ptt, ctrl);
	if (retval != RIG_OK)
		return -RIG_EINVAL;
	return retval;
}

/*
 *	status:	ok, no vfo checks
 */
int ts2000_get_dcs(RIG * rig, vfo_t vfo, tone_t * tone)
{
	char ack[10], tmp[10];
	int retval, acklen, i;
	acklen = 10;
	retval = ts2000_transaction(rig, "qc;", 6, ack, &acklen);
	CHKERR(retval);
	i = int_n(tmp, &ack[2], 3);
	*tone = ts2000_dcs_list[i];
	return RIG_OK;
}

/*
 *	status:	ok, no vfo checks
 */
int ts2000_set_dcs(RIG * rig, vfo_t vfo, tone_t tone)
{
	char cmd[10];
	int cmdlen, i;
	// we only allow exact matches here
	i = 0;
	while (tone != ts2000_dcs_list[i]) {
		if (ts2000_dcs_list[i] == 0)
			return -RIG_EINVAL;
		i++;
	}
	cmdlen = sprintf(cmd, "qc%03u;", i);
	return ts2000_transaction(rig, cmd, cmdlen, NULL, NULL);
}

/*
 *	status:	new, all guesses and assumptions!
 *		know nothing about txwidth (which one?)
 *		or tx = rx + txwidth?
 */
int ts2000_set_split_mode(RIG * rig,
			vfo_t vfo, rmode_t txmode, pbwidth_t txwidth)
{
	vfo_t vtmp;
	switch (vfo) {
	case RIG_VFO_AB:
		vtmp = RIG_VFO_B;
		break;
	case RIG_VFO_BA:
		vtmp = RIG_VFO_A;
		break;
	default:
		return -RIG_EINVAL;
	}
	return ts2000_set_mode(rig, vtmp, txmode, txwidth);
}

int ts2000_get_split_mode(RIG * rig,
			vfo_t vfo, rmode_t * txmode, pbwidth_t * txwidth)
{
	vfo_t vtmp;
	switch (vfo) {
	case RIG_VFO_AB:
		vtmp = RIG_VFO_B;
		break;
	case RIG_VFO_BA:
		vtmp = RIG_VFO_A;
		break;
	default:
		return -RIG_EINVAL;
	}
	return rig_get_mode(rig, vtmp, txmode, txwidth);
}

/*
 *	status:	new/broken
 */
int ts2000_scan(RIG * rig, vfo_t vfo, scan_t scan, int ch)
{
	int retval;
	vfo_t v;

// This is for testing new ts2000_uniq_SetScan() function.
// It should do the right thing, but last two parameters are ignored.
	return ts2000_uniq_SetScan(rig, vfo);

	rig_debug(RIG_DEBUG_ERR, __FUNCTION__ ": starting...\n");
	if (vfo == RIG_VFO_CURR) {
		retval = rig_get_vfo(rig, &v);
		CHKERR(retval);
	} else
		v = vfo;
	// hopefully, this'll work.  rig does nothing if already in scan!
	retval = ts2000_uniq_ScanOff(rig, vfo);
	CHKERR(retval);
	rig_debug(RIG_DEBUG_ERR, __FUNCTION__ ": got VFO = %x\n", v);
	// set proper vfo first (already done?)
	switch (v) {
	case RIG_VFO_MEM:	// Currently selected Main/Sub
	case RIG_MEM_A:	// Main
	case RIG_MEM_C:	// Sub
		// FIXME: we should set the group and fall through
		/* nobreak */
//      case RIG_VFO_VFO:         // Currently selected Main/Sub
	case RIG_VFO_A:	// Main
	case RIG_VFO_B:	// Main
	case RIG_VFO_C:	// Sub
		retval = ts2000_set_vfo(rig, v);	// already set?
		CHKERR(retval);
		break;
	case RIG_CALL_A:	// 
	case RIG_CALL_C:
	default:
		rig_debug(RIG_DEBUG_ERR,
			  __FUNCTION__ ": vfo 'defaulted'\n");
		return -RIG_ENIMPL;	// unimplemented, but valid scan
	}

	rig_debug(RIG_DEBUG_ERR, __FUNCTION__ ": VFO set!\n");
	retval = ts2000_uniq_ScanOff(rig, vfo);
	CHKERR(retval);
	switch (scan) {

	case RIG_SCAN_STOP:
		return ts2000_uniq_ScanOff(rig, vfo);
		break;
	case RIG_SCAN_PROG:
		if (ch < 290)
			return -RIG_EINVAL;
		retval = ts2000_set_mem(rig, vfo, ch);
		CHKERR(retval);
		/* nobreak */
	case RIG_SCAN_MEM:
		/* nobreak */
	case RIG_SCAN_VFO:
		return ts2000_uniq_ScanOn(rig, vfo);	// Look Ma, I'm scanning!
		break;
	case RIG_SCAN_SLCT:
		/* nobreak */
	case RIG_SCAN_PRIO:
		/* nobreak */
	default:
		rig_debug(RIG_DEBUG_ERR,
			  __FUNCTION__ ": scan 'defaulted'\n");
		return -RIG_ENIMPL;	// unimplemented, but valid scan
	}
}


int ts2000_get_parm(RIG * rig, setting_t parm, value_t * val)
{
	int retval, acklen, cmdlen;
	char ack[30], cmd[30];
	switch (parm) {
	case RIG_PARM_BEEP:
		cmdlen = sprintf(cmd, "ex0120000;");
		break;
	case RIG_PARM_BACKLIGHT:
		cmdlen = sprintf(cmd, "ex0000000;");
		break;
	case RIG_PARM_KEYLIGHT:
		cmdlen = sprintf(cmd, "ex0010000;");
		break;
	case RIG_PARM_APO:
		cmdlen = sprintf(cmd, "ex0570000;");
		break;
	case RIG_PARM_ANN:
		return -RIG_ENIMPL;
	case RIG_PARM_TIME:
		return -RIG_ENAVAIL;
	default:
		return -RIG_EINTERNAL;
	}
	acklen = 30;
	retval = ts2000_transaction(rig, cmd, cmdlen, ack, &acklen);
	if (retval == RIG_OK) {
		switch (parm) {
		case RIG_PARM_BEEP:
			val->i = (int) (ack[9] - '0');
			break;
		case RIG_PARM_BACKLIGHT:
		case RIG_PARM_KEYLIGHT:
			val->f = (float) (ack[9] - '0');
			break;
		case RIG_PARM_APO:
			val->i = (int) int_n(cmd, &ack[9], 2);	// cmd is TMP now
			break;
		default:
			return -RIG_EINTERNAL;
		}
	}
	return retval;
}

int ts2000_set_parm(RIG * rig, setting_t parm, value_t val)
{
	char cmd[30];
	int retval, cmdlen;
	rig_debug(RIG_DEBUG_ERR, __FUNCTION__ ": val.i = %u\n", val.i);
	rig_debug(RIG_DEBUG_ERR, __FUNCTION__ ": val.f = %f\n", val.f);
	switch (parm) {
	case RIG_PARM_APO:
		cmdlen = sprintf(cmd, "ex0570000%01u;",
				 (val.i > 180) ? 3 : val.i / 60);
		break;
	case RIG_PARM_BEEP:
		cmdlen =
		    sprintf(cmd, "ex0120000%01u;",
			    (val.i > 9) ? 9 : val.i); break;
	case RIG_PARM_BACKLIGHT:
		cmdlen =
		    sprintf(cmd, "ex0000000%01u;",
			    (int) ((val.f > 1.0) ? 4.0 : val.f * 4.0));
		break;
	case RIG_PARM_KEYLIGHT:
		cmdlen =
		    sprintf(cmd, "ex0010000%01u;",
			    (val.i == 0) ? 0 : 1); break;
	case RIG_PARM_ANN:
		return -RIG_ENIMPL;
	case RIG_PARM_TIME:
	case RIG_PARM_BAT:
		return -RIG_ENAVAIL;
	default:
		return -RIG_EINTERNAL;
	}

	retval = ts2000_transaction(rig, cmd, cmdlen, NULL, NULL);
	return retval;
}

/*
 * ts2000_set_basic(RIG *rig, vfo_t vfo)
 *
 *	Check vfo and change to a simpler mode on the vfo specified.
 *
 *	TS-2000 Only
 */
int ts2000_set_basic(RIG *rig, vfo_t vfo)
{
	char ack[20], tmp[20];
	int retval, acklen, tmplen, Sub, Main;
//	vfo_t vtmp;

	Sub = Main = 0;	// 0 == don't clear
	rig_debug(RIG_DEBUG_VERBOSE, __FUNCTION__ ": checking SAT\n");

/* SAT prevents everything else! */
	acklen = 20;
	retval = ts2000_transaction(rig, "sa;", 3, ack, &acklen);
	CHKERR(retval);
	rig_debug(RIG_DEBUG_VERBOSE, __FUNCTION__ ": SAT = %s\n", ack);
	if(ack[2] == '1' && !(vfo & RIG_CTRL_SAT)) {
		rig_debug(RIG_DEBUG_VERBOSE, __FUNCTION__ ": turning SAT off\n");
		ack[2] = '0'; ack[9] = ';'; ack[10] = '\0';

		retval = ts2000_transaction(rig, ack, acklen+1, NULL, NULL);
		CHKERR(retval);
	}

/* Now, set VFOA, VFOC, or both depending on vfo. Nothing fancy allowed. */
	if(vfo & RIG_CTRL_SAT) {
		Sub = Main = 1;
	}
	if(vfo & RIG_CTRL_MAIN) {
		Main = 1;
	}
	if(vfo & RIG_CTRL_SUB) {
		Sub = 1;
	}

	if(Main) {	// Force VFOA
		rig_debug(RIG_DEBUG_VERBOSE, __FUNCTION__ ": Setting VFOA\n");
		retval = ts2000_set_ctrl(rig, 0, TS2000_CTRL_ON_MAIN);
		acklen = 20;
		retval = ts2000_transaction(rig, "fr0;", 3, NULL, NULL);
		CHKERR(retval);
		acklen = 20;
		retval = ts2000_transaction(rig, "ft0;", 3, NULL, NULL);
		CHKERR(retval);
	}

	if(Sub) {	// Force VFOC
		rig_debug(RIG_DEBUG_VERBOSE, __FUNCTION__ ": Setting VFOC\n");
		retval = ts2000_set_ctrl(rig, 0, TS2000_CTRL_ON_SUB);
		acklen = 20;
		retval = ts2000_transaction(rig, "fr0;", 3, NULL, NULL);
		CHKERR(retval);
		acklen = 20;
		retval = ts2000_transaction(rig, "ft0;", 3, NULL, NULL);
		CHKERR(retval);
		rig_debug(RIG_DEBUG_VERBOSE, __FUNCTION__ ": Set VFOC\n");
	}

	return RIG_OK;
}

/*
 * ts2000_save_channel() (rig_save_channel() doesn't call us! don't write either)
 *
int ts2000_save_channel(RIG *rig, chan_t *ch)
{
	
}
int ts2000_restore_channel(){}
 This part awaiting Hamlib changes
 */

/* The following are "so-to-be" functions:

int ts2000_init_pm(){}
int ts2000_init_menu(){}
int ts2000_menu_list(){}

// The rest is anybody's guess...  --Dale

 * end TODO list
 */

/********************************************************************************
 *			Redesigned and/or New functions				*
 ********************************************************************************/

/*
 * ts2000_uniq_GetCtrl()	Read rig and return the RIG_VFO_PTT and RIG_VFO_CTRL
 *			bits in a vfo_t.
 */
int ts2000_uniq_GetCtrl(RIG *rig, vfo_t *vfo)
{
	int retval, acklen;
	char ack[10];

	acklen = 10;
	retval = ts2000_transaction(rig, "dc;", 3, ack, &acklen);
	CHKERR(retval);

	*vfo = ((ack[2] == '0')? RIG_VFO_PTT : 0) | ((ack[3] == '0')? RIG_VFO_CTRL : 0);

	return RIG_OK;
}

/*
 * ts2000_uniq_SetCtrl()	Set rig PTT/CTRL based on vfo_t bits.
 */
int ts2000_uniq_SetCtrl(RIG *rig, vfo_t vfo)
{
	int cmdlen;
	char cmd[10];

	cmdlen = sprintf(cmd, "dc%c%c;",
			(vfo & RIG_VFO_PTT)? '0': '1' , (vfo & RIG_VFO_CTRL)? '0': '1');

//	if(cmdlen == 0)		// memory check; enable this when all is well
//		return -RIG_EINTERNAL;

	cmdlen = 10;
	return ts2000_transaction(rig, cmd, cmdlen, NULL, NULL);
}

/*
 * ts2000_uniq_GetVfo()	Get the actual vfo_t built from reading rig
 */
int ts2000_uniq_GetVfo(RIG *rig, vfo_t *vfo)
{
	int i, retval, acklen, txlen, rxlen;
	char ack[60], tx[10], rx[10];
	vfo_t cv;	// current vfo

	if(rig == NULL || vfo == NULL)
		return -RIG_EINTERNAL;	// Somebody should have caught this before now!

	cv = 0;
	/* From Past experience, things can change on us.  Now we lock the controls! */
	ts2000_uniq_LockPanel(rig);

	retval = ts2000_uniq_GetCtrl(rig, &cv);
	CHKERR(retval);
	rig_debug(RIG_DEBUG_VERBOSE, __FUNCTION__": CTRL: cv=%s\n", strvfo(cv));

	/* For various reasons we must check SAT first. (RIG BUG) */
	acklen = 60;
	retval = ts2000_transaction(rig, "sa;", 3, ack, &acklen);
	if (retval == RIG_OK) {
		if (ack[2] == '1') {
			cv |= RIG_CTRL_SAT;
			cv |= (ack[4] == '0') ? RIG_CTRL_MAIN : RIG_CTRL_SUB;	// Uplink
			cv |= (ack[4] == '0') ? RIG_VFO_PTT : 0;
			cv |= (ack[5] == '0') ? RIG_VFO_CTRL : 0;
			cv |= (ack[6] == '1') ? RIG_CTRL_SPLIT : 0;
			cv |= (ack[7] == '1') ? RIG_CTRL_REV : 0;	// Trace reverse
			cv |= (ack[8] == '1') ? RIG_CTRL_MEM : 0;

			*vfo = cv;
			rig_debug(RIG_DEBUG_VERBOSE, __FUNCTION__
					": SAT: cv=%s\n", strvfo(cv));
						
			ts2000_uniq_UnlockPanel(rig);	// Oops!

			return RIG_OK;	// Nothing left to do.
		}		// otherwise, continue checks...
	} else {
		ts2000_uniq_UnlockPanel(rig);	// Oops!
		return retval;
	}

	/* For similar reasons, we check scan next. */
	acklen = 60;
	retval = ts2000_transaction(rig, "sc;", 3, ack, &acklen);
	if (retval == RIG_OK) {
		rig_debug(RIG_DEBUG_VERBOSE, __FUNCTION__ ": SCAN=%s\n", ack);
		if (ack[2] != '0') {
			cv |= RIG_CTRL_SCAN;
		}		// continue checks
	} else {
		ts2000_uniq_UnlockPanel(rig);	// Oops!
		return retval;
	}
	rig_debug(RIG_DEBUG_VERBOSE, __FUNCTION__": SCAN: cv=%s\n", strvfo(cv));

	/* We can always derive RIG_CTRL_MAIN/SUB from ts2000_uniq_GetCtrl() */
	if(cv & RIG_VFO_CTRL)
		cv |= RIG_CTRL_MAIN;
	else
		cv |= RIG_CTRL_SUB;

	rig_debug(RIG_DEBUG_VERBOSE, __FUNCTION__": MAIN/SUB: cv=%s\n", strvfo(cv));

	/* Now we read TX and RX and check for split, mem, etc... */
	txlen = 10;
	retval = ts2000_transaction(rig, "ft;", 3, tx, &txlen);
	CHKERR(retval);

	rxlen = 10;
	retval = ts2000_transaction(rig, "fr;", 3, rx, &rxlen);
	CHKERR(retval);

	if(rx[2] != tx[2]) {
		/* Before, I got fooled by scan here. */
		/* Also, the panel is now locked.  Both were a pain. */
		if(cv & ~RIG_CTRL_SCAN)
			cv |= RIG_CTRL_SPLIT;
		/* TODO: We might want to set split to show mem/call scan. */
	}
	if(rx[2] > tx[2]) {
		cv |= RIG_CTRL_REV;
	}
	rig_debug(RIG_DEBUG_VERBOSE, __FUNCTION__": SPLIT: cv=%s\n", strvfo(cv));

	/* We've already set split (if on) but much remains to be done... */
	for(i=0; i<2; i++) {
		switch( (i == 0)? rx[2] : tx[2] ) {
		case '0':
			if(cv & RIG_CTRL_MAIN) cv |= RIG_VFO_A;
			else if(cv & RIG_CTRL_SUB) cv |= RIG_VFO_C;
			break;
	
		case '1':
			if(cv & RIG_CTRL_MAIN) cv |= RIG_VFO_B;
			else if(cv & RIG_CTRL_SUB) {
				ts2000_uniq_UnlockPanel(rig);	// Oops!
				return -RIG_EPROTO;
			}
			break;
	
		case '2':
			if(cv & RIG_CTRL_MAIN) cv |= RIG_MEM_A;
			else if(cv & RIG_CTRL_SUB) cv |= RIG_MEM_C;
			break;
	
		case '3':
			if(cv & RIG_CTRL_MAIN) cv |= RIG_CALL_A;
			else if(cv & RIG_CTRL_SUB) cv |= RIG_CALL_C;
			break;
	
		default:
			rig_debug(RIG_DEBUG_ERR, __FUNCTION__
				": Unknown reply from rig: \'%s\'\n", rx);
			ts2000_uniq_UnlockPanel(rig);	// Oops!
			return -RIG_EPROTO;
		}
	}

	rig_debug(RIG_DEBUG_VERBOSE, __FUNCTION__": cv=%s\n", strvfo(cv));
	/* Unlock the panel now.  Evil User's have been prevented! */
	for(i=10; (retval=ts2000_uniq_UnlockPanel(rig)) != RIG_OK; i++)
		/* empty */ ;
	*vfo = cv;	// This is fairly important.  Things break if its forgotten.

	return retval;
}

/*
 * ts2000_uniq_LockPanel()	We just send "lk11;" an hope...
 */
int ts2000_uniq_LockPanel(RIG *rig)
{
	return ts2000_transaction(rig, "lk11;", 5, NULL, NULL);
}

/*
 * ts2000_uniq_UnlockPanel()	We just send "lk00;" an hope...
 */
int ts2000_uniq_UnlockPanel(RIG *rig)
{
	return ts2000_transaction(rig, "lk00;", 5, NULL, NULL);
}

/*
 * ts2000_uniq_SetVfo()	We expect the rig to be in a reasonable state.
 *	This routine is written to be called within ts2000_uniq_SwitchVfo().
 *
 *	Satellite and Scan have their own routines.
 */
int ts2000_uniq_SetVfo(RIG *rig, vfo_t vfo)
{
//	char cmd[20], ack[20];
	int retval;//, cmdlen, acklen;

	/* Sub/Main already correct.  Set VFO. */
	retval = ts2000_uniq_SetMinor(rig, vfo);
	CHKERR(retval);

	/* Currently, everything is already done.  This routine just looks good.*/
	return ts2000_uniq_SetMajor(rig, vfo);
}

/*
 * ts2000_uniq_SwitchVfo()	This routine doesn't really turn modes
 *	on.  It calls ts2000_uniq_SetVfo() for that function.
 *
 *	to_vfo *cannot* be RIG_VFO_CURR, RIG_VFO_VFO, or similar.
 */
int ts2000_uniq_SwitchVfo(RIG *rig, vfo_t vfo)
{
#define ACKLEN 60
	vfo_t from_vfo, to_vfo, any_vfo;
	int retval, acklen, to, from, any; 
	char ack[ACKLEN];

	if(rig == NULL)
		return -RIG_EINTERNAL;	// Way late for this, so it's internal.

	if(vfo == RIG_VFO_CURR)
		return RIG_OK;		// We're always there!

	if(vfo == RIG_VFO_VFO)
		vfo = RIG_VFO_A;	// Testing
//		return -RIG_ENIMPL;	// TODO: Force simple vfo.

	to_vfo = vfo;

/*
 * The following is only if the from_vfo becomes unknown.
 * (e.g. when we turn off SAT to do simpler stuff) 
 */
Recheck:	// Recursion like entry point.  Be careful!

	/* This forces PTT/CTRL to match, but may change someday. */
	if(to_vfo & RIG_CTRL_MAIN)
		to_vfo |= (RIG_VFO_CTRL | RIG_VFO_PTT);
	else
		to_vfo = to_vfo & ~(RIG_VFO_CTRL | RIG_VFO_PTT);

	retval = ts2000_uniq_SetCtrl(rig, to_vfo);	// SAT has its own PTT/CTRL
	CHKERR(retval);

	retval = ts2000_uniq_GetVfo(rig, &from_vfo);
	CHKERR(retval);

	/* Check the most obvious first. */
	if(to_vfo == from_vfo)
		return RIG_OK;

	any_vfo = to_vfo | from_vfo;	// Hopefully, this will simplify checks.

	/* Just like ts2000_uniq_GetVfo(), we check (order is important!):
	 *	(first) SAT , SCAN, SPLIT, MEM, VFO (last)
	 */
	if(any_vfo & RIG_CTRL_SAT) {

		/* This more properly belongs in ts2000_uniq_SetVfo() but it's trivial. */
		if(to_vfo & RIG_CTRL_SCAN)
			return -RIG_ENAVAIL;	// Maybe later we'll emulate it.

		from = from_vfo & RIG_CTRL_SAT;
		to = to_vfo & RIG_CTRL_SAT;

		if(to && from)	// Both in SAT?  Simple!
			return ts2000_uniq_SetSat(rig, to_vfo);	// Bypass ts2000_uniq_SetVfo()

		if(from) {	// Fairly easy.  Turn SAT off and we're there.
			retval = ts2000_uniq_SatOff(rig);
			CHKERR(retval);
			/*
			 * Rig is in VFO on both transceivers now!
			 * We must now Recheck as we don't know what
			 * state VFO's are in (same when turning Scan off).
			 * The TS2K's PTT/CTRL change to previous when
			 * SAT is turned off.
			 */
			goto Recheck;
		}

		if(to) {	// Must turn off MEM & SCAN on Both Main & Sub
			if(from_vfo & RIG_CTRL_SCAN) {
				retval = ts2000_uniq_ScanOff(rig, RIG_CTRL_MAIN);
				CHKERR(retval);
				retval = ts2000_uniq_ScanOff(rig, RIG_CTRL_SUB);
				CHKERR(retval);
			}
			if(from_vfo & RIG_CTRL_MEM) {
				retval = ts2000_uniq_MemOff(rig, RIG_CTRL_MAIN);
				CHKERR(retval);
				retval = ts2000_uniq_MemOff(rig, RIG_CTRL_SUB);
				CHKERR(retval);
			}

			/* No Recheck required. */
			return ts2000_uniq_SetSat(rig, to_vfo);
		}
	}

	/* Check Scan. */
	if(any_vfo & RIG_CTRL_SCAN) {

		to = to_vfo & RIG_CTRL_SCAN;
		from = from_vfo & RIG_CTRL_SCAN;

		if(from) {	// Scan must always be off, or nothing will change.
			from_vfo = from_vfo & ~RIG_CTRL_SCAN;
			retval = ts2000_uniq_ScanOff(rig, from_vfo);
			CHKERR(retval);
			//goto Recheck;	// This shouldn't be required.  Testing!
		}

		if(to) {	// Just set VFO and start scanning!
			retval = ts2000_uniq_SetVfo(rig, to_vfo);
			CHKERR(retval);
				
			return ts2000_uniq_SetScan(rig, to_vfo);
		}
	}

	/* VFO, CALL, & MEM are all the same to us now. */
	return ts2000_uniq_SetVfo(rig, to_vfo);

#undef ACKLEN
}

/*
 * Do nothing.  Later things may get moved here.
 */
int ts2000_uniq_SetMajor(RIG *rig, vfo_t vfo)
{
	return RIG_OK;
}

/*
 * Send rig codes equivalent to VFO minor.  {like old rig_set_vfo()}
 *	Rig Major codes must be properly set.
 *	No fake VFO's allowed (e.g. RIG_VFO_CURR)
 */
int ts2000_uniq_SetMinor(RIG *rig, vfo_t vfo)
{
	char cmd[10] = "fr0;", tx, rx;
	int retval, cmdlen;
	vfo_t vtmp;

	vtmp = (vfo & ~(RIG_CTRL_SCAN | RIG_VFO_PTT | RIG_VFO_CTRL));
	switch(vtmp) {
	case RIG_VFO_A:	/* no break */
	case RIG_VFO_C:	rx = tx = '0'; break;

	case RIG_VFO_B:	rx = tx = '1'; break;

	case RIG_VFO_AB:
			rx = '0'; tx = '1'; break;

	case RIG_VFO_BA:
			rx = '1'; tx = '0'; break;

	case RIG_MEM_A:	/* no break */
	case RIG_MEM_C:
			rx = tx = '2'; break;

	case RIG_CALL_A:	/* no break */
	case RIG_CALL_C:
			rx = tx = '3'; break;

	default:
		rig_debug(RIG_DEBUG_VERBOSE, __FUNCTION__
			": Using Default (VFOA) to process VFO = %s\n", strvfo(vfo));
		rx = tx = '0'; break;
	}

	/* RX first, and TX only if RX != TX */
	//cmd[0] = 'f'; cmd[1] = 'r';
	cmd[1] = 'r'; cmd[2] = rx;
	//cmd[3] = ';'; cmd[4] = '\0';
	cmdlen = 5;

	/* Rig sets RX=TX on this command */
	retval = ts2000_transaction(rig, cmd, cmdlen, NULL, NULL);
	CHKERR(retval);

	rig_debug(RIG_DEBUG_VERBOSE, __FUNCTION__
			": Sent fr%c; command.\n", rx);

	/* We're split, so now we may change TX. */
	cmd[1] = 't'; cmd[2] = tx;
	
	rig_debug(RIG_DEBUG_VERBOSE, __FUNCTION__
			": Sending fr%c; command.\n", tx);
	return ts2000_transaction(rig, cmd, cmdlen, NULL, NULL);
}

int ts2000_uniq_MemOn(RIG *rig, vfo_t vfo)
{
	return ts2000_uniq_SendVfo(rig, vfo, '2');
}

int ts2000_uniq_MemOff(RIG *rig, vfo_t vfo)
{
	/* This is VFOA on Main or VFOC on Sub. */
	return ts2000_uniq_SendVfo(rig, vfo, '0');

	/* Note: The rig returns to its previous state.  We force VFO. */
}

int ts2000_uniq_SendVfo(RIG *rig, vfo_t vfo, char v)
{
	int retval, cmdlen = 4;
	char cmd[10] = "fr0;";

	switch(vfo & (RIG_CTRL_MAIN | RIG_CTRL_SUB)) {
	case RIG_CTRL_MAIN:
		retval = ts2000_uniq_SetCtrl(rig, (RIG_VFO_PTT | RIG_VFO_CTRL)); break;
	case RIG_CTRL_SUB:
		retval = ts2000_uniq_SetCtrl(rig, 0); break;

	default:
		rig_debug(RIG_DEBUG_VERBOSE, __FUNCTION__
			": Sending VFO command to default transceiver.\n");
		retval = RIG_OK;	// fake it.
		break;
	}
	CHKERR(retval);

	cmdlen = 4;
	retval = ts2000_transaction(rig, cmd, cmdlen, NULL, NULL);
	CHKERR(retval);

	cmd[1] = 't';
	cmdlen = 4;
	return ts2000_transaction(rig, cmd, cmdlen, NULL, NULL);
}

/* No vfo_t since we can only split Main. */
int ts2000_uniq_SplitVfo(RIG *rig, char rx, char tx)
{
	int retval, cmdlen;
	char cmd[10];

	retval = ts2000_uniq_SetCtrl(rig, (RIG_VFO_PTT | RIG_VFO_CTRL));
	CHKERR(retval);

	/* Set Rx first since rig will change Tx to match. */
	cmdlen = sprintf(cmd, "fr%c;", rx);
	retval = ts2000_transaction(rig, cmd, cmdlen, NULL, NULL);
	CHKERR(retval);

	cmd[2] = 't';
	return ts2000_transaction(rig, cmd, cmdlen, NULL, NULL);
}

int ts2000_uniq_SatOn(RIG *rig, vfo_t vfo)
{
	return -RIG_ENIMPL;
}

/* No vfo_t since we SAT applies to both Main and Sub. */
int ts2000_uniq_SatOff(RIG * rig)
{
	char ack[20];
	int acklen, retval;

	acklen = 10;

	retval = ts2000_transaction(rig, "sa;", 3, ack, &acklen);
	if (retval != RIG_OK)
		return retval;

	ack[2] = '0';	// Ascii number 0
	ack[9] = ';';	// Note: we must truncate the name or errors occur!
	ack[10] = '\0';	// null char, just in case...
	acklen = 10;
	return ts2000_transaction(rig, ack, acklen, NULL, NULL);
}

/* we just turn SAT on here. It'll take some doing to run it! */
int ts2000_uniq_SetSat(RIG * rig, vfo_t vfo)
{
	char cmd[20], ack[20];
	int acklen;

	acklen = 20;

	if (!(vfo & RIG_CTRL_SAT))
		return -RIG_EINTERNAL;	// All right.  Who called us!?

//       cmdlen = sprintf(cmd, "sa%07u;", 0);    // Initial string to modify
	acklen = ts2000_transaction(rig, "sa;", 3, ack, &acklen);

	// Sat mode ON
	ack[2] = '1';		// Everything below is *nice*, this is *required*
	ack[9] = ';';		// Everything below is *nice*, this is *required*

//	goto STest;		// testing

/* cmd is already full of '0's, but we set them again explicitly */
	// SAT_VFO or SAT_MEM?
	if (vfo & RIG_CTRL_MEM)
		ack[8] = '1';	// sat mem ch 0-9
	else
		ack[8] = '0';	// sat vfo

	/* Main or Sub as uplink? */
	// Note: if both are set, Main is still uplink
	if (vfo & RIG_CTRL_MAIN) {
		ack[4] = '0';	// sat main=uplink
	} else if (vfo & RIG_CTRL_SUB) {
		ack[4] = '1';	// sat sub=uplink
	}
	// FIXME: Add Sat Trace here!

	// Trace REV
	if (vfo & RIG_CTRL_REV)
		ack[7] = '1';	// sat trace REV 
	else
		ack[7] = '0';

	// CTRL to main or sub?
	if ((vfo & RIG_VFO_CTRL) && (vfo & RIG_CTRL_SUB))
		ack[5] = '1';	// sat CTRL on sub
	else
		ack[5] = '0';	// sat CTRL on main

STest:	// FIXME: To be removed when testing is complete!
	rig_debug(RIG_DEBUG_ERR, __FUNCTION__
		  ": Sending\tsat = %s,\n\t vfo = %s\n", cmd, strvfo(vfo));
	// of course, this is *required* too!
	return ts2000_transaction(rig, ack, acklen, NULL, NULL);
}

int ts2000_uniq_SendScan(RIG * rig, vfo_t vfo, char sc)
{
	char cmd[] = "sc0;", ack[10];
	int retval, acklen;

	switch(vfo & (RIG_CTRL_MAIN | RIG_CTRL_SUB)) {
	case RIG_CTRL_MAIN:
		retval = ts2000_uniq_SetCtrl(rig, (RIG_VFO_PTT | RIG_VFO_CTRL)); break;
	case RIG_CTRL_SUB:
		retval = ts2000_uniq_SetCtrl(rig, 0); break;

	default:
		rig_debug(RIG_DEBUG_VERBOSE, __FUNCTION__
			": Warning: Scan set on default transceiver!\n");
		retval = RIG_OK;	// fake it here too.
	}
	CHKERR(retval);

	rig_debug(RIG_DEBUG_VERBOSE, __FUNCTION__
		": turning scan %s\n", (sc==0)? "off" : "on");

	cmd[2] = sc;
	retval = ts2000_transaction(rig, cmd, 4, NULL, NULL);
	CHKERR(retval);

	return RIG_OK;
}

/*
 *	status:	we just do mem/vfo scan on/off right now...
 */
int ts2000_uniq_SetScan(RIG *rig, vfo_t vfo)
{
	if(vfo & RIG_CTRL_SCAN)
		return ts2000_uniq_ScanOn(rig, vfo);
	else
		return ts2000_uniq_ScanOff(rig, vfo);
}

int ts2000_uniq_ScanOn(RIG * rig, vfo_t vfo)
{
	return ts2000_uniq_SendScan(rig, vfo, '1');
}

int ts2000_uniq_ScanOff(RIG * rig, vfo_t vfo)
{
	return ts2000_uniq_SendScan(rig, vfo, '0');
}

#undef CHKERR
#undef STUFF



// End
