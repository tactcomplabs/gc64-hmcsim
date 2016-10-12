/* 
 * _STREAM_C_ 
 * 
 * HMCSIM STREAM TEST DRIVER 
 * 
 * Attempts to initialize a HMC instantiation
 * and execute a stream test
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "hmc_sim.h"


/* ----------------------------------------------------- FUNCTION PROTOTYPES */
extern int getshiftamount( 	uint32_t num_links, 
				uint32_t capacity, 
				uint32_t bsize, 
				uint32_t *shiftamt );
extern int execute_test(        struct hmcsim_t *hmc,
                                uint64_t *addr_a,
                                uint64_t *addr_b,
                                uint64_t *addr_c,
				uint64_t addr_scalar,
                                long num_req,
                                uint32_t num_threads, 
				uint32_t simd );

/* ----------------------------------------------------- GENRANDS */
static int genrands( 	uint64_t *addr_a,
			uint64_t *addr_b,
			uint64_t *addr_c,
			uint64_t *addr_scalar,
			long num_req, 
			uint32_t num_devs, 
			uint32_t capacity, 
			uint32_t shiftamt )	
{
	/* vars */
	long i		= 0x00l;
	uint64_t base_a	= 0x00ll;
	uint64_t base_b	= 0x00ll;
	uint64_t base_c = 0x00ll;
	uint64_t nrpo	= 0x00ll;
	uint64_t offset	= 0x00ll;
	/* ---- */

	if( addr_a == NULL ){ 
		return -1;
	}else if( addr_b == NULL ){ 
		return -1;
	}else if( addr_c == NULL ){
		return -1;
	}else if( addr_scalar == NULL ){
		return -1;
	}

	/* 
	 * make sure we have enough capacity
	 *
	 */

#define	SLOTS_PER_GB	134217728

	if( (long)((long)capacity*(long)num_devs*(long)SLOTS_PER_GB) <
		(long)((num_req*(long)(3))+1) ) {
		printf( "ERROR : NOT ENOUGH AVAILABLE PHYSICAL STORAGE\n" );
		return -1;
	}	

	/* 
	 * scalar value : set it to 0x00ll
	 * 
	 */
	*addr_scalar	= (uint64_t)( 0x00ll );

	/* 
	 * calculate the base of each vector
	 * 
	 */
	nrpo	= (uint64_t)(num_req)+1;
	offset	= nrpo*0x08ll;

	base_a	= (0x00ll) + ((0x08ll ) << (uint64_t)(shiftamt));
	base_b	= base_a + ( ( offset ) << (uint64_t)(shiftamt) );
	base_c	= base_b + ( ( offset ) << (uint64_t)(shiftamt) );

	/* 
	 * vectors a, b, c
 	 * 
	 */
	for( i=0; i<num_req; i++ ){ 
		addr_a[i]	= (uint64_t)(base_a + (((uint64_t)(i) * 0x08ll)<<(uint64_t)(shiftamt)) );
		addr_b[i]	= (uint64_t)(base_b + (((uint64_t)(i) * 0x08ll)<<(uint64_t)(shiftamt)) );
		addr_c[i]	= (uint64_t)(base_c + (((uint64_t)(i) * 0x08ll)<<(uint64_t)(shiftamt)) );
	}

	return 0;	
}


/* ----------------------------------------------------- MAIN */
/* 
 * MAIN 
 * 
 */
