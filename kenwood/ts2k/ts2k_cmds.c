/*
 * ts2k_cmds.c	Copyright (C) 2002 by Dale E. Edmons.  All rights reserved.
 *
 *	license:	GNU
 */

/*
 * This file implements (almost) every command available to the ts2k.
 *	Most used functions will be implemented first.  All parameters are
 *	placed in a struct and #defines make using them transparent.  I've
 *	categorized all commands and assigned a type# to each uniq class of
 *	parameters.  Unique commands are handled case by case.
 *
 *	The menu "ex;" commands are not handled here yet.
 */

/*
 * Kenwood commands are typically in trios: set, read, answer.  The set
 *	format is the string required when setting parameters in the rig.
 *	read is the format used to request data from the rig.  answer is
 *	the format the rig data is in after the read request.
 */

//#include <hamlib/rig.h>
#include "ts2k_cmds.h"
#include <stdlib.h>

/************************** Generic type commands *********************/

/* generic type commands */

/* See: types-vs-cmds or manual for more "type" details */

/*
 * The ts2k_s_typeNN() is used for setting the parameters
 *	of a command whose Set format is typeNN.
 *
 *	The ts2k_g_typeNN() is used to "get" a specific
 *	command from the rig.  The "get" is used only if
 *	the "Read" command is type08 which is the two
 *	ascii command characters followed by a semicolon.
 *	Others will require special programming but will
 *	be easy to use.
 */

/* <---- Types from here down are untested. */
/*
 * ts2k_?_type01()
 *	status:	s=unchecked; g=unchecked
 *	format:	{CMD, BIN, END}
 */
int ts2k_s_type01(RIG *rig, ts2k_type01_t *p, char *cmd)
{
	TS2K_A_DATANR(TS2K_AM_SIZE);
	TS2K_CMD2(cmd, TS2K_AM_SIZE);
	ans[2] = (p->p1 == 0)? '0' : '1';
	return TS2K_SEND;
}
int ts2k_g_type01(RIG *rig, ts2k_type01_t *p, char *cmd)
{
	TS2K_A_DATA(TS2K_FT_SIZE);
	anslen = TS2K_FT_SIZE;
	ret = TS2K_READ_08(rig, cmd);
	p->p1 = TS2K_CTOI(ans[2]);
	return ret;
}
/*
 * ts2k_?_type02()
 *	status:	s=ok; g=ok
 *	format:	{CMD, FREQ, END}
 */
int ts2k_s_type02(RIG *rig, ts2k_type02_t *p, char *cmd)
{
	TS2K_A_DATANR(TS2K_FA_SIZE);
	anslen = sprintf(ans, "%s%011llu;", cmd, p->p1);	// eleven "el"
	return TS2K_SEND;
}
int ts2k_g_type02(RIG *rig, ts2k_type02_t *p, char *cmd)
{
	TS2K_A_DATA(TS2K_FA_SIZE);
	ret = TS2K_READ_08(rig, cmd);
	ans[TS2K_FA_SIZE - 2] = '\0';
	p->p1 = strtol(&ans[2], NULL, 10);
	return ret;
}

/*
 * ts2k_?_type03()
 *	status:	s=unchecked; g=unchecked
 *	format:	{CMD, HZ, END}
 */
int ts2k_s_type03(RIG *rig, ts2k_type03_t *p, char *cmd)
{
	TS2K_A_DATANR(TS2K_RD_SIZE);
	anslen = sprintf(ans, "%s%05u;", cmd, p->p1);	// eleven "el"
	return TS2K_SEND;
}
int ts2k_g_type03(RIG *rig, ts2k_type03_t *p, char *cmd)
{
	return RIG_ENAVAIL;
}

/*
 * ts2k_?_type04()
 *	status:	s=ok; g=ok
 *	format:	{CMD, INT1, END}
 */
int ts2k_s_type04(RIG *rig, ts2k_type04_t *p, char *cmd)
{
	TS2K_A_DATANR(TS2K_AI_SIZE);
	TS2K_CMD2(cmd, TS2K_AI_SIZE);
	ans[2] = TS2K_ITOC(p->p1);
	return TS2K_SEND;
}
int ts2k_g_type04(RIG *rig, ts2k_type04_t *p, char *cmd)
{
	TS2K_A_DATA(TS2K_AI_SIZE);
	ret = TS2K_READ_08(rig, cmd);
	p->p1 = TS2K_CTOI(ans[2]);
	return ret;
}

/*
 * ts2k_?_type05()
 *	status:	s=unchecked; g=unchecked
 *	format:	{CMD, INT2, END}
 */
int ts2k_s_type05(RIG *rig, ts2k_type05_t *p, char *cmd)
{
	TS2K_A_DATANR(TS2K_CN_SIZE);
	anslen = sprintf(ans, "%s%02u;", cmd, (p->p1 > 99) ? 99 : p->p1);
	return TS2K_SEND;
}
int ts2k_g_type05(RIG *rig, ts2k_type05_t *p, char *cmd)
{
	TS2K_A_DATA(TS2K_CN_SIZE);
	ret = TS2K_READ_08(rig, cmd);
	ans[TS2K_CN_SIZE - 2] = '\0';
	p->p1 = atoi(&ans[2]);
	return ret;
}

/*
 * ts2k_?_type06()
 *	status:	s=unchecked; g=used
 *	format:	{CMD, INT3, END}
 */
int ts2k_s_type06(RIG *rig, ts2k_type06_t *p, char *cmd)
{
	TS2K_A_DATANR(TS2K_AL_SIZE);
	anslen = sprintf(ans, "%s%03u;", cmd, (p->p1 > 999) ? 999 : p->p1);
	return TS2K_SEND;
}
int ts2k_g_type06(RIG *rig, ts2k_type06_t *p, char *cmd)
{
	TS2K_A_DATA(TS2K_AL_SIZE);
	ret = TS2K_READ_08(rig, cmd);
	ans[TS2K_AL_SIZE - 2] = '\0';
	p->p1 = atoi(&ans[2]);
	return ret;
}

/*
 * ts2k_?_type07()
 *	status:	s=unchecked; g=unchecked
 *	format:	{CMD, INT4, END}
 */
int ts2k_s_type07(RIG *rig, ts2k_type07_t *p, char *cmd)
{
	TS2K_A_DATANR(TS2K_FW_SIZE);
	anslen = sprintf(ans, "%s%04u;", cmd, (p->p1 > 999) ? 999 : p->p1);
	return TS2K_SEND;
}
int ts2k_g_type07(RIG *rig, ts2k_type07_t *p, char *cmd)
{
	TS2K_A_DATA(TS2K_FW_SIZE);
	ret = TS2K_READ_08(rig, cmd);
	ans[TS2K_AL_SIZE - 2] = '\0';
	p->p1 = atoi(&ans[2]);
	return ret;
}

/*
 * ts2k_?_type08()
 *	status:	s=unchecked; g=unchecked
 *	format:	{CMD, END}
 */
int ts2k_s_type08(RIG *rig, ts2k_type08_t *p, char *cmd)
{
	TS2K_A_DATANR(TS2K_RX_SIZE);
	TS2K_CMD2(cmd, TS2K_RX_SIZE);
	return TS2K_SEND;
}
int ts2k_g_type08(RIG *rig, ts2k_type08_t *p, char *cmd)
{
	return -RIG_ENAVAIL;
}

/*
 * ts2k_?_type09()
 *	status:	s=unchecked; g=used
 *	format:	{CMD, BIN, BIN, END}
 */
int ts2k_s_type09(RIG *rig, ts2k_type09_t *p, char *cmd)
{
	TS2K_A_DATANR(TS2K_DC_SIZE);
	TS2K_CMD2(cmd, TS2K_DC_SIZE);
	ans[2] = TS2K_ITOC(p->p1);
	ans[3] = TS2K_ITOC(p->p2);
	return TS2K_SEND;
}
int ts2k_g_type09(RIG *rig, ts2k_type09_t *p, char *cmd)
{
	TS2K_A_DATA(TS2K_DC_SIZE);
	ret = TS2K_READ_08(rig, cmd);
	p->p1 = TS2K_CTOI(ans[2]);
	p->p2 = TS2K_CTOI(ans[3]);
	return ret;
}

/*
 * ts2k_?_type10()
 *	status:	s=unchecked; g=unchecked
 *	format:	{CMD, SPC, BIN, END}
 */
int ts2k_s_type10(RIG *rig, ts2k_type10_t *p, char *cmd)
{
	TS2K_A_DATANR(TS2K_TC_SIZE);
	TS2K_CMD2(cmd, TS2K_TC_SIZE);
	ans[2] = ' ';
	ans[3] = (p->p1 == 0)? '0': '1';
	return TS2K_SEND;
}
int ts2k_g_type10(RIG *rig, ts2k_type10_t *p, char *cmd)
{
	TS2K_A_DATA(TS2K_TC_SIZE);
	ret = TS2K_READ_08(rig, cmd);
	p->p1 = TS2K_CTOI(ans[3]);
	return ret;
}

