/* 
 * Hamlib sample program	Check new TS-2000
 */

/*
 * No TX allowed!!  Use 'testptt' program!
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <hamlib/rig.h>
#include "misc.h"
#include "tests.h"
#include "../kenwood/ts2k/ts2k.h"

#define LOG(msg)	fprintf(stderr, msg); fprintf(stdout, msg)

int errcnt = 0, passcnt = 0, expecterr = 0;

int testvfo(RIG *, vfo_t);
int testfreq(RIG *, vfo_t, freq_t);
int testmode(RIG *, vfo_t, rmode_t);
int testtone(RIG *, vfo_t, tone_t);
int testrptrshift(RIG *, vfo_t, rptr_shift_t);
int testrptroffs(RIG *, vfo_t, shortfreq_t);
int testchan(RIG *, vfo_t vfo, channel_t *);

int main(int argc, char *argv[])
{
	RIG *my_rig;		/* handle to rig (nstance) */
	int retcode;		/* generic return code from functions */
	rig_model_t myrig_model;
	channel_t chantest;
	vfo_t vfo;

	printf("\n\t\tHamlib test: TS-2000\n\n");

 	/*
	 * allocate memory, setup & open port 
	 */

	if (argc < 2) {
			port_t myport;
			/* may be overriden by backend probe */
			myport.type.rig = RIG_PORT_SERIAL;
			myport.parm.serial.rate = 9600;
			myport.parm.serial.data_bits = 8;
			myport.parm.serial.stop_bits = 1;
			myport.parm.serial.parity = RIG_PARITY_NONE;
			myport.parm.serial.handshake = RIG_HANDSHAKE_NONE;
			strncpy(myport.pathname, SERIAL_PORT, FILPATHLEN);

			rig_load_all_backends();
			myrig_model = rig_probe(&myport);
	} else {
			myrig_model = atoi(argv[1]);
	}

	my_rig = rig_init(myrig_model);
		
	if (!my_rig) {
		fprintf(stderr,"Unknown rig num: %d\n", myrig_model);
		fprintf(stderr,"Please check riglist.h\n");
		exit(1); /* whoops! something went wrong (mem alloc?) */
	}

	strncpy(my_rig->state.rigport.pathname,SERIAL_PORT,FILPATHLEN);

	retcode = rig_open(my_rig);
	if (retcode != RIG_OK) {
		printf("rig_open: error = %s\n", rigerror(retcode));
		exit(2);
	}

/*	retcode = rig_setup(my_rig, 299, "/dev/ttyS1");
	if (retcode != RIG_OK) {
		printf("rig_open: error = %s\n", rigerror(retcode));
		exit(2);
	}
*/
	printf("Port %s opened ok\n", SERIAL_PORT);


	LOG("\n/********* Begin VFO Test **************/\n\n");

	LOG("\nStarting VFO tests...\n\n");

// VFO always tested first.

// Normal VFO's
	testvfo(my_rig, RIG_VFO_A);
	testvfo(my_rig, RIG_VFO_B);
	testvfo(my_rig, RIG_VFO_C);

// Split VFO's
	testvfo(my_rig, RIG_VFO_AB);
	testvfo(my_rig, RIG_VFO_BA);

// Memories
	testvfo(my_rig, RIG_MEM_A);
	testvfo(my_rig, RIG_MEM_C);
	testvfo(my_rig, RIG_CALL_A);
	testvfo(my_rig, RIG_CALL_C);

// Special Selections
	LOG("The following won't match, but should set bits.\n");
	LOG("\n*******************************************\n\n");

// PTT/CTRL on Main
	testvfo(my_rig, RIG_CTRL_MAIN);
	expecterr++;
	testvfo(my_rig, RIG_CTRL_SUB);
	expecterr++;

// Satellite
	testvfo(my_rig, RIG_CTRL_SAT);
	expecterr++;
	testvfo(my_rig, RIG_SAT_UPLINK);
	expecterr++;
	testvfo(my_rig, RIG_SAT_DNLINK);
	expecterr++;

// XIT/RIT
	// coming soon to a rig near you...
	testvfo(my_rig, RIG_CTRL_RIT);
	expecterr++;
	testvfo(my_rig, RIG_CTRL_XIT);
	expecterr++;

// Crossband repeater
	testvfo(my_rig, RIG_CTRL_CROSS);
	expecterr++;

	LOG("The following shouldn't fail, but should be Error.\n");
	LOG("\n*******************************************\n\n");

