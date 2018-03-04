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

  var = (uint32_t)((header>>7) & 0x1F);
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

/* ------------------------------------------------- BUILD_STENCIL_PAYLOAD */
/* BUILD_STENCIL_PAYLOAD
 *
 */
static void build_stencil_payload( uint8_t point, 
				   uint8_t array_width, 
				   uint8_t array_height,
				   uint8_t * stencil_layout,
				   uint64_t * payload ){
  payload[0] = 0x0ll;
  *(payload) += (point & 0xFF);
  *(payload) += (array_width & 0xFF) << 0x8;
  *(payload) += (array_height & 0xFF) << 0x10;

  const uint8_t num_packets = 1 + ((point * 8 * 3) / 64);
  uint8_t i = 0;
  for(i=0; i < num_packets; i++){
    uint8_t cur_pos = 0;
    if(i==0){
      payload[0] = 0x0ll;
      *(payload) += (point & 0xFF);
      *(payload) += (array_width & 0xFF) << 0x8;
      *(payload) += (array_height & 0xFF) << 0x10;
      cur_pos += 24;
    }
    for(;cur_pos < 64; cur_pos+=8){
      payload[i] += *(stencil_layout+cur_pos*(i+1)) << cur_pos;
    }
  }
}

/* ------------------------------------------------- EXECUTE_TEST */
/*
 * EXECUTE TEST
 *
 */
