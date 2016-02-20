/*
 * _LOAD_FE_C_
 *
 * HMCSIM LOADER TEST FOR ALL FULL/EMPTY CMC OPS
 *
 * Attempts to initialize a HMC instantiation with all FE ops
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
	struct hmcsim_t hmc;
	/* ---- */

	while(( ret = getopt( argc, argv, "b:c:d:hl:n:q:v:x:" )) != -1 )
	{
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
				printf( " -h ...print help\n" );
				printf( " -l <num_links>\n" );
				printf( " -n <num_devs>\n" );
				printf( " -q <queue_depth>\n" );
				printf( " -v <num_vaults>\n" );
				printf( " -x <xbar_depth>\n" );
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
        ret = hmcsim_load_cmc( &hmc, "../../cmc/fullempty/ClrXX/libclrxx.so" );
        if( ret != 0 ){
          printf( "FAILED TO LOAD CMC LIBRARY AT ../../cmc/fullempty/ClrXX/libclrxx.so\n" );
          hmcsim_free( &hmc );
          return -1;
        }else{
          printf( "LOADED CMC LIBRARY AT ../../cmc/fullempty/ClrXX/libclrxx.so\n" );
        }

        ret = hmcsim_load_cmc( &hmc, "../../cmc/fullempty/IncFF/libincff.so" );
        if( ret != 0 ){
          printf( "FAILED TO LOAD CMC LIBRARY AT ../../cmc/fullempty/IncFF/libincff.so\n" );
          hmcsim_free( &hmc );
          return -1;
        }else{
          printf( "LOADED CMC LIBRARY AT ../../cmc/fullempty/IncFF/libincff.so\n" );
        }

        ret = hmcsim_load_cmc( &hmc, "../../cmc/fullempty/ReadEF/libreadef.so" );
        if( ret != 0 ){
          printf( "FAILED TO LOAD CMC LIBRARY AT ../../cmc/fullempty/ReadEF/libreadef.so\n" );
          hmcsim_free( &hmc );
          return -1;
        }else{
          printf( "LOADED CMC LIBRARY AT ../../cmc/fullempty/ReadEF/libreadef.so\n" );
        }

        ret = hmcsim_load_cmc( &hmc, "../../cmc/fullempty/ReadFE/libreadfe.so" );
        if( ret != 0 ){
          printf( "FAILED TO LOAD CMC LIBRARY AT ../../cmc/fullempty/ReadFE/libreadfe.so\n" );
          hmcsim_free( &hmc );
          return -1;
        }else{
          printf( "LOADED CMC LIBRARY AT ../../cmc/fullempty/ReadFE/libreadfe.so\n" );
        }

        ret = hmcsim_load_cmc( &hmc, "../../cmc/fullempty/ReadFF/libreadff.so" );
        if( ret != 0 ){
          printf( "FAILED TO LOAD CMC LIBRARY AT ../../cmc/fullempty/ReadFF/libreadff.so\n" );
          hmcsim_free( &hmc );
          return -1;
        }else{
          printf( "LOADED CMC LIBRARY AT ../../cmc/fullempty/ReadFF/libreadff.so\n" );
        }

        ret = hmcsim_load_cmc( &hmc, "../../cmc/fullempty/ReadXX/libreadxx.so" );
        if( ret != 0 ){
          printf( "FAILED TO LOAD CMC LIBRARY AT ../../cmc/fullempty/ReadXX/libreadxx.so\n" );
          hmcsim_free( &hmc );
          return -1;
        }else{
          printf( "LOADED CMC LIBRARY AT ../../cmc/fullempty/ReadXX/libreadxx.so\n" );
        }

        ret = hmcsim_load_cmc( &hmc, "../../cmc/fullempty/WriteEF/libwriteef.so" );
        if( ret != 0 ){
          printf( "FAILED TO LOAD CMC LIBRARY AT ../../cmc/fullempty/WriteEF/libwriteef.so\n" );
          hmcsim_free( &hmc );
          return -1;
        }else{
          printf( "LOADED CMC LIBRARY AT ../../cmc/fullempty/WriteEF/libwriteef.so\n" );
        }

        ret = hmcsim_load_cmc( &hmc, "../../cmc/fullempty/WriteFF/libwriteff.so" );
        if( ret != 0 ){
          printf( "FAILED TO LOAD CMC LIBRARY AT ../../cmc/fullempty/WriteFF/libwriteff.so\n" );
          hmcsim_free( &hmc );
          return -1;
        }else{
          printf( "LOADED CMC LIBRARY AT ../../cmc/fullempty/WriteFF/libwriteff.so\n" );
        }

        ret = hmcsim_load_cmc( &hmc, "../../cmc/fullempty/WriteXE/libwritexe.so" );
        if( ret != 0 ){
          printf( "FAILED TO LOAD CMC LIBRARY AT ../../cmc/fullempty/WriteXE/libwritexe.so\n" );
          hmcsim_free( &hmc );
          return -1;
        }else{
          printf( "LOADED CMC LIBRARY AT ../../cmc/fullempty/WriteXE/libwritexe.so\n" );
        }

        ret = hmcsim_load_cmc( &hmc, "../../cmc/fullempty/WriteXF/libwritexf.so" );
        if( ret != 0 ){
          printf( "FAILED TO LOAD CMC LIBRARY AT ../../cmc/fullempty/WriteXF/libwritexf.so\n" );
          hmcsim_free( &hmc );
          return -1;
        }else{
          printf( "LOADED CMC LIBRARY AT ../../cmc/fullempty/WriteXF/libwritexf.so\n" );
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
