/* 
 * _GENRAND_C_ 
 * 
 * HMCSIM PHYSRAND TEST 
 * 
 * FUNCTIONS TO GENERATE RANDOM 
 * ADDRESS INPUTS FOR TEST
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/* --------------------------------------------- MACROS */
#define ONEGB	1073741824


/* --------------------------------------------- LONGRANDS */
/* 
 * LONGRANDS
 * 
 */
static long longrands( long max_rand )
{
	/* vars */
	long newrand	= 0x00l;
	long a		= 0x00l;
	long b		= 0x00l;
	long shiftvar	= 0x00l;
	/* ---- */

	/* 
	 * decide if we need to bound the rand
	 *
	 */
	if( max_rand > (long)(RAND_MAX) ){

		/* 
	 	 * no need to bound the compute
		 * 
		 */
		a = (long)(rand());
		b = (long)(rand());
		shiftvar = max_rand - (long)(RAND_MAX);	
		shiftvar = shiftvar % (long)(8);
		newrand = a | (b << shiftvar);

	}else{
		/* 
	 	 * bound the compute 
		 *
		 */
		a = (long)(rand() % (int)(max_rand));

		/* 
		 * no need for 'b'
		 *
		 */

		newrand = a;
	}


	return newrand;
}

/* --------------------------------------------- GENRWMIX */
/* 
 * GENRWMIX
 * 
 */
static int genrwmix(	uint32_t *req, 
			uint32_t read_perct, 
			uint32_t write_perct, 
			long num_req  )
{
	/* vars */
	uint32_t i 	= 0;
	long j		= 0;
	int tmp		= 0;
	uint32_t table[100];
	/* --- */

	/* 
	 * mark the reads
	 * 
	 */
	for( i=0; i<read_perct; i++ )
	{
		table[i] = 1;	
	}

	/* 
 	 * mark the writes
	 * 
	 */
	for( i=read_perct; i<100; i++ )
	{
		table[i] = 2;
	}

	/* 
	 * randomly select reads or writes
	 * 
	 */
	for( j=0; j<num_req; j++ )
	{
		tmp = rand() % 99;	

		req[j] = (long)(table[tmp]);
	}	

	return 0;
}

/* --------------------------------------------- GENRANDS */
/* 
 * GENRANDS
 * 
 */
extern int genrands( 	uint64_t *addr, 
			uint32_t *req, 
			long num_req, 
			uint32_t seed, 
			uint32_t num_devs,
			uint32_t capacity, 
			uint32_t read_perct, 
			uint32_t write_perct, 
			uint32_t shiftamt )
{
	/* vars */
	long i		= 0x00l;
	long max_slot	= 0x00l;
	/* ---- */

	/* 
	 * sanity check 
	 * 
	 */
	if( addr == NULL ){
 		return -1;
	}

	if( req == NULL ){ 
		return -1;
	}


	/* 
	 * figure out where the bounds of the memory is
	 * represent it as the offset in 8 byte quantities
 	 *
 	 */
	max_slot = (long)( ((long)(ONEGB)*(long)(capacity)*(long)(num_devs))/(long)(8) );


	/*
	 * seed the random number generator
	 * 
	 */
	srand( (unsigned int)(seed) );

	/* 
	 * generate the addresses via a randomly generated offset
	 * 
	 */
	for( i=0; i<num_req; i++ ){ 

		addr[i] = (uint64_t)(longrands( max_slot ) * (long)(8))<<(uint64_t)(shiftamt);
	
	}
	
	/* 
	 * generate the read/write mix 
	 * 
	 */
	genrwmix( req, read_perct, write_perct, num_req );

	return 0;
}



/* EOF */

