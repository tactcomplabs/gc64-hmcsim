/* 
 * _EXECUTE_TEST_C_ 
 * 
 * HMC-SIM SPMV TEST EXECUTION FUNCTIONS
 * FOR THE FINITE STATE MACHINE 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "spmv.h"
#include "hmc_sim.h"


/* ------------------------------------------ ZERO_PACKET */
/* 
 * ZERO_PACKET 
 * 
 */
static void zero_packet( uint64_t *packet )
{
	uint64_t i	= 0x00ll;

	/* 
	 * zero the packet 
	 * 
 	 */
	for( i=0; i<HMC_MAX_UQ_PACKET; i++ ){ 
		packet[i] = 0x00ll;
	}

	return ;
}

/* ------------------------------------------ EXECUTE_TEST */
/* 
 * executes a sparse matrix vector multiply 
 * using a compressed sparse row format 
 *
 * #pragma omp parallel shared( mat, product ) 
 * {
 * for( i=0; i<nrows; i++ ){ 
 * 
 * #pragma omp for nowait
 *    for( j=rows[i]; j<rows[i+1]; j++ ){ 
 *       tmp = cols[j];
 *       products[i] += vals[j] * x_vect[tmp];
 *    }
 * 
 *    __amo_add_u8( &product, products[i] );
 * }
 * }
 */
extern int execute_test( struct hmcsim_t *hmc, 
			struct csr_t *mat, 
			uint32_t num_threads, 
			uint32_t simd, 
			int nzero, 
			int nrows, 
			int ncols  )
{
	/* vars */
        uint64_t packet[HMC_MAX_UQ_PACKET];
	uint64_t head		= 0x00ll;
	uint64_t tail		= 0x00ll;
	uint64_t payload[8]     = {0x00ll, 0x00ll, 0x00ll, 0x00ll,
                                   0x00ll, 0x00ll, 0x00ll, 0x00ll };
        uint8_t cub             = 0;
        uint16_t tag            = 1;
        uint8_t link            = 0;
        int ret                 = 0;

	FILE *ofile             = NULL;
        uint32_t done           = 0;
	uint64_t outer		= 0x00ll;
        uint32_t i              = 0;
        uint64_t niter          = 0x00ll;
        uint64_t *start         = NULL;         /* starting point of each thread */
        uint64_t *end           = NULL;         /* ending point of each thread */
        uint64_t *cur           = NULL;         /* current index of each thread */
        uint64_t *count         = NULL;         /* completed requests per thread */
        uint64_t *status        = NULL;         /* status signals for each thread */
        uint64_t *scalar        = NULL;         /* status signals for scalars */		
	/* ---- */

	/* allocate the thread-local memory constructs */
	cur     = malloc( sizeof( uint64_t ) * num_threads );
        count   = malloc( sizeof( uint64_t ) * num_threads );
        start   = malloc( sizeof( uint64_t ) * num_threads );
        end     = malloc( sizeof( uint64_t ) * num_threads );
        status  = malloc( sizeof( uint64_t ) * num_threads );
        scalar  = malloc( sizeof( uint64_t ) * num_threads );

	/* init all the data */

	/* setup the hmc-sim tracing mechansims */
	ofile = fopen( "spmv.out", "w" );
        if( ofile == NULL ){
                printf( "FAILED : COULD NOT OPEN OUPUT FILE stream.out\n" );
                return -1;
        }

        hmcsim_trace_handle( hmc, ofile );
        hmcsim_trace_level( hmc, (HMC_TRACE_BANK|
                                HMC_TRACE_QUEUE|
                                HMC_TRACE_CMD|
                                HMC_TRACE_STALL|
                                HMC_TRACE_LATENCY) );

        printf( "SUCCESS : INITIALIZED TRACE HANDLERS\n" );

	/*
         * zero the packet
         *
         */
        zero_packet( &(packet[0]) );

        printf( "BEGINNING EXECUTION\n" );

	/* 
	 * we implicitly loop around the number of rows
	 * all threads execute this portion of the algorithm 
	 * in parallel, synchronously 
 	 * 
	 */
	for( outer=0; outer<nrows; outer++ ){ 

		/* 
	 	 * the inner loop is explicitly parallelized
	 	 * across the number of available threads.
		 * this is the core of our computational loop
	 	 *
		 */

		/* setup the necessary control */

		while( done < num_threads ){ 

			for( i=0; i<num_threads; i++ ){ 
	
				

			} /* end for loop: i=0; i<num_threads; i++ */

			/* 
			 * drain all the responses off all the links
			 *
			 */
			for( i=0; i<hmc->num_links; i++ ){ 
				ret = HMC_OK;
				while( ret != HMC_STALL ){ 
					ret = hmcsim_recv( hmc, cub, i, &(packet[0]) );
				}
			}

			/*
			 * clock the sim 
			 *
			 */
			hmcsim_clock( hmc );

		}/* end while loop: done < num_threads */

	} /* end for loop: outer=0; outer<nrows; outer++ */

	printf( "SUCCESS : EXECUTION COMPLETE\n" );
		
complete_failure:
	fclose( ofile );
	ofile = NULL;

	free( cur );
	free( count );
	free( start );
	free( end );
	free( status );
	free( scalar );	

	return 0;
}

/* EOF */
