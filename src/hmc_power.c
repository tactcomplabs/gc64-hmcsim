/*
 * _HMC_POWER_C_
 *
 * HYBRID MEMORY CUBE SIMULATION LIBRARY
 *
 * POWER CALCULATION ROUTINES
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hmc_sim.h"

/* ----------------------------------------------------- HMCSIM_POWER_CLEAR */
extern int hmcsim_power_clear( struct hmcsim_t *hmc ){
  if( hmc == NULL ){
    return -1;
  }

  hmc->power.t_link_phy = 0.;
  hmc->power.t_link_local_route = 0.;
  hmc->power.t_link_remote_route = 0.;
  hmc->power.t_xbar_rqst_slot = 0.;
  hmc->power.t_xbar_rsp_slot = 0.;
  hmc->power.t_xbar_route_extern = 0.;
  hmc->power.t_vault_rqst_slot = 0.;
  hmc->power.t_vault_rsp_slot = 0.;
  hmc->power.t_vault_ctrl = 0.;
  hmc->power.t_row_access = 0.;

  return 0;
}

/* ----------------------------------------------------- HMCSIM_POWER_LINKS */
extern int hmcsim_power_links( struct hmcsim_t *hmc ){

  if( hmc == NULL ){
    return -1;
  }

  hmc->power.t_link_phy += (float)(hmc->num_devs*hmc->num_links)
                            * hmc->power.link_phy;

  return 0;
}

/* EOF */
