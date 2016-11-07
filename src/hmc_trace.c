/*
 * _HMC_TRACE_C_
 *
 * HYBRID MEMORY CUBE SIMULATION LIBRARY
 *
 * HMC TRACE CONFIGURATION/EXEC FUNCTIONS
 *
 */


#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "hmc_sim.h"


/* ----------------------------------------------------- FUNCTION PROTOTYPES */
extern void hmcsim_cmc_trace_header( struct hmcsim_t *hmc );

/* ----------------------------------------------------- HMCSIM_TRACE_POWER_VAULT_RSP_SLOT */
extern int      hmcsim_trace_power_vault_rsp_slot( struct hmcsim_t *hmc,
                                                   uint32_t dev,
                                                   uint32_t quad,
                                                   uint32_t vault,
                                                   uint32_t slot ){
  if( hmc == NULL ){
    return -1;
  }

  fprintf( hmc->tfile,
           "%s%"PRIu64"%s%"PRIu32"%s%"PRIu32"%s%"PRIu32"%s%"PRIu32"%s%f\n",
           "HMCSIM_TRACE : ",
           hmc->clk,
           " : VAULT_RSP_SLOT_POWER : ",
           dev,
           ":",
           quad,
           ":",
           vault,
           ":",
           slot,
           ":",
           hmc->power.vault_rsp_slot );

  fprintf( hmc->tfile,
           "%s%"PRIu64"%s%"PRIu32"%s%"PRIu32"%s%"PRIu32"%s%"PRIu32"%s%f\n",
           "HMCSIM_TRACE : ",
           hmc->clk,
           " : VAULT_RSP_SLOT_BTU : ",
           dev,
           ":",
           quad,
           ":",
           vault,
           ":",
           slot,
           ":",
           hmc->power.vault_rsp_slot * HMC_MILLIWATT_TO_BTU );

  return 0;
}

/* ----------------------------------------------------- HMCSIM_TRACE_POWER_VAULT_RQST_SLOT */
extern int      hmcsim_trace_power_vault_rqst_slot( struct hmcsim_t *hmc,
                                                    uint32_t dev,
                                                    uint32_t quad,
                                                    uint32_t vault,
                                                    uint32_t slot ){
  if( hmc == NULL ){
    return -1;
  }

  fprintf( hmc->tfile,
           "%s%"PRIu64"%s%"PRIu32"%s%"PRIu32"%s%"PRIu32"%s%"PRIu32"%s%f\n",
           "HMCSIM_TRACE : ",
           hmc->clk,
           " : VAULT_RQST_SLOT_POWER : ",
           dev,
           ":",
           quad,
           ":",
           vault,
           ":",
           slot,
           ":",
           hmc->power.vault_rqst_slot );

  fprintf( hmc->tfile,
           "%s%"PRIu64"%s%"PRIu32"%s%"PRIu32"%s%"PRIu32"%s%"PRIu32"%s%f\n",
           "HMCSIM_TRACE : ",
           hmc->clk,
           " : VAULT_RQST_SLOT_BTU : ",
           dev,
           ":",
           quad,
           ":",
           vault,
           ":",
           slot,
           ":",
           hmc->power.vault_rqst_slot * HMC_MILLIWATT_TO_BTU );

  return 0;
}

/* ----------------------------------------------------- HMCSIM_TRACE_POWER_ROW_ACCESS */
extern int hmcsim_trace_power_row_access( struct hmcsim_t *hmc,
                                          uint64_t addr,
                                          uint32_t mult ){
  if( hmc == NULL ){
    return -1;
  }

  fprintf( hmc->tfile,
           "%s%"PRIu64"%s0x016%"PRIx64"%s%f\n",
           "HMCSIM_TRACE : ",
           hmc->clk,
           " : ROW_ACCESS_POWER : ",
           addr,
           ":",
           hmc->power.row_access * (float)(mult) );

  fprintf( hmc->tfile,
           "%s%"PRIu64"%s0x016%"PRIx64"%s%f\n",
           "HMCSIM_TRACE : ",
           hmc->clk,
           " : ROW_ACCESS_BTU : ",
           addr,
           ":",
           hmc->power.row_access * (float)(mult) * HMC_MILLIWATT_TO_BTU );

  return 0;
}

