/*
 * _HMC_CONFIG_C_
 *
 * Reads the HMC configuration data from a file
 *
 * COPYRIGHT (c) 2018 TEXAS TECH UNIVERSITY
 * SEE LICENSE FOR DETAILS
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "hmc_sim.h"

/* ----------------------------------------------------- FUNCTION_PROTOTYPES */
extern int hmcsim_lex_config( struct hmcsim_t *hmc, char *config );

/* ----------------------------------------------------- HMCSIM_READ_CONFIG */
extern int hmcsim_read_config( struct hmcsim_t *hmc, char *config ){

  if( hmc == NULL ){
    return -1;
  }else if( config == NULL ){
    return -1;
  }

  return hmcsim_lex_config( hmc, config );
}

/* EOF */

