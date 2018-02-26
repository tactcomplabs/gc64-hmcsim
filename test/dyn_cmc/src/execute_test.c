/*
 * _EXECUTE_TEST_C_
 *
 * HMCSIM DYNAMIC_CMC EXECUTION FUNCTIONS
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "hmc_sim.h"


/* ------------------------------------------------- RQST_PACKET_LENGTH */
uint32_t rqst_packet_length( uint64_t header ){
  uint32_t var = 0x00;

  var = (uint32_t)((header>>7)&0x1F);
  return var;
}

/* ------------------------------------------------- ZERO_PACKET */
/*
 * ZERO_PACKET
 *
 */
static void zero_packet( uint64_t *packet ) {
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
 * EXECUTE TEST
 *
 */
extern int execute_test(        struct hmcsim_t *hmc,
                                uint32_t num_threads,
				uint32_t shiftamt )
{
  /* vars */
  uint64_t head		= 0x00ll;
  uint64_t tail		= 0x00ll;
  uint64_t payload[8]	= {0x00ll, 0x00ll, 0x00ll, 0x00ll,
                           0x00ll, 0x00ll, 0x00ll, 0x00ll };
  int ret		= 0;
  uint8_t cub		= 0;
  uint8_t link		= 0;
  uint16_t tag		= 1;
  uint32_t done		= 0;
  uint32_t i		= 0;
  uint64_t packet[HMC_MAX_UQ_PACKET];

  uint64_t addr         = 0x5B5B0ull;

  FILE *ofile		= NULL;

  uint64_t d_response_head;
  uint64_t d_response_tail;
  hmc_response_t d_type;
  uint8_t d_length;
  uint16_t d_tag;
  uint8_t d_rtn_tag;
  uint8_t d_src_link;
  uint8_t d_rrp;
  uint8_t d_frp;
  uint8_t d_seq;
  uint8_t d_dinv;
  uint8_t d_errstat;
  uint8_t d_rtc;
  uint32_t d_crc;

  uint64_t cycles = 0x00ull;
  uint32_t plen = 0;
  /* ---- */


  /*
   * setup the tracing mechanisms
   *
   */
  printf( "...INITIALIZING TRACE FILE\n" );
  ofile = fopen( "dyn_cmc.out", "w" );
  if( ofile == NULL ){
    printf( "FAILED : COULD NOT OPEN OUPUT FILE mutex.out\n" );
    return -1;
  }

  hmcsim_trace_handle( hmc, ofile );
  hmcsim_trace_level( hmc, (HMC_TRACE_CMD|
                            HMC_TRACE_STALL|
                            HMC_TRACE_LATENCY) );
  hmcsim_trace_header( hmc );

  printf( "SUCCESS : INITIALIZED TRACE HANDLERS\n" );

  /*
   * zero the packet
   *
   */
  zero_packet( &(packet[0]) );

  printf( "BEGINNING EXECUTION\n" );


  /* -- begin cycle cycle loop */
  while( done != 1 ){
    if( cycles == 0 ){
      // send a request
      ret = hmcsim_build_memrequest( hmc,
                                     0,
                                     addr,
                                     tag,
                                     CMC04,    /* DYNAMIC_TEMPLATE_OP */
                                     link,
                                     &(packet[0]),
                                     &head,
                                     &tail);
      if( ret == 0 ){
        plen = rqst_packet_length(head);
        printf( "SENDING PACKET WITH %d FLITS", plen );
        if( plen > 1 ){
          // lay out packet
          packet[0] = head;
          for(i=0; i<((plen-1)*2); i++ ){
            packet[i+1] = payload[i];
          }
          packet[(plen*2)-1] = tail;
        }else{
          packet[0] = head;
          packet[1] = tail;
        }
        ret = hmcsim_send( hmc, &(packet[0]) );
      }else{
        printf( "ERROR : FATAL : MALFORMED PACKET FROM THREAD %d\n", i );
      }

      switch( ret ){
      case 0:
      /* success */
        printf( "SUCCESS : INJECTED DYNAMIC_TEMPLATE_CMC PACKET INTO DEVICE\n" );
        break;
      case HMC_STALL:
        printf( "FAILED : HMC DEVICE WAS STALLED ON FIRST CLOCK CYCLE\n" );
        goto complete_failure;
        break;
      case -1:
      default:
        printf( "FAILED : INITIAL DYNAMIC_TEMPLATE_CMC PACKET SEND FAILURE\n" );
        goto complete_failure;
        break;
      }

    }else{
      // check for response
      for( i=0; i<hmc->num_links; i++ ){
        ret = HMC_OK;
        while( ret != HMC_STALL ){
          ret = hmcsim_recv( hmc, cub, i, &(packet[0]) );

          if( ret == 0 ){
            /* decode the response */
            hmcsim_decode_memresponse( hmc,
                                      &(packet[0]),
                                      &d_response_head,
                                      &d_response_tail,
                                      &d_type,
                                      &d_length,
                                      &d_tag,
                                      &d_rtn_tag,
                                      &d_src_link,
                                      &d_rrp,
                                      &d_frp,
                                      &d_seq,
                                      &d_dinv,
                                      &d_errstat,
                                      &d_rtc,
                                      &d_crc );
            printf( "RECEIVED PACKET WITH %d FLITS\n", d_length );
            done = 1;
          }/* -- good response */
        }/* -- get all the responses */
      }/* -- end response loop */
    }
    cycles++;
    hmcsim_clock( hmc );
  }

  printf( "COMPLETED TEST IN %" PRIu64 " CYCLES\n", cycles );

complete_failure:
  fclose( ofile );
  ofile = NULL;

  return 0;
}


/* EOF */