/* ----------------------------------------------------- HMCSIM_TRACE_POWER_XBAR_RSP_SLOT */
extern int hmcsim_trace_power_xbar_rsp_slot( struct hmcsim_t *hmc,
                                             uint32_t dev,
                                             uint32_t link,
                                             uint32_t slot ){
  if( hmc == NULL ){
    return -1;
  }

  fprintf( hmc->tfile,
           "%s%"PRIu64"%s%"PRIu32"%s%"PRIu32"%s%"PRIu32"%s%f\n",
           "HMCSIM_TRACE : ",
           hmc->clk,
           " : XBAR_RSP_SLOT_POWER : ",
           dev,
           ":",
           link,
           ":",
           slot,
           ":",
           hmc->power.xbar_rsp_slot);

  fprintf( hmc->tfile,
           "%s%"PRIu64"%s%"PRIu32"%s%"PRIu32"%s%"PRIu32"%s%f\n",
           "HMCSIM_TRACE : ",
           hmc->clk,
           " : XBAR_RSP_SLOT_BTU : ",
           dev,
           ":",
           link,
           ":",
           slot,
           ":",
           hmc->power.xbar_rsp_slot * HMC_MILLIWATT_TO_BTU );

  return 0;
}

/* ----------------------------------------------------- HMCSIM_TRACE_POWER_XBAR_RQST_SLOT */
extern int hmcsim_trace_power_xbar_rqst_slot( struct hmcsim_t *hmc,
                                              uint32_t dev,
                                              uint32_t link,
                                              uint32_t slot ){
  if( hmc == NULL ){
    return -1;
  }

  fprintf( hmc->tfile,
           "%s%"PRIu64"%s%"PRIu32"%s%"PRIu32"%s%"PRIu32"%s%f\n",
           "HMCSIM_TRACE : ",
           hmc->clk,
           " : XBAR_RQST_SLOT_POWER : ",
           dev,
           ":",
           link,
           ":",
           slot,
           ":",
           hmc->power.xbar_rqst_slot);

  fprintf( hmc->tfile,
           "%s%"PRIu64"%s%"PRIu32"%s%"PRIu32"%s%"PRIu32"%s%f\n",
           "HMCSIM_TRACE : ",
           hmc->clk,
           " : XBAR_RQST_SLOT_BTU : ",
           dev,
           ":",
           link,
           ":",
           slot,
           ":",
           hmc->power.xbar_rqst_slot * HMC_MILLIWATT_TO_BTU );

  return 0;
}

/* ----------------------------------------------------- HMCSIM_TRACE_POWER_VAULT_CTRL */
extern int hmcsim_trace_power_vault_ctrl( struct hmcsim_t *hmc,
                                          uint32_t vault ){
  if( hmc == NULL ){
    return -1;
  }

  fprintf( hmc->tfile,
           "%s%"PRIu64"%s%"PRIu32"%s%f\n",
           "HMCSIM_TRACE : ",
           hmc->clk,
           " : VAULT_CTRL_POWER : ",
           vault,
           ":",
           hmc->power.vault_ctrl);

  fprintf( hmc->tfile,
           "%s%"PRIu64"%s%"PRIu32"%s%f\n",
           "HMCSIM_TRACE : ",
           hmc->clk,
           " : VAULT_CTRL_BTU : ",
           vault,
           ":",
           hmc->power.vault_ctrl * HMC_MILLIWATT_TO_BTU );

  return 0;
}

