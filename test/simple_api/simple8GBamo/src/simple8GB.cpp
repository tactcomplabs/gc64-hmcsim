/*
 * _SIMPLE8GB_CPP_
 *
 * HMC-Sim SIMPLE API 8GB test
 *
 */

#include <iostream>
#include <stdint.h>
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

  /* perform a simple amo with a write response */
  uint64_t addr = 0x00ull;
  uint8_t data[16];
  int token = -1;
  token = hmcsim_simple_amo( &hmc, addr, &(data[0]), TWOADD8 );
  if( token < 0 ){
    /* the request queue should not be full here */
    std::cout << "error : hmcsim_simple_amo (WR_RS) returned an error : " << token << std::endl;
    hmcsim_free( &hmc );
    return -1;
  }else{
    std::cout << "success : sent amo (WR_RS) request; token = " << token << std::endl;
  }

  /* wait for the read to return */
  bool rsp = false;
  while( !rsp ){
    hmcsim_clock(&hmc);
    ret = hmcsim_simple_stat( &hmc, token, &(data[0]) );
    if( ret == 1 ){
      std::cout << "success : hmcsim_simple_stat found our response" << std::endl;
      rsp = true;
    }else if( ret == -1 ){
      std::cout << "error : hmcsim_simple_stat returned an error : " << ret << std::endl;
      hmcsim_free( &hmc );
      return -1;
    }
  }

  /* perform a simple amo with a read response  */
  token = hmcsim_simple_amo( &hmc, addr, &(data[0]), XOR16 );
  if( token < 0 ){
    /* the request queue should not be full here */
    std::cout << "error : hmcsim_simple_amo (RD_RS) returned an error : " << token << std::endl;
    hmcsim_free( &hmc );
    return -1;
  }else{
    std::cout << "success : sent amo (RD_RS) request; token = " << token << std::endl;
  }

  /* wait for the write response */
  rsp = false;
  while( !rsp ){
    hmcsim_clock(&hmc);
    ret = hmcsim_simple_stat( &hmc, token, &(data[0]) );
    if( ret == 1 ){
      std::cout << "success : hmcsim_simple_stat found our response" << std::endl;
      rsp = true;
    }else if( ret == -1 ){
      std::cout << "error : hmcsim_simple_stat returned an error : " << ret << std::endl;
      hmcsim_free( &hmc );
      return -1;
    }
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
