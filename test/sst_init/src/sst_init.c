/*
 * _SST_INIT_C_
 *
 * HMCSIM SST_INIT TEST DRIVER
 *
 * Attempts to initialize a HMC instantiation
 * using the default SST init values
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include "hmc_sim.h"

/* ------------------------------------------------ MAIN */
int main( int argc, char **argv ){
  /* vars */
  int ret                 = 0;
  uint32_t num_devs       = 1;
  uint32_t num_links      = HMC_MIN_LINKS;
  uint32_t num_vaults     = HMC_MIN_VAULTS;
  uint32_t queue_depth    = 32;
  uint32_t num_banks      = HMC_MIN_BANKS;
  uint32_t num_drams      = HMC_MIN_DRAMS;
  uint32_t capacity       = HMC_MIN_CAPACITY;
  uint32_t xbar_depth     = 4;
  struct hmcsim_t hmc;
  /* ---- */

  ret = hmcsim_init( &hmc,
                     num_devs,
                     num_links,
                     num_vaults,
                     queue_depth,
                     num_banks,
                     num_drams,
                     capacity,
                     xbar_depth );
  if( ret != 0 ){
    printf( "ERROR : hmcsim_init() failed with error code : %d\n", ret );
    return -1;
  }else{
    printf( "hmcsim_init() was successful\n" );
    hmcsim_free( &hmc );
  }

  return 0;
}
