/* 
 * _EXECUTE_TEST_C_
 * 
 * HMCSIM STREAM_TRIAD EXECUTION FUNCTIONS 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "hmc_sim.h"


/* ------------------------------------------------- FUNCTION PROTOTYPES */
extern int getshiftamount(      uint32_t num_links,
                                uint32_t capacity,
                                uint32_t bsize,
                                uint32_t *shiftamt );


/* ------------------------------------------------- TAG MACROS */
#define POLY    	0x0000000000000007UL
#define TAG_INV		0x0000	/* -- tag is invalid */
#define	TAG_RD64	0x0001	/* -- tag is a rd64 request */
#define TAG_WR64	0x0002	/* -- tag is a wr64 request */
#define TAG_RSP		0x0004	/* -- received a response for this tag */

/* ------------------------------------------------- TAG STRUCTURE */
struct memtag_t{
	uint64_t 	addr;	/* addresss of the tag 	*/	
	uint32_t	stat;	/* status flag */
	uint8_t 	tag;	/* the tag itself	*/
};


/* ------------------------------------------------- CONVERT_ADDR */
/* 
 * CONVERT_ADDR
 * 
 */
static int convert_addr(	uint64_t *addr_Table, 
				uint64_t *addr_ran, 
				uint64_t TableSize, 
				int N, 
				uint32_t shiftamt )
{
	/* vars */
	uint64_t base_T	= 0x00ll;
	uint64_t base_r	= 0x00ll;
	uint64_t nrpo	= 0x00ll;
	uint64_t offset	= 0x00ll;
	uint64_t i	= 0;
	/* ---- */

	/* 
	 * calculate the base of each vector
	 * 
	 */
	nrpo	= (uint64_t)(TableSize+1);
	offset	= nrpo * 0x08ll;	/* in bytes */
	
	base_T	= (0x00ll) + ((0x08ll) << (uint64_t)(shiftamt));	
	base_r	= base_T   + ((offset) << (uint64_t)(shiftamt)); 

	/* addr_Table */
	for( i=0; i<TableSize; i++ ){ 

		addr_Table[i]	= (uint64_t)( base_T + (((uint64_t)(i) * 0x08ll)<<(uint64_t)(shiftamt)) );
	}

	/* addr_ran */
	for( i=0; i<N; i++ ){ 

		addr_ran[i]	= (uint64_t)( base_r + (((uint64_t)(i) * 0x08ll)<<(uint64_t)(shiftamt)) );
	}

	return 0;
}
 
/* ------------------------------------------------- ZERO_PACKET */
/* 
 * ZERO_PACKET 
 * 
 */
static void zero_packet( uint64_t *packet )
{
	uint64_t i = 0x00ll;

	/* 
 	 * zero the packet 
 	 * 
 	 */
	for( i=0; i<HMC_MAX_UQ_PACKET; i++ ){ 
		packet[i] = 0x00ll;
	}

	return ;
}

/* ------------------------------------------------- EXECUTE_TEST */
/* 
 * EXECUTE_TEST
 * 
 * Executes a series of parallel GUPS operations
 * 
 * for( i=0; i<NUPDATE; i++ ){ 
 * 	ran[j] = (ran[j]<<1) ^ (ran[j]) < 0 ? POLY : 0)
 * 	Table[ ran[j] & (TableSize-1) ] ^= ran[j]
 * }
 * 
 * Finite State Machine : 
 *
 * local_thread_counter = num_update / num_threads  
 * 
 * - load TableSize scalar [really TableSize-1]
 * - load ran[thread]   
 * -- all loads must return 
 * - shift clock 
 * - compare clock 
 * - logical xor clock 
 * - write ran[j]
 * - "logical and ran[j] & (Tablesize-1)" clock 
 * - load of Table [index]   
 * -- must wait for load to return 
 * - xor clock { Table[ index ] ^ ran[j] }
 * - write Table[ index ] 
 * - goto ==> shift clock [ran[j] is already in register state]
 * 
 */
