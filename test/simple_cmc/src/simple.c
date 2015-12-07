/*
 * _SIMPLE_C_
 *
 * HMCSIM SIMPLE TEST DRIVER W/ CMC SUPPORT
 *
 * Attempts to initialize a HMC instantiation
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "hmc_sim.h"



/* ----------------------------------------------------- MAIN */
/*
 * MAIN
 *
 */
extern int main( int argc, char **argv )
{
	/* vars */
	int ret			= 0;
	uint32_t num_devs	= 0;
	uint32_t num_links	= 0;
	uint32_t num_vaults	= 0;
	uint32_t queue_depth	= 0;
	uint32_t num_banks	= 0;
	uint32_t num_drams	= 0;
	uint32_t capacity	= 0;
	uint32_t xbar_depth	= 0;
        char *cmc_lib           = NULL;
	struct hmcsim_t hmc;
	/* ---- */

	while(( ret = getopt( argc, argv, "b:c:d:hl:n:q:v:x:C:" )) != -1 )
	{
		switch( ret )
		{
                        case 'C':
                               cmc_lib = malloc( sizeof( char) *
                                                 strlen( optarg ) );
                                if( cmc_lib == NULL ){
                                  printf( "ERROR : COULD NOT ALLOCATE MEMORY FOR LIBRARY\n" );
                                  return -1;
                                }

                                sprintf( cmc_lib, "%s", optarg );
                                break;
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
				printf( " -h ...print help\n" );
				printf( " -l <num_links>\n" );
				printf( " -n <num_devs>\n" );
				printf( " -q <queue_depth>\n" );
				printf( " -v <num_vaults>\n" );
				printf( " -x <xbar_depth>\n" );
                                printf( " -C /path/to/cmc/lib.so\n" );
				return 0;
				break;
			case 'l':
				num_links = (uint32_t)(atoi(optarg));
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
			case '?':
			default:
				printf( "%s%s%s\n", "Unknown option: see ", argv[0], " -bcdhlnqvx" );
				return -1;
				break;
		}
	}

	/*
	 * init the library
	 *
	 */

	ret = hmcsim_init(	&hmc,
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
	}else {
		printf( "SUCCESS : INITALIZED HMCSIM\n" );
	}

	/*
	 * set the maximum request size for all devices
	 *
	 */
	ret = hmcsim_util_set_all_max_blocksize( &hmc, 128 );
	if( ret != 0 ){
		printf( "FAILED TO SET MAXIMUM BLOCK SIZE\n" );
		hmcsim_free( &hmc );
		return -1;
	}else {
		printf( "SUCCESS : SET MAXIMUM BLOCK SIZE\n" );
	}

        /*
         * load the cmc library
         *
         */
        if( cmc_lib != NULL ){
          ret = hmcsim_load_cmc( &hmc, cmc_lib );
          if( ret != 0 ){
            printf( "FAILED TO LOAD THE CMC LIBRARY AT %s\n", cmc_lib );
	    hmcsim_free( &hmc );
            return -1;
          }else{
            printf( "SUCCESS : LOADED THE CMC LIBRARY AT %s\n", cmc_lib );
          }
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

        if( cmc_lib != NULL ){
          free( cmc_lib );
        }

	return ret;
}

/* EOF */
