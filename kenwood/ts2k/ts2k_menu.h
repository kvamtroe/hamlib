/*
 * ts2k_menu.h	(C) Copyright 2002 by Dale E. Edmons.  All rights reserved.
 */

/*
 * License:	GNU (same as what hamlib currently is using)
 */

/*
 * status:	uncompiled.  Haven't finished factory defaults.
 *		anything with m_tbd is: to be developed
 */

/* Header to implement menus equivalent to those on the TS-2000. */
/* Header to also implement pm (see bottom of file) */

#ifndef _TS2K_MENU_H
#define _TS2K_MENU_H

#include <stdlib.h>
#include <unistd.h>

// Compile Time Preferences
#define TS2K_BEEP_OFF
#define TS2K_LCD_DIMMEST
#define TS2K_ENFORCE_PM
#define TS2K_PM_AUTO_INIT

#define CHKERR(e) if(e != RIG_OK) return -RIG_EINVAL

// sub-menu defines
#define MENU_A	1
#define MENU_B	2
#define MENU_C	3
#define MENU_D	4
#define MENU_E	5
#define MENU_F	6

// parameter value text.  may be used in any menu.
/*
enum ts2k_m_parm_e {
	M_END = 0, M_ON, M_OFF, M_TO, M_CO, M_H_BOOST, M_B_BOOST, M_F_PASS,
	M_CONVEN, M_USER, M_AUTO, M_NORM, M_INV, M_LOW, M_MID, M_HI,
	M_BURST, M_CONT, M_SLOW, M_FAST, M_MAIN, M_SUB, M_TNC_BAND,
	M_MAIN_SUB, M_MAN, M_MORSE, M_VOICE, M_NEG, M_POS, M_LOCKED,
	M_CROSS, M_CLIENT, M_COMMANDER, M_TRANSPORTER, M_FONT1, M_FONT2,
	M_TBD, M_NUM, M_TEXT, M_MENU_NOT_INITIALIZED = (0xCEEE + 0xDEE), M_NULL
};
*/
// My wife's initials are C.E.E.E. and mine are D.E.E.  0xCEEE+0xDEE = 0xDCDC
// DCDC -> DaleCathyDaleCathy Weird huh?

extern char *ts2k_m_text[];
extern const char m_num[8+2];		// storage for numeric constants
extern const char m_text[8+2];		// storage for text constants

extern const char key_none[];

// PF1 key assignments
#define KEY_MENU_MAX	62
#define KEY_KEY_MAX	90
#define KEY_UNASSIGNED	99
/*
typedef struct {
	char *menu_no;
	char *txt;
	enum ts2k_m_parm_e param[10];
	char txt_val[20];
	int menu[4];
	int val;		// same as P5
} ts2k_menu_t;
*/
/*
 * Defaults for menu_t.val were obtained via minicom
 * on my rig after doing a full reset to factory defaults.
 */

extern ts2k_menu_t ts2k_menus[];

//#define TS2K_MENU_COUNT (sizeof(*ts2k_menus)/sizeof(ts2k_menu_t))
#define TS2K_MENU_COUNT 91

/*
 * Items related to menu structure
 */
/*
// Programmable memories
typedef struct {
	int	curr;	// PM now in use
	int	orig;	// what PM to restore on exit
	int	orig_menu;	// orignal menu in effect

	int	menu_no;	// menuA or menuB of current PM
	ts2k_menu_t	**menu[2];	// pointer to actual menuA/B array

	// the following set which PM's are private or public
	// they are set on init and enforced until exit
	unsigned int	pub;
	unsigned int	priv;
} ts2k_pm_t;
*/
#ifdef TS2K_ENFORCE_PM
# define TS2K_PM_PUB	( 	   	  (1<<2) | (1<<3)		 )
# define TS2K_PM_PRIV	( (1<<0) | (1<<1)	 |	 (1<<4) | (1<<5) )
#else
# define TS2K_PM_PUB	( (1<<0) | (1<<1) | (1<<2) | (1<<3) | (1<<4) | (1<<5) )
# define TS2K_PM_PRIV	(0)
#endif

// PM's 0-5
#define TS2K_PMSIZ	6

// Implemented/coded functions
int ts2k_get_menu(RIG *rig, ts2k_menu_t *menu[], int item, int *val);
ts2k_menu_t **ts2k_get_menuptr(RIG *rig);
char * ts2k_list_menu(RIG *rig, ts2k_menu_t *menu);
int ts2k_menu_init(RIG *rig, ts2k_menu_t *menu[]);
int ts2k_set_menu(RIG *rig, ts2k_menu_t *menu[], int item, int val);
int ts2k_set_pm(RIG *rig, int pm_no);

// Unimplemented/uncoded functions

/*
 * Related functions specific to this rig
 */
// Programmable memories[0, ..., 5] + menu[A, ..., B]
int ts2k_pm_close(RIG *rig);
int ts2k_pm_init(RIG *rig); 

#endif	// _TS2K_MENU_H

// End ts2k_menu.c
