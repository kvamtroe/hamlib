/* 
 * Hamlib sample program	Check new VFO's
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <hamlib/rig.h>
#include "misc.h"
#include "tests.h"

int testvfo(RIG *rig, vfo_t vfo);

int main (int argc, char *argv[])
{ 
	RIG *my_rig;		/* handle to rig (nstance) */
	freq_t freq;		/* frequency  */
	rmode_t rmode;		/* radio mode of operation */
	pbwidth_t width;
	vfo_t vfo;		/* vfo selection */
	int strength;		/* S-Meter level */
	int retcode;		/* generic return code from functions */
	rig_model_t myrig_model;


	printf("testrig:hello, I am your main() !\n");

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

	printf("Port %s opened ok\n", SERIAL_PORT);

	/*
	 * Below are examples of set/get routines.
	 * Must add checking of functionality map prior to command execution -- FS
	 *
	 */

/********* Begin Test **************/

// Normal VFO's
	testvfo(my_rig, RIG_VFO_A);
	testvfo(my_rig, RIG_VFO_B);
	testvfo(my_rig, RIG_VFO_C);

// Split VFO's
	testvfo(my_rig, RIG_VFO_AB);
	testvfo(my_rig, RIG_VFO_BA);

// Memories
	testvfo(my_rig, RIG_VFO_MEM_A);
	testvfo(my_rig, RIG_VFO_MEM_C);
	testvfo(my_rig, RIG_VFO_CALL_A);
	testvfo(my_rig, RIG_VFO_CALL_C);

// Special Selections

// PTT/CTRL on Main
	fprintf(stdout, "The following won't match, but should set bits.\n");
	fprintf(stderr, "The following won't match, but should set bits.\n");
	fprintf(stdout, "***********************************************\n");
	fprintf(stderr, "***********************************************\n");

	testvfo(my_rig, RIG_CTRL_MAIN);
	testvfo(my_rig, RIG_CTRL_SUB);

// Satellite
	testvfo(my_rig, RIG_CTRL_SAT);

// XIT/RIT
	// coming soon to a rig near you...

	fprintf(stdout, "***********************************************\n");
	fprintf(stderr, "***********************************************\n\n");

// VFO we want after Testing.  :)
	fprintf(stdout, "Setting my default mode.\n");
	testvfo(my_rig, RIG_VFO_MEM_C);
	testvfo(my_rig, (RIG_CTRL_SCAN | RIG_VFO_MEM_A));

/********* End of Test **************/

	rig_close(my_rig); /* close port */
	rig_cleanup(my_rig); /* if you care about memory */

	printf("port %s closed ok \n",SERIAL_PORT);

	return 0;
}

int testvfo(RIG *rig, vfo_t vfo)
{
	static int sretval, gretval;
	static vfo_t vtmp;

	sretval = rig_set_vfo(rig, vfo);
	gretval = rig_get_vfo(rig, &vtmp);

	vtmp = vtmp & ~(RIG_VFO_PTT | RIG_VFO_CTRL);

	if(sretval != RIG_OK)
		fprintf(stderr, __FUNCTION__
			": set_vfo() failed: %s\n", rigerror(sretval));
	if(gretval != RIG_OK)
		fprintf(stderr, __FUNCTION__
			": get_vfo() failed: %s\n", rigerror(gretval));

	fprintf(stdout, "sent VFO = %s\n", strvfo(vfo));
	fprintf(stdout, "received VFO = %s\n", strvfo(vtmp));

	fprintf(stdout, "%s\n\n", (vfo==vtmp)? "Correct." : "Error!");

	if(vfo != vtmp) {
		fprintf(stderr, "sent VFO = %s\n", strvfo(vfo));
		fprintf(stderr, "received VFO = %s\n", strvfo(vtmp));
		fprintf(stderr, "%s\n\n", (vfo==vtmp)? "Correct." : "Error!");
	}
	fprintf(stderr, "#************** End of Command ***************\n\n");

	return RIG_OK;
}
