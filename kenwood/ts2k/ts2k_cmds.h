/*
 * ts2k_cmds.h	Copyright (C) 2002 by Dale E. Edmons.  All rights reserved.
 *
 *	license:	GNU
 */

/*
 * This file implements (eventually) every command available to the ts2k.
 *	Most used functions will be implemented first.  All parameters are
 *	placed in a struct and #defines make using them transparent.  I've
 *	categorized all commands and assigned a type# to each uniq class of
 *	parameters.  Unique commands are handled case by case.
 *
 *	The menu commands are not handled here yet.
 */

#include <hamlib/rig.h>
#ifndef _TS_LABEL
# include "ts2k/ts2k.h"
#endif
#include <string.h>

#ifndef _TS2K_CMDS_H
#define _TS2K_CMDS_H

// The following is based upon Hamlib and may be changed if desired.
#define TS2K_READ_08(_r, _c)	ts2k_transaction((_r), (_c), 3, ans, &anslen)
#define TS2K_READ(_r, _c, _s)	anslen=(_s); \
					ts2k_transaction((_r), (_c), (_s), ans, &anslen)
#define TS2K_SEND		ts2k_transaction(rig, ans, anslen-1, NULL, NULL)

#define TS2K_A_DATA(_s)		char ans[_s]; int ret, anslen = (_s)
#define TS2K_A_DATANR(_s)	char ans[_s]; int anslen = (_s)
#define TS2K_S_DATA		int ret
#define TS2K_CMD(_a, _b, _c)	ans[0] = _a; ans[1] = _b; ans[(_c) - 2] = ';'; \
					ans[(_c) - 1] = '\0'
#define TS2K_CMD2(_a, _b)	ans[0] = _a[0]; ans[1] = _a[1]; ans[(_b) - 2] = ';'; \
					ans[(_b) - 1] = '\0'; anslen = (_b)

#define _TS2K_SELECT_ALL
//#undef _TS2K_SELECT_ALL

#ifndef _TS2K_SELECT_ALL
# define TS2K_UNUSED(a)	/* a */
#else
# define TS2K_UNUSED(a)	a
#endif

#define TS2K_CTOI(c)	((c) - '0')
#define TS2K_ITOC(c)	((c) + '0')

/* Create parameter needed ts2k types */

# define CMD	char
# define NONE	void
# define SPC	char
# define BIN	char
# define INT1	unsigned char
# define INT2	unsigned int
# define INT3	unsigned int
# define INT4	unsigned int
# define INT6	unsigned int
# define INT9	unsigned int
# define HZ	unsigned int
# define TXT	char
# define SPCL	char

// Types that conflict with hamlib
#ifndef _RIG_H
# define FREQ	unsigned long
#else
# define FREQ	freq_t
#endif


typedef struct { INT1 p1, p2, p3;} ts2k_typeNN_t;	// Generic type that'll work for many items
typedef struct { BIN p1;	} ts2k_type01_t;
typedef struct { FREQ p1;	} ts2k_type02_t;
typedef struct { HZ p1;		} ts2k_type03_t;
typedef struct { INT1 p1;	} ts2k_type04_t;
typedef struct { INT2 p1;	} ts2k_type05_t;
typedef struct { INT3 p1;	} ts2k_type06_t;
typedef struct { INT4 p1;	} ts2k_type07_t;
// type08 has no data
typedef struct { INT1 p1;	} ts2k_type08_t;
//typedef NONE		 ts2k_type08_t;
typedef struct { BIN p1; BIN p2;	} ts2k_type09_t;
typedef struct { TXT p1; BIN p2;	} ts2k_type10_t;
typedef struct { BIN p1; INT1 p2;	} ts2k_type11_t;
typedef struct { INT1 p1; INT1 p2;	} ts2k_type12_t;
typedef struct { INT2 p1; INT1 p2;	} ts2k_type13_t;
typedef struct { INT1 p1; INT2 p2;	} ts2k_type14_t;
typedef struct { INT2 p1; INT2 p2;	} ts2k_type15_t;
typedef struct { BIN p1; INT3 p2;	} ts2k_type16_t;
typedef struct { INT3 p1; INT3 p2;	} ts2k_type17_t;
typedef struct { INT1 p1; INT4 p2;	} ts2k_type18_t;
typedef struct { INT1 p1; TXT p2[8];	} ts2k_type19_t;
typedef struct { BIN p1; BIN p2; BIN p3;	} ts2k_type20_t;
typedef struct { BIN p1; BIN p2; BIN p3;
		 BIN p4; BIN p5; BIN p6;
		 BIN p7; BIN p8;		} ts2k_type21_t;
typedef struct { FREQ p1;			} ts2k_type22_t;
typedef struct { BIN p1; BIN p2; INT1 p3;	} ts2k_type23_t;
typedef struct { INT3 p1; INT2 p2;
		 INT1 p3; INT1 p4; 		} ts2k_type24_t;
typedef struct { INT1 p1; INT2 p2;
		 FREQ p3; INT1 p4; 		} ts2k_type25_t;
typedef struct { TXT p1; INT4 p2;		} ts2k_type26_t;
typedef struct { INT1 p1[10];			} ts2k_type27_t;
typedef struct { BIN p1; INT1 p2[10];		} ts2k_type27b_t;
typedef struct { TXT p1; TXT p2[25];		} ts2k_type28_t;
typedef struct { BIN p1; INT1 p2; BIN p3;
		 BIN p4; BIN p5; BIN p6;
		 BIN p7; 			} ts2k_type29_t;
typedef struct { INT3 p1; INT2 p2; INT1 p3;
		 INT1 p4; TXT p5[16];		} ts2k_type30_t;
typedef struct { BIN p1; INT1 p2; BIN p3;
		 BIN p4; BIN p5; BIN p6;
		 BIN p7; TXT p8[9];		} ts2k_type31_t;
typedef struct { FREQ p1; TXT p2[13];
		 TXT p3[21]; TXT p4[6];		} ts2k_type32_t;
