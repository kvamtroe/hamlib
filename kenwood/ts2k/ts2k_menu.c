/*
 * ts2k_menu.c	(C) Copyright 2002 by Dale E. Edmons.  All rights reserved.
 */

/*
 * License:	GNU
 */

/*
 * status:	Now compiles!  Working towards functional!
 */

/*
 * Functions to initialize, read, set, and list menus
 * for the TS-2000.  These functions will be added to
 * src/rig.c as rig_*menu_*() as time permits.
 *
 * This is likely a can of worms nobody has wanted to
 * deal with--until now.  The ts2k is especially a
 * pain as you'll soon seen (but cool nonetheless.)
 */

#include <string.h>
#include <ctype.h>
#include <hamlib/rig.h>
#include "ts2k/ts2k.h"
#include "ts2k/ts2k_menu.h"

/*
 * ts2k_menu_init()	Initialize one menu as in ts2k_menus[]
 */
int ts2k_menu_init(RIG * rig, ts2k_menu_t * Menus[])
{
	int retval, i, j, dummy;
	ts2k_menu_t *m, *mref;

	// One set of defaults has been globally defined (mref)
	rig_debug(RIG_DEBUG_VERBOSE, __FUNCTION__ ": begin...\n"); 
	rig_debug(RIG_DEBUG_VERBOSE, __FUNCTION__ ": TS2K_MENU_COUNT = %i\n", TS2K_MENU_COUNT); 
	for (i = 0; i < TS2K_MENU_COUNT; i++) {
		if( !(i%5) ) rig_debug(RIG_DEBUG_ERR, ".");
		m = Menus[i];
		mref = &ts2k_menus[i];

		/* Copy default values from mref */
		m->menu_no = mref->menu_no;
		m->txt = mref->txt;
		for( j=0; j<10; j++) {
			m->param[j] = mref->param[j];
		}
		for( j=0; j<4; j++) {
			m->menu[j] = mref->menu[j];
		}
		m->val = mref->val;
		
		rig_debug(RIG_DEBUG_VERBOSE, __FUNCTION__ ": m_item(%i)\n", i);

		/* NOTE: we may get called by ts2k_get_menu().  Be careful! */
		retval = ts2k_get_menu(rig, Menus, i, &dummy);
		if (retval != RIG_OK)
			return retval;
	}
#ifdef TS2K_LCD_DIMMEST
	rig_debug(RIG_DEBUG_VERBOSE, __FUNCTION__ ": Setting LCD to Dimmest.\n"); 
	retval = ts2k_set_menu(rig, Menus, 0, 1);
	if (retval != RIG_OK)
		return retval;
#endif
#ifdef TS2K_BEEP_OFF
	rig_debug(RIG_DEBUG_VERBOSE, __FUNCTION__ ": Setting Beep to Off.\n"); 
	retval = ts2k_set_menu(rig, Menus, 13, 0);
	if (retval != RIG_OK)
		return retval;
#endif
	rig_debug(RIG_DEBUG_VERBOSE, __FUNCTION__ ": end...\n"); 

	return RIG_OK;
}

/*
 * read the rig and get the parameters for menu[n].
 * convert them to a ts2k_menu_t, and eventually to
 * a rig_menu_t.  I do a lot of checking to ensure
 * nothing breaks (hi!).
 */
