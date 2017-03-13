/*
 * _HMC_SIMPLE_C_
 *
 * Hybrid Memory Cube Simplified API interfaces
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "hmc_sim.h"

/* -------------------------------------------- FUNCTION DEFINITIONS */
extern uint32_t hmcsim_cmc_cmdtoidx( hmc_rqst_t rqst );
extern int  hmcsim_query_cmc( struct hmcsim_t *hmc,
                              hmc_rqst_t type,
                              uint32_t *flits,
                              uint8_t *cmd );

/* -------------------------------------------- INCR_LINK */
static int incr_link( struct hmcsim_t *hmc ){
  hmc->simple_link++;
  if( hmc->simple_link == hmc->num_links ){
    hmc->simple_link = 0;
  }
  return 0;
}

/* -------------------------------------------- FIND_VALID_TOKEN */
static int find_valid_token( struct hmcsim_t *hmc ){
  int i = 0;

  for( i=0; i<1023; i++ ){
    if( hmc->tokens[i].status == 0 ){
      return i;
    }
  }
  return -1;
}

/* -------------------------------------------- HMCSIM_SIMPLE_INIT */
extern int hmcsim_simple_init( struct hmcsim_t *hmc, int size ){
  int i = 0;

  if( hmc == NULL ){
    return -1;
  }

  if( size == 4 ){
#ifdef HMC_DEBUG
    HMCSIM_PRINT_TRACE( "INITIALIZING 4GB DEVICE" );
#endif
    if( hmcsim_init( hmc, 1, 4, 32, 64, 16, 20, 4, 128 ) != 0 ){
      return -1;
    }
#ifdef HMC_DEBUG
    HMCSIM_PRINT_TRACE( "SUCCESS INITIALIZING 4GB DEVICE" );
    HMCSIM_PRINT_TRACE( "INITIALIZING 4GB LINKS" );
#endif
    for( i=0; i<4; i++ ){
      if( hmcsim_link_config( hmc, 2, 0, i, i, HMC_LINK_HOST_DEV ) != 0 ){
        return -1;
      }
    }
#ifdef HMC_DEBUG
    HMCSIM_PRINT_TRACE( "SUCCESS INITIALIZING 4GB LINKS" );
    HMCSIM_PRINT_TRACE( "INITIALIZING 4GB MAX BLOCKSIZE" );
#endif
    if( hmcsim_util_set_all_max_blocksize( hmc, 256 ) != 0 ){
      return -1;
    }
#ifdef HMC_DEBUG
    HMCSIM_PRINT_TRACE( "SUCCESS INITIALIZING 4GB MAX BLOCKSIZE" );
#endif
  }else if( size == 8 ){
#ifdef HMC_DEBUG
    HMCSIM_PRINT_TRACE( "INITIALIZING 8GB DEVICE" );
#endif
    if( hmcsim_init( hmc, 1, 8, 64, 64, 16, 20, 8, 128 ) != 0 ){
      return -1;
    }
#ifdef HMC_DEBUG
    HMCSIM_PRINT_TRACE( "SUCCESS INITIALIZING 8GB DEVICE" );
    HMCSIM_PRINT_TRACE( "INITIALIZING 8GB LINKS" );
#endif
    for( i=0; i<8; i++ ){
      if( hmcsim_link_config( hmc, 2, 0, i, i, HMC_LINK_HOST_DEV ) != 0 ){
        return -1;
      }
    }
#ifdef HMC_DEBUG
    HMCSIM_PRINT_TRACE( "SUCCESS INITIALIZING 8GB LINKS" );
    HMCSIM_PRINT_TRACE( "INITIALIZING 8GB MAX BLOCKSIZE" );
#endif
    if( hmcsim_util_set_all_max_blocksize( hmc, 256 ) != 0 ){
      return -1;
    }
#ifdef HMC_DEBUG
    HMCSIM_PRINT_TRACE( "SUCCESS INITIALIZING 8GB MAX BLOCKSIZE" );
#endif
  }else{
    return -1;
  }

  return 0;
}

