/*
 * _DYN_CMC_C_
 *
 * HMCSIM DYN_CMC TEST DRIVER
 *
 * Attempts to build a dynamic cmc test
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "hmc_sim.h"

/* ----------------------------------------------------- FUNCTION PROTOTYPES */
extern int getshiftamount( 	uint32_t num_links,
				uint32_t capacity,
				uint32_t bsize,
				uint32_t *shiftamt );
extern int execute_test(        struct hmcsim_t *hmc,
                                uint32_t num_threads,
				uint32_t shiftamt,
				uint32_t array_width,
				uint32_t array_height,
				uint32_t array_depth,
				double clock_ghz );

/* ----------------------------------------------------- LOAD_CMC_LIBS */
/*
 * LOAD_CMC_LIBS
 *
 */
static int load_cmc_libs( struct hmcsim_t *hmc, char *libsrc ){
  /* vars */
  int srclen  = strlen( libsrc ) + 1;
  char *lib   = NULL;
  char *mlib = "/stencil_read/stencil_read_3d_1/libstencil_read_3d_1.so";
  /* ---- */

  lib = malloc( sizeof( char ) * (strlen(mlib)+srclen) );
  if( lib == NULL ){
    printf( "ERROR : COULD NOT ALLOCATE MEMORY FOR LIB: %s\n", mlib );
    return -1;
  }

  sprintf( lib, "%s%s%s", libsrc, "/", mlib );


  printf( "LOADING CMC LIBRARY: %s\n", mlib );
  if( hmcsim_load_cmc( hmc, lib ) != 0 ){
    printf( "ERROR : COULD NOT LOAD CMC LIBRARY : %s\n", mlib );
    return -1;
  }else{
    printf( "LOADED CMC LIBRARY: %s\n", mlib );
  }

  free( lib );
  lib = NULL;

  fflush( stdout );

  return 0;
}

/* ----------------------------------------------------- MAIN */
/*
 * MAIN
 *
 */