int ts2k_get_menu(RIG * rig, ts2k_menu_t *Menus[], int item, int *val)
{
	int retval, acklen, cmdlen, i;
	char ack[30], cmd[30];
	ts2k_menu_t *mnu;

	if (Menus == NULL)
		return -RIG_EINVAL;

	if(item >= TS2K_MENU_COUNT) {
		rig_debug(RIG_DEBUG_ERR, __FUNCTION__": parameter 3 too large.\n");
		return -RIG_EINVAL;
	}

	mnu = Menus[item];

	if((mnu->val == M_MENU_NOT_INITIALIZED)
	  || (mnu->menu[0] == M_MENU_NOT_INITIALIZED)) {
		rig_debug(RIG_DEBUG_ERR, __FUNCTION__ ": First access.  Initializing..");
		retval = ts2k_menu_init(rig, Menus);
		if(retval != RIG_OK)
			return -RIG_EINVAL;
		rig_debug(RIG_DEBUG_ERR, " done.\n");
	}

	cmdlen = sprintf(cmd, "ex%03i%02i%1i%1i;",
			mnu->menu[0], mnu->menu[1],
			mnu->menu[2], mnu->menu[3]);

	acklen = 30;
	retval = ts2k_transaction(rig, cmd, cmdlen, ack, &acklen);
	rig_debug(RIG_DEBUG_VERBOSE, __FUNCTION__ ": sent %s for %s\n",
					cmd, ts2k_list_menu(rig, mnu));
	if (retval != RIG_OK)
		return retval;

	/* P5 is variable length starting at ack[9] */
	switch(mnu->menu[0]*100 + mnu->menu[1]) {
	case 29*100 + 1:	// menu 29A
	case 31*100 + 0:	//	31
	case 33*100 + 0:	//	33
	case 51*100 + 1:	//	51A-E
	case 51*100 + 2:
	case 51*100 + 3:
	case 51*100 + 4:
	case 51*100 + 5:
	case 59*100 + 0:	//	59
	case 62*100 + 3:	//	62C
		mnu->val = int_n(&ack[9], 2);
		mnu->txt_val[0] = '\0';
		break;

	case 45*100 + 1:	//	45A
		rig_debug(RIG_DEBUG_VERBOSE, __FUNCTION__": Use DTMF function instead!\n");
//		return -RIG_ENAVAIL;
		break;

	case 62*100 + 1:
	case 62*100 + 2:
		for( i=0; i<8; i++) {
			if(ack[9+i] == ';')
				break;
			mnu->txt_val[i] = ack[9+i];
		}
		mnu->txt_val[i] = '\0';
		mnu->val = -1;
		break;
	
	case 61*100 + 3:
		mnu->val = int_n(&ack[9], 3);
		mnu->txt_val[0] = '\0';
		break;

	default:	// all others are one byte
		mnu->val = ack[9] - '0';
		mnu->txt_val[0] = '\0';
		break;
	}
	rig_debug(RIG_DEBUG_VERBOSE, __FUNCTION__ ": received %s for %s\n",
					ack, ts2k_list_menu(rig, mnu));

	return retval;
}

/*
 * ts2k_set_menu()	Just set the menu and run!
 */
int ts2k_set_menu(RIG * rig, ts2k_menu_t *Menus[], int item, int val)
{
	int retval, cmdlen, tmplen;
	char cmd[30], tmp[10];
	ts2k_menu_t *mnu;

	if (Menus == NULL)
		return -RIG_EINVAL;

	if(item >= TS2K_MENU_COUNT) {
		rig_debug(RIG_DEBUG_ERR, __FUNCTION__": parameter 3 too large.\n");
		return -RIG_EINVAL;
	}

	mnu = Menus[item];

	if((mnu->val == M_MENU_NOT_INITIALIZED)
	  || (mnu->menu[0] == M_MENU_NOT_INITIALIZED)) {
		rig_debug(RIG_DEBUG_ERR, __FUNCTION__ ": First access.  Initializing");
		retval = ts2k_menu_init(rig, Menus);
		if(retval != RIG_OK)
			return -RIG_EINVAL;
		rig_debug(RIG_DEBUG_ERR, " done.\n");
	}

	mnu->val = val;
	if(val != -1) mnu->txt_val[0] = '\0';

	/* Here, the string is only a partial result */
	cmdlen = 0;
	cmdlen += sprintf(cmd, "ex%03i%02i%1i%1i",
			mnu->menu[0], mnu->menu[1],
			mnu->menu[2], mnu->menu[3]);

	/* P5 is variable length starting at ack[9] */
	switch(mnu->menu[0]*100 + mnu->menu[1]) {
	case 29*100 + 1:	// menu 29A
	case 31*100 + 0:	//	31
	case 33*100 + 0:	//	33
	case 51*100 + 1:	//	51A-E
	case 51*100 + 2:
	case 51*100 + 3:
	case 51*100 + 4:
	case 51*100 + 5:
	case 59*100 + 0:	//	59
	case 62*100 + 3:	//	62C
		tmplen = sprintf(tmp, "%02i", val);
		strncat(cmd, tmp, tmplen);
		cmdlen += tmplen;
		break;

	case 45*100 + 1:	//	45A
		rig_debug(RIG_DEBUG_VERBOSE, __FUNCTION__": Use DTMF function instead!\n");
//		return -RIG_ENAVAIL;
		break;

	case 62*100 + 1:
	case 62*100 + 2:
		if(mnu->val != -1) {
			rig_debug(RIG_DEBUG_ERR, __FUNCTION__
				": error: Requires string in ->txt_val, and -1 in ->val\n");
			return -RIG_EINVAL;
		}
		tmplen = sprintf(tmp, "%s", mnu->txt_val);
		strncat(cmd, tmp, tmplen);
		cmdlen += tmplen;
		break;
	
	case 61*100 + 3:
		tmplen = sprintf(tmp, "%03i", val);
		strncat(cmd, tmp, tmplen);
		cmdlen += tmplen;
		break;

	default:	// all others are one byte
		tmp[0] = val + '0';
		tmp[1] = '\0';
		strncat(cmd, tmp, 2);
		cmdlen += 2;
		break;
	}

	strncat(cmd, ";", 2);
	cmdlen++;

	retval = ts2k_transaction(rig, cmd, cmdlen, NULL, NULL);
	rig_debug(RIG_DEBUG_VERBOSE, __FUNCTION__ ": sending %s for %s\n",
					cmd, ts2k_list_menu(rig, mnu));

	return retval;
}