/* -------------------------------------------- HMCSIM_SIMPLE_READ */
extern int hmcsim_simple_read( struct hmcsim_t *hmc, uint64_t addr, int size ){
  int token = -1;
  int ret = 0;
  uint64_t head;
  uint64_t tail;
  uint64_t packet[2];
  hmc_rqst_t type;

  if( hmc == NULL ){
    return -2;
  }

  /* grab a new token */
  token = find_valid_token( hmc );
  if( token == -1 ){
    return -1;
  }

  /* we have a valid token, try to create a request */
  switch( size ){
  case 16:
    type = RD16;
    break;
  case 32:
    type = RD32;
    break;
  case 48:
    type = RD48;
    break;
  case 64:
    type = RD64;
    break;
  case 80:
    type = RD80;
    break;
  case 96:
    type = RD96;
    break;
  case 112:
    type = RD112;
    break;
  case 128:
    type = RD128;
    break;
  case 256:
    type = RD256;
    break;
  default:
    /* invalid packet size */
    return -2;
    break;
  }

  /* create the packet request */
  if( hmcsim_build_memrequest( hmc, 0, addr, (uint16_t)(token), type,
                               hmc->simple_link, NULL, &head, &tail ) != 0 ){
    return -2;
  }
  packet[0] = head;
  packet[1] = tail;

  /* increment the link */
  incr_link(hmc);

  /* try to send the packet */
  ret = hmcsim_send( hmc, &(packet[0]) );
  if( ret == 0 ){
    hmc->tokens[token].status = 1;
    hmc->tokens[token].rsp = RD_RS;
    hmc->tokens[token].rsp_size = size;
    return token;
  }else if( ret == HMC_STALL ){
    return -1;
  }else{
    return -2;
  }
}

/* -------------------------------------------- HMCSIM_SIMPLE_AMO */
extern int hmcsim_simple_amo( struct hmcsim_t *hmc,
                              uint64_t addr,
                              uint8_t *data,
                              hmc_rqst_t op ){
  int token = -1;
  int ret = 0;
  int i = 0;
  int cur = 0;
  int shift = 0;
  int tailp = 0;
  uint32_t size = 0;
  uint32_t rsp_size = 0;
  uint64_t head;
  uint64_t tail;
  uint64_t payload[32];
  uint64_t packet[HMC_MAX_UQ_PACKET];
  hmc_response_t rsp;

  if( hmc == NULL ){
    return -2;
  }
  if( data == NULL ){
    return -2;
  }

  /* grab a new token */
  token = find_valid_token( hmc );
  if( token == -1 ){
    return -1;
  }

  /* setup the packet data */
  switch( op ){
  case TWOADD8:
    rsp = WR_RS;
    rsp_size = 0;
    size = 16;
    break;
  case ADD16:
    rsp = WR_RS;
    rsp_size = 0;
    size = 16;
    break;
  case P_2ADD8:
    rsp = RSP_NONE;
    rsp_size = 0;
    size = 16;
    break;
  case P_ADD16:
    rsp = RSP_NONE;
    rsp_size = 0;
    size = 16;
    break;
  case TWOADDS8R:
    rsp = RD_RS;
    rsp_size = 16;
    size = 16;
    break;
  case ADDS16R:
    rsp = RD_RS;
    rsp_size = 16;
    size = 16;
    break;
  case INC8:
    rsp = WR_RS;
    rsp_size = 0;
    size = 0;
    break;
  case P_INC8:
    rsp = RSP_NONE;
    rsp_size = 0;
    size = 0;
    break;
  case XOR16:
    rsp = RD_RS;
    rsp_size = 16;
    size = 16;
    break;
  case OR16:
    rsp = RD_RS;
    rsp_size = 16;
    size = 16;
    break;
  case NOR16:
    rsp = RD_RS;
    rsp_size = 16;
    size = 16;
    break;
  case AND16:
    rsp = RD_RS;
    rsp_size = 16;
    size = 16;
    break;
  case NAND16:
    rsp = RD_RS;
    rsp_size = 16;
    size = 16;
    break;
  case CASGT8:
    rsp = RD_RS;
    rsp_size = 16;
    size = 16;
    break;
  case CASGT16:
    rsp = RD_RS;
    rsp_size = 16;
    size = 16;
    break;
  case CASLT8:
    rsp = RD_RS;
    rsp_size = 16;
    size = 16;
    break;
  case CASLT16:
    rsp = RD_RS;
    rsp_size = 16;
    size = 16;
    break;
  case CASEQ8:
    rsp = RD_RS;
    rsp_size = 16;
    size = 16;
    break;
  case CASZERO16:
    rsp = RD_RS;
    rsp_size = 16;
    size = 16;
    break;
  case EQ8:
    rsp = WR_RS;
    rsp_size = 0;
    size = 16;
    break;
  case EQ16:
    rsp = WR_RS;
    rsp_size = 0;
    size = 16;
    break;
  case BWR8R:
    rsp = RD_RS;
    rsp_size = 16;
    size = 16;
    break;
  case SWAP16:
    rsp = RD_RS;
    rsp_size = 16;
    size = 16;
    break;
  case BWR:
    rsp = WR_RS;
    rsp_size = 0;
    size = 16;
    break;
  case P_BWR:
    rsp = RSP_NONE;
    rsp_size = 0;
    size = 16;
    break;
  default:
    /* erroneous packet request */
    return -2;
  }

  /* copy the data to the uint64_t payload array */
  do{
    payload[cur] |= ((uint64_t)(data[i])<<(shift*8));
    i++;
    shift++;
    if( shift == 8 ){
      shift = 0;
      cur++;
    }
  }while(i<size);

  /* create the packet request */
  if( hmcsim_build_memrequest( hmc, 0, addr, (uint16_t)(token), op,
                               hmc->simple_link, &(payload[0]), &head, &tail ) != 0 ){
    return -2;
  }
  packet[0] = head;
  packet[tailp] = tail;

  /* increment the link */
  incr_link(hmc);

  /* try to send the packet */
  ret = hmcsim_send( hmc, &(packet[0]) );
  if( ret == 0 ){
    hmc->tokens[token].status = 1;
    hmc->tokens[token].rsp = rsp;
    hmc->tokens[token].rsp_size = rsp_size;
    return token;
  }else if( ret == HMC_STALL ){
    return -1;
  }else{
    return -2;
  }
}