extern int execute_test(        struct hmcsim_t *hmc,
                                uint32_t num_threads,
				uint32_t shiftamt,
				uint32_t array_width,
				uint32_t array_height,
				uint32_t array_depth,
				double clock_ghz )
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
  uint32_t j		= 0;
  uint32_t k		= 0;
  uint64_t packet[HMC_MAX_UQ_PACKET];

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
  ofile = fopen( "stencil_read.out", "w" );
  if( ofile == NULL ){
    printf( "FAILED : COULD NOT OPEN OUPUT FILE stencil_read.out\n" );
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
 
 
  //defines center-inclusive moore neighborhood of r=1
  const uint8_t r = 1;
  const uint8_t dims = 3;
  uint8_t stencil_point = 27;
  uint8_t stencil_layout[stencil_point][dims];
  uint8_t l = 0;
  for(i=-r; i<=r; i++){
    for(j=-r; j<=r; j++){
      for(k=-r; k<=r; k++){
        stencil_layout[l][0] = i & 0xFF;
        stencil_layout[l][1] = j & 0xFF;
        stencil_layout[l][2] = k & 0xFF;
        l++;
      }
    }
  }
  

  build_stencil_payload(stencil_point, array_width, array_height, (uint8_t *)stencil_layout, payload);
   
  uint32_t array_size = array_width * array_height * array_depth;
  uint32_t data_width = 4;  

  /* -- begin cycle cycle loop */
  printf("GENERATING REQUEST ADDRESSES FOR ARRAY OF SIZE: %d\n", array_size);
  uint64_t addr[array_size];
  uint32_t l_size = 0;
  uint32_t curX, curY, curZ;
  for(curX = 1; curX < (array_width - 1); curX++) {
    for(curY = 1; curY < (array_height - 1); curY++){
      for(curZ = 1; curZ < (array_depth - 1); curZ++){
        addr[l_size] = (data_width * curX) + (data_width * curY * array_width) + (data_width * curZ * array_width * array_height);
        l_size++;
      }
    }
  }
  struct dict_entry{
    uint16_t key;
    uint16_t val;
  };
  struct dict_entry dict[l_size];
  // queue for addresses waiting to be written to 
  // because we set queue size to l_size, we don't
  // need to worry about exceeding queue size or wrapping
  uint64_t addr_q[l_size]; 
  uint16_t q_start = 0;
  uint16_t q_end = 0;

  uint32_t sreads_sent = 0;
  uint32_t sreads_recvd = 0;
  // number of stores system is waiting on
  while( done != 1 ){
    if( cycles == 0){
      // first cycle; init the stencil register
      ret = hmcsim_build_memrequest( hmc,
                                     0,
                                     0,
                                     tag,
                                     CMC23,    /* STENCIL_LOAD_3D_1_INIT */
                                     link,
                                     &(packet[0]),
                                     &head,
                                     &tail);
    if( ret == 0 ){
        plen = rqst_packet_length(head);
        printf( "SENDING PACKET WITH %d FLITS\n", plen );
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
        printf("ATTEMPTING TO SET CMC STENCIL REG WITH DATA: 0x%016lx\n", payload[0]);
        ret = hmcsim_send( hmc, &(packet[0]) );
      }else{
        printf( "ERROR : FATAL : MALFORMED PACKET FROM THREAD %d\n", i );
      }

      switch( ret ){
      case 0:
      /* success */
        printf( "SUCCESS : SET STENCIL_READ CMC REG IN DEVICE\n" );
        tag++;
        break;
      case HMC_STALL:
        printf( "FAILED : HMC DEVICE WAS STALLED\n" );
        break;
      case -1:
      default:
        printf( "FAILED : STENCIL_READ_INIT PACKET SEND FAILURE\n" );
        goto complete_failure;
        break;
      }
      zero_packet(&(packet[0]));
    }else if( q_start - q_end != 0 ){
      // if the size of the write queue > 0
      /* push out a store */

      // dequeue oldest element
      uint32_t addr_temp = addr_q[q_start];
      q_start++;
      /* build the request */
      hmcsim_build_memrequest( hmc,
                               0,
			       addr_temp,
                               tag,
                               WR64,
                               link,
                               &(payload[0]),
                               &head,
                               &tail );

      packet[0]       = head;
      packet[1]       = 0x02ll;
      packet[2]       = 0x03ll;
      packet[3]       = 0x04ll;
      packet[4]       = 0x05ll;
      packet[5]       = 0x06ll;
      packet[6]       = 0x07ll;
      packet[7]       = 0x08ll;
      packet[8]       = 0x09ll;
      packet[9]       = tail;
      packet[1]       = tail;

      /* send it */
      ret     = hmcsim_send( hmc, &(packet[0]) );
      /* handle the response */
      switch( ret ){
      case 0:
        /* success */
        printf("SUCCESS : PACKET WRITTEN TO HMC\n");
        tag++;
        break;
      case HMC_STALL:
        /* stalled */
        printf( "STALL: HMC STALLED\n" );
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

    }else if( sreads_sent < l_size ){  
      // send a request
      ret = hmcsim_build_memrequest( hmc,
                                     0,
                                     addr[sreads_sent],
                                     tag,
                                     CMC22,    /* STENCIL_LOAD_3D_1 */
                                     link,
                                     &(packet[0]),
                                     &head,
                                     &tail);
      //add dict entry for tag-addr pair for addr retrieval
      dict[sreads_sent].key = tag;
      dict[sreads_sent].val = addr[sreads_sent];
      // ensure each tag used is unique
      tag++;
      sreads_sent++;
      if( ret == 0 ){
        plen = rqst_packet_length(head);
        printf( "SENDING PACKET WITH %d FLITS\n", plen );
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
        printf("ATTMEPTING TO SEND REQUEST WITH ADDR: 0x%016lx\n", addr[sreads_sent-1]);
        ret = hmcsim_send( hmc, &(packet[0]) );
      }else{
        printf( "ERROR : FATAL : MALFORMED PACKET FROM THREAD %d\n", i );
      }

      switch( ret ){
      case 0:
      /* success */
        printf( "SUCCESS : INJECTED STENCIL_READ PACKET INTO DEVICE\n" );
        tag++;
        break;
      case HMC_STALL:
        printf( "FAILED : HMC DEVICE WAS STALLED\n" );
        break;
      case -1:
      default:
        printf( "FAILED : STENCIL_READ PACKET SEND FAILURE\n" );
        goto complete_failure;
        break;
      }

    }
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
          if( d_type != WR_RS ){
            uint32_t k;
            for(k=0;k<sreads_sent;k++){
	      //enqueue addr that is paired with the tag recieved
              if(dict[k].key == d_tag){
                addr_q[q_end] = dict[k].val;
                q_end++;
              }
            }
            sreads_recvd++;
          }
        }/* -- good response */
      }/* -- get all the responses */
    }/* -- end response loop */
    if(sreads_recvd >= l_size){
      done = 1;
    }
    cycles++;
    hmcsim_clock( hmc );
  }

  printf( "COMPLETED TEST IN %" PRIu64 " CYCLES\n", cycles );
  printf( "Simulated time: %.2f us\n", (double) cycles / (clock_ghz * 1000.0) );
complete_failure:
  fclose( ofile );
  ofile = NULL;

  return 0;
}


/* EOF */
