/*
 * _HMC_INIT_C_
 *
 * HYBRID MEMORY CUBE SIMULATION LIBRARY
 *
 * INITIALIZATION ROUTINES
 *
 * COPYRIGHT (c) 2018 TEXAS TECH UNIVERSITY
 * SEE LICENSE FOR DETAILS
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hmc_sim.h"



/* ----------------------------------------------------- FUNCTION PROTOTYPES */
extern int	hmcsim_allocate_memory( struct hmcsim_t *hmc );
extern int	hmcsim_free_memory( struct hmcsim_t *hmc );
extern int	hmcsim_config_devices( struct hmcsim_t *sim );
extern int	hmc_reset_device( struct hmcsim_t *hmc, uint32_t dev );
extern void     hmcsim_clear_stat( struct hmcsim_t *hmc );


/* ----------------------------------------------------- HMCSIM_INIT_TOKENS */
static void hmcsim_init_tokens( struct hmcsim_t *hmc ){
  int i = 0;
  int j = 0;

  if( hmc == NULL ){
    return ;
  }

  for( i=0; i<1024; i++ ){
    hmc->tokens[i].status  = 0;
    hmc->tokens[i].rsp     = RSP_NONE;
    hmc->tokens[i].rsp_size= 0;
    hmc->tokens[i].device  = 0;
    hmc->tokens[i].link    = 0;
    hmc->tokens[i].slot    = 0;
    hmc->tokens[i].en_clock= 0x00ull;
    for( j=0; j<256; j++ ){
      hmc->tokens[i].data[j] = 0x0;
    }
  }
}

/* ----------------------------------------------------- HMCSIM_INIT_POWER */
static void hmcsim_init_power( struct hmcsim_t *hmc ){
  int i = 0;

  if( hmc == NULL ){
    return ;
  }

  /* -- local values */
  hmc->power.link_phy           = 0.1;
  hmc->power.link_local_route   = 0.1;
  hmc->power.link_remote_route  = 0.1;
  hmc->power.xbar_rqst_slot     = 0.1;
  hmc->power.xbar_rsp_slot      = 0.1;
  hmc->power.xbar_route_extern  = 0.1;
  hmc->power.vault_rqst_slot    = 0.1;
  hmc->power.vault_rsp_slot     = 0.1;
  hmc->power.vault_ctrl         = 0.1;
  hmc->power.row_access         = 0.1;

  /* -- totals */
  hmc->power.t_link_phy           = 0.0;
  hmc->power.t_link_local_route   = 0.0;
  hmc->power.t_link_remote_route  = 0.0;
  hmc->power.t_xbar_rqst_slot     = 0.0;
  hmc->power.t_xbar_rsp_slot      = 0.0;
  hmc->power.t_xbar_route_extern  = 0.0;
  hmc->power.t_vault_rqst_slot    = 0.0;
  hmc->power.t_vault_rsp_slot     = 0.0;
  hmc->power.t_vault_ctrl         = 0.0;
  hmc->power.t_row_access         = 0.0;

  /* -- output formats */
  hmc->power.tecplot              = 0;

  /* -- tecplot values */
  hmc->power.H4L.row_access_power = 0.;
  hmc->power.H4L.row_access_btu = 0.;
  hmc->power.H8L.row_access_power = 0.;
  hmc->power.H8L.row_access_btu = 0.;

  for( i=0; i<32; i++ ){
    hmc->power.H4L.vault_rsp_power[i] = 0.;
    hmc->power.H4L.vault_rqst_power[i] = 0.;
    hmc->power.H4L.vault_ctrl_power[i] = 0.;
    hmc->power.H4L.vault_rsp_btu[i] = 0.;
    hmc->power.H4L.vault_rqst_btu[i] = 0.;
    hmc->power.H4L.vault_ctrl_btu[i] = 0.;

    hmc->power.H8L.vault_rsp_power[i] = 0.;
    hmc->power.H8L.vault_rqst_power[i] = 0.;
    hmc->power.H8L.vault_ctrl_power[i] = 0.;
    hmc->power.H8L.vault_rsp_btu[i] = 0.;
    hmc->power.H8L.vault_rqst_btu[i] = 0.;
    hmc->power.H8L.vault_ctrl_btu[i] = 0.;
  }

  for( i=0; i<4; i++ ){
    hmc->power.H4L.xbar_rqst_power[i] = 0.;
    hmc->power.H4L.xbar_rsp_power[i] = 0.;
    hmc->power.H4L.xbar_route_extern_power[i] = 0.;
    hmc->power.H4L.link_local_route_power[i] = 0.;
    hmc->power.H4L.link_remote_route_power[i] = 0.;
    hmc->power.H4L.link_phy_power[i] = 0.;

    hmc->power.H4L.xbar_rqst_btu[i] = 0.;
    hmc->power.H4L.xbar_rsp_btu[i] = 0.;
    hmc->power.H4L.xbar_route_extern_btu[i] = 0.;
    hmc->power.H4L.link_local_route_btu[i] = 0.;
    hmc->power.H4L.link_remote_route_btu[i] = 0.;
    hmc->power.H4L.link_phy_btu[i] = 0.;
  }

  for( i=0; i<8; i++ ){
    hmc->power.H8L.xbar_rqst_power[i] = 0.;
    hmc->power.H8L.xbar_rsp_power[i] = 0.;
    hmc->power.H8L.xbar_route_extern_power[i] = 0.;
    hmc->power.H8L.link_local_route_power[i] = 0.;
    hmc->power.H8L.link_remote_route_power[i] = 0.;
    hmc->power.H8L.link_phy_power[i] = 0.;

    hmc->power.H8L.xbar_rqst_btu[i] = 0.;
    hmc->power.H8L.xbar_rsp_btu[i] = 0.;
    hmc->power.H8L.xbar_route_extern_btu[i] = 0.;
    hmc->power.H8L.link_local_route_btu[i] = 0.;
    hmc->power.H8L.link_remote_route_btu[i] = 0.;
    hmc->power.H8L.link_phy_btu[i] = 0.;
  }
}