/* -------------------------------------------- HMCSIM_SIMPLE_CMC */
extern int hmcsim_simple_cmc( struct hmcsim_t *hmc,
                              uint64_t addr,
                              uint8_t *data,
                              hmc_rqst_t op ){
  int token = -1;
  int ret = 0;
  int i = 0;
  int cur = 0;
  int shift = 0;
  int tailp = 0;
  uint32_t size = 0;
  uint32_t flits = 0;
  uint8_t cmd = 0;
  uint64_t head;
  uint64_t tail;
  uint64_t payload[32];
  uint64_t packet[HMC_MAX_UQ_PACKET];

  if( hmc == NULL ){
    return -2;
  }
  if( data == NULL ){
    return -2;
  }

  /* grab a new token */
  token = find_valid_token( hmc );
  if( token == -1 ){
    return -1;
  }

  /* query the cmc command */
  if( hmcsim_query_cmc( hmc, op, &flits, &cmd ) != 0 ){
    /* erroneous command */
    return -2;
  }

  /* derive the tail packet location */
  tailp = (flits*2)-1;

  /* derive the size */
  size = (flits*16)-16;

  /* copy the data to the uint64_t payload array */
  if( size > 0 ){
    do{
      payload[cur] |= ((uint64_t)(data[i])<<(shift*8));
      i++;
      shift++;
      if( shift == 8 ){
        shift = 0;
        cur++;
      }
    }while(i<size);
  }

  /* create the packet request */
  if( hmcsim_build_memrequest( hmc, 0, addr, (uint16_t)(token), op,
                               hmc->simple_link, &(payload[0]), &head, &tail ) != 0 ){
    return -2;
  }
  packet[0] = head;
  packet[tailp] = tail;

  /* increment the link */
  incr_link(hmc);

  /* try to send the packet */
  ret = hmcsim_send( hmc, &(packet[0]) );
  if( ret == 0 ){
    hmc->tokens[token].rsp = hmc->cmcs[hmcsim_cmc_cmdtoidx(op)].rsp_cmd;
    hmc->tokens[token].rsp_size = ((hmc->cmcs[hmcsim_cmc_cmdtoidx(op)].rsp_len*16)-16);
    hmc->tokens[token].status = 1;
    return token;
  }else if( ret == HMC_STALL ){
    return -1;
  }else{
    return -2;
  }
}

