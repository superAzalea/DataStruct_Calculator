/*
 * =====================================================================================
 *
 *       Filename:  test.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/26/2016 21:46:35
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author: Azalea, 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "calc.h"


int main(int argc, char *argv[])
{
	assert(argc >= 2);
	
	struct calc_info *pcalc = (struct calc_info *)
		malloc(sizeof(*pcalc));
	assert(pcalc != NULL);

	calc_init(pcalc);

	printf("%s = %d\n", argv[1], 
		pcalc->parser(pcalc, argv[1]));

	calc_destroy(pcalc);
	free(pcalc);
	
	return 0;
}
