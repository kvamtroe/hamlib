/* 
 * Hamlib sample program	Check new VFO's? Nooooo. Check new Parser!!!!
 */

#include <stdio.h>
#include <unistd.h>
#include "tests.h"
#include "rigcmd.h"

int main(int argc, char *argv[])
{
	int retval;

	printf("testvfo: ts2k Command Parser test.\n");

	fprintf(stderr, "Starting Parse!\n");

	retval = 0;
	while(!retval) {
		fprintf(stdout, "\n%s> ", argv[0]);
		retval = yyparse();
	}


	exit(0);
}