/*
 * ts2k_?_type11()
 *	status:	s=unchecked; g=unchecked
 *	format:	{CMD, BIN, INT1, END}
 */
int ts2k_s_type11(RIG *rig, ts2k_type11_t *p, char *cmd)
{
	TS2K_A_DATANR(TS2K_QR_SIZE);
	TS2K_CMD2(cmd, TS2K_QR_SIZE);
	ans[2] = (p->p1 == 0)? '0': '1';
	ans[3] = TS2K_ITOC(p->p2);
	return TS2K_SEND;
}
int ts2k_g_type11(RIG *rig, ts2k_type11_t *p, char *cmd)
{
	TS2K_A_DATA(TS2K_QR_SIZE);
	ret = TS2K_READ_08(rig, cmd);
	p->p1 = TS2K_CTOI(ans[2]);
	p->p2 = TS2K_CTOI(ans[3]);
	return ret;
}

/*
 * ts2k_?_type12()
 *	status:	s=unchecked; g=unchecked
 *	format:	{CMD, INT1, INT1, END}
 */
int ts2k_s_type12(RIG *rig, ts2k_type12_t *p, char *cmd)
{
	TS2K_A_DATANR(TS2K_LK_SIZE);
	TS2K_CMD2(cmd, TS2K_LK_SIZE);
	ans[2] = TS2K_ITOC(p->p1);
	ans[3] = TS2K_ITOC(p->p2);
	return TS2K_SEND;
}
int ts2k_g_type12(RIG *rig, ts2k_type12_t *p, char *cmd)
{
	TS2K_A_DATA(TS2K_LK_SIZE);
	ret = TS2K_READ_08(rig, cmd);
	p->p1 = TS2K_CTOI(ans[2]);
	p->p2 = TS2K_CTOI(ans[3]);
	return ret;
}

/*
 * ts2k_?_type13()
 *	status:	s=unchecked; g=unchecked
 *	format:	{CMD, INT2, INT1, END}
 */
int ts2k_s_type13(RIG *rig, ts2k_type13_t *p, char *cmd)
{
	return -RIG_ENAVAIL;
}
int ts2k_g_type13(RIG *rig, ts2k_type13_t *p, char *cmd)
{
	TS2K_A_DATA(TS2K_TY_SIZE);
	ret = TS2K_READ_08(rig, cmd);
	p->p1 = TS2K_CTOI(ans[2]);
	return ret;
}

/*
 * ts2k_?_type14()
 *	status:	s=unchecked; g=unchecked
 *	format:	{CMD, INT1, INT2, END}
 */
int ts2k_s_type14(RIG *rig, ts2k_type14_t *p, char *cmd)
{
	TS2K_A_DATANR(TS2K_AS_SIZE);
	anslen = sprintf(ans, "%s%01u%02u;", cmd, \
		(p->p1 > 9)? 9 : p->p1, (p->p1 > 99)? 99 : p->p2);
	return TS2K_SEND;
}
int ts2k_g_type14(RIG *rig, ts2k_type14_t *p, char *cmd)
{
	return -RIG_ENAVAIL;
}

/*
 * ts2k_?_type15()
 *	status:	s=unchecked; g=unchecked
 *	format:	{CMD, INT2, INT2, END}
 */
int ts2k_s_type15(RIG *rig, ts2k_type15_t *p, char *cmd)
{
	return -RIG_ENAVAIL;
}
int ts2k_g_type15(RIG *rig, ts2k_type15_t *p, char *cmd)
{
	TS2K_A_DATA(TS2K_RA_SIZE);
	ret = TS2K_READ_08(rig, cmd);
	ans[TS2K_RA_SIZE - 2] = '\0';
	p->p2 = atoi(&ans[4]);	// Order is important!
	p->p1 = atoi(&ans[2]);
	return ret;
}

/*
 * ts2k_?_type16()
 *	status:	s=unchecked; g=buggy
 *	format:	{CMD, BIN, INT3, END}
 */
int ts2k_s_type16(RIG *rig, ts2k_type16_t *p, char *cmd)
{
	TS2K_A_DATANR(TS2K_AG_SIZE);
	/* FIXME: compiler warning on following statement */
	anslen = sprintf(ans, "%s%c%03u;", cmd, \
		(p->p1 > 0)? '1': '0', \
		(p->p2 > 999)? 999: p->p2);
	return TS2K_SEND;
}
int ts2k_g_type16(RIG *rig, ts2k_type16_t *p, char *cmd)
{
	TS2K_A_DATA(TS2K_AG_SIZE);
	ret = TS2K_READ_08(rig, cmd);
	p->p1 = TS2K_CTOI(ans[2]);
	ans[TS2K_AG_SIZE - 2] = '\0';
	p->p2 = atoi(&ans[3]);
	return ret;
}

/*
 * ts2k_?_type17()
 *	status:	s=unchecked; g=unchecked
 *	format:	{CMD, INT3, INT3, END}
 */
int ts2k_s_type17(RIG *rig, ts2k_type17_t *p, char *cmd)
{
	TS2K_A_DATANR(TS2K_PL_SIZE);
	anslen = sprintf(ans, "%s%03u%03u;", cmd, \
		(p->p1 > 999)? 999: p->p2, (p->p2 > 999)? 999: p->p2);
	return TS2K_SEND;
}
int ts2k_g_type17(RIG *rig, ts2k_type17_t *p, char *cmd)
{
	int i;
	TS2K_A_DATA(TS2K_PL_SIZE);
	i = TS2K_PL_SIZE - 2;
	ret = TS2K_READ_08(rig, cmd);
	ans[i] = '\0';
	i -= 3;
	p->p2 = atoi(&ans[i]);
	ans[i] = '\0';
	p->p1 = atoi(&ans[2]);
	return ret;
}

/*
 * ts2k_?_type18()
 *	status:	s=unchecked; g=used
 *	format:	{CMD, INT1, INT4, END}
 */
int ts2k_s_type18(RIG *rig, ts2k_type18_t *p, char *cmd)
{
	return -RIG_ENAVAIL;
}
int ts2k_g_type18(RIG *rig, ts2k_type18_t *p, char *cmd)
{
	TS2K_A_DATA(TS2K_RM_SIZE);
	ret = TS2K_READ_08(rig, cmd);
	p->p1 = TS2K_CTOI(ans[2]);
	ans[TS2K_AG_SIZE - 2] = '\0';
	p->p2 = atoi(&ans[3]);
	return ret;
}

/*
 * ts2k_?_type19()
 *	status:	s=unchecked; g=unchecked
 *	format:	{CMD, INT1, TXT8, END}
 */
int ts2k_s_type19(RIG *rig, ts2k_type19_t *p, char *cmd)
{
	TS2K_A_DATANR(TS2K_SI_SIZE);
	TS2K_CMD2(cmd, TS2K_SI_SIZE);
	ans[2] = TS2K_ITOC(p->p1);
	return TS2K_SEND;
}
int ts2k_g_type19(RIG *rig, ts2k_type19_t *p, char *cmd)
{
	return -RIG_ENAVAIL;
}

/*
 * ts2k_?_type20()
 *	status:	s=unchecked; g=unchecked
 *	format:	{CMD, BIN, BIN, BIN, END}
 */
int ts2k_s_type20(RIG *rig, ts2k_type20_t *p, char *cmd)
{
	TS2K_A_DATANR(TS2K_AR_SIZE);
	TS2K_CMD2(cmd, TS2K_AR_SIZE);
	ans[2] = TS2K_ITOC(p->p1);
	ans[3] = TS2K_ITOC(p->p2);
	ans[4] = TS2K_ITOC(p->p3);
	return TS2K_SEND;
}
int ts2k_g_type20(RIG *rig, ts2k_type20_t *p, char *cmd)
{
	TS2K_A_DATA(TS2K_AR_SIZE);
	ret = TS2K_READ_08(rig, cmd);
	p->p1 = TS2K_CTOI(ans[2]);
	p->p2 = TS2K_CTOI(ans[3]);
	p->p3 = TS2K_CTOI(ans[4]);
	return ret;
}

/*
 * ts2k_?_type21()
 *	status:	s=unchecked; g=unchecked
 *	format:	{CMD, LEN, 8, SPCL, END}
 */
int ts2k_s_type21(RIG *rig, ts2k_type21_t *p, char *cmd)
{
	return -RIG_ENAVAIL;
}
int ts2k_g_type21(RIG *rig, ts2k_type21_t *p, char *cmd)
{
	TS2K_A_DATA(TS2K_FD_SIZE);
	ret = TS2K_READ_08(rig, cmd);
	ans[TS2K_RA_SIZE - 2] = '\0';
	ans[0] = '0'; ans[1] = 'x'; 
	p->p1 = strtol(&ans[0], NULL, 16);
	return ret;
}

