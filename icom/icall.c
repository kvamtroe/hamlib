/*
 * hamlib - (C) Frank Singleton 2000 (vk3fcs@ix.netcom.com)
 *
 * icall.c - Copyright (C) 2000 Stephane Fillod
 * This shared library provides an API for communicating
 * via serial interface to a virtual do-it-all Icom (for debug purpose)
 * using the "CI-V" interface.
 *
 *
 * 		$Id: icall.c,v 1.2 2001-03-04 13:03:41 f4cfe Exp $  
 *
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 * 
 */

#include <stdlib.h>
#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <sys/ioctl.h>

#include <hamlib/rig.h>
#include <hamlib/riglist.h>
#include "icom.h"


#define ICALL_ALL_RX_MODES (RIG_MODE_AM|RIG_MODE_CW|RIG_MODE_USB|RIG_MODE_LSB|RIG_MODE_RTTY|RIG_MODE_FM)
#define ICALL_1MHZ_TS_MODES (RIG_MODE_AM|RIG_MODE_FM)
#define ICALL_1HZ_TS_MODES (RIG_MODE_CW|RIG_MODE_USB|RIG_MODE_LSB|RIG_MODE_RTTY)

/* tx doesn't have WFM.
 * 100W in all modes but AM (40W)
 */ 
#define ICALL_OTHER_TX_MODES (RIG_MODE_AM|RIG_MODE_CW|RIG_MODE_USB|RIG_MODE_LSB|RIG_MODE_RTTY|RIG_MODE_FM)
#define ICALL_AM_TX_MODES (RIG_MODE_AM)

#define ICALL_FUNC_ALL (RIG_FUNC_FAGC|RIG_FUNC_NB|RIG_FUNC_COMP|RIG_FUNC_VOX|RIG_FUNC_TONE|RIG_FUNC_TSQL|RIG_FUNC_SBKIN|RIG_FUNC_FBKIN)

#define ICALL_LEVEL_ALL (RIG_LEVEL_PREAMP|RIG_LEVEL_ATT|RIG_LEVEL_AGC|RIG_LEVEL_SQLSTAT|RIG_LEVEL_STRENGTH)

static const struct icom_priv_caps icall_priv_caps = {
	0x58,   /* whatever! */
	0,      /* 731 mode */
	ic706_ts_sc_list,
	EMPTY_STR_CAL
};

/*
 */
