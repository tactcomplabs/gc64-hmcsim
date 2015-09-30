/* 
 * _EXECUTE_TEST_C_ 
 * 
 * HMCSIM PHYSRAND TEST EXECUTION FUNCTIONS
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "hmc_sim.h"


/* ---------------------------------------------------- ZERO_PACKET */
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

/* ---------------------------------------------------- EXECUTE_TEST */
/* 
 * EXECUTE TEST
 * 
 */
extern int execute_test(	struct hmcsim_t *hmc, 
				uint64_t *addr, 
				uint32_t *req, 
				long num_req )
{
	/* vars */
	uint32_t z		= 0x00;
	long iter		= 0x00l;
	uint64_t head		= 0x00ll;
	uint64_t tail		= 0x00ll;
	uint64_t payload[8]	= {0x00ll,0x00ll,0x00ll,0x00ll,
				   0x00ll,0x00ll,0x00ll,0x00ll};
	uint64_t packet[HMC_MAX_UQ_PACKET];
	uint8_t	cub		= 0;
	uint16_t tag		= 0;
	uint8_t link		= 0;
	int ret			= HMC_OK;
	int stall_sig		= 0;
	
	FILE *ofile		= NULL;
	int *rtns		= NULL;
	long all_sent		= 0;
	long all_recv		= 0;
	int done		= 0;
	/* ---- */

	rtns = malloc( sizeof( int ) * hmc->num_links );
	memset( rtns, 0, sizeof( int ) * hmc->num_links );

	/* 
	 * Setup the tracing mechanisms
	 * 
	 */
	ofile = fopen( "physrand.out", "w" );	
	if( ofile == NULL ){ 
		printf( "FAILED : COULD NOT OPEN OUTPUT FILE physrand.out\n" );
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

	printf( "SUCCESS : ZERO'D PACKETS\n" );
	printf( "SUCCESS : BEGINNING TEST EXECUTION\n" );

	/* 
	 * Attempt to execute all the requests
	 * Push requests into the device
	 * until we get a stall signal 
 	 */ 
	while( done != 1 ){


		/* 
	 	 * attempt to push a request in 
		 * as long as we don't stall
		 *
	 	 */		
		if( iter >= num_req ){ 
			/* everything is sent, go to receive side */
			goto packet_recv;
		}

		printf( "....sending packets\n" );
		while( ret != HMC_STALL ){

			/* 
			 * try to push another request 
			 * 
			 * Step 1: Build the Request
			 * 
			 */		
			if( req[(int)(iter)] == 1 ){
				/* 
			 	 * read request
				 *
				 */
				printf( "...building read request for device : %d\n", cub );
				hmcsim_build_memrequest( hmc, 
							cub, 
							addr[iter], 
							tag, 
							RD64, 
							link, 
							&(payload[0]), 
							&head, 
							&tail );
				/* 
				 * read packets have: 
				 * head + 
				 * tail
				 * 
				 */
				packet[0] = head;
				packet[1] = tail;
			}else {
				/* 
				 * write request
				 *
				 */
				printf( "...building write request for device : %d\n", cub );
				hmcsim_build_memrequest( hmc, 
							cub, 
							addr[iter], 
							tag, 
							WR64, 
							link, 
							&(payload[0]), 
							&head, 
							&tail );
				/* 
				 * write packets have: 
				 * head + 
				 * data + 
				 * data + 
				 * data + 
				 * data + 
				 * data + 
				 * data + 
				 * data + 
				 * data + 
				 * tail
				 * 
				 */
				packet[0] = head;
				packet[1] = 0x05ll;
				packet[2] = 0x06ll;
				packet[3] = 0x07ll;
				packet[4] = 0x08l;
				packet[5] = 0x09ll;
				packet[6] = 0x0All;
				packet[7] = 0x0Bll;
				packet[8] = 0x0Cll;
				packet[9] = tail;
			}
			
			/* 
			 * Step 2: Send it 
			 *
			 */
			printf( "...sending packet : base addr=0x%016llx\n", addr[iter] );

#if 0
			for( i=0; i<HMC_MAX_UQ_PACKET; i++){ 
				printf( "packet[%" PRIu64 "] = 0x%016llx\n", i, packet[i] );
			}
#endif
	
			ret = hmcsim_send( hmc, &(packet[0]) );

			switch( ret ){ 
				case 0: 
					printf( "SUCCESS : PACKET WAS SUCCESSFULLY SENT\n" );
					all_sent++;
					iter++;
					break;
				case HMC_STALL:
					printf( "STALLED : PACKET WAS STALLED IN SENDING\n" );
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

			tag++;
			if( tag == (UINT8_MAX-1) ){
				tag = 0;
			}	

			link++;
			if( link == hmc->num_links ){
				/* -- TODO : look at the number of connected links
				 * to the host processor
				 */
				link = 0;
			}

			/* 
			 * check to see if we're at the end of the packet queue
			 *
			 */
			if( iter >= num_req ){ 
				goto packet_recv;
			}

			/* DONE SENDING REQUESTS */
		}

packet_recv:
		/* 
		 * reset the return code for receives
		 * 
		 */
		ret = HMC_OK;

		/* 
		 * We hit a stall or an error
		 * 
		 * Try to drain the responses off all the links
		 * 
		 */
		printf( "...reading responses\n" );
		while( ret != HMC_STALL ){ 

			for( z=0; z<hmc->num_links; z++){ 
				
				rtns[z] = hmcsim_recv( hmc, cub, z, &(packet[0]) );

				if( rtns[z] == HMC_STALL ){ 
					stall_sig++;
				}else{ 
					/* successfully received a packet */
					printf( "SUCCESS : RECEIVED A SUCCESSFUL PACKET RESPONSE\n" );	
					all_recv++;
				}

				/* 
				 * zero the packet 
				 * 
				 */
				zero_packet( &(packet[0]) );
			}

			/* count the number of stall signals received */
			if( stall_sig == hmc->num_links ){ 
				/* 
				 * if all links returned stalls, 
				 * then we're done receiving packets
				 *
				 */
				
				printf( "STALLED : STALLED IN RECEIVING\n" );
				ret = HMC_STALL;
			}

			stall_sig = 0;
			for( z=0; z<hmc->num_links; z++){
				rtns[z] = HMC_OK;
			}
		}

		/* 
		 * reset the return code
		 * 
		 */
		stall_sig = 0;
		for( z=0; z<hmc->num_links; z++ ){ 
			rtns[z] = HMC_OK;
		}
		ret = HMC_OK;
	

		/* 
	 	 * done with sending/receiving packets
		 * update the clock 
		 */
		printf( "SIGNALING HMCSIM TO CLOCK\n" );
		hmcsim_clock( hmc );

		printf( "ALL_SENT = %ld\n", all_sent );
		printf( "ALL_RECV = %ld\n", all_recv );
		fflush( stdout );


		if( all_sent == num_req ){ 
			if( all_recv == num_req ){ 
				done = 1;
			}
		}
	}


complete_failure:

	fclose( ofile );
	ofile = NULL;

	free( rtns ); 
	rtns = NULL;

	return 0;
}

/* EOF */