/*
 * ts2k_?_type22()
 *	status:	s=ok; g=ok
 *	format:	{CMD, LEN, 9, INT, END}
 */
int ts2k_s_type22(RIG *rig, ts2k_type22_t *p, char *cmd)
{
	TS2K_A_DATANR(TS2K_OF_SIZE);
	//anslen = sprintf(ans, "%s%09lu;", cmd, 
	anslen = sprintf(ans, "%s%09llu;", cmd, \
		(p->p1 > 999999999) ? 999999999 : p->p1);
	return TS2K_SEND;
}
int ts2k_g_type22(RIG *rig, ts2k_type22_t *p, char *cmd)
{
	TS2K_A_DATA(TS2K_OF_SIZE);
	ret = TS2K_READ_08(rig, cmd);
	ans[TS2K_OF_SIZE - 2] = '\0';
	p->p1 = strtol(&ans[2], NULL, 10);
	return ret;
}

/*
 * ts2k_?_type23()
 *	status:	s=unchecked; g=unchecked
 *	format:	{CMD, BIN, BIN, INT1, END}
 */
int ts2k_s_type23(RIG *rig, ts2k_type23_t *p, char *cmd)
{
	TS2K_A_DATANR(TS2K_AC_SIZE);
	TS2K_CMD2(cmd, TS2K_AC_SIZE);
	ans[1] = (p->p1 == 0)? '0': '1';
	ans[3] = (p->p2 == 0)? '0': '1';
	ans[4] = TS2K_ITOC(p->p3);
	return TS2K_SEND;
}
int ts2k_g_type23(RIG *rig, ts2k_type23_t *p, char *cmd)
{
	TS2K_A_DATA(TS2K_AC_SIZE);
	ret = TS2K_READ_08(rig, cmd);
	p->p1 = TS2K_CTOI(ans[2]);
	p->p2 = TS2K_CTOI(ans[3]);
	p->p3 = TS2K_CTOI(ans[4]);
	return ret;
}

// FIXME: The following isn't currently available...
/*
 * ts2k_?_type24()
 *	status:	s=unchecked; g=unchecked
 *	format:	{CMD, INT3, INT2, INT1, INT1, END}
 */
int ts2k_s_type24(RIG *rig, ts2k_type24_t *p, char *cmd)
{
	return -RIG_ENAVAIL;
}
int ts2k_g_type24(RIG *rig, ts2k_type24_t *p, char *cmd)
{
	// use the ts2k_get_menu() instead!
	return -RIG_ENAVAIL;
}

/*
 * ts2k_?_type25()
 *	status:	s=unchecked; g=unchecked
 *	format:	{CMD, INT1, INT2, FREQ, INT1, END}
 */
int ts2k_s_type25(RIG *rig, ts2k_type25_t *p, char *cmd)
{ 
	TS2K_A_DATANR(TS2K_AS_SIZE);
	//anslen = sprintf(ans, "%s%01u%02u%011lu%01u;", cmd,
	anslen = sprintf(ans, "%s%01u%02u%011llu%01u;", cmd, \
		(p->p1 > 9)? 9 : p->p1, \
		(p->p2 > 99)? 99 : p->p2, \
		 p->p3, \
		(p->p4 > 9)? 9 : p->p4);
	return TS2K_SEND;
}
int ts2k_g_type25(RIG *rig, ts2k_type25_t *p, char *cmd)
{
	TS2K_A_DATA(TS2K_AS_SIZE);
	int i;
	i = TS2K_AS_SIZE - 3;
	ret = TS2K_READ_08(rig, cmd);
	p->p4 = TS2K_CTOI(ans[i]);	// Order is important!
	ans[i] = '\0';
	p->p3 = strtol(&ans[2], NULL, 10);
	i -= 11;
	ans[i] = '\0';
	i -= 2;
	p->p2 = atoi(&ans[i]);
	p->p1 = TS2K_CTOI(ans[2]);	// Order is important!
	return ret;
}

/*
 * ts2k_?_type26()
 *	status:	s=unchecked; g=unchecked
 *	format:	{CMD, TXT1, INT4, END}
 */
int ts2k_s_type26(RIG *rig, ts2k_type26_t *p, char *cmd)
{
	TS2K_A_DATANR(TS2K_IS_SIZE);
	anslen = sprintf(ans, "%s %04u;", cmd, (p->p2 > 9999)? 9999: p->p2);
	return TS2K_SEND;
}
int ts2k_g_type26(RIG *rig, ts2k_type26_t *p, char *cmd)
{
	TS2K_A_DATA(TS2K_IS_SIZE);
	ret = TS2K_READ_08(rig, cmd);
	ans[TS2K_OF_SIZE - 2] = '\0';
	p->p2 = atoi(&ans[3]);
	return ret;
}

/*
 * ts2k_?_type27()
 *	status:	s=unchecked; g=unchecked
 *	format:	{CMD, LEN, 10, SPCL, END}
 */
int ts2k_s_type27(RIG *rig, ts2k_type27_t *p, char *cmd)
{
	int i;
	TS2K_A_DATANR(TS2K_MU_SIZE);
	TS2K_CMD2(cmd, TS2K_MU_SIZE);
	for(i=0; i<10; i++) {
		ans[i+3] = TS2K_ITOC(p->p1[i]);
	}
	return TS2K_SEND;
}
int ts2k_g_type27(RIG *rig, ts2k_type27_t *p, char *cmd)
{
	int i;
	TS2K_A_DATA(TS2K_MU_SIZE);
	ret = TS2K_READ_08(rig, cmd);
	for(i=0; i<10; i++) {
		p->p1[i] = TS2K_CTOI(ans[2+i]);
	}
	return ret;
}

/*
 * ts2k_?_type27b()	(odd name; my fault for missing it... D. E.)
 *	status:	s=unchecked; g=unchecked
 *	format:	{CMD, BIN, LEN, 10, SPCL, END}
 */
int ts2k_s_type27b(RIG *rig, ts2k_type27b_t *p, char *cmd)
{
	int i;
	TS2K_A_DATANR(TS2K_SU_SIZE);
	TS2K_CMD2(cmd, TS2K_SU_SIZE);
	ans[2] = (p->p1 == 0)? '0': '1';
	for(i=0; i<11; i++) {
		ans[3+i] = TS2K_ITOC(p->p2[i]);
	}
	return TS2K_SEND;
}
int ts2k_g_type27b(RIG *rig, ts2k_type27b_t *p, char *cmd)
{
	int i;
	TS2K_A_DATA(TS2K_SU_SIZE);
	ret = TS2K_READ_08(rig, cmd);
	p->p1 = TS2K_CTOI(ans[2]);
	for(i=0; i<10; i++) {
		p->p2[i] = TS2K_CTOI(ans[3+i]);
	}
	return ret;
}

/*
 * ts2k_?_type28()
 *	status:	s=unchecked; g=unchecked
 *	format:	{CMD, SPC, LEN, 24, TXT, END}
 */
int ts2k_s_type28(RIG *rig, ts2k_type28_t *p, char *cmd)
{
	TS2K_A_DATANR(TS2K_KY_SIZE);
	p->p2[24] = '\0';	// force null termination
	anslen = sprintf(ans, "%s %-24s;", cmd, p->p2);	// padding after text
	return TS2K_SEND;
}
int ts2k_g_type28(RIG *rig, ts2k_type28_t *p, char *cmd)
{
	return -RIG_ENAVAIL;	// Too bad...  :)
}

/*
 * ts2k_?_type29()
 *	status:	s=unchecked; g=unchecked
 *	format:	{CMD, BIN, INT1, BIN, BIN, BIN, BIN, BIN, END}
 */
int ts2k_s_type29(RIG *rig, ts2k_type29_t *p, char *cmd)
{
	TS2K_A_DATANR(TS2K_SA_SIZE);
	TS2K_CMD2(cmd, TS2K_SA_SIZE);
	ans[2] = (p->p1 == 0)? '0': '1';
	ans[3] = TS2K_ITOC(p->p2);
	ans[4] = (p->p3 == 0)? '0': '1';
	ans[5] = (p->p4 == 0)? '0': '1';
	ans[6] = (p->p5 == 0)? '0': '1';
	ans[7] = (p->p6 == 0)? '0': '1';
	ans[8] = (p->p7 == 0)? '0': '1';
	return TS2K_SEND;
}
int ts2k_g_type29(RIG *rig, ts2k_type29_t *p, char *cmd)
{
	return -RIG_ENAVAIL;
}

// FIXME: The following currently isn't available...
/*
 * ts2k_?_type30()
 *	status:	s=unchecked; g=unchecked
 *	format:	{CMD, INT3, INT2, INT1, INT1, LEN, 16, TXT, END}
 */