/* ----------------------------------------------------- HMCSIM_TRACE_POWER_ROUTE_EXTERN */
extern int hmcsim_trace_power_route_extern( struct hmcsim_t *hmc,
                                            uint32_t srcdev,
                                            uint32_t srclink,
                                            uint32_t srcslot,
                                            uint32_t destdev,
                                            uint32_t destlink,
                                            uint32_t destslot ){
  if( hmc == NULL ){
    return -1;
  }

  fprintf( hmc->tfile,
           "%s%"PRIu64"%s%"PRIu32"%s%"PRIu32"%s%"PRIu32"%s%"PRIu32"%s%"PRIu32"%s%"PRIu32"%s%f\n",
           "HMCSIM_TRACE : ",
           hmc->clk,
           " : XBAR_ROUTE_EXTERN_POWER : ",
           srcdev,
           ":",
           srclink,
           ":",
           srcslot,
           ":",
           destdev,
           ":",
           destlink,
           ":",
           destslot,
           ":",
           hmc->power.xbar_rsp_slot);

  fprintf( hmc->tfile,
           "%s%"PRIu64"%s%"PRIu32"%s%"PRIu32"%s%"PRIu32"%s%"PRIu32"%s%"PRIu32"%s%"PRIu32"%s%f\n",
           "HMCSIM_TRACE : ",
           hmc->clk,
           " : XBAR_ROUTE_EXTERN_BTU : ",
           srcdev,
           ":",
           srclink,
           ":",
           srcslot,
           ":",
           destdev,
           ":",
           destlink,
           ":",
           destslot,
           ":",
           hmc->power.xbar_rsp_slot * HMC_MILLIWATT_TO_BTU );

  return 0;
}

/* ----------------------------------------------------- HMCSIM_TRACE_POWER_LOCAL_ROUTE */
extern int hmcsim_trace_power_local_route( struct hmcsim_t *hmc,
                                           uint32_t dev,
                                           uint32_t link,
                                           uint32_t slot,
                                           uint32_t quad,
                                           uint32_t vault ){
  if( hmc == NULL ){
    return -1;
  }

  fprintf( hmc->tfile,
           "%s%"PRIu64"%s%"PRIu32"%s%"PRIu32"%s%"PRIu32"%s%"PRIu32"%s%"PRIu32"%s%f\n",
           "HMCSIM_TRACE : ",
           hmc->clk,
           " : LINK_LOCAL_ROUTE_POWER : ",
           dev,
           ":",
           link,
           ":",
           quad,
           ":",
           vault,
           ":",
           slot,
           ":",
           hmc->power.link_local_route);

  fprintf( hmc->tfile,
           "%s%"PRIu64"%s%"PRIu32"%s%"PRIu32"%s%"PRIu32"%s%"PRIu32"%s%"PRIu32"%s%f\n",
           "HMCSIM_TRACE : ",
           hmc->clk,
           " : LINK_LOCAL_ROUTE_BTU : ",
           dev,
           ":",
           link,
           ":",
           quad,
           ":",
           vault,
           ":",
           slot,
           ":",
           hmc->power.link_local_route * HMC_MILLIWATT_TO_BTU );

  return 0;
}

/* ----------------------------------------------------- HMCSIM_TRACE_POWER_REMOTE_ROUTE */
extern int hmcsim_trace_power_remote_route( struct hmcsim_t *hmc,
                                            uint32_t dev,
                                            uint32_t link,
                                            uint32_t slot,
                                            uint32_t quad,
                                            uint32_t vault ){
  if( hmc == NULL ){
    return -1;
  }

  fprintf( hmc->tfile,
           "%s%"PRIu64"%s%"PRIu32"%s%"PRIu32"%s%"PRIu32"%s%"PRIu32"%s%"PRIu32"%s%f\n",
           "HMCSIM_TRACE : ",
           hmc->clk,
           " : LINK_REMOTE_ROUTE_POWER : ",
           dev,
           ":",
           link,
           ":",
           quad,
           ":",
           vault,
           ":",
           slot,
           ":",
           hmc->power.link_remote_route);

  fprintf( hmc->tfile,
           "%s%"PRIu64"%s%"PRIu32"%s%"PRIu32"%s%"PRIu32"%s%"PRIu32"%s%"PRIu32"%s%f\n",
           "HMCSIM_TRACE : ",
           hmc->clk,
           " : LINK_REMOTE_ROUTE_BTU : ",
           dev,
           ":",
           link,
           ":",
           quad,
           ":",
           vault,
           ":",
           slot,
           ":",
           hmc->power.link_remote_route * HMC_MILLIWATT_TO_BTU );

  return 0;
}

