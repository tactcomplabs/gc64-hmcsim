/* 
 * _HMC_PROCESS_PACKET_C_
 * 
 * HYBRID MEMORY CUBE SIMULATION LIBRARY 
 * 
 * HMC PACKET PROCESSORS FOR MEM OPS
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hmc_sim.h"

/* ----------------------------------------------------- FUNCTION PROTOTYPES */
extern int	hmcsim_trace( struct hmcsim_t *hmc, char *str );
extern int	hmcsim_trace_rqst( 	struct hmcsim_t *hmc, 
					char *rqst, 
					uint32_t dev, 
					uint32_t quad, 
					uint32_t vault, 
					uint32_t bank, 
					uint64_t addr1, 
					uint32_t size );
extern int	hmcsim_trace_stall(	struct hmcsim_t *hmc, 
					uint32_t dev, 
					uint32_t quad, 
					uint32_t vault, 
					uint32_t src, 
					uint32_t dest, 
					uint32_t link, 
					uint32_t slot, 
					uint32_t type );
extern int	hmcsim_util_zero_packet( struct hmc_queue_t *queue );
extern int	hmcsim_util_decode_bank( struct hmcsim_t *hmc, 
					uint32_t dev, 
					uint32_t bsize, 
					uint64_t addr,
					uint32_t *bank );
extern int	hmcsim_decode_rsp_cmd( 	hmc_response_t rsp_cmd, 
					uint8_t *cmd );


/* ----------------------------------------------------- HMCSIM_PROCESS_RQST */
/* 
 * HMCSIM_PROCESS_RQST
 * 
 */