// Silly requests.
	testvfo(my_rig, RIG_VFO_CURR);
	expecterr++;
	testvfo(my_rig, RIG_VFO_CURR);
	expecterr++;		// debug
	testvfo(my_rig, RIG_VFO_VFO);
	expecterr++;

	LOG("\n/********* Begin FREQ Test **************/\n\n");

	LOG("\nStarting FREQ tests...\n\n");

// Set a simple VFO to start with
	testvfo(my_rig, RIG_VFO_A);

// Normal VFO's
	testfreq(my_rig, RIG_VFO_CURR, MHz(145.0));
	testfreq(my_rig, RIG_VFO_A, MHz(145.1));
	testfreq(my_rig, RIG_VFO_B, MHz(145.2));
	testfreq(my_rig, RIG_VFO_C, MHz(145.3));
	{
		freq_t a, b, c;

		rig_get_freq(my_rig, RIG_VFO_A, &a);
		rig_get_freq(my_rig, RIG_VFO_B, &b);
		rig_get_freq(my_rig, RIG_VFO_C, &c);

		if (a != MHz(145.1) || b != MHz(145.2) || c != MHz(145.3)) {
			LOG("Individual frequencies not set or read correctly!\n");
			LOG("Error!\n");
			errcnt++;
		} else {
			passcnt++;
		}
	}
// Simulate VFO scan for timing test
/*	{
		int e, x, p;
		freq_t f, t;

		testvfo(my_rig, RIG_VFO_A);

		p = passcnt;
		e = errcnt;
		x = expecterr;
		passcnt = expecterr = errcnt = 0;

		for (f = MHz(0); f <= MHz(148 - 144); f += Hz(100)) {
			rig_get_freq(my_rig, RIG_VFO_CURR, &t);
			rig_set_freq(my_rig, RIG_VFO_CURR, t + f);
		}

		fprintf(stdout, "Expected Error count =\t%i\n", expecterr);
		fprintf(stdout, "Actual Error count =\t%i\n", errcnt);
		fprintf(stdout, "Pass count =\t%i\n", passcnt);

		passcnt = p;
		errcnt = e;
		expecterr = x;
	}
*/
// Split VFO's
//      testfreq(my_rig, RIG_VFO_B, MHz(29.360));
//      testfreq(my_rig, RIG_VFO_BA, MHz(29.370));

// more to be developed...

	LOG("\n/************* Mode Test ***************/\n\n");

	testmode(my_rig, RIG_VFO_C, RIG_MODE_AM);	// VFOC is AM/FM only
	testmode(my_rig, RIG_VFO_B, RIG_MODE_CW);
	testmode(my_rig, RIG_VFO_A, RIG_MODE_LSB);
	testmode(my_rig, RIG_VFO_B, RIG_MODE_USB);
	testmode(my_rig, RIG_VFO_A, RIG_MODE_RTTY);
	testmode(my_rig, RIG_VFO_A, RIG_MODE_FM);	// FM last.  Needed for rptr_shift.

	LOG("\n/************* Tone Test ***************/\n\n");


	/* The kHz() is a misnomer.  It's 103.5 Hz. */

	// This one should round up to something, but will cause an Error.
//	testtone(my_rig, RIG_VFO_A, kHz(0.990));
//	expecterr++;
	// These two should be exact.
	testvfo(my_rig, RIG_VFO_A);
	testtone(my_rig, RIG_VFO_A, kHz(1.622));
	testtone(my_rig, RIG_VFO_A, kHz(2.035));

	LOG("\n/************* Repeater Shift Test ***************/\n\n");

	/* VFO must be in FM mode or we fail! */
	testrptrshift(my_rig, RIG_VFO_A, RIG_RPT_SHIFT_PLUS);
	testrptrshift(my_rig, RIG_VFO_A, RIG_RPT_SHIFT_MINUS);
	testrptrshift(my_rig, RIG_VFO_A, RIG_RPT_SHIFT_NONE);

	LOG("\n/************* Repeater Offset Test ***************/\n\n");

	testrptroffs(my_rig, RIG_VFO_A, kHz(600));
	testrptroffs(my_rig, RIG_VFO_A, MHz(59));

	LOG("\n/************* Channel Read/Write Test ***************/\n\n");

	chantest.channel_num = 90;	// My test channel
	vfo = RIG_VFO_MEM;		// Direct access.
	testchan(my_rig, vfo, &chantest);

	LOG("\n/******* Set State after Test **********/\n\n");

