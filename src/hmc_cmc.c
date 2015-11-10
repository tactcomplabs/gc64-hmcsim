/*
 * _HMC_CMC_C_
 *
 * Hybrid memory cube simulation library
 *
 * Custom memory cube functionality
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hmc_sim.h"

extern int      hmcsim_load_cmc( struct hmcsim_t *hmc, char *cmc_lib ){

  if( hmc == NULL ){
    return -1;
  }else if( cmc_lib == NULL ){
    return -1;
  }

  return 0;
}

/* EOF */
