/*
 * _HMC_IO_C_
 *
 * HYBRID MEMORY CUBE SIMULATION LIBRARY
 *
 * IO FUNCTIONS
 *
 * COPYRIGHT (c) 2018 TEXAS TECH UNIVERSITY
 * SEE LICENSE FOR DETAILS
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "hmc_sim.h"

/* ----------------------------------------------------- HMCSIM_READ_CMCREG */
extern int hmcsim_read_cmcreg( struct hmcsim_t *hmc,
                               uint32_t dev,
                               uint64_t idx,
                               uint64_t *data ){
  /* sanity check */
  if( hmc == NULL ){
    return -1;
  }else if( dev > (hmc->num_devs-1) ){
    return -1;
  }else if( idx > (HMC_NUM_CMC_REGS-1) ){
    return -1;
  }else if( data == NULL ){
    return -1;
  }

  *data = hmc->devs[dev].cmc_reg[idx];
  return 0;
}

/* ----------------------------------------------------- HMCSIM_WRITE_CMCREG */
extern int hmcsim_write_cmcreg( struct hmcsim_t *hmc,
                                uint32_t dev,
                                uint64_t idx,
                                uint64_t data ){
  /* sanity check */
  if( hmc == NULL ){
    return -1;
  }else if( dev > (hmc->num_devs-1) ){
    return -1;
  }else if( idx > (HMC_NUM_CMC_REGS-1) ){
    return -1;
  }

  hmc->devs[dev].cmc_reg[idx] = data;
  return 0;
}

/* ----------------------------------------------------- HMCSIM_READMEM */
extern int  hmcsim_readmem( struct hmcsim_t *hmc,
                            uint64_t addr,
                            uint64_t *data,
                            uint32_t len ){
#ifdef HMC_ALLOC_MEM
  uint64_t phys = 0x00ull;
  uint64_t i    = 0x00ull;
  uint64_t *ptr = hmc->__ptr_stor;

  phys  = (uint64_t)(ptr) + addr + (len*8);

  if( phys > (uint64_t)(hmc->__ptr_end) ){
    /* out of bounds */
    return -1;
  }else{
    /* set the physical address */
    ptr = (uint64_t *)(phys);
  }

  for( i=0; i<len; i++ ){
    data[i] = ptr[i];
  }

  return 0;

#else
  return 0;
#endif
}

/* ----------------------------------------------------- HMCSIM_WRITEMEM */
extern int  hmcsim_writemem( struct hmcsim_t *hmc,
                             uint64_t addr,
                             uint64_t *data,
                             uint32_t len ){
#ifdef HMC_ALLOC_MEM
  uint64_t phys = 0x00ull;
  uint64_t i    = 0x00ull;
  uint64_t *ptr = hmc->__ptr_stor;

  phys  = (uint64_t)(ptr) + addr + (len*8);

  if( phys > (uint64_t)(hmc->__ptr_end) ){
    /* out of bounds */
    return -1;
  }else{
    /* set the physical address */
    ptr = (uint64_t *)(phys);
  }

  for( i=0; i<len; i++ ){
    ptr[i]  = data[i];
  }

  return 0;

#else
  return 0;
#endif
}

/* EOF */