int ts2k_s_type30(RIG *rig, ts2k_type30_t *p, char *cmd)
{
	// use the ts2k_set_menu() instead!
	return -RIG_ENAVAIL;
}
int ts2k_g_type30(RIG *rig, ts2k_type30_t *p, char *cmd)
{
	// use the ts2k_get_menu() instead!
	return -RIG_ENAVAIL;
}

/*
 * ts2k_?_type31()
 *	status:	s=unchecked; g=used
 *	format:	{CMD, BIN, INT1, BIN, BIN, BIN, BIN, BIN,
 *						TXT8, END}
 */
int ts2k_s_type31(RIG *rig, ts2k_type31_t *p, char *cmd)
{
	TS2K_A_DATANR(TS2K_SA_SIZE);
	TS2K_CMD2(cmd, TS2K_SA_SIZE);
	ans[2] = (p->p1 == 0)? '0': '1';
	ans[3] = TS2K_ITOC(p->p2);
	ans[4] = (p->p3 == 0)? '0': '1';
	ans[5] = (p->p4 == 0)? '0': '1';
	ans[6] = (p->p5 == 0)? '0': '1';
	ans[7] = (p->p6 == 0)? '0': '1';
	ans[8] = (p->p7 == 0)? '0': '1';
	strncpy(p->p8, &ans[9], 8);
	return TS2K_SEND;
}
int ts2k_g_type31(RIG *rig, ts2k_type31_t *p, char *cmd)
{
	TS2K_A_DATA(TS2K_SA_SIZE);
	ret = TS2K_READ_08(rig, cmd);
	p->p1 = TS2K_CTOI(ans[2]);
	p->p2 = TS2K_CTOI(ans[3]);
	p->p3 = TS2K_CTOI(ans[4]);
	p->p4 = TS2K_CTOI(ans[5]);
	p->p5 = TS2K_CTOI(ans[6]);
	p->p6 = TS2K_CTOI(ans[7]);
	p->p7 = TS2K_CTOI(ans[8]);
// FIXME: check if string exists!
	strncpy(p->p8, &ans[9], 8);
	return ret;
}

/*
 * ts2k_?_type32()
 *	status:	s=unchecked; g=unchecked
 *	format:	{CMD, FREQ, LEN, 12, TXT, LEN, 20, TXT,
 *					LEN, 5, TXT, END}
 */
int ts2k_s_type32(RIG *rig, ts2k_type32_t *p, char *cmd)
{
	return -RIG_ENAVAIL;
}
int ts2k_g_type32(RIG *rig, ts2k_type32_t *p, char *cmd)
{
	int i;
	TS2K_A_DATA(TS2K_PK_SIZE);
	ret = TS2K_READ_08(rig, cmd);
	i = TS2K_PK_SIZE - 7;
	strncpy(p->p4, &ans[i], 5);	i -= 20;
	strncpy(p->p4, &ans[i], 20);	i -= 12;
	strncpy(p->p4, &ans[i], 12);
	ans[i] = '\0';
	p->p1 = strtol(&ans[2], NULL, 10);
	return ret;
}

/*
 * ts2k_?_type33()
 *	status:	s=n/a; g=ok
 *	format:	{CMD, FREQ, INT4, LEN, 6, INT, BIN, BIN,
 *		INT3, BIN, INT1, INT1, INT1, INT1, INT1,
 *					INT2, INT1, END}
 */
int ts2k_s_type33(RIG *rig, ts2k_type33_t *p, char *cmd)
{
	return -RIG_ENAVAIL;
}
int ts2k_g_type33(RIG *rig, ts2k_type33_t *p, char *cmd)
{
	int i;
	TS2K_A_DATA(TS2K_IF_SIZE);
	ret = TS2K_READ_08(rig, cmd);
	i = TS2K_IF_SIZE - 3;
	p->p15 = TS2K_CTOI(ans[i]);	ans[i] = '\0';	i -= 2;
	p->p14 = atoi(&ans[i]);		ans[i] = '\0';	i--;
	p->p13 = TS2K_CTOI(ans[i]);			i--;
	p->p12 = TS2K_CTOI(ans[i]);			i--;
	p->p11 = TS2K_CTOI(ans[i]);			i--;
	p->p10 = TS2K_CTOI(ans[i]);			i--;
	p->p9 = TS2K_CTOI(ans[i]);			i--;
	p->p8 = TS2K_CTOI(ans[i]);	ans[i] = '\0';	i -= 3;
	p->p6p7 = atoi(&ans[i]);		ans[i] = '\0';	i--;
	p->p5 = TS2K_CTOI(ans[i]);	ans[i] = '\0';	i--;
	p->p4 = TS2K_CTOI(ans[i]);	ans[i] = '\0';	i -= 6;
	p->p3 = atoi(&ans[i]);		ans[i] = '\0';	i -= 4;
	p->p2 = atoi(&ans[i]);		ans[i] = '\0';	i -= 11;
	p->p1 = strtol(&ans[2], NULL, 10);
	return ret;
}

/*
 * ts2k_?_type34()
 *	status:	s=unchecked; g=unchecked
 *	format:	{CMD, BIN, INT3, FREQ, INT1, BIN, INT1,
 *		INT2, INT2, INT3, BIN, INT1, LEN, 9, INT,
 *				INT2, INT1, TXT8, END}
 */
int ts2k_s_type34(RIG *rig, ts2k_type34_t *p, char *cmd)
{
	// This test string has been verified to work!
	char tstcmd[] = "mw009000145440000402292900002059950000005Dale;";
//	char buf[20];
	char buf[100];	// debugging

#define SEND(_A_ , _B_, _M_)	anslen = sprintf(ans, _A_, ( _B_ > _M_)? _M_ : _B_); \
			ret = ts2k_transaction(rig, ans, anslen, NULL, NULL); \
			if(ret != RIG_OK) { \
				ts2k_transaction(rig, ";", 1, NULL, NULL); \
				return ret; \
			}

//	TS2K_A_DATA(TS2K_MW_SIZE);
	TS2K_A_DATA(TS2K_MW_SIZE * 2);	// big buffer for debugging

//	rig_debug(RIG_DEBUG_VERBOSE, __FUNCTION__
//		":\tchan # = %i\n", p->p2p3);

// debug for above test string:	anslen = sprintf(ans, &tstcmd[0]);

// The following is ugly (lots of code and calls).
// I'll do something better soon.  But, it Works!
	ts2k_transaction(rig, cmd, 2, NULL, NULL);

	SEND("%01u", p->p1,  9);	// P1	Rx/Tx
	SEND("%03u", p->p2p3, 300);	// P2,P3, channel #
	SEND("%011llu", p->p4, 99999999999);	// P4, freq
	SEND("%01u", p->p5,  9);	// P5,
	SEND("%01u", p->p6,  9);	// P6,
	SEND("%01u", p->p7,  9);	// P7,
	SEND("%02u", p->p8,  99);	// P8, tone
	SEND("%02u", p->p9,  99);	// P9, ctcss
	SEND("%03u", p->p10, 999);	// P10, dcs
	SEND("%01u", p->p11, 9);	// P11, 
	SEND("%01u", p->p12, 9);	// P12, 
	SEND("%09u", p->p13, 999999999);// P13, offset freq
	SEND("%02u", p->p14, 99);	// P14, 
	SEND("%01u", p->p15, 9);	// P15, group

#undef SEND

	ts2k_transaction(rig, p->p16, 8, NULL, NULL);
	ts2k_transaction(rig, ";", 1, NULL, NULL);

/* Do NOT uncomment the following without rewriting the above! */
//	anslen++;
//	ret = TS2K_SEND;

	return ret;
}
int ts2k_g_type34(RIG *rig, ts2k_type34_t *p, char *cmd)
{
	int i, j;
	TS2K_A_DATA(TS2K_MR_SIZE);
	char sdat[10];

	// Request is not type08, unlike 99% of commands
	sprintf(sdat, "%s%01i%03i;", cmd, p->p1, p->p2p3);
	anslen = TS2K_MR_SIZE;
	ret = TS2K_READ(rig, sdat, TS2K_MR_SIZE);

rig_debug(RIG_DEBUG_VERBOSE, __FUNCTION__
		":\tchan # = %i\n", p->p2p3);

	i = TS2K_MR_SIZE - 10;

	// Remove semicolon from end
	for(j=0; j<10; j++) {
		if(ans[i+j] == ';') {
			ans[i+j] = '\0';
			break;
		}
	}
rig_debug(RIG_DEBUG_VERBOSE, __FUNCTION__
		":\tans.CallSign = %s\n", &ans[i]);
	strncpy(&p->p16[0], &ans[i], 9);ans[i] = '\0';	i --;
	p->p15 = TS2K_CTOI(ans[i]); 			i -= 2;
	p->p14 = atoi(&ans[i]); 	ans[i] = '\0';	i -= 9;
	p->p13 = atoi(&ans[i]);	 			i--;
	p->p12 = TS2K_CTOI(ans[i]); 	 		i--;
	p->p11 = TS2K_CTOI(ans[i]); 	ans[i] = '\0';	i -= 3;
	p->p10 = atoi(&ans[i]); 	ans[i] = '\0';	i -= 2;
	p->p9 =  atoi(&ans[i]); 	ans[i] = '\0';	i -= 2;
	p->p8 =  atoi(&ans[i]); 			i--;
	p->p7 =  TS2K_CTOI(ans[i]); 			i--;
	p->p6 =	 TS2K_CTOI(ans[i]); 			i--;
	p->p5 =  TS2K_CTOI(ans[i]);	ans[i] = '\0';	i -= 11; 
	p->p4 =  atoi(&ans[i]); 	ans[i] = '\0';	i -= 3;
	p->p2p3 =  atoi(&ans[i]);  			i--;
	p->p1 =	 TS2K_CTOI(ans[i]);

	return ret;
}
// Whew!

