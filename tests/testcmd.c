/* 
 * Hamlib sample program	Check new VFO's? Nooooo. Check new Parser!!!!
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <hamlib/rig.h>
#include "misc.h"
#include "tests.h"

int main(int argc, char *argv[])
{
	int retval;

	printf("testvfo: ts2k Command Parser test.\n");

	fprintf(stderr, "Starting Parse!\n");

	retval = 0;
	while(!retval) {
		fprintf(stdout, "\n%s> ", argv[0]);
		retval = ts2k_yyparse();
	}


	exit(0);
}
