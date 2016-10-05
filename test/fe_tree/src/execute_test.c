/*
 * _EXECUTE_TEST_C_
 *
 * HMCSIM MUTEX EXECUTION FUNCTIONS
 *
 *
 * rsp_payload[0] = data
 * rsp_payload[1] = success bit
 */

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "hmc_sim.h"


/* ------------------------------------------------- DATATYPES */
struct node{
  int root;           /* -- is this a root node? */
  int parent;         /* -- is this a parent node? */
  int pnode;          /* -- who is my parent node? */
  int lnode;          /* -- ptr to left node */
  int rnode;          /* -- ptr to right node */
  int nchild;         /* -- number of children */
  int arrived;        /* -- has the parent arrived */
  int mlock;          /* -- the lock bits */
  uint64_t lock;      /* -- the lock for the node */
};

struct mylock{
  int64_t tid;
  uint64_t mlock;
};

/* ------------------------------------------------- PROTOTYPES */
void recur( struct node *tnodes,
            int num_threads,
            int start,
            int end,
            int parent );

/* ------------------------------------------------- TAG MACROS */
#define TAG_START       0x0000  /* -- start state */
#define TAG_RD_LOCAL_S  0x0001  /* -- ReadEF(LocalLock) send */
#define TAG_RD_LOCAL_R  0x0002  /* -- ReadEF(LocalLock) recv */
#define TAG_WR_SENSE0_S 0x0003  /* -- WriteXE(Sense) send */
#define TAG_WR_SENSE0_R 0x0004  /* -- WriteXE(Sense) recv */
#define TAG_WR_SENSE1_S 0x0005  /* -- WriteXF(Sense) send */
#define TAG_WR_SENSE1_R 0x0006  /* -- WriteXF(Sense) recv */
#define TAG_INC_S       0x0007  /* -- IncFF(ParentLock) send */
#define TAG_INC_R       0x0008  /* -- IncFF(ParentLock) recv */
#define TAG_RXX_S       0x0009  /* -- ReadXX(Sense) send */
#define TAG_RXX_R       0x000A  /* -- ReadXX(Sense) recv */
#define TAG_RF_LOCAL_S  0x000B  /* -- ReadFF(LocalLock) send */
#define TAG_RF_LOCAL_R  0x000C  /* -- ReadFF(LocalLock) recv */
#define TAG_DONE        0x000F  /* -- DONE! */

/* ------------------------------------------------- PRINT_TREE */
void print_tree( struct node *tnodes,
                 int num_threads ){
  int i = 0;

  for( i=0; i<num_threads; i++ ){
    printf( "\n" );
    printf( "Node %d\n", i );
    printf( "-- root   = %d\n", tnodes[i].root );
    printf( "-- parent = %d\n", tnodes[i].parent );
    printf( "-- pnode  = %d\n", tnodes[i].pnode );
    printf( "-- lnode  = %d\n", tnodes[i].lnode );
    printf( "-- rnode  = %d\n", tnodes[i].rnode );
    printf( "-- nchild = %d\n", tnodes[i].nchild );
    printf( "-- lock   = 0x%016llx\n", tnodes[i].lock );
  }
}