// (: VFO we want after Testing.  :)
	fprintf(stdout, "Setting my default mode.\n");
	//testvfo(my_rig, (RIG_CTRL_SCAN | RIG_MEM_C));
	testvfo(my_rig, (RIG_MEM_C));
	//testvfo(my_rig, (RIG_CTRL_SCAN | RIG_MEM_A));
	testvfo(my_rig, (RIG_MEM_A));

	fprintf(stdout, "Expected Error count =\t%i\n", expecterr);
	fprintf(stdout, "Actual Error count =\t%i\n", errcnt);
	fprintf(stdout, "Pass count =\t%i\n", passcnt);


	LOG("\n/*********** End of Testing **************/\n\n");

	rig_close(my_rig);	/* close port */
	rig_cleanup(my_rig);	/* if you care about memory */

	printf("port %s closed ok \n", SERIAL_PORT);

	return 0;
}

/*
 * This test is more extensive that the others I use.
 */
int testvfo(RIG * rig, vfo_t vfo)
{
	static int sretval, gretval;
	static vfo_t vtmp;

	fprintf(stderr, "/****** " __FUNCTION__ " ******/\n");

	sretval = rig_set_vfo(rig, vfo);
	gretval = rig_get_vfo(rig, &vtmp);

	vtmp = vtmp & ~(RIG_VFO_PTT | RIG_VFO_CTRL);

	if (sretval != RIG_OK)
		fprintf(stderr, __FUNCTION__
			": set_vfo() failed: %s\n", rigerror(sretval));
	if (gretval != RIG_OK)
		fprintf(stderr, __FUNCTION__
			": get_vfo() failed: %s\n", rigerror(gretval));

	fprintf(stdout, "sent\t\tVFO = %s\n", strvfo(vfo));
	fprintf(stdout, "received\tVFO = %s\n", strvfo(vtmp));

	if (vfo != vtmp) {
		errcnt++;
		fprintf(stderr, "sent\t\tVFO = %s\n", strvfo(vfo));
		fprintf(stderr, "received\tVFO = %s\n", strvfo(vtmp));
		LOG("Error!\n\n");
	} else {
		passcnt++;
		fprintf(stdout, "Correct.\n\n");
	}

	fprintf(stderr,
		"#************** End of Command ***************\n\n");

	return RIG_OK;
}

int testfreq(RIG * rig, vfo_t vfo, freq_t freq)
{
	static int sretval, gretval;
	static freq_t ftmp;

	fprintf(stderr, "/****** " __FUNCTION__ " ******/\n");

	sretval = rig_set_freq(rig, vfo, freq);
	gretval = rig_get_freq(rig, vfo, &ftmp);

	vfo = vfo & ~(RIG_VFO_PTT | RIG_VFO_CTRL);

	if (sretval != RIG_OK)
		fprintf(stderr, __FUNCTION__
			": set_freq() failed: %s\n", rigerror(sretval));
	if (gretval != RIG_OK)
		fprintf(stderr, __FUNCTION__
			": get_freq() failed: %s\n", rigerror(gretval));

	fprintf(stdout, "sent\t\tFREQ = %lli Hz\n", (long long)(freq_t)freq);
	fprintf(stdout, "received\tFREQ = %lli Hz\n", (long long)(freq_t)ftmp);

	if (freq != ftmp) {
		errcnt++;
		fprintf(stderr, "sent FREQ = %lli Hz\n", (long long)freq);
		fprintf(stderr, "received FREQ = %lli Hz\n", (long long)ftmp);
		LOG("Error!\n\n");
	} else {
		passcnt++;
		fprintf(stdout, "Correct.\n\n");
	}

	fprintf(stderr,
		"#************** End of Command ***************\n\n");

	return RIG_OK;
}

int testmode(RIG * rig, vfo_t vfo, rmode_t mode)
{
	static int sretval, gretval;
	static rmode_t mtmp;
	static pbwidth_t pb = 0;

	fprintf(stderr, "/****** " __FUNCTION__ " ******/\n");

	sretval = rig_set_mode(rig, vfo, mode, pb);
	gretval = rig_get_mode(rig, vfo, &mtmp, &pb);

	vfo = vfo & ~(RIG_VFO_PTT | RIG_VFO_CTRL);

	if (sretval != RIG_OK)
		fprintf(stderr, __FUNCTION__
			": set_mode() failed: %s\n", rigerror(sretval));
	if (gretval != RIG_OK)
		fprintf(stderr, __FUNCTION__
			": get_mode() failed: %s\n", rigerror(gretval));

	fprintf(stdout, "sent\t\tMODE = %s\n", strrmode(mode));
	fprintf(stdout, "received\tMODE = %s\n", strrmode(mtmp));

	if (mode != mtmp) {
		errcnt++;
		fprintf(stderr, "sent MODE = %s\n", strrmode(mode));
		fprintf(stderr, "received MODE = %s\n", strrmode(mtmp));
		LOG("Error!\n\n");
	} else {
		passcnt++;
		fprintf(stdout, "Correct.\n\n");
	}

	fprintf(stderr,
		"#************** End of Command ***************\n\n");

	return RIG_OK;
}