// The following combines p6,p7 into just INT3 p7
typedef struct { FREQ p1; INT4 p2; INT6 p3;
		 BIN p4; BIN p5; INT1 p6; INT3 p7;
		 BIN p8; INT1 p9; INT1 p10;
		 INT1 p11; INT1 p12; INT1 p13;
		 INT2 p14; INT1 p15;		} ts2k_type33_t;
typedef struct { BIN p1; INT3 p2; FREQ p3;
		 INT1 p4; BIN p5; INT1 p6;
		 INT2 p7; INT2 p8; INT3 p9;
		 BIN p10; INT1 p11; INT9 p12;
		 INT2 p13; INT1 p14; TXT p15[9];	} ts2k_type34_t;

#define TS2K_AC_T	ts2k_type23_t
#define TS2K_AC_R_T	ts2k_type08_t
#define TS2K_AG_T	ts2k_type16_t
#define TS2K_AG_R_T	ts2k_type01_t
#define TS2K_AI_T	ts2k_type04_t
#define TS2K_AI_R_T	ts2k_type08_t
#define TS2K_AL_T	ts2k_type06_t
#define TS2K_AL_R_T	ts2k_type08_t
#define TS2K_AM_T	ts2k_type01_t
#define TS2K_AM_R_T	ts2k_type08_t
#define TS2K_AN_T	ts2k_type04_t
#define TS2K_AN_R_T	ts2k_type08_t
#define TS2K_AR_T	ts2k_type20_t
#define TS2K_AR_R_T	ts2k_type01_t
#define TS2K_AS_T	ts2k_type25_t
#define TS2K_AS_R_T	ts2k_type14_t
#define TS2K_BC_T	ts2k_type04_t
#define TS2K_BC_R_T	ts2k_type08_t
#define TS2K_BD_T	ts2k_type08_t
#define TS2K_BP_T	ts2k_type06_t
#define TS2K_BP_R_T	ts2k_type08_t
#define TS2K_BU_T	ts2k_type08_t
#define TS2K_BY_T	ts2k_type09_t
#define TS2K_BY_R_T	ts2k_type08_t
#define TS2K_CA_T	ts2k_type01_t
#define TS2K_CA_R_T	ts2k_type08_t
#define TS2K_CG_T	ts2k_type06_t
#define TS2K_CG_R_T	ts2k_type08_t
#define TS2K_CH_T	ts2k_type01_t
#define TS2K_CI_T	ts2k_type08_t
#define TS2K_CM_T	ts2k_type01_t
#define TS2K_CM_R_T	ts2k_type08_t
#define TS2K_CN_T	ts2k_type05_t
#define TS2K_CN_R_T	ts2k_type08_t
#define TS2K_CT_T	ts2k_type01_t
#define TS2K_CT_R_T	ts2k_type08_t
#define TS2K_DC_T	ts2k_type09_t
#define TS2K_DC_R_T	ts2k_type08_t
#define TS2K_DN_T	ts2k_type05_t
#define TS2K_DN_R_T	ts2k_type08_t
#define TS2K_DQ_T	ts2k_type01_t
#define TS2K_DQ_R_T	ts2k_type08_t
#define TS2K_EX_T	ts2k_type30_t
#define TS2K_EX_R_T	ts2k_type24_t
#define TS2K_FA_T	ts2k_type02_t
#define TS2K_FA_R_T	ts2k_type08_t
#define TS2K_FB_T	ts2k_type02_t
#define TS2K_FB_R_T	ts2k_type08_t
#define TS2K_FC_T	ts2k_type02_t
#define TS2K_FC_R_T	ts2k_type08_t
#define TS2K_FD_T	ts2k_type21_t
#define TS2K_FD_R_T	ts2k_type08_t
#define TS2K_FR_T	ts2k_type04_t
#define TS2K_FR_R_T	ts2k_type08_t
#define TS2K_FS_T	ts2k_type01_t
#define TS2K_FS_R_T	ts2k_type08_t
#define TS2K_FT_T	ts2k_type04_t
#define TS2K_FT_R_T	ts2k_type08_t
#define TS2K_FW_T	ts2k_type07_t
#define TS2K_FW_R_T	ts2k_type08_t
#define TS2K_GT_T	ts2k_type06_t
#define TS2K_GT_R_T	ts2k_type08_t
#define TS2K_ID_T	ts2k_type06_t
#define TS2K_ID_R_T	ts2k_type08_t
#define TS2K_IF_T	ts2k_type33_t
#define TS2K_IF_R_T	ts2k_type08_t
#define TS2K_IS_T	ts2k_type26_t
#define TS2K_IS_R_T	ts2k_type08_t
#define TS2K_KS_T	ts2k_type06_t
#define TS2K_KS_R_T	ts2k_type08_t
#define TS2K_KY_S_T	ts2k_type28_t
#define TS2K_KY_R_T	ts2k_type08_t
#define TS2K_KY_A_T	ts2k_type01_t
//#define TS2K_KS_T	ts2k_type26_t
//#define TS2K_KS_R_T	ts2k_type08_t
#define TS2K_LK_T	ts2k_type12_t
#define TS2K_LK_R_T	ts2k_type08_t
#define TS2K_LM_T	ts2k_type04_t
#define TS2K_LM_R_T	ts2k_type08_t
#define TS2K_LT_T	ts2k_type01_t
#define TS2K_LT_R_T	ts2k_type08_t
#define TS2K_MC_T	ts2k_type06_t
#define TS2K_MC_R_T	ts2k_type08_t
#define TS2K_MD_T	ts2k_type04_t
#define TS2K_MD_R_T	ts2k_type08_t
#define TS2K_MF_T	ts2k_type01_t
#define TS2K_MF_R_T	ts2k_type08_t
#define TS2K_MG_T	ts2k_type06_t
#define TS2K_MG_R_T	ts2k_type08_t
#define TS2K_ML_T	ts2k_type06_t
#define TS2K_ML_R_T	ts2k_type08_t
#define TS2K_MO_T	ts2k_type01_t
#define TS2K_MR_T	ts2k_type34_t
#define TS2K_MR_R_T	ts2k_type16_t
#define TS2K_MU_T	ts2k_type27_t
#define TS2K_MU_R_T	ts2k_type08_t
#define TS2K_MW_T	ts2k_type34_t
#define TS2K_NB_T	ts2k_type01_t
#define TS2K_NB_R_T	ts2k_type08_t
#define TS2K_NL_T	ts2k_type06_t
#define TS2K_NL_R_T	ts2k_type08_t
#define TS2K_NR_T	ts2k_type06_t
#define TS2K_NR_R_T	ts2k_type08_t
#define TS2K_NT_T	ts2k_type01_t
#define TS2K_NT_R_T	ts2k_type08_t
#define TS2K_OF_T	ts2k_type22_t
#define TS2K_OF_R_T	ts2k_type08_t
#define TS2K_OI_T	ts2k_type33_t
#define TS2K_OI_R_T	ts2k_type08_t
#define TS2K_OS_T	ts2k_type04_t
#define TS2K_OS_R_T	ts2k_type08_t
#define TS2K_PA_S_T	ts2k_type01_t
#define TS2K_PA_R_T	ts2k_type08_t
#define TS2K_PA_A_T	ts2k_type09_t
#define TS2K_PB_T	ts2k_type04_t
#define TS2K_PB_R_T	ts2k_type08_t
#define TS2K_PC_T	ts2k_type06_t
#define TS2K_PC_R_T	ts2k_type08_t
#define TS2K_PI_T	ts2k_type04_t
#define TS2K_PK_T	ts2k_type32_t
#define TS2K_PL_T	ts2k_type17_t
#define TS2K_PL_R_T	ts2k_type08_t
#define TS2K_PM_T	ts2k_type04_t
#define TS2K_PM_R_T	ts2k_type08_t
#define TS2K_PR_T	ts2k_type01_t
#define TS2K_PR_R_T	ts2k_type08_t
#define TS2K_PS_T	ts2k_type01_t
#define TS2K_PS_R_T	ts2k_type08_t
#define TS2K_QC_T	ts2k_type06_t
#define TS2K_QC_R_T	ts2k_type08_t
#define TS2K_QI_T	ts2k_type08_t
#define TS2K_QR_T	ts2k_type11_t
#define TS2K_QR_R_T	ts2k_type08_t
#define TS2K_RA_S_T	ts2k_type05_t
#define TS2K_RA_R_T	ts2k_type08_t
#define TS2K_RA_A_T	ts2k_type15_t
#define TS2K_RC_T	ts2k_type08_t
#define TS2K_RD_S_T	ts2k_type03_t
#define TS2K_RD_R_T	ts2k_type08_t
#define TS2K_RD_A_T	ts2k_type04_t
#define TS2K_RG_T	ts2k_type06_t
#define TS2K_RG_R_T	ts2k_type08_t
#define TS2K_RL_T	ts2k_type05_t
#define TS2K_RL_R_T	ts2k_type08_t
#define TS2K_RM_S_T	ts2k_type34_t
#define TS2K_RM_R_T	ts2k_type08_t
#define TS2K_RM_A_T	ts2k_type18_t
#define TS2K_RT_T	ts2k_type01_t
#define TS2K_RT_R_T	ts2k_type08_t
#define TS2K_RU_S_T	ts2k_type03_t
#define TS2K_RU_S2_T	ts2k_type08_t
#define TS2K_RU_R_T	ts2k_type08_t
#define TS2K_RU_A_T	ts2k_type04_t
#define TS2K_RX_T	ts2k_type08_t
#define TS2K_SA_S_T	ts2k_type29_t
#define TS2K_SA_R_T	ts2k_type08_t
#define TS2K_SA_A_T	ts2k_type31_t
#define TS2K_SB_T	ts2k_type01_t
#define TS2K_SB_R_T	ts2k_type08_t
#define TS2K_SC_T	ts2k_type04_t
#define TS2K_SC_R_T	ts2k_type08_t
#define TS2K_SD_T	ts2k_type07_t
#define TS2K_SD_R_T	ts2k_type08_t
#define TS2K_SH_T	ts2k_type05_t
#define TS2K_SH_R_T	ts2k_type08_t
#define TS2K_SI_T	ts2k_type19_t
#define TS2K_SL_T	ts2k_type05_t
#define TS2K_SL_R_T	ts2k_type08_t
#define TS2K_SM_T	ts2k_type18_t
#define TS2K_SM_R_T	ts2k_type04_t
#define TS2K_SQ_T	ts2k_type11_t
#define TS2K_SQ_R_T	ts2k_type04_t
#define TS2K_SR_T	ts2k_type04_t
#define TS2K_SS_T	ts2k_type04_t
#define TS2K_SS_R_T	ts2k_type08_t
#define TS2K_ST_T	ts2k_type05_t
#define TS2K_ST_R_T	ts2k_type08_t
#define TS2K_SU_T	ts2k_type27b_t
#define TS2K_SU_R_T	ts2k_type08_t
#define TS2K_SV_T	ts2k_type08_t
#define TS2K_TC_T	ts2k_type10_t
#define TS2K_TC_R_T	ts2k_type08_t
#define TS2K_TD_T	ts2k_type05_t
#define TS2K_TI_T	ts2k_type20_t
#define TS2K_TI_R_T	ts2k_type08_t
#define TS2K_TN_T	ts2k_type05_t
#define TS2K_TN_R_T	ts2k_type08_t
#define TS2K_TO_T	ts2k_type04_t
#define TS2K_TO_R_T	ts2k_type08_t
#define TS2K_TS_T	ts2k_type01_t
#define TS2K_TS_R_T	ts2k_type08_t
#define TS2K_TX_T	ts2k_type08_t
#define TS2K_TY_T	ts2k_type13_t
#define TS2K_TY_R_T	ts2k_type08_t
#define TS2K_UL_T	ts2k_type01_t
#define TS2K_UP_T	ts2k_type05_t
#define TS2K_UP_S_T	ts2k_type08_t
#define TS2K_VD_T	ts2k_type07_t
#define TS2K_VD_R_T	ts2k_type08_t
#define TS2K_VG_T	ts2k_type06_t
#define TS2K_VG_R_T	ts2k_type08_t
#define TS2K_VR_T	ts2k_type01_t
#define TS2K_VX_T	ts2k_type01_t
#define TS2K_VX_R_T	ts2k_type08_t
#define TS2K_XT_T	ts2k_type01_t
#define TS2K_XT_R_T	ts2k_type08_t