/*
 * ts2k_list_menu()
 *	1) call ts2k_get_menu()
 *	2) Then call this function to get the text.
 *	3) Copy the text to a safe location.
 *
 *	Returns a static pointer to the built string.
 *	This string is destroyed on the next call.
 */
char * ts2k_list_menu(RIG *rig, ts2k_menu_t *menu)
{
	static char text[80] = "", *mnu, *desc, *mnuval, valtmp[20];
//	ts2k_menu_t *m, *mref;

	mnu = menu->menu_no;
	desc = menu->txt;
	if(menu->val != -1) {
		sprintf(valtmp, "%i", menu->val);
		mnuval = valtmp;
	}
	else if(menu->txt_val[0] != '\0') {
		mnuval = menu->txt_val;
	} else {
		valtmp[0] = '\0';
		mnuval = valtmp;
	}

	sprintf(text, "Menu %3s: %s = %s\n", mnu, desc, mnuval);
//	sprintf(text, "Menu %03s: %s = %s\n", mnu, submnu, desc, mnuval);

	return text;
}

/*
 * ts2k_pm_init()	Uses modified rig.h.
 *
 *	Here we allocate the PM and attach the menu pointers.  The
 *	full set of memories are allocated.
 *
 *	called by ts2k_open() {rig->caps->open()}
 */