extern int	hmcsim_process_rqst( 	struct hmcsim_t *hmc, 
					uint32_t dev, 
					uint32_t quad, 
					uint32_t vault, 
					uint32_t slot )
{
	/* vars */
	struct hmc_queue_t *queue	= NULL;
	uint64_t head			= 0x00ll;
	uint64_t tail			= 0x00ll;

	uint64_t rsp_head		= 0x00ll;
	uint64_t rsp_tail		= 0x00ll;
	uint64_t rsp_slid		= 0x00ll;
	uint64_t rsp_tag		= 0x00ll;
	uint64_t rsp_crc		= 0x00ll;
	uint64_t rsp_rtc		= 0x00ll;
	uint64_t rsp_seq		= 0x00ll;
	uint64_t rsp_frp		= 0x00ll;
	uint64_t rsp_rrp		= 0x00ll;
	uint32_t rsp_len		= 0x00;
	uint64_t packet[HMC_MAX_UQ_PACKET];
	
	uint32_t cur			= 0x00;
	uint32_t error			= 0x00;
	uint32_t t_slot			= hmc->queue_depth+1;
	uint32_t j			= 0x00;
	uint32_t length			= 0x00;
	uint32_t cmd			= 0x00;
	uint32_t tag			= 0x00;
	uint32_t bsize			= 0x00;
	uint32_t bank			= 0x00;
	uint64_t addr			= 0x00ll; 
	int no_response			= 0x00;
	hmc_response_t rsp_cmd		= RSP_ERROR;
	uint8_t tmp8			= 0x0;
	/* ---- */


	/* 	
	 * -- Description of error types --
	 * Given that the various requests can return 
	 * varying results and errors, we define a 
 	 * generic error type above that is handled
	 * when building the response packets. 
	 * In this manner, we can signal a varying
	 * number of errors in the packet handlers
	 * without disrupting everything too much. 
	 * The error codes are described as follows: 
	 * 
	 * error = 0 : no error has occurred [default]
	 * error = 1 : packet request exceeds maximum 
	 *             block size [bsize] 
	 * 
 	 */


	if( hmc == NULL ){ 
		return -1;
	}

	/* 
	 * Step 1: get the request
	 * 
	 */
	queue	= &(hmc->devs[dev].quads[quad].vaults[vault].rqst_queue[slot]);
	head	= queue->packet[0];	

	/* -- get the packet length [10:7] */
	length 	= (uint32_t)( (head >> 7) & 0x0F );

	/* -- cmd = [5:0] */
	cmd	= (uint32_t)(head & 0x3F);

	if( cmd == 0x00 ){ 
		/* command is flow control, dump out */	
		no_response = 1;
		goto step4_vr;	
	}

	/* -- decide where the tail is */	
	tail	= queue->packet[ ((length*2)-1) ];

	/* 
	 * Step 2: decode it 
	 * 
 	 */
	/* -- cmd = [5:0] */
	cmd	= (uint32_t)(head & 0x3F);

	/* -- tag = [23:15] */
	tag	= (uint32_t)((head >> 15) & 0x1FF);

	/* -- addr = [57:24] */
	addr	= ((head >> 24) & 0x3FFFFFFFF );

	/* -- block size */
	hmcsim_util_get_max_blocksize( hmc, dev, &bsize );

	/* -- get the bank */
	hmcsim_util_decode_bank( hmc, dev, bsize, addr, &bank );

	/* 
 	 * Step 3: find a response slot
	 *         if no slots available, then this operation must stall
	 * 
 	 */

	/* -- find a response slot */
	cur = hmc->queue_depth-1;

	for( j=0; j<hmc->queue_depth; j++){

		if( hmc->devs[dev].quads[quad].vaults[vault].rsp_queue[cur].valid == HMC_RQST_INVALID ){
			t_slot = cur;
		}

		cur--;
	}

	if( t_slot == hmc->queue_depth+1 ){

		/* STALL */

		queue->valid = HMC_RQST_STALLED;

		/* 
		 * print a stall trace
		 * 
		 */
		if( (hmc->tracelevel & HMC_TRACE_STALL) > 0 ){ 
			hmcsim_trace_stall(	hmc, 
						dev, 
						quad, 
						vault, 
						0, 
						0,
						0,
						slot, 
						1 );
		}

		return HMC_STALL;
	}	

	/* 
	 * Step 3: perform the op 
	 * 
 	 */
	switch( cmd )
	{
		case 0x08:
			/* WR16 */

			if( (hmc->tracelevel & HMC_TRACE_CMD) > 0 ){ 
				hmcsim_trace_rqst(	hmc, 
							"WR16", 
							dev, 
							quad,
							vault, 
							bank, 
							addr, 
							length );
			}

			/* set the response command */
			rsp_cmd = WR_RS; 		

			/* set the response length in FLITS */
			rsp_len = 1;

			break;
		case 0x09:
			/* WR32 */

			if( (hmc->tracelevel & HMC_TRACE_CMD) > 0 ){ 
				hmcsim_trace_rqst(	hmc, 
							"WR32", 
							dev, 
							quad,
							vault, 
							bank, 
							addr, 
							length );
			}

			/* set the response command */
			rsp_cmd = WR_RS; 		

			/* set the response length in FLITS */
			rsp_len = 1;

			break;
		case 0x0A:
			/* WR48 */

			/* 
			 * check to see if we exceed maximum block size 
			 * 
			 */
			if( bsize < 48 ){ 
				error = 1;
				break;
			}

			if( (hmc->tracelevel & HMC_TRACE_CMD) > 0 ){ 
				hmcsim_trace_rqst(	hmc, 
							"WR48", 
							dev, 
							quad,
							vault, 
							bank, 
							addr, 
							length );
			}

			/* set the response command */
			rsp_cmd = WR_RS; 		

			/* set the response length in FLITS */
			rsp_len = 1;

			break;
		case 0x0B:
			/* WR64 */

			/* 
			 * check to see if we exceed maximum block size 
			 * 
			 */
			if( bsize < 64 ){ 
				error = 1;
				break;
			}

			if( (hmc->tracelevel & HMC_TRACE_CMD) > 0 ){ 
				hmcsim_trace_rqst(	hmc, 
							"WR64", 
							dev, 
							quad,
							vault, 
							bank, 
							addr, 
							length );
			}

			/* set the response command */
			rsp_cmd = WR_RS; 		

			/* set the response length in FLITS */
			rsp_len = 1;

			break;
		case 0x0C:
			/* WR80 */

			/* 
			 * check to see if we exceed maximum block size 
			 * 
			 */
			if( bsize < 80 ){ 
				error = 1;
				break;
			}

			if( (hmc->tracelevel & HMC_TRACE_CMD) > 0 ){ 
				hmcsim_trace_rqst(	hmc, 
							"WR80", 
							dev, 
							quad,
							vault, 
							bank, 
							addr, 
							length );
			}

			/* set the response command */
			rsp_cmd = WR_RS; 		

			/* set the response length in FLITS */
			rsp_len = 1;

			break;
		case 0x0D:
			/* WR96 */

			/* 
			 * check to see if we exceed maximum block size 
			 * 
			 */
			if( bsize < 96 ){ 
				error = 1;
				break;
			}

			if( (hmc->tracelevel & HMC_TRACE_CMD) > 0 ){ 
				hmcsim_trace_rqst(	hmc, 
							"WR96", 
							dev, 
							quad,
							vault, 
							bank, 
							addr, 
							length );
			}

			/* set the response command */
			rsp_cmd = WR_RS; 		

			/* set the response length in FLITS */
			rsp_len = 1;

			break;
		case 0x0E:
			/* WR112 */

			/* 
			 * check to see if we exceed maximum block size 
			 * 
			 */
			if( bsize < 112 ){ 
				error = 1;
				break;
			}

			if( (hmc->tracelevel & HMC_TRACE_CMD) > 0 ){ 
				hmcsim_trace_rqst(	hmc, 
							"WR112", 
							dev, 
							quad,
							vault, 
							bank, 
							addr, 
							length );
			}

			/* set the response command */
			rsp_cmd = WR_RS; 		

			/* set the response length in FLITS */
			rsp_len = 1;

			break;
		case 0x0F:
			/* WR128 */

			/* 
			 * check to see if we exceed maximum block size 
			 * 
			 */
			if( bsize < 128 ){ 
				error = 1;
				break;
			}

			if( (hmc->tracelevel & HMC_TRACE_CMD) > 0 ){ 
				hmcsim_trace_rqst(	hmc, 
							"WR128", 
							dev, 
							quad,
							vault, 
							bank, 
							addr, 
							length );
			}

			/* set the response command */
			rsp_cmd = WR_RS; 		

			/* set the response length in FLITS */
			rsp_len = 1;

			break;
		case 0x10:
			/* MD_WR */

			if( (hmc->tracelevel & HMC_TRACE_CMD) > 0 ){ 
				hmcsim_trace_rqst(	hmc, 
							"MD_WR", 
							dev, 
							quad,
							vault, 
							bank, 
							addr, 
							length );
			}

			/* set the response command */
			rsp_cmd = MD_WR_RS; 		

			/* set the response length in FLITS */
			rsp_len = 1;

			break;
		case 0x11:
			/* BWR */

			if( (hmc->tracelevel & HMC_TRACE_CMD) > 0 ){ 
				hmcsim_trace_rqst(	hmc, 
							"BWR", 
							dev, 
							quad,
							vault, 
							bank, 
							addr, 
							length );
			}

			/* set the response command */
			rsp_cmd = WR_RS; 		

			break;
		case 0x12:
			/* TWOADD8 */

			if( (hmc->tracelevel & HMC_TRACE_CMD) > 0 ){ 
				hmcsim_trace_rqst(	hmc, 
							"TWOADD8", 
							dev, 
							quad,
							vault, 
							bank, 
							addr, 
							length );
			}

			/* set the response command */
			rsp_cmd = WR_RS; 		

			break;
		case 0x13:
			/* ADD16 */

			if( (hmc->tracelevel & HMC_TRACE_CMD) > 0 ){ 
				hmcsim_trace_rqst(	hmc, 
							"ADD16", 
							dev, 
							quad,
							vault, 
							bank, 
							addr, 
							length );
			}

			/* set the response command */
			rsp_cmd = WR_RS; 		

			break;
		case 0x18:
			/* P_WR16 */

			if( (hmc->tracelevel & HMC_TRACE_CMD) > 0 ){ 
				hmcsim_trace_rqst(	hmc, 
							"P_WR16", 
							dev, 
							quad,
							vault, 
							bank, 
							addr, 
							length );
			}

			/* set the response command */
			rsp_cmd = RSP_NONE; 		

			break;
		case 0x19:
			/* P_WR32 */

			if( (hmc->tracelevel & HMC_TRACE_CMD) > 0 ){ 
				hmcsim_trace_rqst(	hmc, 
							"P_WR32", 
							dev, 
							quad,
							vault, 
							bank, 
							addr, 
							length );
			}

			/* set the response command */
			rsp_cmd = RSP_NONE; 		

			break;
		case 0x1A:
			/* P_WR48 */

			/* 
			 * check to see if we exceed maximum block size 
			 * 
			 */
			if( bsize < 48 ){ 
				error = 1;
				break;
			}

			if( (hmc->tracelevel & HMC_TRACE_CMD) > 0 ){ 
				hmcsim_trace_rqst(	hmc, 
							"P_WR48", 
							dev, 
							quad,
							vault, 
							bank, 
							addr, 
							length );
			}

			/* set the response command */
			rsp_cmd = RSP_NONE; 		

			break;
		case 0x1B:
			/* P_WR64 */

			/* 
			 * check to see if we exceed maximum block size 
			 * 
			 */
			if( bsize < 64 ){ 
				error = 1;
				break;
			}

			if( (hmc->tracelevel & HMC_TRACE_CMD) > 0 ){ 
				hmcsim_trace_rqst(	hmc, 
							"P_WR64", 
							dev, 
							quad,
							vault, 
							bank, 
							addr, 
							length );
			}

			/* set the response command */
			rsp_cmd = RSP_NONE; 		

			break;
		case 0x1C:
			/* P_WR80 */

			/* 
			 * check to see if we exceed maximum block size 
			 * 
			 */
			if( bsize < 80 ){ 
				error = 1;
				break;
			}

			if( (hmc->tracelevel & HMC_TRACE_CMD) > 0 ){ 
				hmcsim_trace_rqst(	hmc, 
							"P_WR80", 
							dev, 
							quad,
							vault, 
							bank, 
							addr, 
							length );
			}

			/* set the response command */
			rsp_cmd = RSP_NONE; 		

			break;
		case 0x1D:
			/* P_WR96 */

			/* 
			 * check to see if we exceed maximum block size 
			 * 
			 */
			if( bsize < 96 ){ 
				error = 1;
				break;
			}

			if( (hmc->tracelevel & HMC_TRACE_CMD) > 0 ){ 
				hmcsim_trace_rqst(	hmc, 
							"P_WR96", 
							dev, 
							quad,
							vault, 
							bank, 
							addr, 
							length );
			}

			/* set the response command */
			rsp_cmd = RSP_NONE; 		

			break;
		case 0x1E:
			/* P_WR112 */

			/* 
			 * check to see if we exceed maximum block size 
			 * 
			 */
			if( bsize < 112 ){ 
				error = 1;
				break;
			}

			if( (hmc->tracelevel & HMC_TRACE_CMD) > 0 ){ 
				hmcsim_trace_rqst(	hmc, 
							"P_WR112", 
							dev, 
							quad,
							vault, 
							bank, 
							addr, 
							length );
			}

			/* set the response command */
			rsp_cmd = RSP_NONE; 		

			break;
		case 0x1F:
			/* P_WR128 */

			/* 
			 * check to see if we exceed maximum block size 
			 * 
			 */
			if( bsize < 128 ){ 
				error = 1;
				break;
			}

			if( (hmc->tracelevel & HMC_TRACE_CMD) > 0 ){ 
				hmcsim_trace_rqst(	hmc, 
							"P_WR128", 
							dev, 
							quad,
							vault, 
							bank, 
							addr, 
							length );
			}

			/* set the response command */
			rsp_cmd = RSP_NONE; 		

			break;
		case 0x21:
			/* P_BWR */

			if( (hmc->tracelevel & HMC_TRACE_CMD) > 0 ){ 
				hmcsim_trace_rqst(	hmc, 
							"P_BWR", 
							dev, 
							quad,
							vault, 
							bank, 
							addr, 
							length );
			}

			/* set the response command */
			rsp_cmd = RSP_NONE; 		

			break;
		case 0x22:
			/* P2ADD8 */

			if( (hmc->tracelevel & HMC_TRACE_CMD) > 0 ){ 
				hmcsim_trace_rqst(	hmc, 
							"P2ADD8", 
							dev, 
							quad,
							vault, 
							bank, 
							addr, 
							length );
			}

			/* set the response command */
			rsp_cmd = RSP_NONE; 		

			break;
		case 0x23:
			/* P_ADD16 */

			if( (hmc->tracelevel & HMC_TRACE_CMD) > 0 ){ 
				hmcsim_trace_rqst(	hmc, 
							"P_ADD16", 
							dev, 
							quad,
							vault, 
							bank, 
							addr, 
							length );
			}

			/* set the response command */
			rsp_cmd = RSP_NONE; 		

			break;
		case 0x30:
			/* RD16 */

			if( (hmc->tracelevel & HMC_TRACE_CMD) > 0 ){ 
				hmcsim_trace_rqst(	hmc, 
							"RD16", 
							dev, 
							quad,
							vault, 
							bank, 
							addr, 
							length );
			}

			/* set the response command */
			rsp_cmd = RD_RS; 		

			/* set the response length in FLITS */
			rsp_len = 2;

			break;
		case 0x31:
			/* RD32 */

			if( (hmc->tracelevel & HMC_TRACE_CMD) > 0 ){ 
				hmcsim_trace_rqst(	hmc, 
							"RD32", 
							dev, 
							quad,
							vault, 
							bank, 
							addr, 
							length );
			}

			/* set the response command */
			rsp_cmd = RD_RS; 		

			/* set the response length in FLITS */
			rsp_len = 3;

			break;
		case 0x32:
			/* RD48 */

			/* 
			 * check to see if we exceed maximum block size 
			 * 
			 */
			if( bsize < 48 ){ 
				error = 1;
				break;
			}

			if( (hmc->tracelevel & HMC_TRACE_CMD) > 0 ){ 
				hmcsim_trace_rqst(	hmc, 
							"RD48", 
							dev, 
							quad,
							vault, 
							bank, 
							addr, 
							length );
			}

			/* set the response command */
			rsp_cmd = RD_RS; 		

			/* set the response length in FLITS */
			rsp_len = 4;

			break;
		case 0x33:
			/* RD64 */

			/* 
			 * check to see if we exceed maximum block size 
			 * 
			 */
			if( bsize < 64 ){ 
				error = 1;
				break;
			}

			if( (hmc->tracelevel & HMC_TRACE_CMD) > 0 ){ 
				hmcsim_trace_rqst(	hmc, 
							"RD64", 
							dev, 
							quad,
							vault, 
							bank, 
							addr, 
							length );
			}

			/* set the response command */
			rsp_cmd = RD_RS; 		

			/* set the response length in FLITS */
			rsp_len = 5;

			break;
		case 0x34:
			/* RD80 */

			/* 
			 * check to see if we exceed maximum block size 
			 * 
			 */
			if( bsize < 80 ){ 
				error = 1;
				break;
			}

			if( (hmc->tracelevel & HMC_TRACE_CMD) > 0 ){ 
				hmcsim_trace_rqst(	hmc, 
							"RD80", 
							dev, 
							quad,
							vault, 
							bank, 
							addr, 
							length );
			}

			/* set the response command */
			rsp_cmd = RD_RS; 		

			/* set the response length in FLITS */
			rsp_len = 6;

			break;
		case 0x35:
			/* RD96 */

			/* 
			 * check to see if we exceed maximum block size 
			 * 
			 */
			if( bsize < 96 ){ 
				error = 1;
				break;
			}

			if( (hmc->tracelevel & HMC_TRACE_CMD) > 0 ){ 
				hmcsim_trace_rqst(	hmc, 
							"RD96", 
							dev, 
							quad,
							vault, 
							bank, 
							addr, 
							length );
			}

			/* set the response command */
			rsp_cmd = RD_RS; 		

			/* set the response length in FLITS */
			rsp_len = 7;

			break;
		case 0x36:
			/* RD112 */

			/* 
			 * check to see if we exceed maximum block size 
			 * 
			 */
			if( bsize < 112 ){ 
				error = 1;
				break;
			}

			if( (hmc->tracelevel & HMC_TRACE_CMD) > 0 ){ 
				hmcsim_trace_rqst(	hmc, 
							"RD112", 
							dev, 
							quad,
							vault, 
							bank, 
							addr, 
							length );
			}

			/* set the response command */
			rsp_cmd = RD_RS; 		

			/* set the response length in FLITS */
			rsp_len = 8;

			break;
		case 0x37:
			/* RD128 */

			/* 
			 * check to see if we exceed maximum block size 
			 * 
			 */
			if( bsize < 128 ){ 
				error = 1;
				break;
			}

			if( (hmc->tracelevel & HMC_TRACE_CMD) > 0 ){ 
				hmcsim_trace_rqst(	hmc, 
							"RD128", 
							dev, 
							quad,
							vault, 
							bank, 
							addr, 
							length );
			}

			/* set the response command */
			rsp_cmd = RD_RS; 		

			/* set the response length in FLITS */
			rsp_len = 9;

			break;
		case 0x28:
			/* MD_RD */

			if( (hmc->tracelevel & HMC_TRACE_CMD) > 0 ){ 
				hmcsim_trace_rqst(	hmc, 
							"MD_RD", 
							dev, 
							quad,
							vault, 
							bank, 
							addr, 
							length );
			}

			/* set the response command */
			rsp_cmd = MD_RD_RS; 		

			/* set the response length in FLITS */
			rsp_len = 2;

			break;
		case 0x00:
			/* FLOW_NULL */

			if( (hmc->tracelevel & HMC_TRACE_CMD) > 0 ){ 
				hmcsim_trace_rqst(	hmc, 
							"FLOW_NULL", 
							dev, 
							quad,
							vault, 
							bank, 
							addr, 
							length );
			}

			/* signal no response packet required */
			no_response = 1;

			break;
		case 0x01:
			/* PRET */

			if( (hmc->tracelevel & HMC_TRACE_CMD) > 0 ){ 
				hmcsim_trace_rqst(	hmc, 
							"PRET", 
							dev, 
							quad,
							vault, 
							bank, 
							addr, 
							length );
			}

			/* signal no response packet required */
			no_response = 1;

			break;
		case 0x02:
			/* TRET */

			if( (hmc->tracelevel & HMC_TRACE_CMD) > 0 ){ 
				hmcsim_trace_rqst(	hmc, 
							"TRET", 
							dev, 
							quad,
							vault, 
							bank, 
							addr, 
							length );
			}

			/* signal no response packet required */
			no_response = 1;

			break;
		case 0x03:
			/* IRTRY */

			if( (hmc->tracelevel & HMC_TRACE_CMD) > 0 ){ 
				hmcsim_trace_rqst(	hmc, 
							"IRTRY", 
							dev, 
							quad,
							vault, 
							bank, 
							addr, 
							length );
			}

			/* signal no response packet required */
			no_response = 1;

			break;
		default:
			break;
	}

	/* 
 	 * Step 4: build and register the response with vault response queue
	 * 
 	 */
step4_vr:
	if( no_response == 0 ){

		/* -- build the response */
		rsp_slid 	= ((tail>>24) & 0x07);
		rsp_tag		= ((head>>15) & 0x1FF );
		rsp_crc		= ((tail>>32) & 0xFFFFFFFF);
		rsp_rtc		= ((tail>>27) & 0x3F);
		rsp_seq		= ((tail>>16) & 0x07);
		rsp_frp		= ((tail>>8) & 0xFF);
		rsp_rrp		= (tail & 0xFF);

		/* -- decode the repsonse command : see hmc_response.c */
		hmcsim_decode_rsp_cmd( rsp_cmd, &(tmp8) );

		/* -- packet head */
		rsp_head	|= (tmp8 & 0x3F);
		rsp_head	|= (rsp_len<<8);
		rsp_head	|= (rsp_len<<11);
		rsp_head	|= (rsp_tag<<15);
		rsp_head	|= (rsp_slid<<39); 

		/* -- packet tail */
		rsp_tail	|= (rsp_rrp);
		rsp_tail	|= (rsp_frp<<8);
		rsp_tail	|= (rsp_seq<<16);
		rsp_tail	|= (rsp_rtc<<27);
		rsp_tail	|= (rsp_crc<<32); 

		packet[0] 		= rsp_head;
		packet[((rsp_len*2)-1)]	= rsp_tail;

		/* -- register the response */
		hmc->devs[dev].quads[quad].vaults[vault].rsp_queue[t_slot].valid = HMC_RQST_VALID;
		for( j=0; j<rsp_len; j++ ){ 
			hmc->devs[dev].quads[quad].vaults[vault].rsp_queue[t_slot].packet[j] = packet[j];
		}

	}/* else, no response required, probably flow control */

	/* 
	 * Step 5: invalidate the request queue slot
	 * 
 	 */	
	hmcsim_util_zero_packet( queue );

	return 0;
}

/* EOF */
