/*
 * _SIMPLE8GB_CPP_
 *
 * HMC-Sim SIMPLE API 4GB test
 *
 */

#include <iostream>
#include "hmc_sim.h"

int main( int argc, char **argv ){

  /* variables */
  int ret = 0;
  struct hmcsim_t hmc;

  /* init the environment */
  ret = hmcsim_simple_init( &hmc, 8 );
  if( ret != 0 ){
    std::cout << "error : hmcsim_simple_init returned an error : " << ret << std::endl;
    return -1;
  }

  /* free the environment */
  ret = hmcsim_free( &hmc );
  if( ret != 0 ){
    std::cout << "error : hmcsim_free returned an error : " << ret << std::endl;
    return -1;
  }

  return 0;
}

/* EOF */