/* ----------------------------------------------------- HMCSIM_TRACE_POWER_LINKS */
extern int hmcsim_trace_power_links( struct hmcsim_t *hmc ){
  uint32_t i = 0;
  uint32_t j = 0;
  if( hmc == NULL ){
    return -1;
  }

  for( j=0; j<hmc->num_devs; j++ ){

    for( i=0; i<hmc->num_links; i++ ){
      fprintf( hmc->tfile,
             "%s%"PRIu64"%s%"PRIu32"%s%"PRIu32"%s%f\n",
             "HMCSIM_TRACE : ",
             hmc->clk,
             " : LINK_PHY_POWER : ",
             j,/*dev*/
             ":",
             i,/*link*/
             ":",
             hmc->power.link_phy );
      fprintf( hmc->tfile,
             "%s%"PRIu64"%s%"PRIu32"%s%"PRIu32"%s%f\n",
             "HMCSIM_TRACE : ",
             hmc->clk,
             " : LINK_PHY_BTU : ",
             j,/*dev*/
             ":",
             i,/*link*/
             ":",
             hmc->power.link_phy * HMC_MILLIWATT_TO_BTU );
    }
  }

  return 0;
}


/* ----------------------------------------------------- HMCSIM_TRACE_POWER */
/*
 * HMCSIM_TRACE_POWER
 *
 */
