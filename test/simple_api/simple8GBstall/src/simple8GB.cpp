/*
 * _SIMPLE8GB_CPP_
 *
 * HMC-Sim SIMPLE API 8GB test
 *
 */

#include <iostream>
#include <stdint.h>
#include <vector>
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

  /* perform a simple 16byte read */
  uint64_t addr = 0x00ull;
  uint8_t data[16];
  int token = -1;
  int total = 0;
  bool stalled = false;
  std::vector<int> tokens;

  do{
    token = hmcsim_simple_read( &hmc, addr, 16 );
    if( token < -1 ){
      /* error */
      std::cout << "error : hmcsim_simple_read returned as error : " << token << std::endl;
    }else if( token == -1 ){
      /* stalled */
      std::cout << "hmcsim_simple_read returned a stall signal " << std::endl;
      stalled = true;
    }else{
      std::cout << "token response = " << token << std::endl;
      tokens.push_back(token);
      total++;
    }
    addr+=64;
    hmcsim_clock(&hmc);
  }while( !stalled );

  std::cout << "Initiated " << total << " read packets" << std::endl;

  bool done = false;
  int j = 0;

  while( !done ){
    ret = hmcsim_simple_stat( &hmc, tokens[j], &(data[0]) );
    if( ret == 1 ){
      std::cout << "found response for token " << tokens[j] << std::endl;
      j++;
    }else if( ret == -1 ){
      std::cout << "error : hmcsim_simple_stat returned an error: token=" <<
        tokens[j] << std::endl;
      hmcsim_free(&hmc);
      return -1;
    }
    if( j==total ){
      done = true;
    }
    hmcsim_clock(&hmc);
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