// The following sizes include one byte for null
#define TS2K_AC_SIZE	7
#define TS2K_AG_SIZE	8
#define TS2K_AI_SIZE	5
#define TS2K_AL_SIZE	7
#define TS2K_AM_SIZE	5
#define TS2K_AN_SIZE	5
#define TS2K_AR_SIZE	7
#define TS2K_AS_SIZE	19
#define TS2K_BC_SIZE	5
#define TS2K_BD_SIZE	4
#define TS2K_BP_SIZE	7
#define TS2K_BU_SIZE	4
#define TS2K_BY_SIZE	4
#define TS2K_CA_SIZE	5
#define TS2K_CG_SIZE	7
#define TS2K_CH_SIZE	5
#define TS2K_CI_SIZE	4
#define TS2K_CM_SIZE	5
#define TS2K_CN_SIZE	6
#define TS2K_CT_SIZE	5
#define TS2K_DC_SIZE	6
#define TS2K_DN_SIZE	6
#define TS2K_DQ_SIZE	5
#define TS2K_EX_SIZE	27
#define TS2K_FA_SIZE	15
#define TS2K_FB_SIZE	15
#define TS2K_FC_SIZE	15
#define TS2K_FD_SIZE	12
#define TS2K_FR_SIZE	5
#define TS2K_FS_SIZE	5
#define TS2K_FT_SIZE	5
#define TS2K_FW_SIZE	8
#define TS2K_GT_SIZE	7
#define TS2K_ID_SIZE	7
#define TS2K_IF_SIZE	39
#define TS2K_IS_SIZE	9
#define TS2K_KS_SIZE	7
#define TS2K_KY_SIZE	29
#define TS2K_LK_SIZE	6
#define TS2K_LM_SIZE	5
#define TS2K_LT_SIZE	5
#define TS2K_MC_SIZE	7
#define TS2K_MD_SIZE	5
#define TS2K_MF_SIZE	5
#define TS2K_MG_SIZE	7
#define TS2K_ML_SIZE	7
#define TS2K_MO_SIZE	5
#define TS2K_MR_SIZE	51
#define TS2K_MU_SIZE	14
#define TS2K_MW_SIZE	51
#define TS2K_NB_SIZE	5
#define TS2K_NL_SIZE	7
#define TS2K_NR_SIZE	5
#define TS2K_NT_SIZE	5
#define TS2K_OF_SIZE	13
#define TS2K_OI_SIZE	39
#define TS2K_OS_SIZE	5
#define TS2K_PA_SIZE	5
#define TS2K_PB_SIZE	5
#define TS2K_PC_SIZE	7
#define TS2K_PI_SIZE	5
#define TS2K_PK_SIZE	52
#define TS2K_PL_SIZE	10
#define TS2K_PM_SIZE	5
#define TS2K_PR_SIZE	5
#define TS2K_PS_SIZE	5
#define TS2K_QC_SIZE	7
#define TS2K_QI_SIZE	4
#define TS2K_QR_SIZE	6
#define TS2K_RA_SIZE	8
#define TS2K_RC_SIZE	4
#define TS2K_RD_SIZE	9
#define TS2K_RG_SIZE	7
#define TS2K_RL_SIZE	6
#define TS2K_RM_SIZE	9
#define TS2K_RT_SIZE	5
#define TS2K_RU_SIZE	9
#define TS2K_RX_SIZE	4
#define TS2K_SA_SIZE	19
#define TS2K_SB_SIZE	5
#define TS2K_SC_SIZE	5
#define TS2K_SD_SIZE	8
#define TS2K_SH_SIZE	6
#define TS2K_SI_SIZE	13
#define TS2K_SL_SIZE	6
#define TS2K_SM_SIZE	9
#define TS2K_SQ_SIZE	8
#define TS2K_SR_SIZE	5
#define TS2K_SS_SIZE	5
#define TS2K_ST_SIZE	6
#define TS2K_SU_SIZE	15
#define TS2K_SV_SIZE	4
#define TS2K_TC_SIZE	6
#define TS2K_TD_SIZE	6
#define TS2K_TI_SIZE	7
#define TS2K_TN_SIZE	6
#define TS2K_TO_SIZE	5
#define TS2K_TS_SIZE	5
#define TS2K_TX_SIZE	4
#define TS2K_TY_SIZE	7
#define TS2K_UL_SIZE	5
#define TS2K_UP_SIZE	6
#define TS2K_VD_SIZE	8
#define TS2K_VG_SIZE	7
#define TS2K_VR_SIZE	5
#define TS2K_VX_SIZE	5
#define TS2K_XT_SIZE	5