extern int main( int argc, char **argv )
{
	/* vars */
	int ret			= 0;
	uint32_t i		= 0;
	uint32_t num_devs	= 0;
	uint32_t num_links	= 0;
	uint32_t num_vaults	= 0;
	uint32_t queue_depth	= 0;
	uint32_t num_banks	= 0;
	uint32_t num_drams	= 0;
	uint32_t capacity	= 0;
	uint32_t xbar_depth	= 0;		
	uint32_t num_threads	= 2;
	uint32_t bsize		= 128;
	uint32_t simd		= 1;
	uint32_t shiftamt	= 0;
	long num_req		= 0;
	uint64_t *addr_a	= NULL;
	uint64_t *addr_b	= NULL;
	uint64_t *addr_c	= NULL;
	uint64_t addr_scalar	= 0x00ll;
	struct hmcsim_t hmc;
	/* ---- */

	while(( ret = getopt( argc, argv, "b:c:d:hl:m:n:q:s:v:x:N:T:" )) != -1 )
	{
		switch( ret )
		{
			case 'b': 
				num_banks = (uint32_t)(atoi(optarg));
				break;
			case 'c':
				capacity = (uint32_t)(atoi(optarg));
				break;
			case 'd': 
				num_drams = (uint32_t)(atoi(optarg));
				break;
			case 'h': 
				printf( "%s%s%s\n", "usage : ", argv[0], " -bcdhlnqvx" );
				printf( " -b <num_banks>\n" );
				printf( " -c <capacity>\n" );
				printf( " -d <num_drams>\n" );
				printf( " -h ...print help\n" );
				printf( " -l <num_links>\n" );
				printf( " -m <block_size>\n" );
				printf( " -n <num_devs>\n" );
				printf( " -q <queue_depth>\n" );
				printf( " -s <simd_width>\n" );
				printf( " -v <num_vaults>\n" );
				printf( " -x <xbar_depth>\n" );
				printf( " -N <num_requests>\n" );
				printf( " -T <num_threads>\n" );
				return 0;
				break;
			case 'l':
				num_links = (uint32_t)(atoi(optarg));
				break;
			case 'm': 
				bsize = (uint32_t)(atoi(optarg));
				break;
			case 'n':
				num_devs = (uint32_t)(atoi(optarg));
				break;
			case 'q':
				queue_depth = (uint32_t)(atoi(optarg));
				break;
			case 's':
				simd	= (uint32_t)(atoi(optarg));
				break;
			case 'v': 
				num_vaults = (uint32_t)(atoi(optarg));
				break;
			case 'x': 
				xbar_depth = (uint32_t)(atoi(optarg));
				break;
			case 'N':
				num_req	= (long)(atol(optarg));
				break;
			case 'T':
				num_threads = (uint32_t)(atoi(optarg));
				break;
			case '?':
			default:
				printf( "%s%s%s\n", "Unknown option: see ", argv[0], " -bcdhlmnqsvxNT" );
				return -1;
				break;
		}
	}

	if( ((long)(num_req) % (long)( (long)(simd) * (long)(num_threads) )) != 0 ){
		printf( "ERROR : NUM_REQ MUST BE A DIVISIBLE EVENLY BY NUM_THREADS * SIMD\n" );
		return -1;
	}


	/* 
	 * allocate memory 
	 * 
	 */
	addr_a = malloc( sizeof( uint64_t ) * num_req );
	if( addr_a == NULL ){ 
		printf( "FAILED TO ALLOCATE MEMORY FOR ADDR_A\n" );
		return -1;
	}
	addr_b = malloc( sizeof( uint64_t ) * num_req );
	if( addr_b == NULL ){ 
		printf( "FAILED TO ALLOCATE MEMORY FOR ADDR_B\n" );
		free( addr_a );
		return -1;
	}

	addr_c = malloc( sizeof( uint64_t ) * num_req );
	if( addr_c == NULL ){ 
		printf( "FAILED TO ALLOCATE MEMORY FOR ADDR_C\n" );
		free( addr_a );
		free( addr_b );
		return -1;
	}

	/* 
	 * get the shift amount based upon
	 * the max block size, device size and link count
 	 * 
	 */
	if( getshiftamount( num_links, capacity, bsize, &shiftamt ) != 0 ){ 
		printf( "FAILED TO RETRIEVE SHIFT AMOUNT\n" );
		hmcsim_free( &hmc );
		free( addr_a );
		free( addr_b );
		free( addr_c );
	}


	/* 
	 * setup the addressing structure 
	 * decide where to start the respective arrays
	 * 
 	 */
	if( genrands( 	addr_a,
			addr_b,
			addr_c,
			&addr_scalar,
			num_req, 
			num_devs, 
			capacity, 
			shiftamt ) !=0 ){
		printf( "FAILED TO GENERATE ADDRESSING SCHEMA\n" );
		return -1;
	}



	/* 
	 * init the library 
	 * 
	 */

	ret = hmcsim_init(	&hmc, 
				num_devs, 
				num_links, 
				num_vaults, 
				queue_depth, 
				num_banks, 
				num_drams, 
				capacity, 
				xbar_depth );
	if( ret != 0 ){ 
		printf( "FAILED TO INIT HMCSIM\n" );	
		return -1;
	}else {
		printf( "SUCCESS : INITIALIZED HMCSIM\n" );
	}

	/* 
	 * setup the device topology
	 * 
	 */
	if( num_devs > 1 ){ 
		/* 
		 * MULTIPLE DEVICES
		 *
		 */
	}else{
		/* 
		 * SINGLE DEVICE
		 *
		 */

		for( i=0; i<num_links; i++ ){

                        ret = hmcsim_link_config( &hmc,
                                                (num_devs+1),
                                                0,
                                                i,
                                                i,
                                                HMC_LINK_HOST_DEV );

                        if( ret != 0 ){
                                printf( "FAILED TO INIT LINK %d\n", i );
                                hmcsim_free( &hmc );
				free( addr_a );
				free( addr_b );
				free( addr_c );
                                return -1;
                        }else{
                                printf( "SUCCESS : INITIALIZED LINK %d\n", i );
                        }
                }

	}	

	/* 
	 * init the max request block size 
 	 * 
	 */
	ret = hmcsim_util_set_all_max_blocksize( &hmc, bsize );

	if( ret != 0 ){ 
		printf( "FAILED TO INIT MAX BLOCKSIZE\n" );
		hmcsim_free( &hmc );
		return -1;
	}else {
		printf( "SUCCESS : INITIALIZED MAX BLOCK SIZE\n" );
	}


	/* 
	 * execute the test
	 * 
	 */
	if( execute_test(	&hmc,
            	            	addr_a,
               		      	addr_b,
               	            	addr_c,
                             	addr_scalar,
                             	num_req,
                             	num_threads, 
				simd ) != 0 ){ 
		printf( "ERROR : FAILED TO EXECUTE THE TEST\n" );
		ret = -1;
		goto cleanup;
	}

cleanup:
	free( addr_a );
	free( addr_b );
	free( addr_c );
	
	
	/* 
	 * free the library data
	 * 
	 */	
	ret = hmcsim_free( &hmc );
	
	if( ret != 0 ){ 
		printf( "FAILED TO FREE HMCSIM\n" );
		return -1;
	}else {
		printf( "SUCCESS : FREE'D HMCSIM\n" );
	}
	
	return ret;
}

/* EOF */
