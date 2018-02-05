/*
 * _EXECUTE_TEST_C_
 *
 * HMCSIM MUTEX EXECUTION FUNCTIONS
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "hmc_sim.h"


/* ------------------------------------------------- INC_TAG */
/* INC_TAG */
static void inc_tag( uint16_t *tag ){
  uint16_t ltag = *tag;
  ltag++;
  if( ltag == (UINT8_MAX-1)){
    ltag = 1;
  }

  *tag = ltag;
}

/* ------------------------------------------------- INC_LINK */
/* INC_LINK */
static void inc_link( struct hmcsim_t *hmc, uint8_t *link ){
  uint8_t llink = *link;
  llink++;
  if( llink == hmc->num_links ){
    llink = 0;
  }
  *link = llink;
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
 * FINITE STATE MACHINE FOR MULTITHREADED MUTEX TEST
 * ALL THREADS ATTEMPT TO ACQUIRE THE LOCK FOR AT LEAST ONE CYCLE
 * THIS IS DELIBERATEY A NAIVE IMPLEMENTATION
 *
 * WHILE( NUM_LOCKS < NUM_THREADS ){
 *    IF( NOT_LOCKED ){
 *      ATTEMPT TO LOCK()
 *      IF( SUCCESS ){
 *        UPDATE GLOBAL
 *        UNLOCK
 *      }ELSE{
 *        TRYLOCK UNTIL SUCCESS
 *        UNLOCK
 *      }
 *    }
 * }
 *
 */
extern int execute_test(        struct hmcsim_t *hmc,
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

  uint64_t addr         = 0x00000001a45e4600;

  FILE *ofile		= NULL;

  struct mylock lock;

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

  struct mylock *wlocks = NULL;         /* current thread lock copy */
  uint64_t *status      = NULL;         /* current thread status */
  uint64_t *cycles      = NULL;         /* thread local cycle count */
  int *wstatus          = NULL;         /* current thread wait status */
  uint16_t *wtags       = NULL;         /* current thread wait tags */
  /* ---- */


  /* allocate memory & init thread state */
  cycles  = malloc( sizeof( uint64_t ) * num_threads );
  status  = malloc( sizeof( uint64_t ) * num_threads );
  wstatus = malloc( sizeof( int ) * num_threads );
  wtags   = malloc( sizeof( uint16_t ) * num_threads );
  wlocks  = malloc( sizeof( struct mylock ) * num_threads );
  for( i=0; i<num_threads; i++ ){
    status[i]   = TAG_START;
    cycles[i]   = 0x00ll;
    wstatus[i]  = -1;
    wtags[i]    = 0;
    wlocks[i].tid = (int64_t)(-1);
    wlocks[i].mlock= (uint64_t)(0);
  }

  /* init the lock */
  lock.tid  = (int64_t)(-1);
  lock.mlock = (uint64_t)(0);

  /*
   * setup the tracing mechanisms
   *
   */
  printf( "...INITIALIZING TRACE FILE\n" );
  ofile = fopen( "mutex.out", "w" );
  if( ofile == NULL ){
    printf( "FAILED : COULD NOT OPEN OUPUT FILE mutex.out\n" );
    return -1;
  }

  hmcsim_trace_handle( hmc, ofile );
#if 0
  hmcsim_trace_level( hmc, (HMC_TRACE_CMD|
                            HMC_TRACE_STALL|
                            HMC_TRACE_LATENCY) );
  hmcsim_trace_header( hmc );
#endif

  printf( "SUCCESS : INITIALIZED TRACE HANDLERS\n" );

  /*
   * zero the packet
   *
   */
  zero_packet( &(packet[0]) );

  printf( "BEGINNING EXECUTION\n" );


  /* -- begin cycle loop */
  while( done < num_threads ){

    /* -- cycle through all the threads */
    for( i=0; i<num_threads; i++ ){

      /* -- if the thread is not done, try to perform an op */
      switch( status[i] ){

      /* -- TAG_START */
      case TAG_START:
        cycles[i]++;
        payload[0] = (uint64_t)(i);
        ret = hmcsim_build_memrequest( hmc,
                                       0,
                                       addr,
                                       tag,
                                       CMC125,    /* LOCK */
                                       link,
                                       &(payload[0]),
                                       &head,
                                       &tail);
        if( ret == 0 ){
          packet[0] = head;
          packet[1] = (uint64_t)(i);
          packet[2] = 0x00ll;
          packet[3] = tail;
          ret = hmcsim_send( hmc, &(packet[0]) );
        }else{
          printf( "ERROR : FATAL : MALFORMED PACKET FROM THREAD %d\n", i );
        }
        switch( ret ){
        case 0:
          /* success */
          status[i]       = TAG_LOCK_RECV;
          wlocks[i].mlock  = 0;
          wstatus[i]      = 0;
          wtags[i]        = tag;
          inc_tag( &tag );
          inc_link(hmc, &link);
          /* try to set the lock */
          if( lock.mlock == 0 ){
            //printf( "...thread %d acquired the lock\n", i );
            lock.mlock = 1;
            lock.tid  = i;
          }
          //printf( "THREAD %d SENT HMC_LOCK PACKET; TAG=%d\n", i, wtags[i] );
          break;
        case HMC_STALL:
          /* stalled */
          wstatus[i]  = HMC_STALL;
          wtags[i]    = 0;
          /* drop to trylock */
          status[i]   = TAG_LOCK_SEND;
          break;
        case -1:
        default:
          printf( "FAILED : LOCK PACKET SEND FAILURE\n" );
          goto complete_failure;
          break;
        }

        break;

      /* -- TAG_LOCK_RECV */
      case TAG_LOCK_RECV:
        cycles[i]++;

        switch( wlocks[i].mlock ){
        case 0:
          /* still waiting */
          break;
        case 1:
          /* i have the lock */
          status[i]       = TAG_ULOCK_SEND;
          wlocks[i].mlock = 0;
          wtags[i]        = 0;
          //printf( "THREAD %d RECEIVED A RESPONSE: I HAVE THE LOCK\n", i );
          break;
        case 2:
          /* the lock is set, but it is not me */
          status[i]       = TAG_TLOCK_SEND;
          wlocks[i].mlock = 0;
          wtags[i]        = 0;
          //printf( "THREAD %d RECEIVED A RESPONSE: I DO NOT HAVE THE LOCK\n", i );
          break;
        default:
          printf( "FAILED : LOCK PACKET RECV FAILURE\n" );
          goto complete_failure;
          break;
        }

        break;

      /* -- TAG_TLOCK_SEND */
      case TAG_TLOCK_SEND:
        cycles[i]++;

        payload[0] = (uint64_t)(i);
        ret = hmcsim_build_memrequest( hmc,
                                       0,
                                       addr,
                                       tag,
                                       CMC126,    /* TLOCK */
                                       link,
                                       &(payload[0]),
                                       &head,
                                       &tail);
        if( ret == 0 ){
          packet[0] = head;
          packet[1] = (uint64_t)(i);
          packet[2] = 0x00ll;
          packet[3] = tail;
          ret = hmcsim_send( hmc, &(packet[0]) );
        }else{
          printf( "ERROR : FATAL : MALFORMED PACKET FROM THREAD %d\n", i );
        }
        switch( ret ){
        case 0:
          /* success */
          status[i]       = TAG_TLOCK_RECV;
          wlocks[i].mlock = 0;
          wstatus[i]      = 0;
          wtags[i]        = tag;
          inc_tag( &tag );
          inc_link(hmc, &link);
          /* try to set the lock */
          if( lock.mlock == 0 ){
            lock.mlock = 1;
            lock.tid  = i;
          }
          //printf( "THREAD %d SENT HMC_TRYLOCK PACKET; TAG=%d\n", i, wtags[i] );
          break;
        case HMC_STALL:
          /* stalled */
          wstatus[i]  = HMC_STALL;
          wtags[i]    = 0;
          break;
        case -1:
        default:
          printf( "FAILED : TLOCK PACKET SEND FAILURE\n" );
          goto complete_failure;
          break;
        }

        break;

      /* -- TAG_TLOCK_RECV */
      case TAG_TLOCK_RECV:
        cycles[i]++;

        switch( wlocks[i].mlock ){
        case 0:
          /* still waiting */
          break;
        case 1:
          /* i have the lock */
          status[i]       = TAG_ULOCK_SEND;
          wlocks[i].mlock = 0;
          wtags[i]        = 0;
          //printf( "THREAD %d RECEIVED A RESPONSE: I HAVE THE LOCK\n", i );
          break;
        case 2:
          /* the lock is set, but it is not me */
          status[i]       = TAG_TLOCK_SEND;
          wlocks[i].mlock = 0;
          wtags[i]        = 0;
          //printf( "THREAD %d RECEIVED A RESPONSE: I DO NOT HAVE THE LOCK\n", i );
          break;
        default:
          printf( "FAILED : TLOCK PACKET RECV FAILURE\n" );
          goto complete_failure;
          break;
        }

        break;

      /* -- TAG_ULOCK_SEND */
      case TAG_ULOCK_SEND:
        cycles[i]++;

        payload[0] = (uint64_t)(i);
        ret = hmcsim_build_memrequest( hmc,
                                       0,
                                       addr,
                                       tag,
                                       CMC127,    /* ULOCK */
                                       link,
                                       &(payload[0]),
                                       &head,
                                       &tail);
        if( ret == 0 ){
          packet[0] = head;
          packet[1] = (uint64_t)(i);
          packet[2] = 0x00ll;
          packet[3] = tail;
          ret = hmcsim_send( hmc, &(packet[0]) );
        }else{
          printf( "ERROR : FATAL : MALFORMED PACKET FROM THREAD %d\n", i );
        }
        switch( ret ){
        case 0:
          /* success */
          status[i]       = TAG_ULOCK_RECV;
          wlocks[i].mlock = 0;
          wstatus[i]      = 0;
          wtags[i]        = tag;
          inc_tag( &tag );
          inc_link(hmc, &link);
          /* try to set the lock */
          if( (lock.mlock == 1) /*&& (lock.tid == i)*/ ){
            lock.mlock = 0;
            lock.tid  = i;
            //printf( "THREAD %d SENT HMC_ULOCK PACKET; TAG=%d\n", i, wtags[i] );
          }
          break;
        case HMC_STALL:
          /* stalled */
          wstatus[i]  = HMC_STALL;
          wtags[i]    = 0;
          break;
        case -1:
        default:
          printf( "FAILED : ULOCK PACKET SEND FAILURE\n" );
          goto complete_failure;
          break;
        }
        break;

      /* -- TAG_ULOCK_RECV */
      case TAG_ULOCK_RECV:
        cycles[i]++;

        switch( wlocks[i].mlock ){
        case 0:
          /* still waiting */
          break;
        case 1:
        case 2:
        default:
          /* i have the lock */
          status[i]       = TAG_DONE;
          done++;
          //printf( "THREAD %d RECEIVED A RESPONSE: UNLOCK SUCCESS\n", i );
          break;
        }

        break;
      case TAG_DONE:
        /* thread is done, do nothing */
        break;
      default:
        /* error */
        break;
      }

    }/*-- end for loop */


    /* drain all the responses */
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

          trigger_mutex_response( d_type,
                                  d_tag,
                                  wtags,
                                  wlocks,
                                  status,
                                  wstatus,
                                  lock,
                                  num_threads );

        }/* -- good response */
      }/* -- get all the responses */
    }/* -- end response loop */


    /*
     * clock the sim
     *
     */
    hmcsim_clock( hmc );
  }
  /* -- end cycle loop */

  /* print a summary */
  printf( " SUMMARY : NUM_THREADS = %d\n", num_threads );
  printf( " MIN_CYCLE             = %" PRIu64 "\n",
          find_min_cycle( cycles, num_threads ) );
  printf( " MAX_CYCLE             = %" PRIu64 "\n",
          find_max_cycle( cycles, num_threads ) );
  printf( " AVG_CYCLE             = %f\n",
          find_avg_cycle( cycles, num_threads ) );

complete_failure:
  fclose( ofile );
  ofile = NULL;

  free( cycles );
  free( status);
  free( wstatus);
  free( wtags);
  free( wlocks);

  return 0;
}


/* EOF */