/* -------------------------------------------- HMCSIM_SIMPLE_WRITE */
extern int hmcsim_simple_write( struct hmcsim_t *hmc, uint64_t addr,
                                int size, uint8_t *data ){
  int token = -1;
  int ret = 0;
  int i = 0;
  int cur = 0;
  int shift = 0;
  int tailp = 0;
  uint64_t head;
  uint64_t tail;
  uint64_t payload[32];
  uint64_t packet[HMC_MAX_UQ_PACKET];
  hmc_rqst_t type;

  if( hmc == NULL ){
    return -2;
  }
  if( data == NULL ){
    return -2;
  }

  /* grab a new token */
  token = find_valid_token( hmc );
  if( token == -1 ){
    return -1;
  }

  /* we have a valid token, try to create a request */
  switch( size ){
  case 16:
    type = WR16;
    tailp = 3;
    break;
  case 32:
    type = WR32;
    tailp = 5;
    break;
  case 48:
    type = WR48;
    tailp = 7;
    break;
  case 64:
    type = WR64;
    tailp = 9;
    break;
  case 80:
    type = WR80;
    tailp = 11;
    break;
  case 96:
    type = WR96;
    tailp = 13;
    break;
  case 112:
    type = WR112;
    tailp = 15;
    break;
  case 128:
    type = WR128;
    tailp = 17;
    break;
  case 256:
    type = WR256;
    tailp = 33;
    break;
  default:
    /* invalid packet size */
    return -2;
    break;
  }

  /* copy the data to the uint64_t payload array */
  do{
    payload[cur] |= ((uint64_t)(data[i])<<(shift*8));
    i++;
    shift++;
    if( shift == 8 ){
      shift = 0;
      cur++;
    }
  }while(i<size);

  /* create the packet request */
  if( hmcsim_build_memrequest( hmc, 0, addr, (uint16_t)(token), type,
                               hmc->simple_link, &(payload[0]), &head, &tail ) != 0 ){
    return -2;
  }
  packet[0] = head;
  packet[tailp] = tail;

  /* increment the link */
  incr_link(hmc);

  /* try to send the packet */
  ret = hmcsim_send( hmc, &(packet[0]) );
  if( ret == 0 ){
    hmc->tokens[token].status = 1;
    hmc->tokens[token].rsp = WR_RS;
    hmc->tokens[token].rsp_size = 0;
    return token;
  }else if( ret == HMC_STALL ){
    return -1;
  }else{
    return -2;
  }
}

/* -------------------------------------------- HMCSIM_SIMPLE_STAT */
extern int hmcsim_simple_stat( struct hmcsim_t *hmc, int token, uint8_t *data ){
  int i = 0;

  /* sanity check */
  if( hmc == NULL ){
    return -1;
  }
  if( (token < 0) || (token > 1023) ){
    return -1;
  }
  if( hmc->tokens[token].status == 0 ){
    return -1;
  }else if( hmc->tokens[token].status == 1 ){
    return 0;
  }

  /* response is ready */
  if( hmc->tokens[token].rsp == RD_RS ){
    if( data == NULL ){
      /* nowehere to put the response data */
      return -1;
    }
    for( i=0; i<hmc->tokens[token].rsp_size; i++ ){
      data[i] = hmc->tokens[token].data[i];
    }
  }/* else, no response data is required */

  /* clear the token */
  hmc->tokens[token].status = 0;
  hmc->tokens[token].rsp = RSP_NONE;
  hmc->tokens[token].rsp_size = 0;
  hmc->tokens[token].en_clock = 0x00ull;
  for( i=0; i<256; i++ ){
    hmc->tokens[token].data[i] = 0x0;
  }

  /* clear the xbar response slot */
  hmc->devs[hmc->tokens[token].device].xbar[hmc->tokens[token].link].xbar_rsp[hmc->tokens[token].slot].valid = HMC_RQST_INVALID;
  for( i=0; i<HMC_MAX_UQ_PACKET; i++ ){
    hmc->devs[hmc->tokens[token].device].xbar[hmc->tokens[token].link].xbar_rsp[hmc->tokens[token].slot].packet[i] = 0x00ull;
  }

  return 1;
}

/* EOF */
