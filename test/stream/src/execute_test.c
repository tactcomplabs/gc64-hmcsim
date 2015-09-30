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
 * Executes a Stream Triad : 
 * 
 * a[j] = b[j] + scalar * c[j];
 * where, 0 <= j < num_req
 * 
 */
extern int execute_test(        struct hmcsim_t *hmc,
                                uint64_t *addr_a,
                                uint64_t *addr_b,
                                uint64_t *addr_c,
                                uint64_t addr_scalar,
                                long num_req,
                                uint32_t num_threads,  
                                uint32_t simd )
{
	/* vars */
	uint64_t head		= 0x00ll;
	uint64_t tail		= 0x00ll;
	uint64_t payload[8]	= {0x00ll, 0x00ll, 0x00ll, 0x00ll,
				   0x00ll, 0x00ll, 0x00ll, 0x00ll };
	uint64_t packet[HMC_MAX_UQ_PACKET];
	uint8_t cub		= 0;
	uint16_t tag		= 1;
	uint8_t link		= 0;
	int ret			= 0;
	
	FILE *ofile		= NULL;
	uint32_t done		= 0;
	uint32_t i		= 0;
	uint64_t niter		= 0x00ll;
	uint64_t *start		= NULL;		/* starting point of each thread */
	uint64_t *end		= NULL;		/* ending point of each thread */
	uint64_t *cur		= NULL;		/* current index of each thread */
	uint64_t *count		= NULL;		/* completed requests per thread */
	uint64_t *status	= NULL;		/* status signals for each thread */
	uint64_t *scalar	= NULL;		/* status signals for scalars */
	/* ---- */

	cur	= malloc( sizeof( uint64_t ) * num_threads );
	count	= malloc( sizeof( uint64_t ) * num_threads );
	start	= malloc( sizeof( uint64_t ) * num_threads );
	end	= malloc( sizeof( uint64_t ) * num_threads );
	status	= malloc( sizeof( uint64_t ) * num_threads );
	scalar	= malloc( sizeof( uint64_t ) * num_threads );

	/* 
	 * setup the address strides for each thread
	 * 
	 */
	niter	= (uint64_t)(num_req)/(uint64_t)(num_threads);


	for( i=0; i<num_threads; i++ ){ 
		count[i] 	= 0x00ll;
		start[i]	= niter * (uint64_t)(i);	
		cur[i]		= start[i];
		status[i]	= 0x00ll;
		scalar[i]	= 0x00ll;

		if( i == (num_threads-1) ){
			/* last thread */
			end[i]	= (uint64_t)(num_req-1);
		}else{
			end[i]	= start[i]+(niter-1);
		}

#if 0
		printf( "start[%"PRIu32"] = %"PRIu64"\n", i, start[i] );
		printf( "end[%"PRIu32"]   = %"PRIu64"\n", i, end[i] );
#endif
	}
		

	/* 
	 * setup the tracing mechanisms
	 * 
	 */
	ofile = fopen( "stream.out", "w" );
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
			if( cur[i] == end[i] ){ 
				/* this thread is done */
			}else if( scalar[i] == 0 ){
				/* request the scalar */
				
				/* -- build the request */
				hmcsim_build_memrequest( hmc,
                                                        0,
                                                        addr_scalar,
                                                        tag,
                                                        RD64,
                                                        link,
                                                        &(payload[0]),
                                                        &head,
                                                        &tail );
	
				packet[0]	= head;
				packet[1]	= tail;

				ret	= hmcsim_send( hmc, &(packet[0]) );

				/* handle the response */
				switch( ret ){	
					case 0:
						/* success */
						scalar[i]++;
						count[i] = 0x00ll;

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
						printf( "FAILED : PACKET SEND FAILED\n" );
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
				/* push loads for the current thread at b[j] */

				/* reset the status */			
				ret = HMC_OK;

				while(	(count[i] < (uint64_t)(simd) ) && 
					( ret != HMC_STALL) ){
	
					/* push out a load */

					/* build the request */
					hmcsim_build_memrequest( hmc,
                                                 	       	0,
                                                       	 	addr_b[cur[i]+count[i]],
                                            		        tag,
                                                        	RD64,
                                                        	link,
                                                        	&(payload[0]),
                                                        	&head,
                                                        	&tail );
	
					packet[0]	= head;
					packet[1]	= tail;

					/* send it */
					ret	= hmcsim_send( hmc, &(packet[0]) );

					/* handle the response */
					switch( ret ){	
						case 0:
							/* success */
							count[i]++;
					
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
							break;
						case -1:
						default:
							printf( "FAILED : PACKET SEND FAILED\n" );
							goto complete_failure;
							break;
					}

					/* 
					 * zero the packet 
					 *
					 */
					zero_packet( &(packet[0]) );

				}

				ret = HMC_OK;

				if( count[i] == simd ){ 	
					/* b[j] loads are done */
					status[i]++;
					count[i] = 0x00ll;
				}


			}else if( status[i] == 1 ){
				/* push the loads for the current thread at c[j] */

				/* reset the status */			
				ret = HMC_OK;

				while(	(count[i] < (uint64_t)(simd) ) && 
					( ret != HMC_STALL) ){
			
					/* push out a load */

					/* build the request */
					hmcsim_build_memrequest( hmc,
                                                 	       	0,
                                                       	 	addr_c[cur[i]+count[i]],
                                            		        tag,
                                                        	RD64,
                                                        	link,
                                                        	&(payload[0]),
                                                        	&head,
                                                        	&tail );
	
					packet[0]	= head;
					packet[1]	= tail;

					/* send it */
					ret	= hmcsim_send( hmc, &(packet[0]) );

					/* handle the response */
					switch( ret ){	
						case 0:
							/* success */
							count[i]++;
					
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
							break;
						case -1:
						default:
							printf( "FAILED : PACKET SEND FAILED\n" );
							goto complete_failure;
							break;
					}

					/* 
					 * zero the packet 
					 *
					 */
					zero_packet( &(packet[0]) );

				}	

				ret = HMC_OK;

				if( count[i] == simd ){ 	
					/* c[j] loads are done */
					status[i]++;
					count[i] = 0x00ll;
				}

			}else if( status[i] == 2 ){
				/* pause this clock cycle to compute */

				/* reset the status */			
				ret = HMC_OK;
				

				status[i]++;
				count[i] = 0x00ll;	

			}else if( status[i] == 3 ){ 
				/* push stores for the current thread */

				/* reset the status */			
				ret = HMC_OK;
				

				while(	(count[i] < (uint64_t)(simd) ) && 
					( ret != HMC_STALL) ){
			
					/* push out a store */

					/* build the request */
					hmcsim_build_memrequest( hmc,
                                                 	       	0,
                                                       	 	addr_a[cur[i]+count[i]],
                                            		        tag,
                                                        	WR64,
                                                        	link,
                                                        	&(payload[0]),
                                                        	&head,
                                                        	&tail );
	
					packet[0]	= head;
					packet[1]	= 0x02ll;
					packet[2]	= 0x03ll;
					packet[3]	= 0x04ll;
					packet[4]	= 0x05ll;
					packet[5]	= 0x06ll;
					packet[6]	= 0x07ll;
					packet[7]	= 0x08ll;
					packet[8]	= 0x09ll;
					packet[9]	= tail;
					packet[1]	= tail;

					/* send it */
					ret	= hmcsim_send( hmc, &(packet[0]) );

					/* handle the response */
					switch( ret ){	
						case 0:
							/* success */
							count[i]++;
					
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
							break;
						case -1:
						default:
							printf( "FAILED : PACKET SEND FAILED\n" );
							goto complete_failure;
							break;
					}

					/* 
					 * zero the packet 
					 *
					 */
					zero_packet( &(packet[0]) );

				}

				ret = HMC_OK;

				if( count[i] == simd ){ 	
					/* c[j] loads are done */
					status[i] 	= 0x00ll;
					count[i] 	= 0x00ll;

					cur[i] += (uint64_t)(simd);
					

					if( cur[i] >= end[i] ){ 
						cur[i] = end[i];
						printf( "thread %"PRIu32" done; done = %"PRIu32 "\n", i, done );
						done++;
					}
				}
			
			}/* -- status == 3 */
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
	free( start );	
	free( end );	
	free( status );	
	free( scalar );	


	return 0;
}


/* EOF */