int testtone(RIG * rig, vfo_t vfo, tone_t tone)
{
	static int sretval, gretval;
	static tone_t ttmp;

	fprintf(stderr, "/****** " __FUNCTION__ " ******/\n");

	sretval = rig_set_tone(rig, vfo, tone);
	gretval = rig_get_tone(rig, vfo, &ttmp);

	vfo = vfo & ~(RIG_VFO_PTT | RIG_VFO_CTRL);

	if (sretval != RIG_OK)
		fprintf(stderr, __FUNCTION__
			": set_tone() failed: %s\n", rigerror(sretval));
	if (gretval != RIG_OK)
		fprintf(stderr, __FUNCTION__
			": get_tone() failed: %s\n", rigerror(gretval));

	fprintf(stdout, "sent\t\tTONE = %i\n", (tone));
	fprintf(stdout, "received\tTONE = %i\n", (ttmp));

	if (tone != ttmp) {
		errcnt++;
		fprintf(stderr, "sent TONE = %i\n", (tone));
		fprintf(stderr, "received TONE = %i\n", (ttmp));
		LOG("Error!\n\n");
	} else {
		passcnt++;
		fprintf(stdout, "Correct.\n\n");
	}

	fprintf(stderr,
		"#************** End of Command ***************\n\n");

	return RIG_OK;
}

int testrptrshift(RIG * rig, vfo_t vfo, rptr_shift_t shift)
{
	static int sretval, gretval;
	static rptr_shift_t stmp;

	fprintf(stderr, "/****** " __FUNCTION__ " ******/\n");

	sretval = rig_set_rptr_shift(rig, vfo, shift);
	gretval = rig_get_rptr_shift(rig, vfo, &stmp);

	vfo = vfo & ~(RIG_VFO_PTT | RIG_VFO_CTRL);

	if (sretval != RIG_OK)
		fprintf(stderr, __FUNCTION__
			": set_rptr_shift() failed: %s\n", rigerror(sretval));
	if (gretval != RIG_OK)
		fprintf(stderr, __FUNCTION__
			": get_rptr_shift() failed: %s\n", rigerror(gretval));

	fprintf(stdout, "sent\t\tShift = %s\n", strptrshift(shift));
	fprintf(stdout, "received\tShift = %s\n", strptrshift(stmp));

	if (shift != stmp) {
		errcnt++;
		fprintf(stderr, "sent Shift = %s\n", strptrshift(shift));
		fprintf(stderr, "received Shift = %s\n", strptrshift(stmp));
		LOG("Error!\n\n");
	} else {
		passcnt++;
		fprintf(stdout, "Correct.\n\n");
	}

	fprintf(stderr,
		"#************** End of Command ***************\n\n");

	return RIG_OK;
}

int testrptroffs(RIG * rig, vfo_t vfo, shortfreq_t offset)
{
	static int sretval, gretval;
	static shortfreq_t otmp;

	fprintf(stderr, "/****** " __FUNCTION__ " ******/\n");

	sretval = rig_set_rptr_offs(rig, vfo, offset);
	gretval = rig_get_rptr_offs(rig, vfo, &otmp);

	vfo = vfo & ~(RIG_VFO_PTT | RIG_VFO_CTRL);

	if (sretval != RIG_OK)
		fprintf(stderr, __FUNCTION__
			": set_rptr_offs() failed: %s\n", rigerror(sretval));
	if (gretval != RIG_OK)
		fprintf(stderr, __FUNCTION__
			": get_rptr_offs() failed: %s\n", rigerror(gretval));

	fprintf(stdout, "sent\t\tOffset = %i\n", (int)offset);
	fprintf(stdout, "received\tOffset = %i\n", (int)otmp);

	if (offset != otmp) {
		errcnt++;
		fprintf(stderr, "sent Offset = %i\n", (int)(offset));
		fprintf(stderr, "received Offset = %i\n", (int)(otmp));
		LOG("Error!\n\n");
	} else {
		passcnt++;
		fprintf(stdout, "Correct.\n\n");
	}

	fprintf(stderr,
		"#************** End of Command ***************\n\n");

	return RIG_OK;
}

