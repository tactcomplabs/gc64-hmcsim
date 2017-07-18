/*
 * _HMC_SEND_C_
 *
 * HYBRID MEMORY CUBE SIMULATION LIBRARY
 *
 * MEMORY SEND FUNCTIONS
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hmc_sim.h"


/* ----------------------------------------------------- FUNCTION_PROTOTYPES */
extern int hmcsim_util_zero_packet( struct hmc_queue_t *queue );
extern uint32_t hmcsim_cmc_rawtoidx( uint32_t raw );


/* ----------------------------------------------------- HMCSIM_POSTED_RSP */
/*
 * HMCSIM_POSTED_RSP
 *
 */
static void hmcsim_posted_rsp( struct hmcsim_t *hmc, uint64_t hdr ){
  uint32_t tag = 0;
  uint32_t cmd = 0;

  tag = (uint32_t)((hdr>>12)&0x7FF);
  cmd = (uint32_t)(hdr&0x7F);

  switch( cmd ){
  case 24:
  case 25:
  case 26:
  case 27:
  case 28:
  case 29:
  case 30:
  case 31:
  case 95:
  case 34:
  case 35:
  case 84:
  case 33:
    /* posted-type packet; no response */
    hmc->tokens[tag].status = 2;
    break;
  case 4:
  case 5:
  case 6:
  case 7:
  case 8:
  case 20:
  case 21:
  case 22:
  case 23:
  case 32:
  case 36:
  case 37:
  case 38:
  case 39:
  case 41:
  case 42:
  case 43:
  case 44:
  case 45:
  case 46:
  case 47:
  case 56:
  case 57:
  case 58:
  case 59:
  case 60:
  case 61:
  case 62:
  case 63:
  case 69:
  case 70:
  case 71:
  case 72:
  case 73:
  case 74:
  case 75:
  case 76:
  case 77:
  case 78:
  case 85:
  case 86:
  case 87:
  case 88:
  case 89:
  case 90:
  case 91:
  case 92:
  case 93:
  case 94:
  case 102:
  case 103:
  case 107:
  case 108:
  case 109:
  case 110:
  case 111:
  case 112:
  case 113:
  case 114:
  case 115:
  case 116:
  case 117:
  case 118:
  case 120:
  case 121:
  case 122:
  case 123:
  case 124:
  case 125:
  case 126:
  case 127:
    /* cmc packet, check its response */
    if( hmc->cmcs[hmcsim_cmc_rawtoidx(cmd)].rsp_cmd == RSP_NONE ){
      /* cmc with posted response */
      hmc->tokens[tag].status = 2;
    }
    break;
  default:
    /* normal response, just return */
    break;
  }
  hmc->tokens[tag].en_clock = hmc->clk;
}

/* ----------------------------------------------------- HMCSIM_SEND */
/*
 * HMCSIM_SEND
 *
 */
extern int	hmcsim_send( struct hmcsim_t *hmc, uint64_t *packet )
{
	/* vars */
	uint64_t header = 0x00ll;
	uint64_t tail 	= 0x00ll;
	uint32_t len	= 0;
	uint32_t t_len	= 0;
	uint32_t target	= hmc->xbar_depth+1;	/* -- out of bounds to check for stalls */
	uint32_t i	= 0;
	uint32_t cur	= 0;
	uint8_t link	= 0;
	uint8_t cub	= 0;
	struct hmc_queue_t *queue = NULL;
	/* ---- */

	if( hmc == NULL ){
		return -1;
	}

	if( packet == NULL ){
		return -1;
	}

	/* 
	 * pull the packet header
	 * we need to know the packet length and the packet destination
	 *
	 */
	header = packet[0];

#ifdef HMC_DEBUG
	HMCSIM_PRINT_ADDR_TRACE( "PACKET HEADER", header );
#endif

	/*
	 * pull the packet length and grab the tail
	 *
	 */
        len = (uint32_t)( (header >> 7) & 0x1F);
	t_len = len * 2;

	tail = packet[t_len-1];

#ifdef HMC_DEBUG
	HMCSIM_PRINT_ADDR_TRACE( "PACKET TAIL", tail );
	HMCSIM_PRINT_INT_TRACE( "PACKET T_LEN", (int)(t_len) );
#endif

	/*
	 * grab the cub
	 *
	 */
	cub = (uint8_t)( (header >> 61) & 0x7 );

#ifdef HMC_DEBUG
	HMCSIM_PRINT_INT_TRACE( "PACKET CUB", (int)(cub) );
#endif

	/*
	 * grab the link id
	 *
	 */
        link = (uint8_t)( (tail >> 26) & 0x7 );

#ifdef HMC_DEBUG
	HMCSIM_PRINT_INT_TRACE( "PACKET LINK", (int)(link) );
#endif


	/*
	 * validate the cub:link
	 *
	 */
	if( cub > (hmc->num_devs+1) ){
		return -1;
	}else if( cub == hmc->num_devs ){
		return -1;
	}

	if( hmc->devs[cub].links[link].type != HMC_LINK_HOST_DEV ){
		/* 
		 * NOT A HOST LINK!!
	 	 * 
	 	 */

		return -1;
	}

#ifdef HMC_DEBUG
	HMCSIM_PRINT_TRACE( "FOUND A VALID PACKET STRUCTURE" );
#endif

	/*
	 * Now that we have the locality details
	 * of the packet request destination,
	 * go out and walk the respective link
	 * xbar queues and try to push the request
	 * into the first empty queue slot.
	 *
	 * NOTE: this will likely need to be changed
	 *       if we ever support proper ordering
	 * 	 constraints on the devices
	 *
	 */

	cur = hmc->xbar_depth-1;
	for( i=0; i<hmc->xbar_depth; i++ ){
          if( hmc->devs[cub].xbar[link].xbar_rqst[cur].valid
              == HMC_RQST_INVALID ){
            target = cur;
          }
	  cur--;
	}

#ifdef HMC_DEBUG
	HMCSIM_PRINT_INT_TRACE( "TARGET SLOT", (int)(target) );
#endif


	if( target == (hmc->xbar_depth+1) ) {
          /*
           * stall the request
           *
           */
          return HMC_STALL;
	}

	/* else, push the packet into the designate queue slot */
	queue = &( hmc->devs[cub].xbar[link].xbar_rqst[target] );

	hmcsim_util_zero_packet( queue );

	/* set the packet to valid */
	queue->valid = HMC_RQST_VALID;

	for( i=0; i<t_len; i++ ) {
          queue->packet[i] = packet[i];
	}

        /* check for posted packets */
        hmcsim_posted_rsp(hmc, packet[0]);

#ifdef HMC_DEBUG
	HMCSIM_PRINT_TRACE( "PACKET INJECTION SUCCESSFUL" );
#endif

	return 0;
}

/* EOF */
