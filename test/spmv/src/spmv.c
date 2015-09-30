/* 
 * _SPMV_C_
 * 
 * HMCSIM SPMV TEST DRIVER 
 * 
 * Attempts to initialize a HMC instantiation
 * and execute an spmv test
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "spmv.h"
#include "hmc_sim.h"


/* ----------------------------------------------------- FUNCTION PROTOTYPES */
extern int getshiftamount( 	uint32_t num_links, 
				uint32_t capacity, 
				uint32_t bsize, 
				uint32_t *shiftamt );
extern int execute_test(        struct hmcsim_t *hmc,
				struct csr_t *mat,
                                uint32_t num_threads, 
				uint32_t simd, 
				int nzero, 
				int nrows, 
				int ncols  );
extern int genmat(		int nzero, 
				int nrows, 
				int ncols, 
				struct csr_t *mat );

/* ----------------------------------------------------- FREE_MEMORY */
static int free_memory( struct csr_t *mat )
{
	if( mat->vals != NULL ){ 
		free( mat->vals );
	}

	if( mat->cols != NULL ){ 
		free( mat->cols );
	}

	if( mat->rows != NULL ){ 
		free( mat->rows );
	}

	if( mat->x_vect != NULL ){ 
		free( mat->x_vect );
	}

	if( mat->product != NULL ){ 
		free( mat->product );
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
	int nzero		= 64;	/* number of nonzero elements */
	int nrows		= 1024;	/* number of rows */
	int ncols		= 1024;	/* number of columns */
	char matfile[128];
	struct csr_t mat;	
	struct hmcsim_t hmc;
	/* ---- */

	while(( ret = getopt( argc, argv, "b:c:d:hl:m:n:q:s:v:x:F:N:T:X:Y:Z:" )) != -1 )
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
				printf( " -F <matfile>\n" );
				printf( " -T <num_threads>\n" );
				printf( " -X <nrows>\n" );
				printf( " -Y <ncols>\n" );
				printf( " -Z <nzero>\n" );
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
			case 'F':
				sprintf( matfile, "%s", optarg );
				break;
			case 'T':
				num_threads = (uint32_t)(atoi(optarg));
				break;
			case 'X': 
				nrows	= (int)(atoi(optarg));
				break;
			case 'Y':
				ncols	= (int)(atoi(optarg));
				break;
			case 'Z':
				nzero	= (int)(atoi(optarg));
				break;	
			case '?':
			default:
				printf( "%s%s%s\n", "Unknown option: see ", argv[0], " -bcdhlmnqsvxFTXYZ" );
				return -1;
				break;
		}
	}

	/* 
	 * generate the matrix file 
	 * 
	 */
	if( genmat( nzero, nrows, ncols, &mat ) != 0 ){ 
		printf( "FAILED TO GENERATE THE INPUT MATRIX\n" );
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
		free_memory( &mat );
		return -1;
	}


#if 0
	/* redo this call */
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
#endif



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
				free_memory( &mat );
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
				&mat,
                             	num_threads, 
				simd, 
				nzero, 
				nrows, 
				ncols  ) != 0 ){ 
		printf( "ERROR : FAILED TO EXECUTE THE TEST\n" );
		ret = -1;
		goto cleanup;
	}

cleanup:
	free_memory( &mat );
	
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
