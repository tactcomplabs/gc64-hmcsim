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

/* ----------------------------------------------------- HMCSIM_POWER_ROUTE_EXTERN */
extern int hmcsim_power_route_extern( struct hmcsim_t *hmc,
                                      uint32_t srcdev,
                                      uint32_t srclink,
                                      uint32_t srcslot,
                                      uint32_t destdev,
                                      uint32_t destlink,
                                      uint32_t destslot ){
  if( hmc == NULL ){
    return -1;
  }

  hmc->power.t_xbar_route_extern += hmc->power.xbar_route_extern;

  return 0;
}

/* ----------------------------------------------------- HMCSIM_POWER_LOCAL_ROUTE */
extern int hmcsim_power_local_route( struct hmcsim_t *hmc,
                                 uint32_t dev,
                                 uint32_t link,
                                 uint32_t slot,
                                 uint32_t quad,
                                 uint32_t vault ){
  if( hmc == NULL ){
    return -1;
  }

  hmc->power.t_link_local_route += hmc->power.link_local_route;

  return 0;
}

/* ----------------------------------------------------- HMCSIM_POWER_REMOTE_ROUTE */
extern int hmcsim_power_remote_route( struct hmcsim_t *hmc,
                                 uint32_t dev,
                                 uint32_t link,
                                 uint32_t slot,
                                 uint32_t quad,
                                 uint32_t vault ){
  if( hmc == NULL ){
    return -1;
  }

  hmc->power.t_link_remote_route += hmc->power.link_remote_route;

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
