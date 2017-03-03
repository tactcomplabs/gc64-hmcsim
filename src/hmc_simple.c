/*
 * _HMC_SIMPLE_C_
 *
 * Hybrid Memory Cube Simplified API interfaces
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "hmc_sim.h"

extern int hmcsim_simple_init( struct hmcsim_t *hmc, int size ){
  int i = 0;

  if( hmc == NULL ){
    return -1;
  }

  if( size == 4 ){
    if( hmcsim_init( hmc, 1, 4, 32, 64, 16, 20, 4, 128 ) != 0 ){
      return -1;
    }
    for( i=0; i<4; i++ ){
      if( hmcsim_link_config( hmc, 2, 0, i, i, HMC_LINK_HOST_DEV ) != 0 ){
        return -1;
      }
    }
    if( hmcsim_util_set_all_max_blocksize( hmc, 256 ) != 0 ){
      return -1;
    }
  }else if( size = 8 ){
    if( hmcsim_init( hmc, 1, 8, 64, 64, 16, 20, 8, 128 ) != 0 ){
      return -1;
    }
    for( i=0; i<8; i++ ){
      if( hmcsim_link_config( hmc, 2, 0, i, i, HMC_LINK_HOST_DEV ) != 0 ){
        return -1;
      }
    }
    if( hmcsim_util_set_all_max_blocksize( hmc, 256 ) != 0 ){
      return -1;
    }
  }else{
    return -1;
  }

  return 0;
}

extern int hmcsim_simple_read( struct hmcsim_t *hmc, uint64_t addr, int size ){
  if( hmc == NULL ){
    return -1;
  }
}

extern int hmcsim_simple_write( struct hmcsim_t *hmc, uint64_t addr, int size ){
  if( hmc == NULL ){
    return -1;
  }
}

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
  }

  /* clear the token */
  hmc->tokens[token].status = 0;
  hmc->tokens[token].rsp = RSP_NONE;
  hmc->tokens[token].rsp_size = 0;
  hmc->tokens[token].en_clock = 0x00ull;
  for( i=0; i<256; i++ ){
    hmc->tokens[token].data[i] = 0x0;
  }

  return 1;
}

/* EOF */