extern int      hmcsim_trace_power( struct hmcsim_t *hmc ){
  if( hmc == NULL ){
    return -1;
  }else if( hmc->tfile == NULL ){
    return -1;
  }

  /*
   * write out the total trace values for power on this clock cycle
   *
   */
    fprintf( hmc->tfile, "%s%"PRIu64"%s%f\n",
                         "HMCSIM_TRACE : ",
                         hmc->clk,
                         " : T_LINK_PHY_POWER : ",
                         hmc->power.t_link_phy );
    fprintf( hmc->tfile, "%s%"PRIu64"%s%f\n",
                         "HMCSIM_TRACE : ",
                         hmc->clk,
                         " : T_LINK_LOCAL_ROUTE_POWER : ",
                         hmc->power.t_link_local_route );
    fprintf( hmc->tfile, "%s%"PRIu64"%s%f\n",
                         "HMCSIM_TRACE : ",
                         hmc->clk,
                         " : T_LINK_REMOTE_ROUTE_POWER : ",
                         hmc->power.t_link_remote_route );
    fprintf( hmc->tfile, "%s%"PRIu64"%s%f\n",
                         "HMCSIM_TRACE : ",
                         hmc->clk,
                         " : T_XBAR_RQST_SLOT_POWER : ",
                         hmc->power.t_xbar_rqst_slot);
    fprintf( hmc->tfile, "%s%"PRIu64"%s%f\n",
                         "HMCSIM_TRACE : ",
                         hmc->clk,
                         " : T_XBAR_RSP_SLOT_POWER : ",
                         hmc->power.t_xbar_rsp_slot);
    fprintf( hmc->tfile, "%s%"PRIu64"%s%f\n",
                         "HMCSIM_TRACE : ",
                         hmc->clk,
                         " : T_XBAR_ROUTE_EXTERN : ",
                         hmc->power.t_xbar_route_extern);
    fprintf( hmc->tfile, "%s%"PRIu64"%s%f\n",
                         "HMCSIM_TRACE : ",
                         hmc->clk,
                         " : T_VAULT_RQST_SLOT_POWER : ",
                         hmc->power.t_vault_rqst_slot);
    fprintf( hmc->tfile, "%s%"PRIu64"%s%f\n",
                         "HMCSIM_TRACE : ",
                         hmc->clk,
                         " : T_VAULT_RSP_SLOT_POWER : ",
                         hmc->power.t_vault_rsp_slot);
    fprintf( hmc->tfile, "%s%"PRIu64"%s%f\n",
                         "HMCSIM_TRACE : ",
                         hmc->clk,
                         " : T_VAULT_CTRL_POWER : ",
                         hmc->power.t_vault_ctrl);
    fprintf( hmc->tfile, "%s%"PRIu64"%s%f\n",
                         "HMCSIM_TRACE : ",
                         hmc->clk,
                         " : T_ROW_ACCESS_POWER : ",
                         hmc->power.t_row_access);

    /* write out the thermal totals */
    fprintf( hmc->tfile, "%s%"PRIu64"%s%f\n",
                         "HMCSIM_TRACE : ",
                         hmc->clk,
                         " : T_LINK_PHY_BTU : ",
                         hmc->power.t_link_phy * HMC_MILLIWATT_TO_BTU );
    fprintf( hmc->tfile, "%s%"PRIu64"%s%f\n",
                         "HMCSIM_TRACE : ",
                         hmc->clk,
                         " : T_LINK_LOCAL_ROUTE_BTU : ",
                         hmc->power.t_link_local_route * HMC_MILLIWATT_TO_BTU );
    fprintf( hmc->tfile, "%s%"PRIu64"%s%f\n",
                         "HMCSIM_TRACE : ",
                         hmc->clk,
                         " : T_LINK_REMOTE_ROUTE_BTU : ",
                         hmc->power.t_link_remote_route * HMC_MILLIWATT_TO_BTU );
    fprintf( hmc->tfile, "%s%"PRIu64"%s%f\n",
                         "HMCSIM_TRACE : ",
                         hmc->clk,
                         " : T_XBAR_RQST_SLOT_BTU : ",
                         hmc->power.t_xbar_rqst_slot* HMC_MILLIWATT_TO_BTU );
    fprintf( hmc->tfile, "%s%"PRIu64"%s%f\n",
                         "HMCSIM_TRACE : ",
                         hmc->clk,
                         " : T_XBAR_RSP_SLOT_BTU : ",
                         hmc->power.t_xbar_rsp_slot* HMC_MILLIWATT_TO_BTU );
    fprintf( hmc->tfile, "%s%"PRIu64"%s%f\n",
                         "HMCSIM_TRACE : ",
                         hmc->clk,
                         " : T_XBAR_ROUTE_EXTERN : ",
                         hmc->power.t_xbar_route_extern* HMC_MILLIWATT_TO_BTU );
    fprintf( hmc->tfile, "%s%"PRIu64"%s%f\n",
                         "HMCSIM_TRACE : ",
                         hmc->clk,
                         " : T_VAULT_RQST_SLOT_BTU : ",
                         hmc->power.t_vault_rqst_slot* HMC_MILLIWATT_TO_BTU );
    fprintf( hmc->tfile, "%s%"PRIu64"%s%f\n",
                         "HMCSIM_TRACE : ",
                         hmc->clk,
                         " : T_VAULT_RSP_SLOT_BTU : ",
                         hmc->power.t_vault_rsp_slot* HMC_MILLIWATT_TO_BTU );
    fprintf( hmc->tfile, "%s%"PRIu64"%s%f\n",
                         "HMCSIM_TRACE : ",
                         hmc->clk,
                         " : T_VAULT_CTRL_BTU : ",
                         hmc->power.t_vault_ctrl* HMC_MILLIWATT_TO_BTU );
    fprintf( hmc->tfile, "%s%"PRIu64"%s%f\n",
                         "HMCSIM_TRACE : ",
                         hmc->clk,
                         " : T_ROW_ACCESS_BTU : ",
                         hmc->power.t_row_access* HMC_MILLIWATT_TO_BTU );


  return 0;
}