int ts2k_pm_init(RIG *rig)
{
	// The following line taken from tests/dumpcaps.c
	const struct rig_caps *caps;

	char pmcmd[10], mfcmd[10];
	int retval, i, j, k, pm_orig, pmsiz, mfsiz, mf_orig, dummy;
	ts2k_menu_t **menus;
	ts2k_pm_t **pm_all, *pm, **rig_pm;
//	ts2k_pm_t *pm_all[TS2K_PMSIZ], *pm;

	rig_debug(RIG_DEBUG_WARN, __FUNCTION__ ": begin.\n");

	if(rig == NULL) {
		rig_debug(RIG_DEBUG_WARN, __FUNCTION__ ": Invalid rig pointer\n");
		return -RIG_EINTERNAL;
	}

	if((caps = rig->caps) == NULL) {
		rig_debug(RIG_DEBUG_WARN, __FUNCTION__ ": Invalid caps pointer\n");
		return -RIG_EINTERNAL;
	}

	pm_all = calloc(TS2K_PMSIZ, sizeof(ts2k_pm_t *));

	if(pm_all == NULL) {
		rig_debug(RIG_DEBUG_WARN, __FUNCTION__ ": calloc failed\n");
		return -RIG_ENOMEM;
	}

	rig_debug(RIG_DEBUG_WARN, __FUNCTION__ ": Setting caps pointer.\n");
	caps->pm = pm_all;

	/* get current values of rig to be restored on close */
	pmsiz = 10;
	retval = ts2k_transaction(rig, "pm;", 3, pmcmd, &pmsiz);
	if(retval != RIG_OK)
		return -RIG_EINVAL;

	mfsiz = 10;
	retval = ts2k_transaction(rig, "mf;", 3, mfcmd, &mfsiz);
	if(retval != RIG_OK)
		return -RIG_EINVAL;

	pm_orig = pmcmd[2] - '0';
	mf_orig = mfcmd[2] - '0';

	rig_debug(RIG_DEBUG_WARN, __FUNCTION__ ": found pm=%i, mf=%i\n", pm_orig, mf_orig);

	// allocate array of ts2k_pm_t's
	for( i=(TS2K_PMSIZ-1); i>=0; --i) {
		rig_debug(RIG_DEBUG_WARN, __FUNCTION__ \
			": Allocating PM(%i)\n", i);
		pm = malloc( sizeof(ts2k_pm_t) );
		if(pm == NULL)
			return -RIG_ENOMEM;
		pm_all[i] = pm;

		pm_all[i]->pub = TS2K_PM_PUB;
		pm_all[i]->priv = TS2K_PM_PRIV;

		// Use orginal values for initialization.  We don't use them here.
		pm->curr = pm->orig = pm_orig;
		pm->menu_no = pm->orig_menu = mf_orig;

/*		pmcmd[2] = '0' + i;
		retval = ts2k_transaction(rig, pmcmd, 4, NULL, NULL);
		if(retval != RIG_OK)
			return -RIG_EINVAL;

*/
	//	sleep(1);	// Enforce switch-over delay or we get aborted!

		for( j=0; j<2; j++) {
			rig_debug(RIG_DEBUG_WARN, __FUNCTION__ \
				":\tAllocating Menu %i...\n", j);
			menus = calloc(TS2K_MENU_COUNT, sizeof(ts2k_menu_t *));
			pm->menu[j] = menus;
			if(pm->menu[j] == NULL)
				return -RIG_ENOMEM;

			// Doing ts2k_menu_init() here was way too slow!!
			// This is a compromise.
			for( k=0; k<TS2K_MENU_COUNT; k++) {
				menus[k] = malloc(sizeof(ts2k_menu_t));
				menus[k]->val = M_MENU_NOT_INITIALIZED;
				menus[k]->menu[0] = M_MENU_NOT_INITIALIZED;
			}
		}
		rig_debug(RIG_DEBUG_WARN, __FUNCTION__ \
			": Finished PM(%i)\n", i);
	}
	rig_debug(RIG_DEBUG_WARN, __FUNCTION__ ": Setting first available Public PM.\n");

	// Set first PM that is public
	for( i=0; i<TS2K_PMSIZ; i++){
		retval = ts2k_set_pm(rig, i);
		if(retval == RIG_OK) {
			rig_debug(RIG_DEBUG_WARN, __FUNCTION__
				": setting PM(%i)\n", i);

			menus = ts2k_get_menuptr(rig);

			/* We force ts2k_menu_init() by reading uninitialized menu */
			retval = ts2k_get_menu(rig, menus, 0, &dummy);
			if(retval != RIG_OK)
				return -RIG_EINVAL;
			return RIG_OK;
		}
	}

	rig_debug(RIG_DEBUG_ERR, __FUNCTION__ ": All PM's set to private!\n");
	return -RIG_EINTERNAL;	// At least one PM must be public!
}

/*
 * ts2k_pm_close()	Restores the original PM then
 *	frees all the memory.
 */