/* ------------------------------------------------- RHS */
void recur( struct node *tnodes,
            int num_threads,
            int start,
            int end,
            int parent ){

  /* lhs */
  if( (parent-start) == 0 ){
    /* no lhs children */
    return;
  }else if( (parent-start) == 1 ){
    /* 1 lhs child */
    tnodes[start].lnode   = -1;
    tnodes[start].rnode   = -1;
    tnodes[start].nchild  = 0;
    tnodes[start].pnode   = parent;
    tnodes[start].parent  = -1;
    tnodes[parent].parent = 1;
    tnodes[parent].lnode  = start;
    tnodes[parent].nchild = 1;
  }else{
    /* > 1 lhs children */
    tnodes[parent].lnode  = start + ((parent-start)/2);
    tnodes[parent].nchild = 1;
    tnodes[parent].parent = 1;
    tnodes[tnodes[parent].lnode].pnode  = parent;
    tnodes[tnodes[parent].lnode].parent = 1;
    recur(tnodes, num_threads,
        start, parent-1,
        tnodes[parent].lnode);
  }

  /* rhs */
  if( (end-parent) == 0 ){
    /* no rhs children */
    return;
  }else if( (end-parent) == 1 ){
    /* 1 rhs child */
    tnodes[end].lnode     = -1;
    tnodes[end].rnode     = -1;
    tnodes[end].nchild    = 0;
    tnodes[end].pnode     = parent;
    tnodes[end].parent    = -1;
    tnodes[parent].parent = 1;
    tnodes[parent].rnode  = end;
    tnodes[parent].nchild = 2;
  }else{
    /* > 1 rhs children */
    tnodes[parent].rnode = ((end-parent)/2) + parent + 1;
    tnodes[parent].nchild = 2;
    tnodes[parent].parent = 1;
    tnodes[tnodes[parent].rnode].pnode  = parent;
    tnodes[tnodes[parent].rnode].parent = 1;
    recur(tnodes, num_threads,
        parent+1, end,
        tnodes[parent].rnode);
  }
}

/* ------------------------------------------------- INIT_TREE */
void init_tree( struct node *tnodes, int num_threads ){

  int root = 0;
  int i   = 0;

  /* init everything */
  for( i=0; i<num_threads; i++ ){
    tnodes[i].root    = -1;
    tnodes[i].parent  = -1;
    tnodes[i].pnode   = -1;
    tnodes[i].lnode   = -1;
    tnodes[i].rnode   = -1;
    tnodes[i].nchild  = -1;
    tnodes[i].arrived = 0;
    tnodes[i].mlock   = 0;
    tnodes[i].lock    = 0x5B5B0ull+(8*i);
  }

  /* if the node count is small, do it manually */
  if( num_threads == 2 ){
   tnodes[0].pnode  = 1;
   tnodes[1].root   = 1;
   tnodes[1].parent = 1;
   tnodes[1].lnode  = 0;
   tnodes[1].rnode  = -1;
   tnodes[1].nchild = 1;
   return ;
  }

  if( num_threads == 3 ){
   tnodes[0].pnode  = 1;
   tnodes[2].pnode  = 1;
   tnodes[1].root   = 1;
   tnodes[1].parent = 1;
   tnodes[1].lnode  = 0;
   tnodes[1].rnode  = 2;
   tnodes[1].nchild = 2;
   return ;
  }

  /* set the root */
  root = num_threads/2;
  tnodes[root].nchild = 2;
  tnodes[root].root = 1;
  tnodes[root].parent = 1;
  tnodes[root].pnode = -1;
  tnodes[root].lnode = root/2;
  tnodes[root].rnode = ((num_threads-1-root)/2) + root + 1;

  /* setup the lhs and rhs parents */
  tnodes[tnodes[root].lnode].pnode  = root;
  tnodes[tnodes[root].rnode].pnode  = root;

  /* initiate rhs and lhs */
  /* -- lhs */
  recur( tnodes, num_threads, 0, root-1, tnodes[root].lnode );

  if( (num_threads-1-root) == 1){
    /* one rhs node */
    tnodes[root+1].nchild = 0;
    tnodes[root+1].lnode  = -1;
    tnodes[root+1].rnode  = -1;
  }else{
    /* -- rhs */
    recur( tnodes, num_threads, root+1, num_threads-1, tnodes[root].rnode );
  }
}

/* ------------------------------------------------- FIND_MIN_CYCLE */
static uint64_t find_min_cycle( uint64_t *cycles, uint32_t num_threads ){
  uint32_t i = 0;
  uint64_t m = 0;

  for( i=1; i<num_threads; i++ ){
    if( cycles[i] < cycles[m] ){
      m = i;
    }
  }

  return cycles[m];
}

