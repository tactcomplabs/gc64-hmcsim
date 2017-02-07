/*
 * _HMC_TECPLOT_C_
 *
 * HYBRID MEMORY CUBE SIMULATION LIBRARY
 *
 * TECPLOT OUTPUT FUNCTIONS
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hmc_sim.h"

static int hmcsim_tecplot8( struct HMC8LinkTec Tec, uint64_t clock ){
  return 0;
}

static int hmcsim_tecplot4( struct HMC4LinkTec Tec, uint64_t clock ){
  return 0;
}

extern int hmcsim_tecplot( struct hmcsim_t *hmc ){

  if( hmc->num_links == 4 ){
    return hmcsim_tecplot4( hmc->power.H4L, hmc->clk );
  }else if( hmc->num_links == 8 ){
    return hmcsim_tecplot8( hmc->power.H8L, hmc->clk );
  }else{
    return -1;
  }

  return 0;
}