// prototypes
int ts2k_s_type01(RIG *rig, ts2k_type01_t *p, char *cmd);
int ts2k_g_type01(RIG *rig, ts2k_type01_t *p, char *cmd);
int ts2k_s_type02(RIG *rig, ts2k_type02_t *p, char *cmd);
int ts2k_g_type02(RIG *rig, ts2k_type02_t *p, char *cmd);
int ts2k_s_type04(RIG *rig, ts2k_type04_t *p, char *cmd);
int ts2k_g_type04(RIG *rig, ts2k_type04_t *p, char *cmd);
//int ts2k_s_type05(RIG *rig, TS2K_CN_T *p, char c1, char c2);
//int ts2k_g_type05(RIG *rig, TS2K_CN_T *p, char *cmd);
//int ts2k_s_type06(RIG *rig, ts2k_type06_t *p, char c1, char c2);
//int ts2k_g_type06(RIG *rig, ts2k_type06_t *p, char *cmd);
// new types follow
int ts2k_s_type05(RIG *rig, ts2k_type05_t *p, char *cmd);
int ts2k_g_type05(RIG *rig, ts2k_type05_t *p, char *cmd);
int ts2k_s_type06(RIG *rig, ts2k_type06_t *p, char *cmd);
int ts2k_g_type06(RIG *rig, ts2k_type06_t *p, char *cmd);
int ts2k_s_type07(RIG *rig, ts2k_type07_t *p, char *cmd);
int ts2k_g_type07(RIG *rig, ts2k_type07_t *p, char *cmd);
int ts2k_s_type08(RIG *rig, ts2k_type08_t *p, char *cmd);
int ts2k_g_type08(RIG *rig, ts2k_type08_t *p, char *cmd);
int ts2k_s_type09(RIG *rig, ts2k_type09_t *p, char *cmd);
int ts2k_g_type09(RIG *rig, ts2k_type09_t *p, char *cmd);
int ts2k_s_type10(RIG *rig, ts2k_type10_t *p, char *cmd);
int ts2k_g_type10(RIG *rig, ts2k_type10_t *p, char *cmd);
int ts2k_s_type11(RIG *rig, ts2k_type11_t *p, char *cmd);
int ts2k_g_type11(RIG *rig, ts2k_type11_t *p, char *cmd);
int ts2k_s_type12(RIG *rig, ts2k_type12_t *p, char *cmd);
int ts2k_g_type12(RIG *rig, ts2k_type12_t *p, char *cmd);
int ts2k_s_type13(RIG *rig, ts2k_type13_t *p, char *cmd);
int ts2k_g_type13(RIG *rig, ts2k_type13_t *p, char *cmd);
int ts2k_s_type14(RIG *rig, ts2k_type14_t *p, char *cmd);
int ts2k_g_type14(RIG *rig, ts2k_type14_t *p, char *cmd);
int ts2k_s_type15(RIG *rig, ts2k_type15_t *p, char *cmd);
int ts2k_g_type15(RIG *rig, ts2k_type15_t *p, char *cmd);
int ts2k_s_type16(RIG *rig, ts2k_type16_t *p, char *cmd);
int ts2k_g_type16(RIG *rig, ts2k_type16_t *p, char *cmd);
int ts2k_s_type17(RIG *rig, ts2k_type17_t *p, char *cmd);
int ts2k_g_type17(RIG *rig, ts2k_type17_t *p, char *cmd);
int ts2k_s_type18(RIG *rig, ts2k_type18_t *p, char *cmd);
int ts2k_g_type18(RIG *rig, ts2k_type18_t *p, char *cmd);
int ts2k_s_type19(RIG *rig, ts2k_type19_t *p, char *cmd);
int ts2k_g_type19(RIG *rig, ts2k_type19_t *p, char *cmd);
int ts2k_s_type20(RIG *rig, ts2k_type20_t *p, char *cmd);
int ts2k_g_type20(RIG *rig, ts2k_type20_t *p, char *cmd);
int ts2k_s_type21(RIG *rig, ts2k_type21_t *p, char *cmd);
int ts2k_g_type21(RIG *rig, ts2k_type21_t *p, char *cmd);
int ts2k_s_type22(RIG *rig, ts2k_type22_t *p, char *cmd);
int ts2k_g_type22(RIG *rig, ts2k_type22_t *p, char *cmd);
int ts2k_s_type23(RIG *rig, ts2k_type23_t *p, char *cmd);
int ts2k_g_type23(RIG *rig, ts2k_type23_t *p, char *cmd);
int ts2k_s_type24(RIG *rig, ts2k_type24_t *p, char *cmd);
int ts2k_g_type24(RIG *rig, ts2k_type24_t *p, char *cmd);
int ts2k_s_type25(RIG *rig, ts2k_type25_t *p, char *cmd);
int ts2k_g_type25(RIG *rig, ts2k_type25_t *p, char *cmd);
int ts2k_s_type26(RIG *rig, ts2k_type26_t *p, char *cmd);
int ts2k_g_type26(RIG *rig, ts2k_type26_t *p, char *cmd);
int ts2k_s_type27(RIG *rig, ts2k_type27_t *p, char *cmd);
int ts2k_g_type27(RIG *rig, ts2k_type27_t *p, char *cmd);
int ts2k_s_type27b(RIG *rig, ts2k_type27b_t *p, char *cmd);
int ts2k_g_type27b(RIG *rig, ts2k_type27b_t *p, char *cmd);
int ts2k_s_type28(RIG *rig, ts2k_type28_t *p, char *cmd);
int ts2k_g_type28(RIG *rig, ts2k_type28_t *p, char *cmd);
int ts2k_s_type29(RIG *rig, ts2k_type29_t *p, char *cmd);
int ts2k_g_type29(RIG *rig, ts2k_type29_t *p, char *cmd);
int ts2k_s_type30(RIG *rig, ts2k_type30_t *p, char *cmd);
int ts2k_g_type30(RIG *rig, ts2k_type30_t *p, char *cmd);
int ts2k_s_type31(RIG *rig, ts2k_type31_t *p, char *cmd);
int ts2k_g_type31(RIG *rig, ts2k_type31_t *p, char *cmd);
int ts2k_s_type32(RIG *rig, ts2k_type32_t *p, char *cmd);
int ts2k_g_type32(RIG *rig, ts2k_type32_t *p, char *cmd);
int ts2k_s_type33(RIG *rig, ts2k_type33_t *p, char *cmd);
int ts2k_g_type33(RIG *rig, ts2k_type33_t *p, char *cmd);
int ts2k_s_type34(RIG *rig, ts2k_type34_t *p, char *cmd);
int ts2k_g_type34(RIG *rig, ts2k_type34_t *p, char *cmd);