/************************** Specific commands *********************/

/* ac:	tuner status */
int ts2k_s_ac(RIG *rig, TS2K_AC_T *p)
{
	return ts2k_s_type23(rig, p, "ac");
}
int ts2k_g_ac(RIG *rig, TS2K_AC_T *p)
{
	return ts2k_g_type23(rig, p, "ac;");
}

/* ag: AF gain level on Main/Sub */
int ts2k_s_ag(RIG *rig, TS2K_AG_T *p)
{
	return ts2k_s_type16(rig, p, "ag");
}
int ts2k_g_ag(RIG *rig, TS2K_AG_T *p)	// Type01 read!
{
	return ts2k_g_type16(rig, p, "ag;");
}

/* ai: auto information */
int ts2k_s_ai(RIG *rig, TS2K_AI_T *p)
{
	return ts2k_s_type04(rig, p, "ai");
}
int ts2k_g_ai(RIG *rig, TS2K_AI_T *p)
{
	return ts2k_g_type04(rig, p, "ai;");
}

/* al: auto notch level */
int ts2k_s_al(RIG *rig, TS2K_AL_T *p)
{
	return ts2k_s_type06(rig, p, "al"); 
}
int ts2k_g_al(RIG *rig, TS2K_AL_T *p)
{
	return ts2k_g_type06(rig, p, "al;");
}

/* am: auto mode */
int ts2k_s_am(RIG *rig, TS2K_AM_T *p)
{
	return ts2k_s_type01(rig, p, "am");
}
int ts2k_g_am(RIG *rig, TS2K_AM_T *p)
{
	return ts2k_g_type01(rig, p, "am;");
}

/* an: antenna */
int ts2k_s_an(RIG *rig, TS2K_AN_T *p)
{
	return ts2k_s_type04(rig, p, "an");
}
int ts2k_g_an(RIG *rig, TS2K_AN_T *p)
{
	return ts2k_g_type04(rig, p, "an;");
}

/* ar: ASC status */
int ts2k_s_ar(RIG *rig, TS2K_AR_T *p)
{
	return ts2k_s_type20(rig, p, "ar");
}
int ts2k_g_ar(RIG *rig, TS2K_AR_T *p)	// Type01 read!
{
	return ts2k_g_type20(rig, p, "ar;");
}

/* as: Auto Mode parameters */
int ts2k_s_as(RIG *rig, TS2K_AS_T *p)
{
	return ts2k_s_type25(rig, p, "as");
}
int ts2k_g_as(RIG *rig, TS2K_AS_T *p)	// Type14 read!
{
	return ts2k_g_type25(rig, p, "as;");
}

/* bc: beat canceller */
int ts2k_s_bc(RIG *rig, TS2K_BC_T *p)
{
	return ts2k_s_type04(rig, p, "bc");
}
int ts2k_g_bc(RIG *rig, TS2K_BC_T *p)
{
	return ts2k_g_type04(rig, p, "bc;");
}

/* bd:	band down */
int ts2k_s_bd(RIG *rig, TS2K_BD_T *p)
{
	return ts2k_s_type08(rig, p, "bd;");
}
int ts2k_g_bd(RIG *rig, TS2K_BD_T *p)
{
	return -RIG_ENAVAIL;
}

/* bp: beat canceller */
int ts2k_s_bp(RIG *rig, TS2K_BP_T *p)
{
	return ts2k_s_type06(rig, p, "bp");
}
int ts2k_g_bp(RIG *rig, TS2K_BP_T *p)
{
	return ts2k_g_type06(rig, p, "bp;");
}

/* bu:	band up */
int ts2k_s_bu(RIG *rig, TS2K_BU_T *p)
{
	return ts2k_s_type08(rig, p, "bu;");
}
int ts2k_g_bu(RIG *rig, TS2K_BD_T *p)
{
	return -RIG_ENAVAIL;
}

/* by: Busy */
int ts2k_s_by(RIG *rig, TS2K_BY_T *p)
{
	return -RIG_ENAVAIL;
}
int ts2k_g_by(RIG *rig, TS2K_DC_T *p)
{
	return ts2k_g_type09(rig, p, "by;");
}

/* ca: CW autotune */
int ts2k_s_ca(RIG *rig, TS2K_CA_T *p)
{
	return ts2k_s_type01(rig, p, "ca");
}
int ts2k_g_ca(RIG *rig, TS2K_CA_T *p)
{
	return ts2k_g_type01(rig, p, "ca;");
}

/* cg: carrier gain */
int ts2k_s_cg(RIG *rig, TS2K_CG_T *p)
{
	return ts2k_s_type06(rig, p, "cg");
}
int ts2k_g_cg(RIG *rig, TS2K_CG_T *p)
{
	return ts2k_g_type06(rig, p, "cg;");
}

/* ch: multi/ch up/down */
int ts2k_s_ch(RIG *rig, TS2K_CH_T *p)
{
	return ts2k_s_type01(rig, p, "ch");
}
int ts2k_g_ch(RIG *rig, TS2K_CH_T *p)
{
	return -RIG_ENAVAIL;
}

/* ci: CALL set (vfo/mem->call) */
int ts2k_s_ci(RIG *rig, TS2K_CI_T *p)
{
	return ts2k_s_type08(rig, p, "ci;");
}
int ts2k_g_ci(RIG *rig, TS2K_CI_T *p)
{
	return -RIG_ENAVAIL;
}

/* cm: P.C.T. on/off */
int ts2k_s_cm(RIG *rig, TS2K_CM_T *p)
{
	return ts2k_s_type01(rig, p, "cm");
}
int ts2k_g_cm(RIG *rig, TS2K_CM_T *p)
{
	return ts2k_g_type01(rig, p, "cm;");
}

/* cn: CTCSS tone (page 35 for table) */
int ts2k_s_cn(RIG *rig, TS2K_CN_T *p)
{
	return ts2k_s_type05(rig, p, "cn");
}
int ts2k_g_cn(RIG *rig, TS2K_CN_T *p)
{
	return ts2k_g_type05(rig, p, "cn;");
}

/* ct: CTCSS on/off */
int ts2k_s_ct(RIG *rig, TS2K_CT_T *p)
{
	return ts2k_s_type01(rig, p, "ct");
}
int ts2k_g_ct(RIG *rig, TS2K_CT_T *p)
{
	return ts2k_g_type01(rig, p, "ct;");
}

/* dc: PTT/CTRL for Main/Sub */
int ts2k_s_dc(RIG *rig, TS2K_DC_T *p)
{
	return ts2k_s_type09(rig, p, "dc");
}
int ts2k_g_dc(RIG *rig, TS2K_DC_T *p)
{
	return ts2k_g_type09(rig, p, "dc;");
}

/* dn: Down (mic down emulation) */
int ts2k_s_dn(RIG *rig, TS2K_DN_T *p)
{
	return ts2k_s_type05(rig, p, "dn");
}
int ts2k_g_dn(RIG *rig, TS2K_DN_T *p)
{
	return -RIG_ENAVAIL;
}

/* dq: DCS on/off */
int ts2k_s_dq(RIG *rig, TS2K_DQ_T *p)
{
	return ts2k_s_type01(rig, p, "dq");
}
int ts2k_g_dq(RIG *rig, TS2K_DQ_T *p)
{
	return ts2k_g_type01(rig, p, "dq;");
}

/* ex: Menu */
/* See ts2k_menu.[ch].  This command warrants its own routines. */
int ts2k_s_ex(RIG *rig, TS2K_EX_T *p)
{
	return ts2k_s_type30(rig, p, "ex");
}
int ts2k_g_ex(RIG *rig, TS2K_EX_T *p)	// Type24 read!
{
	return ts2k_g_type30(rig, p, "ex");
}