int testchan(RIG * rig, vfo_t vfo, channel_t *chan)
{
	int sretval, gretval, chan_err;
	channel_t ctmp;

	fflush(stdout);	// maybe this'll work instead...
	fprintf(stderr, "/****** " __FUNCTION__ " ******/\n");

	ctmp.vfo = chan->vfo = vfo;
	ctmp.channel_num = chan->channel_num;

	gretval = rig_get_channel(rig, chan);

#define COPYCHAN(_a)	ctmp._a = chan->_a;

	ctmp.next = NULL;	// debug
//	COPYCHAN(next);
	COPYCHAN(channel_num);
	COPYCHAN(bank_num);
	COPYCHAN(vfo);
	COPYCHAN(freq);
	COPYCHAN(mode);
	COPYCHAN(tx_vfo);	// obsolete
	COPYCHAN(tx_freq);
	COPYCHAN(tx_mode);
	COPYCHAN(tx_width);
	COPYCHAN(split);
	COPYCHAN(rptr_shift);
	COPYCHAN(rptr_offs);
	COPYCHAN(tuning_step);	// Value wrong on GetMem
	COPYCHAN(rit);		// ENAVAIL
	COPYCHAN(xit);		// ENAVAIL
	COPYCHAN(funcs);
	COPYCHAN(levels[0]);	// ENAVAIL
	COPYCHAN(tone);
	COPYCHAN(tone_sql);	// ENIMPL
	COPYCHAN(ctcss);
	COPYCHAN(ctcss_sql);	// ENIMPL
	COPYCHAN(dcs);
	COPYCHAN(dcs_sql);	// ENIMPL
	COPYCHAN(scan);		// ENAVAIL
	COPYCHAN(scan_group);
	COPYCHAN(flags);
	COPYCHAN(ext_levels);	// ENIMPL

#undef COPYCHAN

	sretval = rig_set_channel(rig, &ctmp);

	vfo = vfo & ~(RIG_VFO_PTT | RIG_VFO_CTRL);

	if (gretval != RIG_OK)
		fprintf(stderr, __FUNCTION__
			": rig_get_channel() failed: %s\n", rigerror(gretval));
	if (sretval != RIG_OK)
		fprintf(stderr, __FUNCTION__
			": rig_set_channel() failed: %s\n", rigerror(sretval));

	fprintf(stdout, "received\tmem = %i\n", (int)ctmp.channel_num);
	fprintf(stdout, "sent\t\tmem = %i\n", (int)chan->channel_num);

	chan_err = 0;

#define TESTCHAN(_a)	chan_err |= chan->_a == ctmp._a
	TESTCHAN(next);
	TESTCHAN(channel_num);
	TESTCHAN(bank_num);
	TESTCHAN(vfo);
	TESTCHAN(freq);
	TESTCHAN(mode);
//	TESTCHAN(tx_vfo);	// obsolete
//	TESTCHAN(tx_freq);
//	TESTCHAN(tx_mode);
//	TESTCHAN(tx_width);
	TESTCHAN(split);
	TESTCHAN(rptr_shift);
	TESTCHAN(rptr_offs);
	TESTCHAN(tuning_step);	// Value wrong on GetMem
//	TESTCHAN(rit);		// ENAVAIL
//	TESTCHAN(xit);		// ENAVAIL
	TESTCHAN(funcs);
	//TESTCHAN(levels[0]);	// ENAVAIL
	TESTCHAN(tone);
//	TESTCHAN(tone_sql);	// ENIMPL
	TESTCHAN(ctcss);
//	TESTCHAN(ctcss_sql);	// ENIMPL
	TESTCHAN(dcs);
//	TESTCHAN(dcs_sql);	// ENIMPL
//	TESTCHAN(scan);		// ENAVAIL
	TESTCHAN(scan_group);
	TESTCHAN(flags);
//	TESTCHAN(ext_levels);	// ENIMPL
#undef TESTCHAN

	if (chan_err) {
		errcnt++;
		fprintf(stderr, "Channel Mismatch!\n");
		fprintf(stderr, "\n\n/**** Received the Following Channel ****/\n");
		ts2k_uniq_PrintChan(rig, chan);
		fprintf(stderr, "\n\n/****** Sent the Following Channel ******/\n");
		ts2k_uniq_PrintChan(rig, &ctmp);
		fprintf(stderr, "\n/*************** Done *******************/\n");
		LOG("Error!\n\n");
	} else {
		passcnt++;
		fprintf(stdout, "Channel Correct.\n\n");
	}

	fprintf(stderr,
		"#************** End of Command ***************\n\n");

	return RIG_OK;
}


