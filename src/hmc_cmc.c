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


/* conversion table for cmc request enums, opcodes and struct indices */
struct cmc_table{
  hmc_rqst_t type;
  uint32_t cmd;
  uint32_t idx;
};

struct cmc_table ctable[HMC_MAX_CMC] = {

  {CMC01,1,0},
  {CMC02,2,1},
  {CMC03,3,2},
  {CMC04,4,3},
  {CMC05,5,4},
  {CMC06,6,5},
  {CMC07,7,6},
  {CMC20,20,7},
  {CMC21,21,8},
  {CMC22,22,9},
  {CMC23,23,10},
  {CMC32,32,11},
  {CMC36,36,12},
  {CMC37,37,13},
  {CMC38,38,14},
  {CMC39,39,15},
  {CMC41,41,16},
  {CMC42,42,17},
  {CMC43,43,18},
  {CMC44,44,19},
  {CMC45,45,20},
  {CMC46,46,21},
  {CMC47,47,22},
  {CMC56,56,23},
  {CMC57,57,24},
  {CMC58,58,25},
  {CMC59,59,26},
  {CMC60,60,27},
  {CMC61,61,28},
  {CMC62,62,29},
  {CMC63,63,30},
  {CMC69,69,31},
  {CMC70,70,32},
  {CMC71,71,33},
  {CMC72,72,34},
  {CMC73,73,35},
  {CMC74,74,36},
  {CMC75,75,37},
  {CMC76,76,38},
  {CMC77,77,39},
  {CMC78,78,40},
  {CMC85,85,41},
  {CMC86,86,42},
  {CMC87,87,43},
  {CMC88,88,44},
  {CMC89,89,45},
  {CMC90,90,46},
  {CMC91,91,47},
  {CMC92,92,48},
  {CMC93,93,49},
  {CMC94,94,50},
  {CMC102,102,51},
  {CMC103,103,52},
  {CMC107,107,53},
  {CMC108,108,54},
  {CMC109,109,55},
  {CMC110,110,56},
  {CMC111,111,57},
  {CMC112,112,58},
  {CMC113,113,59},
  {CMC114,114,60},
  {CMC115,115,61},
  {CMC116,116,62},
  {CMC117,117,63},
  {CMC118,118,64},
  {CMC120,120,65},
  {CMC121,121,66},
  {CMC122,122,67},
  {CMC123,123,68},
  {CMC124,124,69},
  {CMC125,125,70},
  {CMC126,126,71},
  {CMC127,127,72}

};

/* Prototypes of the library functions */
/* int hmcsim_register_cmc( hmc_cmcop_t, hmc_rqst_t, uint32_t cmd ) */

/* ----------------------------------------------------- HMCSIM_CMC_IDXTOCMD */
extern hmc_rqst_t hmcsim_cmc_idxtocmd( uint32_t idx ){
  return ctable[idx].type;
}

/* ----------------------------------------------------- HMCSIM_CMC_CMDTOIDX */
extern uint32_t hmcsim_cmc_cmdtoidx( hmc_rqst_t rqst ){
  uint32_t i = 0;

  for( i=0; i<HMC_MAX_CMC; i++ ){
    if( ctable[i].type == rqst ){
      return i;
    }
  }
  return HMC_MAX_CMC; /* redundant, but squashes gcc warning */
}

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
