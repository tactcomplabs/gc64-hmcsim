/* 
 * _GUPS_C_ 
 * 
 * HMCSIM GUPS TEST DRIVER 
 * 
 * Attempts to initialize a HMC instantiation
 * and execute a gups test
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "hmc_sim.h"

//#define TWO_GB	2147483648
//#define TWO_GB	250000000
#define TWO_GB  268435456
#define POLY	0x0000000000000007UL
#define PERIOD	1317624576693539401L

/* ----------------------------------------------------- FUNCTION PROTOTYPES */
extern int getshiftamount( 	uint32_t num_links, 
				uint32_t capacity, 
				uint32_t bsize, 
				uint32_t *shiftamt );
extern int execute_test(        struct hmcsim_t *hmc,
                                uint64_t *Table,
                                uint64_t *ran,
				uint64_t TableSize,
                                long num_req,
                                uint32_t num_threads, 
				uint32_t simd, 
				uint32_t shiftamt );

/* ----------------------------------------------------- HPCC_starts */
uint64_t HPCC_starts( uint64_t n )
{
	int i, j;
	uint64_t m2[64];
	uint64_t temp, ran;

	while (n < 0) { 
		n += PERIOD;
	}
	
	while (n > PERIOD) {
		n -= PERIOD;
	}

	if( n == 0 ){ return 0x1; }

	temp = 0x1;
	for( i=0; i<64; i++ ){ 
		m2[i] 	= temp;
		temp	= (temp << 1) ^ ((uint64_t)temp < 0 ? POLY : 0 );
		temp	= (temp << 1) ^ ((uint64_t)temp < 0 ? POLY : 0 );
	}

	for( i=62; i>=0; i-- ){ 
		if ((n >> i) & 1 )
			break;
	}

	ran = 0x2;
	while( i > 0 ){ 
		temp = 0;
		for( j=0; j<64; j++ ){ 
			if ((ran>>j) & 1 ){
				temp ^= m2[j];
			}
		}
		ran = temp;

		 i-=1;
		if(( n >> i) & 1){
			ran = (ran << 1) ^ ((uint64_t) ran < 0 ? POLY : 0 );
		}
	}

	return ran;
}

/* ----------------------------------------------------- GENRANDS */
static int genrands( 	uint64_t *Table,
			uint64_t *ran,
			long num_req, 
			uint32_t num_devs, 
			uint32_t capacity, 
			uint32_t shiftamt, 
			uint32_t num_threads, 
			uint32_t simd  )	
{
	/* vars */
	uint64_t i 	= 0x00ll;
	/* ---- */

	if( Table == NULL ){ 
		return -1;
	}else if( ran == NULL ){ 
		return -1;
	}

	/* 
	 * get all the addresses for the first
 	 * run of the Table
 	 * 
	 */
	for( i=0; i<TWO_GB; i++ ){ 
		Table[i] = ((uint64_t)(i) << (uint64_t)(shiftamt)); 
	}

	/* 
	 * init the rans; mimic HPCC_starts
	 * 
	 */
	for( i=0; i<(num_threads*simd); i++ ){ 
		ran[i] = HPCC_starts( i );
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

	uint64_t *Table		= NULL;
	uint64_t *ran		= NULL;
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
	Table	= malloc( sizeof( uint64_t ) * TWO_GB );
	if( Table == NULL ){ 
		printf( "FAILED TO ALLOCATE MEMORY FOR TABLE\n" );
		return -1;
	} 

	ran	= malloc( sizeof( uint64_t ) * simd * num_threads );
	if( ran == NULL ){ 
		printf( "FAILED TO ALLOCATE MEMORY FOR RAN\n" );
		free( Table );
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
		free( Table );
		free( ran );
	}


	/* 
	 * setup the addressing structure 
	 * decide where to start the respective arrays
	 * 
 	 */
	if( genrands( 	Table,
			ran,
			num_req, 
			num_devs, 
			capacity, 
			shiftamt,
			num_threads,
			simd ) !=0 ){
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
				free( Table );
				free( ran );
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
				Table,
				ran,
				(uint64_t)(TWO_GB),
                             	num_req,
                             	num_threads, 
				simd, 
				shiftamt ) != 0 ){ 
		printf( "ERROR : FAILED TO EXECUTE THE TEST\n" );
		ret = -1;
		goto cleanup;
	}

cleanup:
	free( Table );
	free( ran );
	
	
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