extern int execute_test(        struct hmcsim_t *hmc,
                                uint64_t *Table,
				uint64_t *ran,
				uint64_t TableSize,
                                long num_req,
                                uint32_t num_threads,  
                                uint32_t simd, 
				uint32_t shiftamt )
{
	/* vars */
	uint64_t head		= 0x00ll;
	uint64_t tail		= 0x00ll;
	uint64_t payload[8]	= {0x00ll, 0x00ll, 0x00ll, 0x00ll,
				   0x00ll, 0x00ll, 0x00ll, 0x00ll };
	uint64_t packet[HMC_MAX_UQ_PACKET];
	uint64_t addr_TableSize	= 0x00ll;
	uint8_t cub		= 0;
	uint16_t tag		= 1;
	uint8_t link		= 0;
	int ret			= 0;
	
	FILE *ofile		= NULL;
	uint32_t done		= 0;
	uint32_t i		= 0;
	uint64_t niter		= 0x00ll;
	uint64_t *cur		= NULL;		/* current index of each thread */
	uint64_t *count		= NULL;		/* completed requests per thread */
	uint64_t *status	= NULL;		/* status signals for each thread */
	uint64_t *scalar	= NULL;		/* status signals for scalars */

	uint64_t *addr_Table	= NULL;		/* addresses of the Table array */
	uint64_t *addr_ran	= NULL;		/* addresses of the ran array */
	/* ---- */

	cur	= malloc( sizeof( uint64_t ) * num_threads );
	count	= malloc( sizeof( uint64_t ) * num_threads );
	status	= malloc( sizeof( uint64_t ) * num_threads );
	scalar	= malloc( sizeof( uint64_t ) * num_threads );
	addr_Table	= malloc( sizeof( uint64_t ) * TableSize );
	addr_ran	= malloc( sizeof( uint64_t ) * (num_threads * simd) );

	/* 
	 * convert all the addresses to physical (HMC) 
	 * 
	 */
	printf( "...CONVERTING ADDRESSES TO PHYSICAL\n" );
	if( convert_addr( addr_Table, addr_ran, TableSize, num_threads*simd, shiftamt ) != 0 ){ 
		/* error occurred */
		printf( "ERROR : could not perform virtual to physical translation\n" );
		goto complete_failure;
	}

	/* 
 	 * figure out the number of updates per thread
	 * 
	 */
	niter	= (uint64_t)(num_req)/(uint64_t)(num_threads);

	/*
	 * setup all the thread-local data 
	 *
	 */
	printf( "...INITIALIZING THREAD-LOCAL DATA\n" );
	for( i=0; i<num_threads; i++ ){ 
		cur[i]		= i;
		count[i]	= niter;
		status[i]	= 0;
		scalar[i]	= 0;		
	}

	count[num_threads-1] = num_req-count[num_threads-2];

	/* 
	 * setup the tracing mechanisms
	 * 
	 */
	printf( "...INITIALIZING TRACE FILE\n" );
	ofile = fopen( "gups.out", "w" );
	if( ofile == NULL ){ 
		printf( "FAILED : COULD NOT OPEN OUPUT FILE gups.out\n" );
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

	/* -- begin cycle loop */
	while( done < num_threads ){

		/* 
	 	 * each thread needs to push out requests for its given	
		 * simd width worth of loads
		 * 
		 */
		for( i=0; i<num_threads; i++ ){ 

			/* 
			 * for each thread, attempt to push "simd" 
			 * width of requests out on each clock 
			 * 
			 */

			if( count[i] == 0 ){ 
				/* this thread is done */
			}else if( scalar[i] == 0 ){ 
				/* load TableSize scalar [just 0x00ll] */
				hmcsim_build_memrequest( hmc,
                                                        0,
                                                        addr_TableSize,
                                                        tag,
                                                        RD64,
                                                        link,
                                                        &(payload[0]),
                                                        &head,
                                                        &tail );

				packet[0]	= head;
				packet[1]	= tail;
				
				ret	 = hmcsim_send( hmc, &(packet[0]) );
				
				/* handle the response */
				switch( ret ){	
					case 0:
						/* success */
						scalar[i] 	= 1;
						status[i] 	= 0;

						tag++;
						if( tag == (UINT8_MAX-1)) {
							tag = 1;
						}

						link++;
						if( link == hmc->num_links ){ 
							link = 0;
						}

						break;
					case HMC_STALL:
						/* stalled */	
						scalar[i] = 0x00ll;
						break;
					case -1:
					default:
						printf( "FAILED : SCALAR PACKET SEND FAILED\n" );
						goto complete_failure;
						break;
				}

				/* 
				 * zero the packet 
				 *
				 */
				zero_packet( &(packet[0]) );

				ret = HMC_OK;
				

			}else if( status[i] == 0 ){ 
				/* load ran[j] */
				hmcsim_build_memrequest( hmc,
                                                        0,
                                                        addr_ran[i],
                                                        tag,
                                                        RD64,
                                                        link,
                                                        &(payload[0]),
                                                        &head,
                                                        &tail );
				
				packet[0]	= head;
				packet[1]	= tail;
				
				ret	 = hmcsim_send( hmc, &(packet[0]) );
				
				/* handle the response */
				switch( ret ){	
					case 0:
						/* success */
						status[i]++;

						tag++;
						if( tag == (UINT8_MAX-1)) {
							tag = 1;
						}

						link++;
						if( link == hmc->num_links ){ 
							link = 0;
						}

						break;
					case HMC_STALL:
						/* stalled */	
						status[i] = 0x00ll;
						break;
					case -1:
					default:
						printf( "FAILED : LOAD RAN[i] PACKET SEND FAILED\n" );
						goto complete_failure;
						break;
				}

				/* 
				 * zero the packet 
				 *
				 */
				zero_packet( &(packet[0]) );

				ret = HMC_OK;
				
				/* all loads must return */

			}else if( status[i] == 1 ){ 
				/* shift clock */

				/* reset the status */
				ret = HMC_OK;

				status[i]++;

			}else if( status[i] == 2 ){ 
				/* compare clock */

				/* reset the status */
				ret = HMC_OK;

				status[i]++;

			}else if( status[i] == 3 ){
				/* logical xor clock */

				/* reset the status */
				ret = HMC_OK;

				status[i]++;

			}else if( status[i] == 4 ){ 
				/* write ran[j] */

				/* build the request */
                                hmcsim_build_memrequest( hmc,
                                                         0,
                                                         addr_ran[i],
                                                         tag,
                                                         WR64,
                                                         link,
                                                         &(payload[0]),
                                                         &head,
                                                         &tail );

				packet[0]	= head;
				packet[1]	= (ran[i]<<1) ^ (ran[i] < 0 ? POLY : 0);
				packet[2]	= 0x00ll;
				packet[3]	= 0x00ll;
				packet[4]	= 0x00ll;
				packet[5]	= 0x00ll;
				packet[6]	= 0x00ll;
				packet[7]	= 0x00ll;
				packet[8]	= 0x00ll;
				packet[9]	= tail;

				ret	= hmcsim_send( hmc, &(packet[0]) );

				/* handle the response */
				switch( ret ){ 
					case 0: 
						/* success */
						status[i]++;

						tag++;
						if( tag == (UINT8_MAX-1)) {
							tag = 1;
						}

						link++;
						if( link == hmc->num_links ){ 
							link = 0;
						}
						
						ran[i] = (ran[i]<<1) ^ (ran[i] < 0 ? POLY : 0);

						break;
					case HMC_STALL:
						/* request stalled, do nothing */
						status[i] = 0x04ll;
						break;
					case -1:
					default:
						printf( "FAILED : PACKED W64 REQUEST FAILED\n" );
						goto complete_failure;
						break;
				}

				/* 
				 * zero the packet 
				 *
				 */
				zero_packet( &(packet[0]) );

				ret = HMC_OK;

			}else if( status[i] == 5 ){ 
 				/* logical and ran[j] & (Tablesize-1) clock */

				/* reset the status */
				ret = HMC_OK;

				status[i]++;

			}else if( status[i] == 6 ){
				/* load of Table[index] */
				/*Table[ ran[j] & (TableSize-1) ]*/
				hmcsim_build_memrequest( hmc,
                                                        0,
                                                        addr_Table[ ran[i] & (TableSize-1) ],
                                                        tag,
                                                        RD64,
                                                        link,
                                                        &(payload[0]),
                                                        &head,
                                                        &tail );
				
				packet[0]	= head;
				packet[1]	= tail;
				
				ret	 = hmcsim_send( hmc, &(packet[0]) );
				
				/* handle the response */
				switch( ret ){	
					case 0:
						/* success */
						status[i]++;

						tag++;
						if( tag == (UINT8_MAX-1)) {
							tag = 1;
						}

						link++;
						if( link == hmc->num_links ){ 
							link = 0;
						}

						break;
					case HMC_STALL:
						/* stalled */	
						status[i] = 0x06ll;
						break;
					case -1:
					default:
						printf( "FAILED : LOAD Table[index] PACKET SEND FAILED\n" );
						goto complete_failure;
						break;
				}

				/* 
				 * zero the packet 
				 *
				 */
				zero_packet( &(packet[0]) );

				ret = HMC_OK;
				
				
				/* must wait for load to return */

			}else if( status[i] == 7 ){
 				/* xor clock { Table[ index ] ^ ran[j] } */
				/* reset the status */
				ret = HMC_OK;

				status[i]++;

			}else if( status[i] == 8 ){
				/* write Table[ index ] */

				/* build the request */
                                hmcsim_build_memrequest( hmc,
                                                         0,
                                                         addr_Table[ ran[i] & (TableSize-1) ],
                                                         tag,
                                                         WR64,
                                                         link,
                                                         &(payload[0]),
                                                         &head,
                                                         &tail );

				packet[0]	= head;
				packet[1]	= (ran[i]<<1) ^ (ran[i] < 0 ? POLY : 0);
				packet[2]	= 0x00ll;
				packet[3]	= 0x00ll;
				packet[4]	= 0x00ll;
				packet[5]	= 0x00ll;
				packet[6]	= 0x00ll;
				packet[7]	= 0x00ll;
				packet[8]	= 0x00ll;
				packet[9]	= tail;

				ret	= hmcsim_send( hmc, &(packet[0]) );

				/* handle the response */
				switch( ret ){ 
					case 0: 
						/* success */
						status[i]++;

						tag++;
						if( tag == (UINT8_MAX-1)) {
							tag = 1;
						}

						link++;
						if( link == hmc->num_links ){ 
							link = 0;
						}

						Table[ ran[i] & (TableSize-1) ]	^= ran[i];
	
						break;
					case HMC_STALL:
						/* request stalled, do nothing */
						status[i] = 0x08ll;
						break;
					case -1:
					default:
						printf( "FAILED : PACKED W64 REQUEST FAILED\n" );
						goto complete_failure;
						break;
				}

				/* 
				 * zero the packet 
				 *
				 */
				zero_packet( &(packet[0]) );

				ret = HMC_OK;

			}else if( status[i] == 9 ){
				/* goto shift clock; status == 1 */
				/* reset the status */
				ret = HMC_OK;

				status[i] = 0;

				/* decrement our iter count */
				count[i]--;

				cur[i] += niter;
				
				if( count[i] <= 0 ){ 
					/* this thread is done */
					done++;
					printf( "THREAD %d COMPLETED\n", i );
				}

			}
			/* !! END OF STATE MACHINE !! */

		}/* -- end threads loop */

		/*
		 * drain all the responses 
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


	} /* -- end cycle loop */

	printf( "SUCCESS : EXECUTION COMPLETE\n" );

complete_failure:
	fclose( ofile );
	ofile = NULL;

	free( cur ); 
	free( count );	
	free( status );	
	free( scalar );	
	free( addr_Table );
	free( addr_ran );

	return 0;
}


/* EOF */
