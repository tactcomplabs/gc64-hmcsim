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
  int i         = 0;
  int j         = 0;
  int cur       = 0;
  /* ---- */

  /* -- build the file names */
  fname_p = malloc( sizeof( char ) * (strlen(prefix)+40) );
  if( fname_p == NULL ){
    return -1;
  }
  fname_t = malloc( sizeof( char ) * (strlen(prefix)+40) );
  if( fname_t == NULL ){
    free( fname_p );
    fname_p = NULL;
    return -1;
  }

  sprintf( fname_p, "%s%s%"PRIu64"%s", prefix, "-power.", clock, ".tec" );
  sprintf( fname_t, "%s%s%"PRIu64"%s", prefix, "-therm.", clock, ".tec" );

  /* -- write the power results */
  /* -- open the file */
  ofile = fopen( fname_p, "w" );

  /* -- write all the data */

  /* -- -- header */
  fprintf( ofile, "%s%"PRIu64"%s\n",
           "TITLE = \"HMC 8Link Simulation Clock ", clock, "\"" );
  fprintf( ofile, "%s\n", "VARIABLES = \"X\", \"Y\", \"Z\", \"Power\"" );
  fprintf( ofile, "%s\n", "ZONE T=\"HMC\", I=144, J=144, F=POINT" );
  fprintf( ofile, "\n\n" );

  /* -- -- link+quad 0,1 */
  for( i=0; i<2; i++ ){
    /* X Y Z Data */
    fprintf( ofile, "%s%f%s%f\n",
             "18 4.5 ", ((float)(i)*(float)(36.0))+(float)(18.0), " ",
             Tec.link_phy_power[i] );
    fprintf( ofile, "%s%f%s%f\n",
             "18 13.5 ", ((float)(i)*(float)(36.0))+(float)(18.0), " ",
             Tec.xbar_rqst_power[i] );
    fprintf( ofile, "%s%f%s%f\n",
             "18 22.5 ", ((float)(i)*(float)(36.0))+(float)(18.0), " ",
             Tec.xbar_rsp_power[i] );
    fprintf( ofile, "%s%f%s%f\n",
             "18 27 ", ((float)(i)*(float)(36.0))+(float)(18.0), " ",
             Tec.link_local_route_power[i] );
    fprintf( ofile, "%s%f%s%f\n",
             "18 31.5 ", ((float)(i)*(float)(36.0))+(float)(18.0), " ",
             Tec.link_remote_route_power[i] );

    /* vault data */
    for( j=0; j<2; j++ ){
      fprintf( ofile, "%s%f%s%f\n",
               "4.5 40.5 ",
               ((float)(i)*(float)(36.0)) + ((float)(j)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_rqst_power[cur] );
      fprintf( ofile, "%s%f%s%f\n",
               "4.5 49.5 ",
               ((float)(i)*(float)(36.0)) + ((float)(j)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_rsp_power[cur] );
      fprintf( ofile, "%s%f%s%f\n",
               "4.5 58.5 ",
               ((float)(i)*(float)(36.0)) + ((float)(j)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_ctrl_power[cur] );
      cur++;
    }
    for( j=2; j<4; j++ ){
      fprintf( ofile, "%s%f%s%f\n",
               "13.5 40.5 ",
               ((float)(i)*(float)(36.0)) + ((float)(j-2)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_rqst_power[cur] );
      fprintf( ofile, "%s%f%s%f\n",
               "13.5 49.5 ",
               ((float)(i)*(float)(36.0)) + ((float)(j-2)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_rsp_power[cur] );
      fprintf( ofile, "%s%f%s%f\n",
               "13.5 58.5 ",
               ((float)(i)*(float)(36.0)) + ((float)(j-2)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_ctrl_power[cur] );
      cur++;
    }
  }

  /* -- -- link+quad 2,3 */
  for( i=2; i<4; i++ ){
    fprintf( ofile, "%s%f%s%f\n",
             "54 4.5 ", ((float)(i-2)*(float)(36.0))+(float)(18.0), " ",
             Tec.link_phy_power[i] );
    fprintf( ofile, "%s%f%s%f\n",
             "54 13.5 ", ((float)(i-2)*(float)(36.0))+(float)(18.0), " ",
             Tec.xbar_rqst_power[i] );
    fprintf( ofile, "%s%f%s%f\n",
             "54 22.5 ", ((float)(i-2)*(float)(36.0))+(float)(18.0), " ",
             Tec.xbar_rsp_power[i] );
    fprintf( ofile, "%s%f%s%f\n",
             "54 27 ", ((float)(i-2)*(float)(36.0))+(float)(18.0), " ",
             Tec.link_local_route_power[i] );
    fprintf( ofile, "%s%f%s%f\n",
             "54 31.5 ", ((float)(i-2)*(float)(36.0))+(float)(18.0), " ",
             Tec.link_remote_route_power[i] );

    for( j=0; j<2; j++ ){
      fprintf( ofile, "%s%f%s%f\n",
               "22.5 40.5 ",
               ((float)(i-2)*(float)(36.0)) + ((float)(j)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_rqst_power[cur] );
      fprintf( ofile, "%s%f%s%f\n",
               "22.5 49.5 ",
               ((float)(i-2)*(float)(36.0)) + ((float)(j)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_rsp_power[cur] );
      fprintf( ofile, "%s%f%s%f\n",
               "22.5 58.5 ",
               ((float)(i-2)*(float)(36.0)) + ((float)(j)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_ctrl_power[cur] );
      cur++;
    }
    for( j=2; j<4; j++ ){
      fprintf( ofile, "%s%f%s%f\n",
               "31.5 40.5 ",
               ((float)(i-2)*(float)(36.0)) + ((float)(j-2)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_rqst_power[cur] );
      fprintf( ofile, "%s%f%s%f\n",
               "31.5 49.5 ",
               ((float)(i-2)*(float)(36.0)) + ((float)(j-2)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_rsp_power[cur] );
      fprintf( ofile, "%s%f%s%f\n",
               "31.5 58.5 ",
               ((float)(i-2)*(float)(36.0)) + ((float)(j-2)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_ctrl_power[cur] );
      cur++;
    }
  }

  /* -- -- link+quad 4,5 */
  for( i=4; i<6; i++ ){
    fprintf( ofile, "%s%f%s%f\n",
             "90 4.5 ", ((float)(i-4)*(float)(36.0))+(float)(18.0), " ",
             Tec.link_phy_power[i] );
    fprintf( ofile, "%s%f%s%f\n",
             "90 13.5 ", ((float)(i-4)*(float)(36.0))+(float)(18.0), " ",
             Tec.xbar_rqst_power[i] );
    fprintf( ofile, "%s%f%s%f\n",
             "90 22.5 ", ((float)(i-4)*(float)(36.0))+(float)(18.0), " ",
             Tec.xbar_rsp_power[i] );
    fprintf( ofile, "%s%f%s%f\n",
             "90 27 ", ((float)(i-4)*(float)(36.0))+(float)(18.0), " ",
             Tec.link_local_route_power[i] );
    fprintf( ofile, "%s%f%s%f\n",
             "90 31.5 ", ((float)(i-4)*(float)(36.0))+(float)(18.0), " ",
             Tec.link_remote_route_power[i] );

    for( j=0; j<2; j++ ){
      fprintf( ofile, "%s%f%s%f\n",
               "40.5 40.5 ",
               ((float)(i-4)*(float)(36.0)) + ((float)(j)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_rqst_power[cur] );
      fprintf( ofile, "%s%f%s%f\n",
               "40.5 49.5 ",
               ((float)(i-4)*(float)(36.0)) + ((float)(j)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_rsp_power[cur] );
      fprintf( ofile, "%s%f%s%f\n",
               "40.5 58.5 ",
               ((float)(i-4)*(float)(36.0)) + ((float)(j)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_ctrl_power[cur] );
      cur++;
    }
    for( j=2; j<4; j++ ){
      fprintf( ofile, "%s%f%s%f\n",
               "49.5 40.5 ",
               ((float)(i-4)*(float)(36.0)) + ((float)(j-2)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_rqst_power[cur] );
      fprintf( ofile, "%s%f%s%f\n",
               "49.5 49.5 ",
               ((float)(i-4)*(float)(36.0)) + ((float)(j-2)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_rsp_power[cur] );
      fprintf( ofile, "%s%f%s%f\n",
               "49.5 58.5 ",
               ((float)(i-4)*(float)(36.0)) + ((float)(j-2)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_ctrl_power[cur] );
      cur++;
    }
  }

  /* -- -- link+quad 6,7 */
  for( i=6; i<8; i++ ){
    fprintf( ofile, "%s%f%s%f\n",
             "126 4.5 ", ((float)(i-6)*(float)(36.0))+(float)(18.0), " ",
             Tec.link_phy_power[i] );
    fprintf( ofile, "%s%f%s%f\n",
             "126 13.5 ", ((float)(i-6)*(float)(36.0))+(float)(18.0), " ",
             Tec.xbar_rqst_power[i] );
    fprintf( ofile, "%s%f%s%f\n",
             "126 22.5 ", ((float)(i-6)*(float)(36.0))+(float)(18.0), " ",
             Tec.xbar_rsp_power[i] );
    fprintf( ofile, "%s%f%s%f\n",
             "126 27 ", ((float)(i-6)*(float)(36.0))+(float)(18.0), " ",
             Tec.link_local_route_power[i] );
    fprintf( ofile, "%s%f%s%f\n",
             "126 31.5 ", ((float)(i-6)*(float)(36.0))+(float)(18.0), " ",
             Tec.link_remote_route_power[i] );

    for( j=0; j<2; j++ ){
      fprintf( ofile, "%s%f%s%f\n",
               "58.5 40.5 ",
               ((float)(i-6)*(float)(36.0)) + ((float)(j)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_rqst_power[cur] );
      fprintf( ofile, "%s%f%s%f\n",
               "58.5 49.5 ",
               ((float)(i-6)*(float)(36.0)) + ((float)(j)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_rsp_power[cur] );
      fprintf( ofile, "%s%f%s%f\n",
               "58.5 58.5 ",
               ((float)(i-6)*(float)(36.0)) + ((float)(j)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_ctrl_power[cur] );
      cur++;
    }
    for( j=2; j<4; j++ ){
      fprintf( ofile, "%s%f%s%f\n",
               "67.5 40.5 ",
               ((float)(i-6)*(float)(36.0)) + ((float)(j-2)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_rqst_power[cur] );
      fprintf( ofile, "%s%f%s%f\n",
               "67.5 49.5 ",
               ((float)(i-6)*(float)(36.0)) + ((float)(j-2)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_rsp_power[cur] );
      fprintf( ofile, "%s%f%s%f\n",
               "67.5 58.5 ",
               ((float)(i-6)*(float)(36.0)) + ((float)(j-2)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_ctrl_power[cur] );
      cur++;
    }
  }

  /* -- -- dram data */
  fprintf( ofile, "%s%f\n", "72 63 72 ", Tec.row_access_power );

  /* -- close the file */
  fclose(ofile);
  ofile = NULL;

  /* -- write the thermal results */
  /* -- open the file */
  ofile = fopen( fname_t, "w" );

  /* -- write all the data */
  /* -- -- header */
  fprintf( ofile, "%s%"PRIu64"%s\n",
           "TITLE = \"HMC 8Link Simulation Clock ", clock, "\"" );
  fprintf( ofile, "%s\n", "VARIABLES = \"X\", \"Y\", \"Z\", \"Btu\"" );
  fprintf( ofile, "%s\n", "ZONE T=\"HMC\", I=144, J=144, F=POINT" );
  fprintf( ofile, "\n\n" );

  cur = 0;

  /* -- -- link+quad 0,1 */
  for( i=0; i<2; i++ ){
    /* X Y Z Data */
    fprintf( ofile, "%s%f%s%f\n",
             "18 4.5 ", ((float)(i)*(float)(36.0))+(float)(18.0), " ",
             Tec.link_phy_btu[i] );
    fprintf( ofile, "%s%f%s%f\n",
             "18 13.5 ", ((float)(i)*(float)(36.0))+(float)(18.0), " ",
             Tec.xbar_rqst_btu[i] );
    fprintf( ofile, "%s%f%s%f\n",
             "18 22.5 ", ((float)(i)*(float)(36.0))+(float)(18.0), " ",
             Tec.xbar_rsp_btu[i] );
    fprintf( ofile, "%s%f%s%f\n",
             "18 27 ", ((float)(i)*(float)(36.0))+(float)(18.0), " ",
             Tec.link_local_route_btu[i] );
    fprintf( ofile, "%s%f%s%f\n",
             "18 31.5 ", ((float)(i)*(float)(36.0))+(float)(18.0), " ",
             Tec.link_remote_route_btu[i] );

    /* vault data */
    for( j=0; j<2; j++ ){
      fprintf( ofile, "%s%f%s%f\n",
               "4.5 40.5 ",
               ((float)(i)*(float)(36.0)) + ((float)(j)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_rqst_btu[cur] );
      fprintf( ofile, "%s%f%s%f\n",
               "4.5 49.5 ",
               ((float)(i)*(float)(36.0)) + ((float)(j)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_rsp_btu[cur] );
      fprintf( ofile, "%s%f%s%f\n",
               "4.5 58.5 ",
               ((float)(i)*(float)(36.0)) + ((float)(j)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_ctrl_btu[cur] );
      cur++;
    }
    for( j=2; j<4; j++ ){
      fprintf( ofile, "%s%f%s%f\n",
               "13.5 40.5 ",
               ((float)(i)*(float)(36.0)) + ((float)(j-2)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_rqst_btu[cur] );
      fprintf( ofile, "%s%f%s%f\n",
               "13.5 49.5 ",
               ((float)(i)*(float)(36.0)) + ((float)(j-2)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_rsp_btu[cur] );
      fprintf( ofile, "%s%f%s%f\n",
               "13.5 58.5 ",
               ((float)(i)*(float)(36.0)) + ((float)(j-2)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_ctrl_btu[cur] );
      cur++;
    }
  }

  /* -- -- link+quad 2,3 */
  for( i=2; i<4; i++ ){
    fprintf( ofile, "%s%f%s%f\n",
             "54 4.5 ", ((float)(i-2)*(float)(36.0))+(float)(18.0), " ",
             Tec.link_phy_btu[i] );
    fprintf( ofile, "%s%f%s%f\n",
             "54 13.5 ", ((float)(i-2)*(float)(36.0))+(float)(18.0), " ",
             Tec.xbar_rqst_btu[i] );
    fprintf( ofile, "%s%f%s%f\n",
             "54 22.5 ", ((float)(i-2)*(float)(36.0))+(float)(18.0), " ",
             Tec.xbar_rsp_btu[i] );
    fprintf( ofile, "%s%f%s%f\n",
             "54 27 ", ((float)(i-2)*(float)(36.0))+(float)(18.0), " ",
             Tec.link_local_route_btu[i] );
    fprintf( ofile, "%s%f%s%f\n",
             "54 31.5 ", ((float)(i-2)*(float)(36.0))+(float)(18.0), " ",
             Tec.link_remote_route_btu[i] );

    for( j=0; j<2; j++ ){
      fprintf( ofile, "%s%f%s%f\n",
               "22.5 40.5 ",
               ((float)(i-2)*(float)(36.0)) + ((float)(j)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_rqst_btu[cur] );
      fprintf( ofile, "%s%f%s%f\n",
               "22.5 49.5 ",
               ((float)(i-2)*(float)(36.0)) + ((float)(j)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_rsp_btu[cur] );
      fprintf( ofile, "%s%f%s%f\n",
               "22.5 58.5 ",
               ((float)(i-2)*(float)(36.0)) + ((float)(j)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_ctrl_btu[cur] );
      cur++;
    }
    for( j=2; j<4; j++ ){
      fprintf( ofile, "%s%f%s%f\n",
               "31.5 40.5 ",
               ((float)(i-2)*(float)(36.0)) + ((float)(j-2)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_rqst_btu[cur] );
      fprintf( ofile, "%s%f%s%f\n",
               "31.5 49.5 ",
               ((float)(i-2)*(float)(36.0)) + ((float)(j-2)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_rsp_btu[cur] );
      fprintf( ofile, "%s%f%s%f\n",
               "31.5 58.5 ",
               ((float)(i-2)*(float)(36.0)) + ((float)(j-2)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_ctrl_btu[cur] );
      cur++;
    }
  }

  /* -- -- link+quad 4,5 */
  for( i=4; i<6; i++ ){
    fprintf( ofile, "%s%f%s%f\n",
             "90 4.5 ", ((float)(i-4)*(float)(36.0))+(float)(18.0), " ",
             Tec.link_phy_btu[i] );
    fprintf( ofile, "%s%f%s%f\n",
             "90 13.5 ", ((float)(i-4)*(float)(36.0))+(float)(18.0), " ",
             Tec.xbar_rqst_btu[i] );
    fprintf( ofile, "%s%f%s%f\n",
             "90 22.5 ", ((float)(i-4)*(float)(36.0))+(float)(18.0), " ",
             Tec.xbar_rsp_btu[i] );
    fprintf( ofile, "%s%f%s%f\n",
             "90 27 ", ((float)(i-4)*(float)(36.0))+(float)(18.0), " ",
             Tec.link_local_route_btu[i] );
    fprintf( ofile, "%s%f%s%f\n",
             "90 31.5 ", ((float)(i-4)*(float)(36.0))+(float)(18.0), " ",
             Tec.link_remote_route_btu[i] );

    for( j=0; j<2; j++ ){
      fprintf( ofile, "%s%f%s%f\n",
               "40.5 40.5 ",
               ((float)(i-4)*(float)(36.0)) + ((float)(j)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_rqst_btu[cur] );
      fprintf( ofile, "%s%f%s%f\n",
               "40.5 49.5 ",
               ((float)(i-4)*(float)(36.0)) + ((float)(j)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_rsp_btu[cur] );
      fprintf( ofile, "%s%f%s%f\n",
               "40.5 58.5 ",
               ((float)(i-4)*(float)(36.0)) + ((float)(j)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_ctrl_btu[cur] );
      cur++;
    }
    for( j=2; j<4; j++ ){
      fprintf( ofile, "%s%f%s%f\n",
               "49.5 40.5 ",
               ((float)(i-4)*(float)(36.0)) + ((float)(j-2)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_rqst_btu[cur] );
      fprintf( ofile, "%s%f%s%f\n",
               "49.5 49.5 ",
               ((float)(i-4)*(float)(36.0)) + ((float)(j-2)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_rsp_btu[cur] );
      fprintf( ofile, "%s%f%s%f\n",
               "49.5 58.5 ",
               ((float)(i-4)*(float)(36.0)) + ((float)(j-2)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_ctrl_btu[cur] );
      cur++;
    }
  }

  /* -- -- link+quad 6,7 */
  for( i=6; i<8; i++ ){
    fprintf( ofile, "%s%f%s%f\n",
             "126 4.5 ", ((float)(i-6)*(float)(36.0))+(float)(18.0), " ",
             Tec.link_phy_btu[i] );
    fprintf( ofile, "%s%f%s%f\n",
             "126 13.5 ", ((float)(i-6)*(float)(36.0))+(float)(18.0), " ",
             Tec.xbar_rqst_btu[i] );
    fprintf( ofile, "%s%f%s%f\n",
             "126 22.5 ", ((float)(i-6)*(float)(36.0))+(float)(18.0), " ",
             Tec.xbar_rsp_btu[i] );
    fprintf( ofile, "%s%f%s%f\n",
             "126 27 ", ((float)(i-6)*(float)(36.0))+(float)(18.0), " ",
             Tec.link_local_route_btu[i] );
    fprintf( ofile, "%s%f%s%f\n",
             "126 31.5 ", ((float)(i-6)*(float)(36.0))+(float)(18.0), " ",
             Tec.link_remote_route_btu[i] );

    for( j=0; j<2; j++ ){
      fprintf( ofile, "%s%f%s%f\n",
               "58.5 40.5 ",
               ((float)(i-6)*(float)(36.0)) + ((float)(j)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_rqst_btu[cur] );
      fprintf( ofile, "%s%f%s%f\n",
               "58.5 49.5 ",
               ((float)(i-6)*(float)(36.0)) + ((float)(j)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_rsp_btu[cur] );
      fprintf( ofile, "%s%f%s%f\n",
               "58.5 58.5 ",
               ((float)(i-6)*(float)(36.0)) + ((float)(j)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_ctrl_btu[cur] );
      cur++;
    }
    for( j=2; j<4; j++ ){
      fprintf( ofile, "%s%f%s%f\n",
               "67.5 40.5 ",
               ((float)(i-6)*(float)(36.0)) + ((float)(j-2)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_rqst_btu[cur] );
      fprintf( ofile, "%s%f%s%f\n",
               "67.5 49.5 ",
               ((float)(i-6)*(float)(36.0)) + ((float)(j-2)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_rsp_btu[cur] );
      fprintf( ofile, "%s%f%s%f\n",
               "67.5 58.5 ",
               ((float)(i-6)*(float)(36.0)) + ((float)(j-2)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_ctrl_btu[cur] );
      cur++;
    }
  }

  /* -- -- dram data */
  fprintf( ofile, "%s%f\n", "72 63 72 ", Tec.row_access_btu );

  /* -- close the file */
  fclose( ofile );
  ofile = NULL;

  free( fname_p );
  free( fname_t );
  fname_p = NULL;
  fname_t = NULL;


  return 0;
}

static int hmcsim_tecplot4( struct HMC4LinkTec Tec,
                            uint64_t clock,
                            char *prefix ){
  /* vars */
  char *fname_p = NULL;
  char *fname_t = NULL;
  FILE *ofile   = NULL;
  int i         = 0;
  int j         = 0;
  int cur       = 0;
  /* ---- */

  /* -- build the file names */
  fname_p = malloc( sizeof( char ) * (strlen(prefix)+40) );
  if( fname_p == NULL ){
    return -1;
  }
  fname_t = malloc( sizeof( char ) * (strlen(prefix)+40) );
  if( fname_t == NULL ){
    free( fname_p );
    fname_p = NULL;
    return -1;
  }

  sprintf( fname_p, "%s%s%"PRIu64"%s", prefix, "-power.", clock, ".tec" );
  sprintf( fname_t, "%s%s%"PRIu64"%s", prefix, "-therm.", clock, ".tec" );

  /* -- write the power results */
  /* -- open the file */
  ofile = fopen( fname_p, "w" );

  /* -- write all the data */

  /* -- -- header */
  fprintf( ofile, "%s%"PRIu64"%s\n",
           "TITLE = \"HMC 4Link Simulation Clock ", clock, "\"" );
  fprintf( ofile, "%s\n", "VARIABLES = \"X\", \"Y\", \"Z\", \"Power\"" );
  fprintf( ofile, "%s\n", "ZONE T=\"HMC\", I=72, J=72, F=POINT" );
  fprintf( ofile, "\n\n" );

  /* -- -- link+quad 0,1 */
  for( i=0; i<2; i++ ){
    /* X Y Z Data */
    fprintf( ofile, "%s%f%s%f\n",
             "18 4.5 ", ((float)(i)*(float)(36.0))+(float)(18.0), " ",
             Tec.link_phy_power[i] );
    fprintf( ofile, "%s%f%s%f\n",
             "18 13.5 ", ((float)(i)*(float)(36.0))+(float)(18.0), " ",
             Tec.xbar_rqst_power[i] );
    fprintf( ofile, "%s%f%s%f\n",
             "18 22.5 ", ((float)(i)*(float)(36.0))+(float)(18.0), " ",
             Tec.xbar_rsp_power[i] );
    fprintf( ofile, "%s%f%s%f\n",
             "18 27 ", ((float)(i)*(float)(36.0))+(float)(18.0), " ",
             Tec.link_local_route_power[i] );
    fprintf( ofile, "%s%f%s%f\n",
             "18 31.5 ", ((float)(i)*(float)(36.0))+(float)(18.0), " ",
             Tec.link_remote_route_power[i] );

    /* vault data */
    for( j=0; j<2; j++ ){
      fprintf( ofile, "%s%f%s%f\n",
               "4.5 40.5 ",
               ((float)(i)*(float)(36.0)) + ((float)(j)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_rqst_power[cur] );
      fprintf( ofile, "%s%f%s%f\n",
               "4.5 49.5 ",
               ((float)(i)*(float)(36.0)) + ((float)(j)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_rsp_power[cur] );
      fprintf( ofile, "%s%f%s%f\n",
               "4.5 58.5 ",
               ((float)(i)*(float)(36.0)) + ((float)(j)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_ctrl_power[cur] );
      cur++;
    }
    for( j=2; j<4; j++ ){
      fprintf( ofile, "%s%f%s%f\n",
               "13.5 40.5 ",
               ((float)(i)*(float)(36.0)) + ((float)(j-2)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_rqst_power[cur] );
      fprintf( ofile, "%s%f%s%f\n",
               "13.5 49.5 ",
               ((float)(i)*(float)(36.0)) + ((float)(j-2)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_rsp_power[cur] );
      fprintf( ofile, "%s%f%s%f\n",
               "13.5 58.5 ",
               ((float)(i)*(float)(36.0)) + ((float)(j-2)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_ctrl_power[cur] );
      cur++;
    }
    for( j=4; j<6; j++ ){
      fprintf( ofile, "%s%f%s%f\n",
               "22.5 40.5 ",
               ((float)(i)*(float)(36.0)) + ((float)(j-4)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_rqst_power[cur] );
      fprintf( ofile, "%s%f%s%f\n",
               "22.5 49.5 ",
               ((float)(i)*(float)(36.0)) + ((float)(j-4)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_rsp_power[cur] );
      fprintf( ofile, "%s%f%s%f\n",
               "22.5 58.5 ",
               ((float)(i)*(float)(36.0)) + ((float)(j-4)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_ctrl_power[cur] );
      cur++;
    }
    for(j=6; j<8; j++ ){
      fprintf( ofile, "%s%f%s%f\n",
               "31.5 40.5 ",
               ((float)(i)*(float)(36.0)) + ((float)(j-6)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_rqst_power[cur] );
      fprintf( ofile, "%s%f%s%f\n",
               "31.5 49.5 ",
               ((float)(i)*(float)(36.0)) + ((float)(j-6)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_rsp_power[cur] );
      fprintf( ofile, "%s%f%s%f\n",
               "31.5 58.5 ",
               ((float)(i)*(float)(36.0)) + ((float)(j-6)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_ctrl_power[cur] );
      cur++;
    }
  }

  /* -- -- link+quad 2,3 */
  for( i=2; i<4; i++ ){
    fprintf( ofile, "%s%f%s%f\n",
             "54 4.5 ", ((float)(i-2)*(float)(36.0))+(float)(18.0), " ",
             Tec.link_phy_power[i] );
    fprintf( ofile, "%s%f%s%f\n",
             "54 13.5 ", ((float)(i-2)*(float)(36.0))+(float)(18.0), " ",
             Tec.xbar_rqst_power[i] );
    fprintf( ofile, "%s%f%s%f\n",
             "54 22.5 ", ((float)(i-2)*(float)(36.0))+(float)(18.0), " ",
             Tec.xbar_rsp_power[i] );
    fprintf( ofile, "%s%f%s%f\n",
             "54 27 ", ((float)(i-2)*(float)(36.0))+(float)(18.0), " ",
             Tec.link_local_route_power[i] );
    fprintf( ofile, "%s%f%s%f\n",
             "54 31.5 ", ((float)(i-2)*(float)(36.0))+(float)(18.0), " ",
             Tec.link_remote_route_power[i] );

    for( j=0; j<2; j++ ){
      fprintf( ofile, "%s%f%s%f\n",
               "40.5 40.5 ",
               ((float)(i-2)*(float)(36.0)) + ((float)(j)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_rqst_power[cur] );
      fprintf( ofile, "%s%f%s%f\n",
               "40.5 49.5 ",
               ((float)(i-2)*(float)(36.0)) + ((float)(j)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_rsp_power[cur] );
      fprintf( ofile, "%s%f%s%f\n",
               "40.5 58.5 ",
               ((float)(i-2)*(float)(36.0)) + ((float)(j)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_ctrl_power[cur] );
      cur++;
    }
    for( j=2; j<4; j++ ){
      fprintf( ofile, "%s%f%s%f\n",
               "49.5 40.5 ",
               ((float)(i-2)*(float)(36.0)) + ((float)(j-2)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_rqst_power[cur] );
      fprintf( ofile, "%s%f%s%f\n",
               "49.5 49.5 ",
               ((float)(i-2)*(float)(36.0)) + ((float)(j-2)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_rsp_power[cur] );
      fprintf( ofile, "%s%f%s%f\n",
               "49.5 58.5 ",
               ((float)(i-2)*(float)(36.0)) + ((float)(j-2)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_ctrl_power[cur] );
      cur++;
    }
    for( j=4; j<6; j++ ){
      fprintf( ofile, "%s%f%s%f\n",
               "58.5 40.5 ",
               ((float)(i-2)*(float)(36.0)) + ((float)(j-4)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_rqst_power[cur] );
      fprintf( ofile, "%s%f%s%f\n",
               "58.5 49.5 ",
               ((float)(i-2)*(float)(36.0)) + ((float)(j-4)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_rsp_power[cur] );
      fprintf( ofile, "%s%f%s%f\n",
               "58.5 58.5 ",
               ((float)(i-2)*(float)(36.0)) + ((float)(j-4)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_ctrl_power[cur] );
      cur++;
    }
    for( j=6; j<8; j++ ){
      fprintf( ofile, "%s%f%s%f\n",
               "67.5 40.5 ",
               ((float)(i-2)*(float)(36.0)) + ((float)(j-6)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_rqst_power[cur] );
      fprintf( ofile, "%s%f%s%f\n",
               "67.5 49.5 ",
               ((float)(i-2)*(float)(36.0)) + ((float)(j-6)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_rsp_power[cur] );
      fprintf( ofile, "%s%f%s%f\n",
               "67.5 58.5 ",
               ((float)(i-2)*(float)(36.0)) + ((float)(j-6)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_ctrl_power[cur] );
      cur++;
    }
  }

  /* -- -- dram data */
  fprintf( ofile, "%s%f\n", "36 63 36 ", Tec.row_access_power );

  /* -- close the file */
  fclose(ofile);
  ofile = NULL;

  /* -- write the thermal results */
  /* -- open the file */
  ofile = fopen( fname_t, "w" );

  /* -- write all the data */
  /* -- -- header */
  fprintf( ofile, "%s%"PRIu64"%s\n",
           "TITLE = \"HMC 4Link Simulation Clock ", clock, "\"" );
  fprintf( ofile, "%s\n", "VARIABLES = \"X\", \"Y\", \"Z\", \"Btu\"" );
  fprintf( ofile, "%s\n", "ZONE T=\"HMC\", I=72, J=72, F=POINT" );
  fprintf( ofile, "\n\n" );

  cur = 0;

  /* -- -- link+quad 0,1 */
  for( i=0; i<2; i++ ){
    /* X Y Z Data */
    fprintf( ofile, "%s%f%s%f\n",
             "18 4.5 ", ((float)(i)*(float)(36.0))+(float)(18.0), " ",
             Tec.link_phy_btu[i] );
    fprintf( ofile, "%s%f%s%f\n",
             "18 13.5 ", ((float)(i)*(float)(36.0))+(float)(18.0), " ",
             Tec.xbar_rqst_btu[i] );
    fprintf( ofile, "%s%f%s%f\n",
             "18 22.5 ", ((float)(i)*(float)(36.0))+(float)(18.0), " ",
             Tec.xbar_rsp_btu[i] );
    fprintf( ofile, "%s%f%s%f\n",
             "18 27 ", ((float)(i)*(float)(36.0))+(float)(18.0), " ",
             Tec.link_local_route_btu[i] );
    fprintf( ofile, "%s%f%s%f\n",
             "18 31.5 ", ((float)(i)*(float)(36.0))+(float)(18.0), " ",
             Tec.link_remote_route_btu[i] );

    /* vault data */
    for( j=0; j<2; j++ ){
      fprintf( ofile, "%s%f%s%f\n",
               "4.5 40.5 ",
               ((float)(i)*(float)(36.0)) + ((float)(j)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_rqst_btu[cur] );
      fprintf( ofile, "%s%f%s%f\n",
               "4.5 49.5 ",
               ((float)(i)*(float)(36.0)) + ((float)(j)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_rsp_btu[cur] );
      fprintf( ofile, "%s%f%s%f\n",
               "4.5 58.5 ",
               ((float)(i)*(float)(36.0)) + ((float)(j)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_ctrl_btu[cur] );
      cur++;
    }
    for( j=2; j<4; j++ ){
      fprintf( ofile, "%s%f%s%f\n",
               "13.5 40.5 ",
               ((float)(i)*(float)(36.0)) + ((float)(j-2)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_rqst_btu[cur] );
      fprintf( ofile, "%s%f%s%f\n",
               "13.5 49.5 ",
               ((float)(i)*(float)(36.0)) + ((float)(j-2)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_rsp_btu[cur] );
      fprintf( ofile, "%s%f%s%f\n",
               "13.5 58.5 ",
               ((float)(i)*(float)(36.0)) + ((float)(j-2)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_ctrl_btu[cur] );
      cur++;
    }
    for( j=4; j<6; j++ ){
      fprintf( ofile, "%s%f%s%f\n",
               "22.5 40.5 ",
               ((float)(i)*(float)(36.0)) + ((float)(j-4)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_rqst_btu[cur] );
      fprintf( ofile, "%s%f%s%f\n",
               "22.5 49.5 ",
               ((float)(i)*(float)(36.0)) + ((float)(j-4)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_rsp_btu[cur] );
      fprintf( ofile, "%s%f%s%f\n",
               "22.5 58.5 ",
               ((float)(i)*(float)(36.0)) + ((float)(j-4)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_ctrl_btu[cur] );
      cur++;
    }
    for(j=6; j<8; j++ ){
      fprintf( ofile, "%s%f%s%f\n",
               "31.5 40.5 ",
               ((float)(i)*(float)(36.0)) + ((float)(j-6)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_rqst_btu[cur] );
      fprintf( ofile, "%s%f%s%f\n",
               "31.5 49.5 ",
               ((float)(i)*(float)(36.0)) + ((float)(j-6)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_rsp_btu[cur] );
      fprintf( ofile, "%s%f%s%f\n",
               "31.5 58.5 ",
               ((float)(i)*(float)(36.0)) + ((float)(j-6)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_ctrl_btu[cur] );
      cur++;
    }
  }

  /* -- -- link+quad 2,3 */
  for( i=2; i<4; i++ ){
    fprintf( ofile, "%s%f%s%f\n",
             "54 4.5 ", ((float)(i-2)*(float)(36.0))+(float)(18.0), " ",
             Tec.link_phy_btu[i] );
    fprintf( ofile, "%s%f%s%f\n",
             "54 13.5 ", ((float)(i-2)*(float)(36.0))+(float)(18.0), " ",
             Tec.xbar_rqst_btu[i] );
    fprintf( ofile, "%s%f%s%f\n",
             "54 22.5 ", ((float)(i-2)*(float)(36.0))+(float)(18.0), " ",
             Tec.xbar_rsp_btu[i] );
    fprintf( ofile, "%s%f%s%f\n",
             "54 27 ", ((float)(i-2)*(float)(36.0))+(float)(18.0), " ",
             Tec.link_local_route_btu[i] );
    fprintf( ofile, "%s%f%s%f\n",
             "54 31.5 ", ((float)(i-2)*(float)(36.0))+(float)(18.0), " ",
             Tec.link_remote_route_btu[i] );

    for( j=0; j<2; j++ ){
      fprintf( ofile, "%s%f%s%f\n",
               "40.5 40.5 ",
               ((float)(i-2)*(float)(36.0)) + ((float)(j)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_rqst_btu[cur] );
      fprintf( ofile, "%s%f%s%f\n",
               "40.5 49.5 ",
               ((float)(i-2)*(float)(36.0)) + ((float)(j)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_rsp_btu[cur] );
      fprintf( ofile, "%s%f%s%f\n",
               "40.5 58.5 ",
               ((float)(i-2)*(float)(36.0)) + ((float)(j)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_ctrl_btu[cur] );
      cur++;
    }
    for( j=2; j<4; j++ ){
      fprintf( ofile, "%s%f%s%f\n",
               "49.5 40.5 ",
               ((float)(i-2)*(float)(36.0)) + ((float)(j-2)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_rqst_btu[cur] );
      fprintf( ofile, "%s%f%s%f\n",
               "49.5 49.5 ",
               ((float)(i-2)*(float)(36.0)) + ((float)(j-2)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_rsp_btu[cur] );
      fprintf( ofile, "%s%f%s%f\n",
               "49.5 58.5 ",
               ((float)(i-2)*(float)(36.0)) + ((float)(j-2)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_ctrl_btu[cur] );
      cur++;
    }
    for( j=4; j<6; j++ ){
      fprintf( ofile, "%s%f%s%f\n",
               "58.5 40.5 ",
               ((float)(i-2)*(float)(36.0)) + ((float)(j-4)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_rqst_btu[cur] );
      fprintf( ofile, "%s%f%s%f\n",
               "58.5 49.5 ",
               ((float)(i-2)*(float)(36.0)) + ((float)(j-4)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_rsp_btu[cur] );
      fprintf( ofile, "%s%f%s%f\n",
               "58.5 58.5 ",
               ((float)(i-2)*(float)(36.0)) + ((float)(j-4)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_ctrl_btu[cur] );
      cur++;
    }
    for( j=6; j<8; j++ ){
      fprintf( ofile, "%s%f%s%f\n",
               "67.5 40.5 ",
               ((float)(i-2)*(float)(36.0)) + ((float)(j-6)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_rqst_btu[cur] );
      fprintf( ofile, "%s%f%s%f\n",
               "67.5 49.5 ",
               ((float)(i-2)*(float)(36.0)) + ((float)(j-6)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_rsp_btu[cur] );
      fprintf( ofile, "%s%f%s%f\n",
               "67.5 58.5 ",
               ((float)(i-2)*(float)(36.0)) + ((float)(j-6)*(float)(18)) + ((float)(9.0)),
               " ", Tec.vault_ctrl_btu[cur] );
      cur++;
    }
  }

  /* -- -- dram data */
  fprintf( ofile, "%s%f\n", "36 63 36 ", Tec.row_access_btu );

  /* -- close the file */
  fclose( ofile );
  ofile = NULL;

  free( fname_p );
  free( fname_t );
  fname_p = NULL;
  fname_t = NULL;

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