/* ----------------------------------------------------- HMCSIM_TRACE_POWER_HEADER */
static void     hmcsim_trace_power_header( struct hmcsim_t *hmc ){
  if( hmc == NULL ){
    return ;
  }

  fprintf( hmc->tfile, "%s%f\n", "# LINK_PHY           = ", hmc->power.link_phy );
  fprintf( hmc->tfile, "%s%f\n", "# LINK_LOCAL_ROUTE   = ", hmc->power.link_local_route );
  fprintf( hmc->tfile, "%s%f\n", "# LINK_REMOTE_ROUTE  = ", hmc->power.link_remote_route );
  fprintf( hmc->tfile, "%s%f\n", "# XBAR_RQST_SLOT     = ", hmc->power.xbar_rqst_slot );
  fprintf( hmc->tfile, "%s%f\n", "# XBAR_RSP_SLOT      = ", hmc->power.xbar_rsp_slot );
  fprintf( hmc->tfile, "%s%f\n", "# XBAR_ROUTE_EXTERN  = ", hmc->power.xbar_route_extern );
  fprintf( hmc->tfile, "%s%f\n", "# VAULT_RQST_SLOT    = ", hmc->power.vault_rqst_slot );
  fprintf( hmc->tfile, "%s%f\n", "# VAULT_RSP_SLOT     = ", hmc->power.vault_rsp_slot );
  fprintf( hmc->tfile, "%s%f\n", "# VAULT_CTRL         = ", hmc->power.vault_ctrl );
  fprintf( hmc->tfile, "%s%f\n", "# ROW_ACCESS         = ", hmc->power.row_access );
  fprintf( hmc->tfile, "%s\n", 	"#---------------------------------------------------------" );

  return ;
}

/* ----------------------------------------------------- HMCSIM_TRACE_HEADER */
/*
 * HMCSIM_TRACE_HEADER
 *
 */
extern int	hmcsim_trace_header( struct hmcsim_t *hmc )
{
        int major = HMC_MAJOR_VERSION;
        int minor = HMC_MINOR_VERSION;
        char text[100];
        time_t now = time(NULL);
        struct tm *t = localtime(&now);

	if( hmc->tfile == NULL ){
		return -1;
	}

        /*
         * get the date+time combo
         *
         */
        strftime(text, sizeof(text)-1, "%d/%m/%Y %H:%M", t );

	/*
	 * write all the necessary sim data to the trace file
	 * as a large comment block
	 *
	 */
	fprintf( hmc->tfile, "%s\n", 	"#---------------------------------------------------------" );
	fprintf( hmc->tfile, "%s%d%s%d\n", 	"# HMC-SIM VERSION : ",major,".",minor ); 
	fprintf( hmc->tfile, "%s%s\n", 	"# DATE: ", text );
	fprintf( hmc->tfile, "%s\n", 	"#---------------------------------------------------------" );
	fprintf( hmc->tfile, "%s%d\n",	"# HMC_NUM_DEVICES       = ", hmc->num_devs );
	fprintf( hmc->tfile, "%s%d\n",  "# HMC_NUM_LINKS         = ", hmc->num_links );
	fprintf( hmc->tfile, "%s%d\n",  "# HMC_NUM_QUADS         = ", hmc->num_quads );
	fprintf( hmc->tfile, "%s%d\n",  "# HMC_NUM_VAULTS        = ", hmc->num_vaults );
	fprintf( hmc->tfile, "%s%d\n",  "# HMC_NUM_BANKS         = ", hmc->num_banks );
	fprintf( hmc->tfile, "%s%d\n",  "# HMC_NUM_DRAMS         = ", hmc->num_drams );
	fprintf( hmc->tfile, "%s%d\n",  "# HMC_CAPACITY_GB       = ", hmc->capacity );
	fprintf( hmc->tfile, "%s%d\n",  "# HMC_VAULT_QUEUE_DEPTH = ", hmc->queue_depth );
	fprintf( hmc->tfile, "%s%d\n",  "# HMC_XBAR_QUEUE_DEPTH  = ", hmc->xbar_depth );
	fprintf( hmc->tfile, "%s\n", 	"#---------------------------------------------------------" );

        /* print the power info */
        hmcsim_trace_power_header(hmc);

        /* print the cmc info */
        hmcsim_cmc_trace_header(hmc);

        fflush( hmc->tfile );

	return 0;
}

