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
#include <inttypes.h>
#include "hmc_sim.h"

static int hmcsim_tecplot8( struct HMC8LinkTec Tec,
                            uint64_t clock,
                            char *prefix ){
  /* vars */
  char *fname_p = NULL;
  char *fname_t = NULL;
  FILE *ofile   = NULL;
  /* ---- */

  /* -- build the file names */
  fname_p = malloc( sizeof( char ) * (strlen(prefix)+40) );
  if( fname_p == NULL ){
    return -1;
  }
  fname_t = malloc( sizeof( char ) * (strlen(prefix)+40) );
  if( fname_t == NULL ){
    return -1;
  }

  sprintf( fname_p, "%s%s%"PRIu64"%s", prefix, "-power.", clock, ".tec" );
  sprintf( fname_t, "%s%s%"PRIu64"%s", prefix, "-therm.", clock, ".tec" );

  /* -- write the power results */
  /* -- open the file */
  ofile = fopen( fname_p, "w" );

  /* -- write all the data */

  /* -- close the file */
  fclose(ofile);
  ofile = NULL;

  /* -- write the thermal results */
  /* -- open the file */
  ofile = fopen( fname_t, "w" );

  /* -- write all the data */

  /* -- close the file */
  fclose( ofile );
  ofile = NULL;

  return 0;
}

static int hmcsim_tecplot4( struct HMC4LinkTec Tec,
                            uint64_t clock,
                            char *prefix ){
  /* vars */
  char *fname_p = NULL;
  char *fname_t = NULL;
  FILE *ofile   = NULL;
  /* ---- */

  /* -- build the file names */
  fname_p = malloc( sizeof( char ) * (strlen(prefix)+40) );
  if( fname_p == NULL ){
    return -1;
  }
  fname_t = malloc( sizeof( char ) * (strlen(prefix)+40) );
  if( fname_t == NULL ){
    return -1;
  }

  sprintf( fname_p, "%s%s%"PRIu64"%s", prefix, "-power.", clock, ".tec" );
  sprintf( fname_t, "%s%s%"PRIu64"%s", prefix, "-therm.", clock, ".tec" );

  /* -- write the power results */
  /* -- open the file */
  ofile = fopen( fname_p, "w" );

  /* -- write all the data */

  /* -- close the file */
  fclose(ofile);
  ofile = NULL;

  /* -- write the thermal results */
  /* -- open the file */
  ofile = fopen( fname_t, "w" );

  /* -- write all the data */

  /* -- close the file */
  fclose( ofile );
  ofile = NULL;

  return 0;
}

extern int hmcsim_tecplot( struct hmcsim_t *hmc ){

  if( hmc->num_links == 4 ){
    return hmcsim_tecplot4( hmc->power.H4L, hmc->clk, hmc->power.prefix );
  }else if( hmc->num_links == 8 ){
    return hmcsim_tecplot8( hmc->power.H8L, hmc->clk, hmc->power.prefix );
  }else{
    return -1;
  }

  return 0;
}