int ts2k_pm_close(RIG *rig)
{
	int i, j, k, orig_pm, retval;
	char cmd[] = "pm0;";
	ts2k_pm_t **pm;
	ts2k_menu_t **m;

	pm = (ts2k_pm_t **)rig->caps->pm;
	orig_pm = pm[0]->orig;

	rig_debug(RIG_DEBUG_ERR, __FUNCTION__": Resetting to PM(%i).\n", orig_pm);

	cmd[2] = orig_pm + '0';
	retval = ts2k_transaction(rig, cmd, 3, NULL, NULL);

	rig_debug(RIG_DEBUG_ERR, __FUNCTION__": Setting memories free...  :)\n"); 

	for( i=0; i<TS2K_PMSIZ; i++) {	// free PM's
		for( j=0; j<2; j++) {	// free MenuA and MenuB
			m = (ts2k_menu_t **)pm[i]->menu[j];
			for( k=0; k<TS2K_MENU_COUNT; k++) {	// free all menu items
				free(m[k]);
			}
			free(m);
		}
		free(pm[i]);
	}
	free(pm);
	rig->caps->pm = NULL;

	return retval;
}

ts2k_menu_t ** ts2k_get_menuptr(RIG *rig)
{
	ts2k_menu_t **mnu;
	ts2k_pm_t **pm;
	int retval, curr_pm, curr_menu;

	if(rig == NULL) {
		rig_debug(RIG_DEBUG_ERR, __FUNCTION__": received NULL rig pointer.\n");
		return NULL;
	}

	pm = (ts2k_pm_t **)rig->caps->pm;
	if(pm == NULL) {
		rig_debug(RIG_DEBUG_ERR, __FUNCTION__": PM not initialized!");
#ifdef TS2K_PM_AUTO_INIT
		rig_debug(RIG_DEBUG_ERR, "  Initializing...");
		retval = ts2k_pm_init(rig);
		if(retval != RIG_OK) {
			rig_debug(RIG_DEBUG_ERR, "  Error occured!\n");
			return NULL;
		}
		rig_debug(RIG_DEBUG_ERR, "  Done.\n");
#else
		rig_debug(RIG_DEBUG_ERR, "\n");
		return NULL;
#endif
	}

	// FIXME: Isn't there an easier way?!!	
	curr_pm = pm[0]->curr;
	curr_menu = pm[curr_pm]->menu_no;
	mnu = (ts2k_menu_t **)pm[curr_pm]->menu[curr_menu];

	return mnu;	// mnu is an array of menu items!
}

/*
 * ts2k_set_pm()
 *		Enable new PM if requested PM is public.
 *		Update PM(0) to reflect new current value.
 */
int ts2k_set_pm(RIG *rig, int pm_no)
{
	int retval;
	char pmcmd[] = "pm0;";
	ts2k_pm_t **pm;
	vfo_t vtmp;

	retval = rig_get_vfo(rig, &vtmp);
	CHKERR(retval);

	/* PM won't change if in SCAN */
	if( vtmp & RIG_CTRL_SCAN ) {
		retval = rig_set_vfo(rig, vtmp & ~RIG_CTRL_SCAN);
		CHKERR(retval);
	}

	pm = (ts2k_pm_t **)rig->caps->pm;
	if(pm_no >= TS2K_PMSIZ)
		return -RIG_ENAVAIL;

	if( !(pm[0]->pub & 1<<pm_no) ) {
		rig_debug(RIG_DEBUG_VERBOSE, __FUNCTION__
			": Attempt made to access private PM = %i!\n", pm_no);
		return -RIG_ENAVAIL;
	}

	pmcmd[2] = pm_no + '0';
	retval = ts2k_transaction(rig, pmcmd, 4, NULL, NULL);
	CHKERR(retval);

	sleep(1);	// enforce delay or we timeout!

	pm[0]->curr = pm_no;

	return RIG_OK;
}

/*
 * ts2k_get_pm()
 */
int ts2k_get_pm(RIG *rig, int *pm_no)
{
	char cmd[] = "pm0;", ack[10];
	int retval, acklen;
	ts2k_pm_t **pm;

	pm = (ts2k_pm_t **)rig->caps->pm;
	acklen = 10;
	retval = ts2k_transaction(rig, cmd, 4, ack, &acklen);
	CHKERR(retval);

	if(ack[0] != 'P' || ack[1] != 'M')
		return -RIG_EINVAL;

	*pm_no = ack[2] - '0';
	pm[0]->curr = *pm_no;	// update, just in case...

	return RIG_OK;
}


// End of ts2k_menu.c