/* fa-c: set freq */
int ts2k_s_fa(RIG *rig, TS2K_FA_T *p)
{
	return ts2k_s_type02(rig, p, "fa");
}
int ts2k_g_fa(RIG *rig, TS2K_FA_T *p)
{
	return ts2k_g_type02(rig, p, "fa;");
}
int ts2k_s_fb(RIG *rig, TS2K_FB_T *p)
{
	return ts2k_s_type02(rig, p, "fb");
}
int ts2k_g_fb(RIG *rig, TS2K_FB_T *p)
{
	return ts2k_g_type02(rig, p, "fb;");
}
int ts2k_s_fc(RIG *rig, TS2K_FC_T *p)
{
	return ts2k_s_type02(rig, p, "fc");
}
int ts2k_g_fc(RIG *rig, TS2K_FC_T *p)
{
	return ts2k_g_type02(rig, p, "fc;");
}

/* fd: Filter Width Display */
int ts2k_s_fd(RIG *rig, TS2K_FD_T *p)
{
	return -RIG_ENAVAIL;
}
int ts2k_g_fd(RIG *rig, TS2K_FD_T *p)
{
	return ts2k_g_type21(rig, p, "fd;");
}

/* fr: read rx vfo */
int ts2k_s_fr(RIG *rig, TS2K_FR_T *p)
{
	return ts2k_s_type04(rig, p, "fr");
}
int ts2k_g_fr(RIG *rig, TS2K_FR_T *p)
{
	return ts2k_g_type04(rig, p, "fr;");
}

/* fs: Fine on/off */
int ts2k_s_fs(RIG *rig, TS2K_FS_T *p)
{
	return ts2k_s_type01(rig, p, "fs");
}
int ts2k_g_fs(RIG *rig, TS2K_FS_T *p)
{
	return ts2k_g_type01(rig, p, "fs;");
}

/* ft: set/read tx vfo */
int ts2k_s_ft(RIG *rig, TS2K_FT_T *p)
{
	return ts2k_s_type04(rig, p, "ft");
}
int ts2k_g_ft(RIG *rig, TS2K_FT_T *p)
{
	return ts2k_g_type04(rig, p, "ft;");
}

/* fw: DSP receive filter width */
int ts2k_s_fw(RIG *rig, TS2K_FW_T *p)
{
	return ts2k_s_type07(rig, p, "fw");
}
int ts2k_g_fw(RIG *rig, TS2K_FW_T *p)
{
	return ts2k_g_type07(rig, p, "fw;");
}

/* gt: AGC speed */
int ts2k_s_gt(RIG *rig, TS2K_GT_T *p)
{
	return ts2k_s_type06(rig, p, "gt");
}
int ts2k_g_gt(RIG *rig, TS2K_GT_T *p)
{
	return ts2k_g_type06(rig, p, "gt;");
}

/* id: identifier */
int ts2k_s_id(RIG *rig, TS2K_ID_T *p)
{
	return -RIG_ENAVAIL;
}
int ts2k_g_id(RIG *rig, TS2K_ID_T *p)
{
	return ts2k_g_type06(rig, p, "id;");
}

/* if: Transceiver Status (of CTRL side) (see also: oi) */
int ts2k_s_if(RIG *rig, TS2K_IF_T *p)
{
	return -RIG_ENAVAIL;
}
int ts2k_g_if(RIG *rig, TS2K_IF_T *p)
{
	return ts2k_g_type33(rig, p, "if;");
}

/* is: IF Shift status (CW only) */
int ts2k_s_is(RIG *rig, TS2K_IS_T *p)
{
	return ts2k_s_type26(rig, p, "is");
}
int ts2k_g_is(RIG *rig, TS2K_IS_T *p)
{
	return ts2k_g_type26(rig, p, "is;");
}

/* ks: keying speed */
int ts2k_s_ks(RIG *rig, TS2K_KS_T *p)
{
	return ts2k_s_type06(rig, p, "ks");
}
int ts2k_g_ks(RIG *rig, TS2K_KS_T *p)
{
	return ts2k_g_type06(rig, p, "ks;");
}

/* ky: Convert and send morse code */
int ts2k_s_ky(RIG *rig, TS2K_KY_S_T *p)
{
	return ts2k_s_type28(rig, p, "ky");
}
int ts2k_g_ky(RIG *rig, TS2K_KY_A_T *p)
{
	return ts2k_g_type01(rig, p, "ky;");
}

/* lk: Lock */
int ts2k_s_lk(RIG *rig, TS2K_LK_T *p)
{
	return ts2k_s_type12(rig, p, "lk");
}
int ts2k_g_lk(RIG *rig, TS2K_LK_T *p)
{
	return ts2k_g_type12(rig, p, "lk;");
}

/* lm: keyer recording status */
int ts2k_s_lm(RIG *rig, TS2K_LM_T *p)
{
	return ts2k_s_type04(rig, p, "lm");
}
int ts2k_g_lm(RIG *rig, TS2K_LM_T *p)
{
	return ts2k_g_type04(rig, p, "lm;");
}

/* lt: ALT on/off */
int ts2k_s_lt(RIG *rig, TS2K_LT_T *p)
{
	return ts2k_s_type01(rig, p, "lt");
}
int ts2k_g_lt(RIG *rig, TS2K_LT_T *p)
{
	return ts2k_g_type01(rig, p, "lt;");
}

/* mc: memory channel number get/set */
int ts2k_s_mc(RIG *rig, TS2K_MC_T *p)
{
	return ts2k_s_type06(rig, p, "mc");
}
int ts2k_g_mc(RIG *rig, TS2K_MC_T *p)
{
	return ts2k_g_type06(rig, p, "mc;");
}

/* md: mode get/set */
int ts2k_s_md(RIG *rig, TS2K_MD_T *p)
{
	return ts2k_s_type04(rig, p, "md");
}
int ts2k_g_md(RIG *rig, TS2K_MD_T *p)
{
	return ts2k_g_type04(rig, p, "md;");
}

/* mf: menu A/B get/set */
int ts2k_s_mf(RIG *rig, TS2K_MF_T *p)
{
	return ts2k_s_type01(rig, p, "mf");
}
int ts2k_g_mf(RIG *rig, TS2K_MF_T *p)
{
	return ts2k_g_type01(rig, p, "mf;");
}

/* mg: Microphone gain */
int ts2k_s_mg(RIG *rig, TS2K_MG_T *p)
{
	return ts2k_s_type06(rig, p, "mg");
}
int ts2k_g_mg(RIG *rig, TS2K_MG_T *p)
{
	return ts2k_g_type06(rig, p, "mg;");
}

/* ml: monitor level (normal mode) */
int ts2k_s_ml(RIG *rig, TS2K_ML_T *p)
{
	return ts2k_s_type06(rig, p, "ml");
}
int ts2k_g_ml(RIG *rig, TS2K_ML_T *p)
{
	return ts2k_g_type06(rig, p, "ml;");
}

/* mo: monitor level (sky command mode)*/
int ts2k_s_mo(RIG *rig, TS2K_MO_T *p)
{
	return ts2k_s_type01(rig, p, "mo");
}
int ts2k_g_mo(RIG *rig, TS2K_MO_T *p)
{
	return ts2k_g_type01(rig, p, "mo;");
}

/* mr: Memory Channel Read */
int ts2k_s_mr(RIG *rig, TS2K_MR_T *p)
{
	return -RIG_ENAVAIL;
}
int ts2k_g_mr(RIG *rig, TS2K_MR_T *p)	// Type16 read!
{
	return ts2k_g_type34(rig, p, "mr");
}

/* mu: Memory Group */
int ts2k_s_mu(RIG *rig, TS2K_MU_T *p)
{
	return ts2k_s_type27(rig, p, "mu");
}
int ts2k_g_mu(RIG *rig, TS2K_MU_T *p)
{
	return ts2k_g_type27(rig, p, "mu;");
}

/* mw: Memory Channel Write */
int ts2k_s_mw(RIG *rig, TS2K_MW_T *p)
{
	return ts2k_s_type34(rig, p, "mw");
}
int ts2k_g_mw(RIG *rig, TS2K_MW_T *p)
{
	return -RIG_ENAVAIL;
}

/* nb: noise blanker on/off */
int ts2k_s_nb(RIG *rig, TS2K_NB_T *p)
{
	return ts2k_s_type01(rig, p, "nb");
}
int ts2k_g_nb(RIG *rig, TS2K_NB_T *p)
{
	return ts2k_g_type01(rig, p, "nb;");
}

/* nl: noise blanker level */
int ts2k_s_nl(RIG *rig, TS2K_NL_T *p)
{
	return ts2k_s_type06(rig, p, "nl");
}
int ts2k_g_nl(RIG *rig, TS2K_NL_T *p)
{
	return ts2k_g_type06(rig, p, "nl;");
}

/* nr: noise reduction */
int ts2k_s_nr(RIG *rig, TS2K_NR_T *p)
{
	return ts2k_s_type06(rig, p, "nr");
}
int ts2k_g_nr(RIG *rig, TS2K_NR_T *p)
{
	return ts2k_g_type06(rig, p, "nr;");
}