const struct rig_caps icall_caps = {
  RIG_MODEL_ICALL, "IC-DoItAll", "Icom", "0.2", "GPL", 
  RIG_STATUS_ALPHA, RIG_TYPE_TRANSCEIVER, 
  RIG_PTT_RIG, RIG_DCD_RIG, RIG_PORT_SERIAL,
  300, 19200, 8, 1, RIG_PARITY_NONE, RIG_HANDSHAKE_NONE, 
  0, 0, 200, 3, 
  ICALL_FUNC_ALL, ICALL_FUNC_ALL, 
  ICALL_LEVEL_ALL, RIG_LEVEL_SET(ICALL_LEVEL_ALL),
  RIG_PARM_NONE, RIG_PARM_NONE,	/* FIXME: parms */
  icom_ctcss_list, NULL,
  { 10, RIG_DBLST_END, },	/* preamp */
  { 20, RIG_DBLST_END, },
  NULL,
  Hz(9999), kHz(2.1),	/* RIT, IF-SHIFT */
  0,			/* FIXME: VFO list */
  0, RIG_TRN_RIG, 
  105, 0, 0,

  /* memory channel list */
  { {  01,  99, RIG_MTYPE_MEM, 0 },
    { 100, 105, RIG_MTYPE_EDGE, 0 },	/* two by two */
    { 106, 107, RIG_MTYPE_CALL, 0 },
    RIG_CHAN_END,
  },

  { RIG_FRNG_END, },	/* FIXME: enter region 1 setting */
  { RIG_FRNG_END, },
  { {kHz(30),MHz(200)-1,ICALL_ALL_RX_MODES,-1,-1},	/* this trx also has UHF */
 	{MHz(400),MHz(470),ICALL_ALL_RX_MODES,-1,-1},
	RIG_FRNG_END, },
  { {kHz(1800),MHz(2)-1,ICALL_OTHER_TX_MODES,5000,100000},	/* 100W class */
    {kHz(1800),MHz(2)-1,ICALL_AM_TX_MODES,2000,40000},	/* 40W class */
    {kHz(3500),MHz(4)-1,ICALL_OTHER_TX_MODES,5000,100000},
    {kHz(3500),MHz(4)-1,ICALL_AM_TX_MODES,2000,40000},
	{MHz(7),kHz(7300),ICALL_OTHER_TX_MODES,5000,100000},
    {MHz(7),kHz(7300),ICALL_AM_TX_MODES,2000,40000},
    {kHz(10100),kHz(10150),ICALL_OTHER_TX_MODES,5000,100000},
    {kHz(10100),kHz(10150),ICALL_AM_TX_MODES,2000,40000},
    {MHz(14),kHz(14350),ICALL_OTHER_TX_MODES,5000,100000},
    {MHz(14),kHz(14350),ICALL_AM_TX_MODES,2000,40000},
    {kHz(18068),kHz(18168),ICALL_OTHER_TX_MODES,5000,100000},
    {kHz(18068),kHz(18168),ICALL_AM_TX_MODES,2000,40000},
    {MHz(21),kHz(21450),ICALL_OTHER_TX_MODES,5000,100000},
    {MHz(21),kHz(21450),ICALL_AM_TX_MODES,2000,40000},
    {kHz(24890),kHz(24990),ICALL_OTHER_TX_MODES,5000,100000},
    {kHz(24890),kHz(24990),ICALL_AM_TX_MODES,2000,40000},
    {MHz(28),kHz(29700),ICALL_OTHER_TX_MODES,5000,100000},
    {MHz(28),kHz(29700),ICALL_AM_TX_MODES,2000,40000},
    {MHz(50),MHz(54),ICALL_OTHER_TX_MODES,5000,100000},
    {MHz(50),MHz(54),ICALL_AM_TX_MODES,2000,40000},
    {MHz(144),MHz(148),ICALL_OTHER_TX_MODES,5000,50000}, /* 50W */
    {MHz(144),MHz(148),ICALL_AM_TX_MODES,2000,20000}, /* AM VHF is 20W */
    {MHz(430),MHz(450),ICALL_OTHER_TX_MODES,5000,20000},
    {MHz(430),MHz(450),ICALL_AM_TX_MODES,2000,8000},
	RIG_FRNG_END, },
	{{ICALL_1HZ_TS_MODES,1},
	 {ICALL_ALL_RX_MODES,10},
	 {ICALL_ALL_RX_MODES,100},
	 {ICALL_ALL_RX_MODES,kHz(1)},
	 {ICALL_ALL_RX_MODES,kHz(5)},
	 {ICALL_ALL_RX_MODES,kHz(9)},
	 {ICALL_ALL_RX_MODES,kHz(10)},
	 {ICALL_ALL_RX_MODES,12500},
	 {ICALL_ALL_RX_MODES,kHz(20)},
	 {ICALL_ALL_RX_MODES,kHz(25)},
	 {ICALL_ALL_RX_MODES,kHz(100)},
	 {ICALL_1MHZ_TS_MODES,MHz(1)},
	 RIG_TS_END,
	},
	/* mode/filter list, remember: order matters! */
	{
		{RIG_MODE_SSB|RIG_MODE_CW|RIG_MODE_RTTY, kHz(2.4)},	/* bultin FL-272 */
		{RIG_MODE_AM, kHz(8)},		/* mid w/ bultin FL-94 */
		{RIG_MODE_AM, kHz(2.4)},	/* narrow w/ bultin FL-272 */
		{RIG_MODE_FM, kHz(15)},		/* ?? TBC, mid w/ bultin FL-23+SFPC455E */
		{RIG_MODE_FM, kHz(8)},		/* narrow w/ bultin FL-94 */
		{RIG_MODE_WFM, kHz(230)},	/* WideFM, filter FL?? */
		RIG_FLT_END,
	},
  (void*)&icall_priv_caps,	/* priv */
  icom_init, icom_cleanup, NULL, NULL, NULL /* probe not supported yet */,
  icom_set_freq, icom_get_freq, icom_set_mode, icom_get_mode, icom_set_vfo,
  NULL, 
  /*
   * FIXME:
   * the use of the following GNU extension (field: value)
   * is bad manner in portable code but admit it, quite handy
   * when testing stuff. --SF
   */
decode_event: icom_decode_event,
set_level: icom_set_level,
get_level: icom_get_level,
set_func: icom_set_func,
get_func: icom_get_func,
set_channel: icom_set_channel,
get_channel: icom_get_channel,
set_mem: icom_set_mem,
mv_ctl: icom_mv_ctl,
set_ptt: icom_set_ptt,
get_ptt: icom_get_ptt,
get_dcd: icom_get_dcd,
set_ts: icom_set_ts,
get_ts: icom_get_ts,
set_rptr_shift: icom_set_rptr_shift,
get_rptr_shift: icom_get_rptr_shift,
set_rptr_offs: icom_set_rptr_offs,
get_rptr_offs: icom_get_rptr_offs,
set_split_freq: icom_set_split_freq,
get_split_freq: icom_get_split_freq,
set_split: icom_set_split,
get_split: icom_get_split,
set_ctcss: icom_set_ctcss,
get_ctcss: icom_get_ctcss,
set_ctcss_sql: icom_set_ctcss_sql,
get_ctcss_sql: icom_get_ctcss_sql,
};


/*
 * Function definitions below
 */