/* ----------------------------------------------------- HMCSIM_INIT_DRAM_LATENCY */
/*
 * HMCSIM_INIT_DRAM_LATENCY
 *
 */
extern int hmcsim_init_dram_latency( struct hmcsim_t *hmc,
                                     uint32_t latency ){
  if( hmc == NULL ){
    return -1;
  }
  hmc->dramlatency = latency;
  return 0;
}

/* ----------------------------------------------------- HMCSIM_INIT */
/*
 * HMCSIM_INIT
 *
 */
extern int hmcsim_init(	struct hmcsim_t *hmc,
			uint32_t num_devs,
			uint32_t num_links,
			uint32_t num_vaults,
			uint32_t queue_depth,
			uint32_t num_banks,
			uint32_t num_drams,
			uint32_t capacity,
			uint32_t xbar_depth )
{
	/* vars */
	uint32_t i	= 0;
        uint64_t j      = 0;
	/* ---- */

	/*
	 * ensure we have a good structure
	 *
	 */
	if( hmc == NULL ){
		return -1;
	}

	/*
	 * sanity check the args
	 *
	 */
	if( (num_devs > HMC_MAX_DEVS) || (num_devs < 1) ){
		return HMC_ERROR_PARAMS;
	}else if( (num_links < HMC_MIN_LINKS) || (num_links > HMC_MAX_LINKS) ){
		return HMC_ERROR_PARAMS;
	}else if( (num_vaults < HMC_MIN_VAULTS) || (num_vaults > HMC_MAX_VAULTS) ){
		return HMC_ERROR_PARAMS;
	}else if( (num_banks < HMC_MIN_BANKS) || (num_banks > HMC_MAX_BANKS) ){
		return HMC_ERROR_PARAMS;
	}else if( (num_drams < HMC_MIN_DRAMS) || (num_drams > HMC_MAX_DRAMS) ){ 
		return HMC_ERROR_PARAMS;
	}else if( (capacity < HMC_MIN_CAPACITY) || (capacity > HMC_MAX_CAPACITY) ){
		return HMC_ERROR_PARAMS;
	}else if( (queue_depth < HMC_MIN_QUEUE_DEPTH ) || (queue_depth > HMC_MAX_QUEUE_DEPTH ) ){
		return HMC_ERROR_PARAMS;
	}else if( (xbar_depth < HMC_MIN_QUEUE_DEPTH ) || (xbar_depth > HMC_MAX_QUEUE_DEPTH ) ){
		return HMC_ERROR_PARAMS;
	}

#ifdef HMC_DEBUG
	HMCSIM_PRINT_TRACE( "PASSED LEVEL1 INIT SANITY CHECK" );
#endif

	/*
	 * look deeper to make sure the default addressing works
	 * and the vault counts
	 *
	 */
	if( (num_banks != 8) && (num_banks != 16) ){
		return HMC_ERROR_PARAMS;
	}else if( (num_links != 4) && (num_links != 8) ){
		return HMC_ERROR_PARAMS;
	}else if( (num_vaults/num_links) != 8 ){
		/* always maintain 4 vaults per quad, or link */
		return HMC_ERROR_PARAMS;
	}else if( (capacity%2) != 0 ){
		return HMC_ERROR_PARAMS;
	}

#ifdef HMC_DEBUG
	HMCSIM_PRINT_TRACE( "PASSED LEVEL2 INIT SANITY CHECK" );
#endif

	/*
	 * go deeper still...
	 *
	 */
	if( (capacity == 2) && ( (num_banks == 16) || (num_links==8) ) ){
		return HMC_ERROR_PARAMS;
	}else if( (capacity == 4) && ( (num_banks == 16) && (num_links==8) ) ){
		return HMC_ERROR_PARAMS;
	}else if( (capacity == 8) && ( (num_banks == 8 ) || (num_links==4) ) ){
		return HMC_ERROR_PARAMS;
	}

#ifdef HMC_DEBUG
	HMCSIM_PRINT_TRACE( "PASSED LEVEL3 INIT SANITY CHECK" );
#endif

	/*
	 * init all the internals
	 *
 	 */
	hmc->tfile	= NULL;
	hmc->tracelevel	= 0x00;

	hmc->num_devs	= num_devs;
	hmc->num_links	= num_links;
	hmc->num_vaults	= num_vaults;
	hmc->num_banks	= num_banks;
	hmc->num_drams	= num_drams;
	hmc->capacity	= capacity;
        hmc->num_cmc    = 0x00;
        hmc->simple_link= 0;
	hmc->queue_depth= queue_depth;
	hmc->xbar_depth	= xbar_depth;
	hmc->dramlatency = HMC_DEF_DRAM_LATENCY;  // default dram latency

	hmc->clk	= 0x00ll;

	if( num_links == 4 ){
		hmc->num_quads = 4;
	}else{
		hmc->num_quads = 8;
	}

	/*
	 * pointers
	 */
	hmc->__ptr_devs			= NULL;
	hmc->__ptr_quads		= NULL;
	hmc->__ptr_vaults		= NULL;
	hmc->__ptr_banks		= NULL;
	hmc->__ptr_drams		= NULL;
	hmc->__ptr_links		= NULL;
	hmc->__ptr_xbars		= NULL;
	hmc->__ptr_stor			= NULL;
	hmc->__ptr_xbar_rqst		= NULL;
	hmc->__ptr_xbar_rsp		= NULL;
	hmc->__ptr_vault_rqst		= NULL;
	hmc->__ptr_vault_rsp		= NULL;

	/*
	 *
	 * allocate memory
	 *
	 */
	if( hmcsim_allocate_memory( hmc ) != 0 ){
		/*
		 * probably ran out of memory
		 *
		 */
#ifdef HMC_DEBUG
		HMCSIM_PRINT_TRACE( "FAILED TO ALLOCATE INTERNAL MEMORY" );
#endif

		return -1;
	}


	/*
	 * configure all the devices
	 *
	 */
	if( hmcsim_config_devices( hmc ) != 0 ){
#ifdef HMC_DEBUG
		HMCSIM_PRINT_TRACE( "FAILED TO CONFIGURE THE INTERNAL DEVICE STRUCTURE" );
#endif

		hmcsim_free_memory( hmc );
		return -1;
	}

	/*
	 * warm reset all the devices
	 *
	 */
	for( i=0; i<hmc->num_devs; i++ ) {
          /* zero all the cmc registers */
          for( j=0; j<HMC_NUM_CMC_REGS; j++ ){
            hmc->devs[i].cmc_reg[j] = 0x00ull;
          }

		//hmc_reset_device( hmc, i );
	}

        /*
         * init the power values
         *
         */
        hmcsim_init_power( hmc );

        /*
         * init the token handlers for simplified api
         *
         */
        hmcsim_init_tokens( hmc );

        /*
         * init the SST Statistics API
         *
         */
        hmcsim_clear_stat( hmc );

	return 0;
}

/* EOF */