/* nt: autonotch on/off */
int ts2k_s_nt(RIG *rig, TS2K_NT_T *p)
{
	return ts2k_s_type01(rig, p, "nt");
}
int ts2k_g_nt(RIG *rig, TS2K_NT_T *p)
{
	return ts2k_g_type01(rig, p, "nt;");
}

/* of: Offset Frequency */
int ts2k_s_of(RIG *rig, TS2K_OF_T *p)
{
	return ts2k_s_type22(rig, p, "of");
}
int ts2k_g_of(RIG *rig, TS2K_OF_T *p)
{
	return ts2k_g_type22(rig, p, "of;");
}

/* oi is actually identical to "if;" but for the non-CTRL transceiver */
/* oi: Read Memory Channel Data */
int ts2k_s_oi(RIG *rig, TS2K_OI_T *p)
{
	return -RIG_ENAVAIL;
}
int ts2k_g_oi(RIG *rig, TS2K_OI_T *p)
{
	return ts2k_g_type33(rig, p, "oi;");
}

/* os: repeater offset (+,-,=) */
int ts2k_s_os(RIG *rig, TS2K_OS_T *p)
{
	return ts2k_s_type04(rig, p, "os");
}
int ts2k_g_os(RIG *rig, TS2K_OS_T *p)
{
	return ts2k_g_type04(rig, p, "os;");
}

/* pa: Pre-Amp on/off for Main/Sub */  
int ts2k_s_pa(RIG *rig, TS2K_PA_S_T *p)
{
	return ts2k_g_type01(rig, p, "pa");
}
int ts2k_g_pa(RIG *rig, TS2K_PA_A_T *p)
{
	return ts2k_g_type09(rig, p, "pa;");
}

/* pb: playback */
int ts2k_s_pb(RIG *rig, TS2K_PB_T *p)
{
	return ts2k_s_type04(rig, p, "pb");
}
int ts2k_g_pb(RIG *rig, TS2K_PB_T *p)
{
	return ts2k_g_type04(rig, p, "pb;");
}

/* pc: output power */
int ts2k_s_pc(RIG *rig, TS2K_PC_T *p)
{
	return ts2k_s_type06(rig, p, "pc");
}
int ts2k_g_pc(RIG *rig, TS2K_PC_T *p)
{
	return ts2k_g_type06(rig, p, "pc;");
}

/* pi: programmable memory store */
int ts2k_s_pi(RIG *rig, TS2K_PI_T *p)
{
	return ts2k_s_type04(rig, p, "pi");
}
int ts2k_g_pi(RIG *rig, TS2K_PI_T *p)
{
	return -RIG_ENAVAIL;
}

/* pk: Read P.C.T. Data */  
int ts2k_s_pk(RIG *rig, TS2K_PK_T *p)
{
	return -RIG_ENAVAIL;
}
int ts2k_g_pk(RIG *rig, TS2K_PK_T *p)
{
	// May no use type08 for read!
	return ts2k_g_type32(rig, p, "pk;");
}

/* pl: Speech Processor */  
int ts2k_s_pl(RIG *rig, TS2K_PL_T *p)
{
	return ts2k_s_type17(rig, p, "pl");
}
int ts2k_g_pl(RIG *rig, TS2K_PL_T *p)
{
	return ts2k_g_type17(rig, p, "pl;");
}

/* pm: programmable memory set */
int ts2k_s_pm(RIG *rig, TS2K_PM_T *p)
{
	return ts2k_s_type04(rig, p, "pm");
}
int ts2k_g_pm(RIG *rig, TS2K_PM_T *p)
{
	return ts2k_g_type04(rig, p, "pm;");
}

/* pr: speech processor on/off */
int ts2k_s_pr(RIG *rig, TS2K_PR_T *p)
{
	return ts2k_s_type01(rig, p, "pr");
}
int ts2k_g_pr(RIG *rig, TS2K_PR_T *p)
{
	return ts2k_g_type01(rig, p, "pr;");
}

/* ps: power on/off */
int ts2k_s_ps(RIG *rig, TS2K_PS_T *p)
{
	return ts2k_s_type01(rig, p, "ps");
}
int ts2k_g_ps(RIG *rig, TS2K_PS_T *p)
{
	return ts2k_g_type01(rig, p, "ps;");
}

/* qc: DCS code */
int ts2k_s_qc(RIG *rig, TS2K_QC_T *p)
{
	return ts2k_s_type06(rig, p, "qc");
}
int ts2k_g_qc(RIG *rig, TS2K_QC_T *p)
{
	return ts2k_g_type06(rig, p, "qc;");
}

/* qi: Quick Memory Set (Q.IN) */
int ts2k_s_qi(RIG *rig, TS2K_QI_T *p)
{
	return ts2k_s_type08(rig, p, "qi;");
}
int ts2k_g_qi(RIG *rig, TS2K_QI_T *p)
{
	return -RIG_ENAVAIL;
}

/* qr: Quick Memory Recall (upper MR) */ 
int ts2k_s_qr(RIG *rig, TS2K_QR_T *p)
{
	return ts2k_s_type11(rig, p, "qr");
}
int ts2k_g_qr(RIG *rig, TS2K_QR_T *p)
{
	return ts2k_g_type11(rig, p, "qr");
}

/* ra: Attenuator */ 
int ts2k_s_ra(RIG *rig, TS2K_RA_S_T *p)
{
	return ts2k_s_type05(rig, p, "ra");
}
int ts2k_g_ra(RIG *rig, TS2K_RA_A_T *p)
{
	return ts2k_g_type15(rig, p, "ra");
}

/* rc: RIT/XIT frequency Clear */ 
int ts2k_s_rc(RIG *rig, TS2K_RC_T *p)
{
	return ts2k_s_type08(rig, p, "rc;");
}
int ts2k_g_rc(RIG *rig, TS2K_RC_T *p)
{
	return -RIG_ENAVAIL;
}

/* rd: RIT/XIT frequency Down */ 
int ts2k_s_rd(RIG *rig, TS2K_RD_S_T *p)
{
	return ts2k_s_type03(rig, p, "rd");
}
int ts2k_g_rd(RIG *rig, TS2K_RD_A_T *p)
{
	return ts2k_s_type04(rig, p, "rd");
}

/* rg: RF gain */
int ts2k_s_rg(RIG *rig, TS2K_RG_T *p)
{
	return ts2k_s_type06(rig, p, "rg");
}
int ts2k_g_rg(RIG *rig, TS2K_RG_T *p)
{
	return ts2k_g_type06(rig, p, "rg;");
}

/* rl: Noise Reduction (NR) level */ 
int ts2k_s_rl(RIG *rig, TS2K_RL_T *p)
{
	return ts2k_s_type05(rig, p, "rl");
}
int ts2k_g_rl(RIG *rig, TS2K_RL_T *p)
{
	return ts2k_g_type05(rig, p, "rl;");
}

/* rm: Meter Scale */ 
int ts2k_s_rm(RIG *rig, TS2K_RM_S_T *p)
{
	return ts2k_s_type04(rig, p, "rm");
}
int ts2k_g_rm(RIG *rig, TS2K_RM_A_T *p)
{
	return ts2k_g_type18(rig, p, "rm;");
}

/* rt: RIT on/off */
int ts2k_s_rt(RIG *rig, TS2K_RT_T *p)
{
	return ts2k_s_type01(rig, p, "rt");
}
int ts2k_g_rt(RIG *rig, TS2K_RT_T *p)
{
	return ts2k_g_type01(rig, p, "rt;");
}

/* ru: RIT/XIT Up */ 
int ts2k_s_ru(RIG *rig, TS2K_RU_S_T *p)
{
	return ts2k_s_type03(rig, p, "ru");
}
int ts2k_s_ru_alt(RIG *rig, TS2K_RU_S2_T *p)
{
	return ts2k_s_type08(rig, p, "ru");
}
int ts2k_g_ru(RIG *rig, TS2K_RU_A_T *p)
{
	return ts2k_s_type04(rig, p, "ru");
}

/* rx: Switch transceiver from TX to RX (Main/Sub) */ 
int ts2k_s_rx(RIG *rig, TS2K_RX_T *p)
{
	return ts2k_s_type08(rig, p, "rx");
}
int ts2k_g_rx(RIG *rig, TS2K_RX_T *p)
{
	return -RIG_ENAVAIL;
}

/* sa: Satellite mode */ 
int ts2k_s_sa(RIG *rig, TS2K_SA_S_T *p)
{
	return ts2k_s_type29(rig, p, "sa");
}
int ts2k_g_sa(RIG *rig, TS2K_SA_A_T *p)
{
	return ts2k_g_type31(rig, p, "sa;");
}

/* sb: Sub TF-W on/off */
int ts2k_s_sb(RIG *rig, TS2K_SB_T *p)
{
	return ts2k_s_type01(rig, p, "sb");
}
int ts2k_g_sb(RIG *rig, TS2K_SB_T *p)
{
	return ts2k_g_type01(rig, p, "sb;");
}