// commands
int ts2k_s_ac(RIG *rig, TS2K_AC_T *p);
int ts2k_g_ac(RIG *rig, TS2K_AC_T *p);
int ts2k_s_ag(RIG *rig, TS2K_AG_T *p);	//-zone
int ts2k_g_ag(RIG *rig, TS2K_AG_T *p);	//-zome
int ts2k_s_ai(RIG *rig, TS2K_AI_T *p);
int ts2k_g_ai(RIG *rig, TS2K_AI_T *p);
int ts2k_s_al(RIG *rig, TS2K_AL_T *p);
int ts2k_g_al(RIG *rig, TS2K_AL_T *p);
int ts2k_s_am(RIG *rig, TS2K_AM_T *p);	//-zone
int ts2k_g_am(RIG *rig, TS2K_AM_T *p);	//-zome
int ts2k_s_an(RIG *rig, TS2K_AN_T *p);
int ts2k_g_an(RIG *rig, TS2K_AN_T *p);
int ts2k_s_ar(RIG *rig, TS2K_AR_T *p);	//-zone
int ts2k_g_ar(RIG *rig, TS2K_AR_T *p);	//-zome
int ts2k_s_as(RIG *rig, TS2K_AS_T *p);	//-zone
int ts2k_g_as(RIG *rig, TS2K_AS_T *p);	//-zome
int ts2k_s_bc(RIG *rig, TS2K_BC_T *p);
int ts2k_g_bc(RIG *rig, TS2K_BC_T *p);
int ts2k_s_bd(RIG *rig, TS2K_BD_T *p);
int ts2k_g_bd(RIG *rig, TS2K_BD_T *p);
int ts2k_s_bu(RIG *rig, TS2K_BU_T *p);
int ts2k_g_bu(RIG *rig, TS2K_BU_T *p);
int ts2k_s_bp(RIG *rig, TS2K_BP_T *p);
int ts2k_g_bp(RIG *rig, TS2K_BP_T *p);
int ts2k_s_bu(RIG *rig, TS2K_BU_T *p);	//-zone
int ts2k_g_bu(RIG *rig, TS2K_BU_T *p);	//-zome
int ts2k_s_by(RIG *rig, TS2K_BY_T *p);	//-zone
int ts2k_g_by(RIG *rig, TS2K_BY_T *p);	//-zome
int ts2k_s_ca(RIG *rig, TS2K_CA_T *p);	//-zone
int ts2k_g_ca(RIG *rig, TS2K_CA_T *p);	//-zome
int ts2k_s_cg(RIG *rig, TS2K_CG_T *p);
int ts2k_g_cg(RIG *rig, TS2K_CG_T *p);
int ts2k_s_ch(RIG *rig, TS2K_CH_T *p);	//-zone
int ts2k_g_ch(RIG *rig, TS2K_CH_T *p);	//-zome
int ts2k_s_ci(RIG *rig, TS2K_CI_T *p);	//-zone
int ts2k_g_ci(RIG *rig, TS2K_CI_T *p);	//-zome
int ts2k_s_cm(RIG *rig, TS2K_CM_T *p);	//-zone
int ts2k_g_cm(RIG *rig, TS2K_CM_T *p);	//-zome
int ts2k_s_cn(RIG *rig, TS2K_CN_T *p);	//-zone
int ts2k_g_cn(RIG *rig, TS2K_CN_T *p);	//-zome
int ts2k_s_ct(RIG *rig, TS2K_CT_T *p);	//-zone
int ts2k_g_ct(RIG *rig, TS2K_CT_T *p);	//-zome
int ts2k_s_dc(RIG *rig, TS2K_DC_T *p);	//-zone
int ts2k_g_dc(RIG *rig, TS2K_DC_T *p);	//-zome
int ts2k_s_dn(RIG *rig, TS2K_DN_T *p);	//-zone
int ts2k_g_dn(RIG *rig, TS2K_DN_T *p);	//-zome
int ts2k_s_dq(RIG *rig, TS2K_DQ_T *p);	//-zone
int ts2k_g_dq(RIG *rig, TS2K_DQ_T *p);	//-zome
int ts2k_s_fa(RIG *rig, TS2K_FA_T *p);
int ts2k_g_fa(RIG *rig, TS2K_FA_T *p);
int ts2k_s_fb(RIG *rig, TS2K_FB_T *p);
int ts2k_g_fb(RIG *rig, TS2K_FB_T *p);
int ts2k_s_fc(RIG *rig, TS2K_FC_T *p);
int ts2k_g_fc(RIG *rig, TS2K_FC_T *p);
int ts2k_s_ex(RIG *rig, TS2K_EX_T *p);	//-zone
int ts2k_g_ex(RIG *rig, TS2K_EX_T *p);	//-zome
int ts2k_s_fa(RIG *rig, TS2K_FA_T *p);	//-zone
int ts2k_g_fa(RIG *rig, TS2K_FA_T *p);	//-zome
int ts2k_s_fb(RIG *rig, TS2K_FB_T *p);	//-zone
int ts2k_g_fb(RIG *rig, TS2K_FB_T *p);	//-zome
int ts2k_s_fc(RIG *rig, TS2K_FC_T *p);	//-zone
int ts2k_g_fc(RIG *rig, TS2K_FC_T *p);	//-zome
int ts2k_s_fd(RIG *rig, TS2K_FD_T *p);	//-zone
int ts2k_g_fd(RIG *rig, TS2K_FD_T *p);	//-zome
int ts2k_s_fr(RIG *rig, TS2K_FR_T *p);
int ts2k_g_fr(RIG *rig, TS2K_FR_T *p);
int ts2k_s_fs(RIG *rig, TS2K_FS_T *p);	//-zone
int ts2k_g_fs(RIG *rig, TS2K_FS_T *p);	//-zome
int ts2k_s_ft(RIG *rig, TS2K_FT_T *p);
int ts2k_g_ft(RIG *rig, TS2K_FT_T *p);
int ts2k_s_fw(RIG *rig, TS2K_FW_T *p);	//-zone
int ts2k_g_fw(RIG *rig, TS2K_FW_T *p);	//-zome
int ts2k_s_gt(RIG *rig, TS2K_GT_T *p);
int ts2k_g_gt(RIG *rig, TS2K_GT_T *p);
int ts2k_s_id(RIG *rig, TS2K_ID_T *p);
int ts2k_g_id(RIG *rig, TS2K_ID_T *p);
int ts2k_s_if(RIG *rig, TS2K_IF_T *p);	//-zone
int ts2k_g_if(RIG *rig, TS2K_IF_T *p);	//-zome
int ts2k_s_is(RIG *rig, TS2K_IS_T *p);	//-zone
int ts2k_g_is(RIG *rig, TS2K_IS_T *p);	//-zome
int ts2k_s_ks(RIG *rig, TS2K_KS_T *p);
int ts2k_g_ks(RIG *rig, TS2K_KS_T *p);
int ts2k_s_ky(RIG *rig, TS2K_KY_S_T *p);	//-zone
int ts2k_g_ky(RIG *rig, TS2K_KY_A_T *p);	//-zome
int ts2k_s_lk(RIG *rig, TS2K_LK_T *p);	//-zone
int ts2k_g_lk(RIG *rig, TS2K_LK_T *p);	//-zome
int ts2k_s_lm(RIG *rig, TS2K_LM_T *p);
int ts2k_g_lm(RIG *rig, TS2K_LM_T *p);
int ts2k_s_lt(RIG *rig, TS2K_LT_T *p);	//-zone
int ts2k_g_lt(RIG *rig, TS2K_LT_T *p);	//-zome
int ts2k_s_mc(RIG *rig, TS2K_MC_T *p);
int ts2k_g_mc(RIG *rig, TS2K_MC_T *p);
int ts2k_s_md(RIG *rig, TS2K_MD_T *p);
int ts2k_g_md(RIG *rig, TS2K_MD_T *p);
int ts2k_s_mf(RIG *rig, TS2K_MF_T *p);	//-zone
int ts2k_g_mf(RIG *rig, TS2K_MF_T *p);	//-zome
int ts2k_s_mg(RIG *rig, TS2K_MG_T *p);	//-zone
int ts2k_g_mg(RIG *rig, TS2K_MG_T *p);	//-zome
int ts2k_s_ml(RIG *rig, TS2K_ML_T *p);
int ts2k_g_ml(RIG *rig, TS2K_ML_T *p);
int ts2k_s_mo(RIG *rig, TS2K_MO_T *p);	//-zone
int ts2k_g_mo(RIG *rig, TS2K_MO_T *p);	//-zome
int ts2k_s_mr(RIG *rig, TS2K_MR_T *p);	//-zone
int ts2k_g_mr(RIG *rig, TS2K_MR_T *p);	//-zome
int ts2k_s_mu(RIG *rig, TS2K_MU_T *p);	//-zone
int ts2k_g_mu(RIG *rig, TS2K_MU_T *p);	//-zome
int ts2k_s_mw(RIG *rig, TS2K_MW_T *p);	//-zone
int ts2k_g_mw(RIG *rig, TS2K_MW_T *p);	//-zome
int ts2k_s_nb(RIG *rig, TS2K_NB_T *p);	//-zone
int ts2k_g_nb(RIG *rig, TS2K_NB_T *p);	//-zome
int ts2k_s_nl(RIG *rig, TS2K_NL_T *p);
int ts2k_g_nl(RIG *rig, TS2K_NL_T *p);
int ts2k_s_nr(RIG *rig, TS2K_NR_T *p);
int ts2k_g_nr(RIG *rig, TS2K_NR_T *p);
int ts2k_s_nt(RIG *rig, TS2K_NT_T *p);	//-zone
int ts2k_g_nt(RIG *rig, TS2K_NT_T *p);	//-zome
int ts2k_s_of(RIG *rig, TS2K_OF_T *p);	//-zone
int ts2k_g_of(RIG *rig, TS2K_OF_T *p);	//-zome
int ts2k_s_oi(RIG *rig, TS2K_OI_T *p);	//-zone
int ts2k_g_oi(RIG *rig, TS2K_OI_T *p);	//-zome
int ts2k_s_os(RIG *rig, TS2K_OS_T *p);
int ts2k_g_os(RIG *rig, TS2K_OS_T *p);
int ts2k_s_pa(RIG *rig, TS2K_PA_S_T *p);	//-zone
int ts2k_g_pa(RIG *rig, TS2K_PA_A_T *p);	//-zome
int ts2k_s_pb(RIG *rig, TS2K_PB_T *p);
int ts2k_g_pb(RIG *rig, TS2K_PB_T *p);
int ts2k_s_pc(RIG *rig, TS2K_PC_T *p);
int ts2k_g_pc(RIG *rig, TS2K_PC_T *p);
int ts2k_s_pi(RIG *rig, TS2K_PI_T *p);
int ts2k_g_pi(RIG *rig, TS2K_PI_T *p);
int ts2k_g_pl(RIG *rig, TS2K_PL_T *p);	//-zome
int ts2k_s_pm(RIG *rig, TS2K_PM_T *p);
int ts2k_g_pm(RIG *rig, TS2K_PM_T *p);
int ts2k_s_pr(RIG *rig, TS2K_PR_T *p);	//-zone
int ts2k_g_pr(RIG *rig, TS2K_PR_T *p);	//-zome
int ts2k_s_ps(RIG *rig, TS2K_PS_T *p);	//-zone
int ts2k_g_ps(RIG *rig, TS2K_PS_T *p);	//-zome
int ts2k_s_qc(RIG *rig, TS2K_QC_T *p);
int ts2k_g_qc(RIG *rig, TS2K_QC_T *p);
int ts2k_s_qi(RIG *rig, TS2K_QI_T *p);	//-zone
int ts2k_g_qi(RIG *rig, TS2K_QI_T *p);	//-zome
int ts2k_s_qr(RIG *rig, TS2K_QR_T *p);	//-zone
int ts2k_g_qr(RIG *rig, TS2K_QR_T *p);	//-zome
int ts2k_s_ra(RIG *rig, TS2K_RA_S_T *p);	//-zone
int ts2k_g_ra(RIG *rig, TS2K_RA_A_T *p);	//-zome
int ts2k_s_rc(RIG *rig, TS2K_RC_T *p);	//-zone
int ts2k_g_rc(RIG *rig, TS2K_RC_T *p);	//-zome
int ts2k_s_rd(RIG *rig, TS2K_RD_S_T *p);	//-zone
int ts2k_g_rd(RIG *rig, TS2K_RD_A_T *p);	//-zome
int ts2k_s_rg(RIG *rig, TS2K_RG_T *p);
int ts2k_g_rg(RIG *rig, TS2K_RG_T *p);
int ts2k_s_rl(RIG *rig, TS2K_RL_T *p);	//-zone
int ts2k_g_rl(RIG *rig, TS2K_RL_T *p);	//-zome
int ts2k_s_rm(RIG *rig, TS2K_RM_S_T *p);	//-zone
int ts2k_g_rm(RIG *rig, TS2K_RM_A_T *p);	//-zome
int ts2k_s_rt(RIG *rig, TS2K_RT_T *p);	//-zone
int ts2k_g_rt(RIG *rig, TS2K_RT_T *p);	//-zome
int ts2k_s_ru(RIG *rig, TS2K_RU_S_T *p);	//-zone
int ts2k_g_ru(RIG *rig, TS2K_RU_A_T *p);	//-zome
int ts2k_s_rx(RIG *rig, TS2K_RX_T *p);	//-zone
int ts2k_g_rx(RIG *rig, TS2K_RX_T *p);	//-zome
int ts2k_s_sa(RIG *rig, TS2K_SA_S_T *p);	//-zone
int ts2k_g_sa(RIG *rig, TS2K_SA_A_T *p);	//-zome
int ts2k_s_sb(RIG *rig, TS2K_SB_T *p);	//-zone
int ts2k_g_sb(RIG *rig, TS2K_SB_T *p);	//-zome
int ts2k_s_sc(RIG *rig, TS2K_SC_T *p);
int ts2k_g_sc(RIG *rig, TS2K_SC_T *p);
int ts2k_s_sd(RIG *rig, TS2K_SD_T *p);	//-zone
int ts2k_g_sd(RIG *rig, TS2K_SD_T *p);	//-zome
int ts2k_s_sh(RIG *rig, TS2K_SH_T *p);	//-zone
int ts2k_g_sh(RIG *rig, TS2K_SH_T *p);	//-zome
int ts2k_s_si(RIG *rig, TS2K_SI_T *p);	//-zone
int ts2k_g_si(RIG *rig, TS2K_SI_T *p);	//-zome
int ts2k_s_sl(RIG *rig, TS2K_SL_T *p);	//-zone
int ts2k_g_sl(RIG *rig, TS2K_SL_T *p);	//-zome
int ts2k_s_sm(RIG *rig, TS2K_SM_T *p);	//-zone
int ts2k_g_sm(RIG *rig, TS2K_SM_T *p);	//-zome
int ts2k_s_sq(RIG *rig, TS2K_SQ_T *p);	//-zone
int ts2k_g_sq(RIG *rig, TS2K_SQ_T *p);	//-zome
int ts2k_s_sr(RIG *rig, TS2K_SR_T *p);
int ts2k_g_sr(RIG *rig, TS2K_SR_T *p);
int ts2k_s_ss(RIG *rig, TS2K_SS_T *p);
int ts2k_g_ss(RIG *rig, TS2K_SS_T *p);
int ts2k_s_st(RIG *rig, TS2K_ST_T *p);	//-zone
int ts2k_g_st(RIG *rig, TS2K_ST_T *p);	//-zome
int ts2k_s_su(RIG *rig, TS2K_SU_T *p);	//-zone
int ts2k_g_su(RIG *rig, TS2K_SU_T *p);	//-zome
int ts2k_s_sv(RIG *rig, TS2K_SV_T *p);	//-zone
int ts2k_g_sv(RIG *rig, TS2K_SV_T *p);	//-zome
int ts2k_s_tc(RIG *rig, TS2K_TC_T *p);	//-zone
int ts2k_g_tc(RIG *rig, TS2K_TC_T *p);	//-zome
int ts2k_s_td(RIG *rig, TS2K_TD_T *p);	//-zone
int ts2k_g_td(RIG *rig, TS2K_TD_T *p);	//-zome
int ts2k_s_ti(RIG *rig, TS2K_TI_T *p);	//-zone
int ts2k_g_ti(RIG *rig, TS2K_TI_T *p);	//-zome
int ts2k_s_tn(RIG *rig, TS2K_TN_T *p);	//-zone
int ts2k_g_tn(RIG *rig, TS2K_TN_T *p);	//-zome
int ts2k_s_to(RIG *rig, TS2K_TO_T *p);
int ts2k_g_to(RIG *rig, TS2K_TO_T *p);
int ts2k_s_ts(RIG *rig, TS2K_TS_T *p);	//-zone
int ts2k_g_ts(RIG *rig, TS2K_TS_T *p);	//-zome
int ts2k_s_tx(RIG *rig, TS2K_TX_T *p);	//-zone
int ts2k_g_tx(RIG *rig, TS2K_TX_T *p);	//-zome
int ts2k_s_ty(RIG *rig, TS2K_TY_T *p);	//-zone
int ts2k_g_ty(RIG *rig, TS2K_TY_T *p);	//-zome
int ts2k_s_ul(RIG *rig, TS2K_UL_T *p);	//-zone
int ts2k_g_ul(RIG *rig, TS2K_UL_T *p);	//-zome
int ts2k_s_up(RIG *rig, TS2K_UP_T *p);	//-zone
int ts2k_g_up(RIG *rig, TS2K_UP_T *p);	//-zome
int ts2k_s_vd(RIG *rig, TS2K_VD_T *p);	//-zone
int ts2k_g_vd(RIG *rig, TS2K_VD_T *p);	//-zome
int ts2k_s_vg(RIG *rig, TS2K_VG_T *p);
int ts2k_g_vg(RIG *rig, TS2K_VG_T *p);
int ts2k_s_vr(RIG *rig, TS2K_VR_T *p);	//-zone
int ts2k_g_vr(RIG *rig, TS2K_VR_T *p);	//-zome
int ts2k_s_vx(RIG *rig, TS2K_VX_T *p);	//-zone
int ts2k_g_vx(RIG *rig, TS2K_VX_T *p);	//-zome
int ts2k_s_xt(RIG *rig, TS2K_XT_T *p);	//-zone
int ts2k_g_xt(RIG *rig, TS2K_XT_T *p);	//-zome

#endif	// _TS2K_CMDS_H
// end