/* ----------------------------------------------------- HMCSIM_TRACE_STALL */
/*
 * HMCSIM_TRACE_STALL
 *
 */
extern int	hmcsim_trace_stall( 	struct hmcsim_t *hmc,
					uint32_t dev,
					uint32_t quad,
					uint32_t vault,
					uint32_t src,
					uint32_t dest,
					uint32_t link,
					uint32_t slot,
					uint32_t type )
{
	if( hmc->tfile == NULL ){
		return -1;
	}

	/*
	 * Determine which stall type
 	 *
	 */
	if( type == 0 ){

		/*
		 * xbar stall
	 	 *
		 */

		fprintf( hmc->tfile, 	"HMCSIM_TRACE : %" PRIu64
					" : XBAR_RQST_STALL"
					" : %" PRIu32
					":%" PRIu32
					":%" PRIu32
					":%" PRIu32 "\n",
					hmc->clk,
					dev,
					quad,
					vault,
					slot );

	} else if( type == 1 ){

		/*
	 	 * vault request stall
		 *
		 */

		fprintf( hmc->tfile, 	"HMCSIM_TRACE : %" PRIu64
					" : VAULT_RQST_STALL"
					" : %" PRIu32
					":%" PRIu32
					":%" PRIu32
					":%" PRIu32 "\n",
					hmc->clk,
					dev,
					quad,
					vault,
					slot );


	} else if( type == 2 ){

		/*
		 * xbar response stall
	 	 *
		 */

		fprintf( hmc->tfile, 	"HMCSIM_TRACE : %" PRIu64
					" : XBAR_RSP_STALL"
					" : %" PRIu32
					":%" PRIu32
					":%" PRIu32
					":%" PRIu32 "\n",
					hmc->clk,
					dev,
					quad,
					vault,
					slot );

	} else if( type == 3 ){

		/*
		 * device request route stall
	 	 *
		 */

		fprintf( hmc->tfile, 	"HMCSIM_TRACE : %" PRIu64
					" : ROUTE_RQST_STALL"
					" : %" PRIu32
					":%" PRIu32
					":%" PRIu32
					":%" PRIu32
					":%" PRIu32 "\n",
					hmc->clk,
					dev,
					src,
					dest,
					link,
					slot );

	}else if( type == 4 ){

		/*
		 * device response route stall
	 	 *
		 */

		fprintf( hmc->tfile, 	"HMCSIM_TRACE : %" PRIu64
					" : ROUTE_RSP_STALL"
					" : %" PRIu32
					":%" PRIu32
					":%" PRIu32
					":%" PRIu32
					":%" PRIu32 "\n",
					hmc->clk,
					dev,
					src,
					dest,
					link,
					slot );

	}else {

		/*
		 * undefined stall event
		 *
		 */

		fprintf( hmc->tfile, 	"HMCSIM_TRACE : %" PRIu64
					" : UNDEF_STALL"
					" : %" PRIu32
					":%" PRIu32
					":%" PRIu32
					":%" PRIu32 "\n",
					hmc->clk,
					dev,
					quad,
					vault,
					slot );

	}

	return 0;
}