/* sc: scan */
int ts2k_s_sc(RIG *rig, TS2K_SC_T *p)
{
	return ts2k_s_type04(rig, p, "sc");
}
int ts2k_g_sc(RIG *rig, TS2K_SC_T *p)
{
	return ts2k_g_type04(rig, p, "sc;");
}

/* sd: CW Break-in Delay */ 
int ts2k_s_sd(RIG *rig, TS2K_SD_T *p)
{
	return ts2k_s_type07(rig, p, "sd");
}
int ts2k_g_sd(RIG *rig, TS2K_SD_T *p)
{
	return ts2k_g_type07(rig, p, "sd;");
}

/* sh: DSP Hi filter frequency (transmit? -- see fw;) */
int ts2k_s_sh(RIG *rig, TS2K_SH_T *p)
{
	return ts2k_s_type05(rig, p, "sh");
}
int ts2k_g_sh(RIG *rig, TS2K_SH_T *p)
{
	return ts2k_g_type05(rig, p, "sh;");
}

/* si: Satellite channel Name */
int ts2k_s_si(RIG *rig, TS2K_SI_T *p)
{
	return ts2k_s_type19(rig, p, "si");
}
int ts2k_g_si(RIG *rig, TS2K_SI_T *p)
{
	return -RIG_ENAVAIL;
}

/* sl: DSP Low filter frequency (transmit? -- see fw;) */
int ts2k_s_sl(RIG *rig, TS2K_SL_T *p)
{
	return ts2k_s_type05(rig, p, "sl");
}
int ts2k_g_sl(RIG *rig, TS2K_SL_T *p)
{
	return ts2k_g_type05(rig, p, "sl;");
}

/* sm: S-Meter Value (Main/Sub) */
int ts2k_s_sm(RIG *rig, TS2K_SM_T *p)
{
	return ts2k_s_type18(rig, p, "sm");
}
int ts2k_g_sm(RIG *rig, TS2K_SM_T *p)	// Type04 read!
{
	return ts2k_g_type18(rig, p, "sm;");
}

/* sq: Squelch */
int ts2k_s_sq(RIG *rig, TS2K_SQ_T *p)
{
	return ts2k_s_type11(rig, p, "sq");
}
int ts2k_g_sq(RIG *rig, TS2K_SQ_T *p)	// Type04 read!
{
	return ts2k_g_type11(rig, p, "sq");
}

/* sr: reset */
int ts2k_s_sr(RIG *rig, TS2K_SR_T *p)
{
	return ts2k_s_type04(rig, p, "sr");
}
int ts2k_g_sr(RIG *rig, TS2K_SR_T *p)
{
	return -RIG_ENAVAIL;
}


/* ss: program scan pause frequency set/clear */
int ts2k_s_ss(RIG *rig, TS2K_SS_T *p)
{
	return ts2k_s_type04(rig, p, "ss");
}
int ts2k_g_ss(RIG *rig, TS2K_SS_T *p)
{
	return ts2k_g_type04(rig, p, "ss;");
}

/* st: Step Frequency for Multi/Ch */
int ts2k_s_st(RIG *rig, TS2K_ST_T *p)
{
	return ts2k_s_type05(rig, p, "st");
}
int ts2k_g_st(RIG *rig, TS2K_ST_T *p)
{
	return ts2k_g_type05(rig, p, "st;");
}

/* su: program scan pause frequency */
int ts2k_s_su(RIG *rig, TS2K_SU_T *p)
{
	return ts2k_s_type27b(rig, p, "su");
}
int ts2k_g_su(RIG *rig, TS2K_SU_T *p)	// Type01 read!
{
	return ts2k_g_type27b(rig, p, "su;");
}

/* sv: memory transfer */
int ts2k_s_sv(RIG *rig, TS2K_SV_T *p)
{
	return ts2k_s_type08(rig, p, "sv;");
}
int ts2k_g_sv(RIG *rig, TS2K_SV_T *p)
{
	return -RIG_ENAVAIL;
}

/* tc: tnc */
int ts2k_s_tc(RIG *rig, TS2K_TC_T *p)
{
	return ts2k_s_type10(rig, p, "tc ");
}
int ts2k_g_tc(RIG *rig, TS2K_TC_T *p)
{
	return ts2k_s_type10(rig, p, "tc;");
}

/* td: DTMF send */
int ts2k_s_td(RIG *rig, TS2K_TD_T *p)
{
	return ts2k_s_type05(rig, p, "td;");
}
int ts2k_g_td(RIG *rig, TS2K_TD_T *p)
{
	return -RIG_ENAVAIL;
}

/* ti: tnc led */
int ts2k_s_ti(RIG *rig, TS2K_TI_T *p)
{
	return -RIG_ENAVAIL;
}
int ts2k_g_ti(RIG *rig, TS2K_TI_T *p)
{
	return ts2k_g_type20(rig, p, "ti;");
}

/* tn: tone frequency */
int ts2k_s_tn(RIG *rig, TS2K_TN_T *p)
{
	return ts2k_s_type05(rig, p, "tn");
}
int ts2k_g_tn(RIG *rig, TS2K_TN_T *p)
{
	return ts2k_g_type05(rig, p, "tn;");
}

/* to: tone on/off */
int ts2k_s_to(RIG *rig, TS2K_TO_T *p)
{
	return ts2k_s_type04(rig, p, "to");
}
int ts2k_g_to(RIG *rig, TS2K_TO_T *p)
{
	return ts2k_g_type04(rig, p, "to;");
}

/* ts: TF-SET toggle,on,off (see manual) */
int ts2k_s_ts(RIG *rig, TS2K_TS_T *p)
{
	return ts2k_s_type01(rig, p, "ts");
}
int ts2k_g_ts(RIG *rig, TS2K_TS_T *p)
{
	return ts2k_g_type01(rig, p, "ts;");
}

/* tx: Switches transceiver from RX to TX */
int ts2k_s_tx(RIG *rig, TS2K_TX_T *p)
{
	return ts2k_s_type08(rig, p, "tx");
}
int ts2k_g_tx(RIG *rig, TS2K_TX_T *p)
{
	return -RIG_ENAVAIL;
}

/* ty: Firmware type */
int ts2k_s_ty(RIG *rig, TS2K_TY_T *p)
{
	return -RIG_ENAVAIL;
}
int ts2k_g_ty(RIG *rig, TS2K_TY_T *p)
{
	return ts2k_g_type13(rig, p, "ty;");
}

/* ul: PLL lock status */
int ts2k_s_ul(RIG *rig, TS2K_UL_T *p)
{
	return -RIG_ENAVAIL;
}
int ts2k_g_ul(RIG *rig, TS2K_UL_T *p)
{
	return ts2k_g_type01(rig, p, "ul;");
}

/* up: mic up key emulation */
int ts2k_s_up(RIG *rig, TS2K_UP_T *p)
{
	return ts2k_s_type05(rig, p, "up;");
}
int ts2k_s_up_alt(RIG *rig, TS2K_UP_T *p)
{
	return ts2k_s_type08(rig, NULL, "up;");
}
int ts2k_g_up(RIG *rig, TS2K_UP_T *p)
{
	return -RIG_ENAVAIL;
}

/* vd: VOX delay */
int ts2k_s_vd(RIG *rig, TS2K_VD_T *p)
{
	return ts2k_s_type07(rig, p, "vd");
}
int ts2k_g_vd(RIG *rig, TS2K_VD_T *p)
{
	return ts2k_g_type07(rig, p, "vd;");
}

/* vg: VOX gain */
int ts2k_s_vg(RIG *rig, TS2K_VG_T *p)
{
	return ts2k_s_type06(rig, p, "vg");
}
int ts2k_g_vg(RIG *rig, TS2K_VG_T *p)
{
	return ts2k_g_type06(rig, p, "vg;");
}

/* vr: Voice 1/2 enable */
int ts2k_s_vr(RIG *rig, TS2K_VR_T *p)
{
	return ts2k_s_type01(rig, p, "vr");
}
int ts2k_g_vr(RIG *rig, TS2K_VR_T *p)
{
	return -RIG_ENAVAIL;
}

/* vx: VOX on/off */
int ts2k_s_vx(RIG *rig, TS2K_VX_T *p)
{
	return ts2k_s_type01(rig, p, "vx");
}
int ts2k_g_vx(RIG *rig, TS2K_VX_T *p)
{
	return ts2k_g_type01(rig, p, "vx;");
}

/* xt: XIT on/off */
int ts2k_s_xt(RIG *rig, TS2K_XT_T *p)
{
	return ts2k_s_type01(rig, p, "xt");
}
int ts2k_g_xt(RIG *rig, TS2K_XT_T *p)
{
	return ts2k_g_type01(rig, p, "xt;");
}

// end, Finally!
