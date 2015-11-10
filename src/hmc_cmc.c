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
#include <dlfcn.h>
#include "hmc_sim.h"

/* ----------------------------------------------------- HMCSIM_REGISTER_FUNCTIONS */
/*
 * HMCSIM_REGISTER_FUNCTIONS
 *
 */
static int    hmcsim_register_functions( struct hmcsim_t *hmc, char *cmc_lib ){

  /* vars */
  void *handle = NULL;
  /* ---- */

  /* attempt to load the library */
  handle = dlopen( cmc_lib, RTLD_NOW );

  if( handle == NULL ){
#ifdef HMC_DEBUG
    HMCSIM_PRINT_TRACE(dlerror());
#endif
    return -1;
  }

  /* library is loaded, resolve the functions */


  /* write the necessary references into the structure */

  return 0;
}

/* ----------------------------------------------------- HMCSIM_LOAD_CMC */
/*
 * HMCSIM_LOAD_CMC
 *
 */
extern int      hmcsim_load_cmc( struct hmcsim_t *hmc, char *cmc_lib ){

  if((hmc == NULL) || (cmc_lib == NULL)){
    return -1;
  }

  /* register the library functions */
  if( hmcsim_register_functions( hmc, cmc_lib ) != 0 ){
    return -1;
  }

  return 0;
}

/* EOF */