/* ----------------------------------------------------- HMCSIM_TRACE_RQST */
/*
 * HMCSIM_TRACE_RQST
 *
 */
extern int	hmcsim_trace_rqst( 	struct hmcsim_t *hmc,
					char *rqst,
					uint32_t dev,
					uint32_t quad,
					uint32_t vault,
					uint32_t bank,
					uint64_t addr1,
					uint32_t size )
{
	if( hmc->tfile == NULL ){
		return -1;
	}

	fprintf( hmc->tfile, 	"HMCSIM_TRACE : %" PRIu64
				" : %s"
				" : %" PRIu32
				":%" PRIu32
				":%" PRIu32
				":%" PRIu32
				":0x%016" PRIx64
				":%" PRIu32 "\n",
				hmc->clk,
				rqst,
				dev,
				quad,
				vault,
				bank,
				addr1,
				size );

	return 0;

}
/* ----------------------------------------------------- HMCSIM_TRACE_BANK_CONFLICT */
/*
 * HMCSIM_TRACE_BANK_CONFLICT
 *
 */
extern int	hmcsim_trace_bank_conflict( struct hmcsim_t *hmc,
						uint32_t dev,
						uint32_t quad,
						uint32_t vault,
						uint32_t bank,
						uint64_t addr1 )
{
	if( hmc->tfile == NULL ){
		return -1;
	}

	fprintf( hmc->tfile, 	"HMCSIM_TRACE : %" PRIu64
				" : BANK_CONFLICT : %" PRIu32
				":%" PRIu32
				":%" PRIu32
				":%" PRIu32
				":0x%016" PRIx64 "\n",
				hmc->clk,
				dev,
				quad,
				vault,
				bank,
				addr1 );

	return 0;

}

/* ----------------------------------------------------- HMCSIM_TRACE_LATENCY */
/*
 * HMCSIM_TRACE_LATENCY
 *
 */
extern int	hmcsim_trace_latency( 	struct hmcsim_t *hmc,
					uint32_t dev,
					uint32_t link,
					uint32_t slot,
					uint32_t quad,
					uint32_t vault )
{
	if( hmc->tfile == NULL ){
		return -1;
	}

	fprintf( hmc->tfile, 	"HMCSIM_TRACE : %" PRIu64
				" : XBAR_LATENCY : %" PRIu32
				":%" PRIu32
				":%" PRIu32
				":%" PRIu32
				":%" PRIu32 "\n",
				hmc->clk,
				dev,
				link,
				slot,
				quad,
				vault );

	return 0;
}

/* ----------------------------------------------------- HMCSIM_TRACE */
/*
 * HMCSIM_TRACE
 *
 */
extern int	hmcsim_trace( struct hmcsim_t *hmc, char *str )
{
	if( hmc->tfile == NULL ){
		return -1;
	}

	fprintf( hmc->tfile, 	"HMCSIM_TRACE : %" PRIu64
				" : %s\n",
				hmc->clk,
				str );

	return 0;
}

/* ----------------------------------------------------- HMCSIM_TRACE_LEVEL */
/*
 * HMCSIM_TRACE_LEVEL
 *
 */
extern int	hmcsim_trace_level( struct hmcsim_t *hmc, uint32_t level )
{
	if( hmc == NULL ){
		return -1;
	}

	hmc->tracelevel	= level;

	return 0;
}

/* ----------------------------------------------------- HMCSIM_TRACE_HANDLE */
/*
 * HMCSIM_TRACE_HANDLE
 *
 */
extern int	hmcsim_trace_handle( struct hmcsim_t *hmc, FILE *tfile )
{
	if( hmc == NULL ){
		return -1;
	}

	if( tfile == NULL ){
		return -1;
	}

	hmc->tfile = tfile;

	return 0;
}

/* EOF */