/* ------------------------------------------------- FIND_MAX_CYCLE */
static uint64_t find_max_cycle( uint64_t *cycles, uint32_t num_threads ){
  uint32_t i = 0;
  uint64_t m = 0;

  for( i=1; i<num_threads; i++ ){
    if( cycles[i] > cycles[m] ){
      m = i;
    }
  }

  return cycles[m];
}

/* ------------------------------------------------- FIND_AVG_CYCLE */
static double find_avg_cycle( uint64_t *cycles, uint32_t num_threads ){
  uint32_t i = 0;
  uint64_t m = 0x00ull;

  for( i=0; i<num_threads; i++ ){
    m += cycles[i];
  }

  return (double)(((double)(m)/(double)(num_threads)));
}

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

/* ------------------------------------------------- TRIGGER_RESPONSE */
static void trigger_mutex_response( hmc_response_t type,
                                    uint16_t tag,
                                    uint16_t *wtags,
                                    struct mylock *wlocks,
                                    uint64_t *status,
                                    int *wstatus,
                                    struct mylock lock,
                                    uint32_t num_threads,
                                    struct node *tnodes ){
  /* vars */
  uint32_t i    = 0;
  uint32_t th   = 0;
  int found     = -1;
  /* ---- */

  for( i=0; i<num_threads; i++ ){
    if( wstatus[i] != HMC_STALL ){
      if( wtags[i] == tag ){
        if( ( (status[i] == TAG_RXX_R)      && (type == RD_RS) ) ||
            ( (status[i] == TAG_INC_R)      && (type == WR_RS) ) ||
            ( (status[i] == TAG_RF_LOCAL_R) && (type == RD_RS) ) ||
            ( (status[i] == TAG_RD_LOCAL_R) && (type == RD_RS) )) {
          th = i;
          found = 1;
          goto complete_trigger;
        }
      }
    }
  }

complete_trigger:
  if( found == 1 ){
    switch( status[i] ){
    case TAG_INC_R:
      if( wstatus[th] == 0 ){
        /* arrived */
        wstatus[th] = 1;
      }else{
        /* cycle */
        wstatus[th] = 3;
      }
      break;
    case TAG_RXX_R:
    case TAG_RF_LOCAL_R:
    case TAG_RD_LOCAL_R:
      wstatus[th] = 1;
      break;
    default:
      printf( "ERROR: FATAL : COULD TRIGGER EVENT ON TAG  = %d; ABORTING\n", tag );
      abort();
      break;
    }/* -- end switch */
  }/* -- end if */
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

  uint64_t addr         = 0x3B5B0ull; /* SENSE */
  uint64_t sense        = 0;

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

  struct node *tnodes   = NULL;         /* tree nodes */
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
  tnodes  = malloc( sizeof( struct node ) * num_threads );
  for( i=0; i<num_threads; i++ ){
    status[i]   = TAG_START;
    cycles[i]   = 0x00ll;
    wstatus[i]  = -1;
    wtags[i]    = 0;
    wlocks[i].tid = (int64_t)(-1);
    wlocks[i].mlock= (uint64_t)(0);
  }

  /* init the tree */
  init_tree( tnodes, num_threads );

  /* temporary */
  //print_tree(tnodes, num_threads );
  //goto complete_failure;

  /* init the lock */
  lock.tid  = (int64_t)(-1);
  lock.mlock = (uint64_t)(0);

  /*
   * setup the tracing mechanisms
   *
   */
  printf( "INITIALIZING TRACE FILE\n" );
  ofile = fopen( "fe_linear.out", "w" );
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


  /* -- begin cycle loop */
  while( done < num_threads ){

    /* -- cycle through all the threads */
    for( i=0; i<num_threads; i++ ){

      /* -- if the thread is not done, try to perform an op */
      switch( status[i] ){

      case TAG_START:
        //printf( "TAG_START: THREAD %d\n", i );
        cycles[i]++;
        if( tnodes[i].parent == 1 ){
          /* parent node */
          status[i] = TAG_RD_LOCAL_S;
        }else{
          /* leaf node */
          status[i] = TAG_INC_S;
        }
        break;

      case TAG_RD_LOCAL_S:
        /* -- Build a ReadEF */
        //printf( "TAG_RD_LOCAL_S: THREAD %d\n", i );
        cycles[i]++;
        ret = hmcsim_build_memrequest( hmc,
                                       0,
                                       tnodes[i].lock,
                                       tag,
                                       CMC71,    /* READEF */
                                       link,
                                       &(payload[0]),
                                       &head,
                                       &tail);
        if( ret == 0 ){
          packet[0] = head;
          packet[1] = tail;
          ret = hmcsim_send( hmc, &(packet[0]) );
        }else{
          printf( "ERROR : FATAL : MALFORMED PACKET FROM THREAD %d\n", i );
        }
        switch( ret ){
        case 0:
          /* success */
          status[i]         = TAG_RD_LOCAL_R;
          tnodes[i].arrived = 1;  /* signal arrival */
          wtags[i]          = tag;
          wlocks[i].mlock   = 0;  /* old */
          wstatus[i]        = 0;  /* old */
          inc_tag( &tag );
          inc_link(hmc, &link);
          break;
        case HMC_STALL:
          /* stalled */
          wstatus[i]  = HMC_STALL;
          wtags[i]    = 0;
          /* drop to lock */
          status[i]   = TAG_RD_LOCAL_S;
          break;
        case -1:
        default:
          printf( "FAILED : INITIAL READEF PACKET SEND FAILURE\n" );
          goto complete_failure;
          break;
        }

        break;

      case TAG_RD_LOCAL_R:
        /* -- READEF Recv */
        //printf( "TAG_RD_LOCAL_S: THREAD %d\n", i );
        cycles[i]++;

        if( wstatus[i] == 1 ){
          if( tnodes[i].mlock == tnodes[i].nchild ){
            /* end while loop */
            if( tnodes[i].root == 1 ){
              /* i am a root node */
              if( sense == 1 ){
                status[i]       = TAG_WR_SENSE0_S;
                wlocks[i].mlock = 0;
                wtags[i]        = 0;
                wstatus[i]      = 0;
              }else{
                status[i]       = TAG_WR_SENSE1_S;
                wlocks[i].mlock = 0;
                wtags[i]        = 0;
                wstatus[i]      = 0;
              }
            }else{
              /* i am a parent, but not a root */
              //printf( "sending parent to INCFF : %d\n", i );
              status[i] = TAG_INC_S;
              wlocks[i].mlock = 0;
              wtags[i]        = 0;
              wstatus[i]      = 0;
            }
          }else{
            /* still waiting; goto ReadFF */
            //printf( "sending parent to READXX : %d\n", i );
            status[i]       = TAG_RF_LOCAL_S;
            wlocks[i].mlock = 0;
            wtags[i]        = 0;
            wstatus[i]      = 0;
          }
        }else{
          /* recv not arrived */
          status[i] = TAG_RD_LOCAL_R;
          wstatus[i]      = 0;
        }

        break;

      case TAG_RF_LOCAL_S:
        /* -- ReadFF Send */
        /* -- Build a ReadEF */
        cycles[i]++;
        ret = hmcsim_build_memrequest( hmc,
                                       0,
                                       tnodes[i].lock,
                                       tag,
                                       CMC73,    /* READFF */
                                       link,
                                       &(payload[0]),
                                       &head,
                                       &tail);
        if( ret == 0 ){
          packet[0] = head;
          packet[1] = tail;
          ret = hmcsim_send( hmc, &(packet[0]) );
        }else{
          printf( "ERROR : FATAL : MALFORMED PACKET FROM THREAD %d\n", i );
        }
        switch( ret ){
        case 0:
          /* success */
          status[i]         = TAG_RF_LOCAL_R;
          tnodes[i].arrived = 1;  /* signal arrival */
          wtags[i]          = tag;
          wlocks[i].mlock   = 0;  /* old */
          wstatus[i]        = 0;  /* old */
          inc_tag( &tag );
          inc_link(hmc, &link);
          break;
        case HMC_STALL:
          /* stalled */
          wstatus[i]  = HMC_STALL;
          wtags[i]    = 0;
          /* drop to lock */
          status[i]   = TAG_RF_LOCAL_S;
          break;
        case -1:
        default:
          printf( "FAILED : INITIAL READEF PACKET SEND FAILURE\n" );
          goto complete_failure;
          break;
        }

        break;
      case TAG_RF_LOCAL_R:
        /* -- ReadFF Recv */
        cycles[i]++;

        if( wstatus[i] == 1 ){
          if( tnodes[i].mlock == tnodes[i].nchild ){
            /* end while loop */
            if( tnodes[i].root == 1 ){
              /* i am a root node */
              if( sense == 1 ){
                status[i]       = TAG_WR_SENSE0_S;
                wlocks[i].mlock = 0;
                wtags[i]        = 0;
                wstatus[i]      = 0;
              }else{
                status[i]       = TAG_WR_SENSE1_S;
                wlocks[i].mlock = 0;
                wtags[i]        = 0;
                wstatus[i]      = 0;
              }
            }else{
              /* i am a parent, but not a root */
              //printf( "sending parent to INCFF %d\n", i );
              status[i] = TAG_INC_S;
              wlocks[i].mlock = 0;
              wtags[i]        = 0;
              wstatus[i]      = 0;
            }
          }else{
            /* still waiting; goto ReadFF */
            status[i]       = TAG_RF_LOCAL_S;
            wlocks[i].mlock = 0;
            wtags[i]        = 0;
            wstatus[i]      = 0;
          }
        }else{
          /* recv not arrived */
          status[i] = TAG_RD_LOCAL_R;
          wstatus[i]      = 0;
        }

        break;
      case TAG_WR_SENSE0_S:
        /* unused */
        break;
      case TAG_WR_SENSE0_R:
        /* unused */
        break;
      case TAG_WR_SENSE1_S:
        /* -- WriteXF(Sense,1) send */
        cycles[i]++;
        payload[0] = (uint64_t)(0x01);
        ret = hmcsim_build_memrequest( hmc,
                                       0,
                                       addr,
                                       tag,
                                       CMC78,    /* WriteXF */
                                       link,
                                       &(payload[0]),
                                       &head,
                                       &tail);
        if( ret == 0 ){
          packet[0] = head;
          packet[1] = 0x01ull;
          packet[2] = 0x00ull;
          packet[3] = tail;
          ret = hmcsim_send( hmc, &(packet[0]) );
        }else{
          printf( "ERROR : FATAL : MALFORMED PACKET FROM THREAD %d\n", i );
        }
        switch( ret ){
        case 0:
          /* success */
          status[i]         = TAG_WR_SENSE1_R;
          wtags[i]          = tag;
          wlocks[i].mlock   = 0;  /* old */
          wstatus[i]        = 0;  /* old */
          sense             = 1;  printf( "SETTING SENSE TO 1\n" );
          inc_tag( &tag );
          inc_link(hmc, &link);
          break;
        case HMC_STALL:
          /* stalled */
          wstatus[i]  = HMC_STALL;
          wtags[i]    = 0;
          /* drop to lock */
          status[i]   = TAG_WR_SENSE1_S;
          break;
        case -1:
        default:
          printf( "FAILED : INITIAL READEF PACKET SEND FAILURE\n" );
          goto complete_failure;
          break;
        }

        break;
      case TAG_WR_SENSE1_R:
        /* -- WriteXE(Sense,0) recv */
        /* ignore me... */
        cycles[i]++;
        status[i] = TAG_DONE;
        done++;
        break;
      case TAG_INC_S:
        //printf( "TAG_INC_S: THREAD %d\n", i );
        cycles[i]++;
        payload[0] = (uint64_t)(0x01);
        ret = hmcsim_build_memrequest( hmc,
                                       0,
                                       addr,
                                       tag,
                                       CMC70,    /* IncFF */
                                       link,
                                       &(payload[0]),
                                       &head,
                                       &tail);
        if( ret == 0 ){
          packet[0] = head;
          packet[1] = 0x01ull;
          packet[2] = 0x00ull;
          packet[3] = tail;
          ret = hmcsim_send( hmc, &(packet[0]) );
        }else{
          printf( "ERROR : FATAL : MALFORMED PACKET FROM THREAD %d\n", i );
        }
        switch( ret ){
        case 0:
          /* success */
          if( tnodes[tnodes[i].pnode].arrived == 1 ){
            /* parent has arrived; inc the lock */
            tnodes[tnodes[i].pnode].mlock++;
            status[i]         = TAG_INC_R;
            wtags[i]          = tag;
            wlocks[i].mlock   = 0;  /* old */
            wstatus[i]        = 0;  /* old */
          }else{
            /* parent has not arrived, cycle */
            status[i]         = TAG_INC_R;
            wtags[i]          = tag;
            wlocks[i].mlock   = 0;  /* old */
            wstatus[i]        = -1;  /* old */
          }
          inc_tag( &tag );
          inc_link(hmc, &link);
          break;
        case HMC_STALL:
          /* stalled */
          wstatus[i]  = HMC_STALL;
          wtags[i]    = 0;
          /* drop to lock */
          status[i]   = TAG_INC_S;
          break;
        case -1:
        default:
          printf( "FAILED : INITIAL READEF PACKET SEND FAILURE\n" );
          goto complete_failure;
          break;
        }

        break;
      case TAG_INC_R:
        /* -- IncFF Recv */
        //printf( "TAG_INC_R: THREAD %d\n", i );
        cycles[i]++;
        if( wstatus[i] == 1 ){
          status[i]         = TAG_RXX_S;
          wlocks[i].mlock   = 0;  /* old */
          wstatus[i]        = 0;  /* old */
        }else if( wstatus[i] == 3 ){
          //printf( "parent not arrived: %d\n", i );
          status[i]         = TAG_INC_S;
          wlocks[i].mlock   = 0;  /* old */
          wstatus[i]        = 0;  /* old */
        }
        break;
      case TAG_RXX_S:
        /* -- ReadXX send */
        cycles[i]++;
        ret = hmcsim_build_memrequest( hmc,
                                       0,
                                       addr,
                                       tag,
                                       CMC74,    /* READXX */
                                       link,
                                       &(payload[0]),
                                       &head,
                                       &tail);
        if( ret == 0 ){
          packet[0] = head;
          packet[1] = tail;
          ret = hmcsim_send( hmc, &(packet[0]) );
        }else{
          printf( "ERROR : FATAL : MALFORMED PACKET FROM THREAD %d\n", i );
        }
        switch( ret ){
        case 0:
          /* success */
          status[i]         = TAG_RXX_R;
          wtags[i]          = tag;
          wlocks[i].mlock   = 0;  /* old */
          wstatus[i]        = 0;  /* old */
          inc_tag( &tag );
          inc_link(hmc, &link);
          break;
        case HMC_STALL:
          /* stalled */
          wstatus[i]  = HMC_STALL;
          wtags[i]    = 0;
          /* drop to lock */
          status[i]   = TAG_RXX_S;
          break;
        case -1:
        default:
          printf( "FAILED : INITIAL READEF PACKET SEND FAILURE\n" );
          goto complete_failure;
          break;
        }

        break;
      case TAG_RXX_R:
        //printf( "checking sense from thread %d\n", i );
        cycles[i]++;
        if( wstatus[i] == 1 ){
          /* recv processed */
          if( sense == 1 ){
            done++;
            status[i] = TAG_DONE;
          }else{
            status[i] = TAG_RXX_S;
            wstatus[i] = 0;
          }
        }else{
          status[i] = TAG_RXX_R;
        }
        break;

      case TAG_DONE:
        /* thread is done, do nothing */
        break;
      default:
        /* error */
        break;

      }/*switch( status[i] )*/

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
                                  num_threads,
                                  tnodes );

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
  free( tnodes );

  return 0;
}


/* EOF */