extern int main( int argc, char **argv ){
  /* vars */
  int ret			= 0;
  uint32_t i		= 0;
  uint32_t num_devs	= 0;
  uint32_t num_links	= 0;
  uint32_t num_vaults	= 0;
  uint32_t queue_depth	= 0;
  uint32_t num_banks	= 0;
  uint32_t num_drams	= 0;
  uint32_t capacity	= 0;
  uint32_t xbar_depth	= 0;
  uint32_t num_threads	= 1;
  uint32_t bsize	= 128;
  uint32_t shiftamt	= 0;
  uint32_t nX		= 0;
  uint32_t nY		= 0;
  uint32_t nZ		= 0;  
  double clock_ghz 	= 1.0;

  char *libsrc          = NULL;
  struct hmcsim_t hmc;
  /* ---- */

  while(( ret = getopt( argc, argv, "b:c:d:h:l:m:n:q:v:x:t:N:T:C:L:i:j:k:" )) != -1 )
  {
    printf("got: %c\n", ret);
    switch( ret )
    {
      case 'b':
        num_banks = (uint32_t)(atoi(optarg));
        break;
      case 'c':
        capacity = (uint32_t)(atoi(optarg));
        break;
      case 'd':
        num_drams = (uint32_t)(atoi(optarg));
        break;
      case 'h':
        printf( "%s%s%s\n", "usage : ", argv[0], " -bcdhlnqvx" );
        printf( " -b <num_banks>\n" );
        printf( " -c <capacity>\n" );
        printf( " -d <num_drams>\n" );
        printf( " -l <num_links>\n" );
        printf( " -m <block_size>\n" );
        printf( " -n <num_devs>\n" );
        printf( " -q <queue_depth>\n" );
        printf( " -v <num_vaults>\n" );
        printf( " -x <xbar_depth>\n" );
	printf( " -i <array_width>\n" );
	printf( " -j <array_height>\n" );
	printf( " -k <array_depth>\n" );
        printf( " -t <num_threads>\n" );
        printf( " -C <clock_ghz>\n" );
        printf( " -h ...print help\n" );
        return 0;
        break;
      case 't':
	num_threads = (uint32_t)(atoi(optarg));
        break;
      case 'l':
        num_links = (uint32_t)(atoi(optarg));
        break;
      case 'm':
        bsize = (uint32_t)(atoi(optarg));
        break;
      case 'n':
        num_devs = (uint32_t)(atoi(optarg));
        break;
      case 'q':
        queue_depth = (uint32_t)(atoi(optarg));
        break;
      case 'v':
        num_vaults = (uint32_t)(atoi(optarg));
        break;
      case 'x':
        xbar_depth = (uint32_t)(atoi(optarg));
        break;
      case 'C':
	clock_ghz = (double)(atof(optarg));
        break;
      case 'L':
        if( libsrc != NULL ){
          printf( "error : libsrc already set\n" );
          return -1;
        }
        libsrc = malloc( sizeof( char ) * (strlen( optarg )+1) );
        sprintf( libsrc, "%s", optarg );
        break;
      case 'i':
	nX = (uint32_t)(atoi(optarg));
	break;
      case 'j':
	nY = (uint32_t)(atoi(optarg));
	break;
      case 'k':
	nZ = (uint32_t)(atoi(optarg));
	break;
      case '?':
        default:
        printf( "%s%s%s\n", "Unknown option: see ", argv[0],
                " -bcdhlmnqsvxNTL" );
        return -1;
        break;
    }
  }



  /*
   * get the shift amount based upon
   * the max block size, device size and link count
   *
   */
  if( getshiftamount( num_links, capacity, bsize, &shiftamt ) != 0 ){
    printf( "FAILED TO RETRIEVE SHIFT AMOUNT\n" );
    hmcsim_free( &hmc );
    free( libsrc );
    libsrc = NULL;
  }


  /*
   * init the library
   *
   */

  ret = hmcsim_init(&hmc,
                    num_devs,
                    num_links,
                    num_vaults,
                    queue_depth,
                    num_banks,
                    num_drams,
                    capacity,
                    xbar_depth );
  if( ret != 0 ){
    printf( "FAILED TO INIT HMCSIM\n" );
    return -1;
  }else{
    printf( "SUCCESS : INITIALIZED HMCSIM\n" );
  }

  /*
   * setup the device topology
   *
   */
  if( num_devs > 1 ){
    /*
     * MULTIPLE DEVICES
     *
     */
  }else{
    /*
     * SINGLE DEVICE
     *
     */

    for( i=0; i<num_links; i++ ){
      ret = hmcsim_link_config( &hmc,
                                (num_devs+1),
                                0,
                                i,
                                i,
                                HMC_LINK_HOST_DEV );

      if( ret != 0 ){
        printf( "FAILED TO INIT LINK %d\n", i );
        hmcsim_free( &hmc );
        free( libsrc );
        libsrc = NULL;
        return -1;
      }else{
        printf( "SUCCESS : INITIALIZED LINK %d\n", i );
      }
    }
  }

  /*
   * init the max request block size
   *
   */
  ret = hmcsim_util_set_all_max_blocksize( &hmc, bsize );

  if( ret != 0 ){
    printf( "FAILED TO INIT MAX BLOCKSIZE\n" );
    hmcsim_free( &hmc );
    free( libsrc );
    libsrc = NULL;
    return -1;
  }else {
    printf( "SUCCESS : INITIALIZED MAX BLOCK SIZE\n" );
  }

  /*
   * load the cmc libs
   *
   */
  if( load_cmc_libs( &hmc, libsrc ) != 0 ){
    printf( "ERROR : FAILED TO LOAD THE CMC LIBS\n" );
    ret = -1;
    goto cleanup;
  }

  /*
   * execute the test
   *
   */
  if( execute_test( &hmc,
                    num_threads,
                    shiftamt,
		    nX,
		    nY,
		    nZ,
                    clock_ghz ) != 0 ){
    printf( "ERROR : FAILED TO EXECUTE THE TEST\n" );
    ret = -1;
    goto cleanup;
  }

cleanup:
  if( libsrc!= NULL){
    free( libsrc );
    libsrc = NULL;
  }

  /*
   * free the library data
   *
   */
  ret = hmcsim_free( &hmc );
  if( ret != 0 ){
    printf( "FAILED TO FREE HMCSIM\n" );
    return -1;
  }else {
    printf( "SUCCESS : FREE'D HMCSIM\n" );
  }

  return ret;
}

/* EOF */
