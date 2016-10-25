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

/* ----------------------------------------------------- FUNCTION_PROTOTYPES */
extern int      hmcsim_trace_power_vault_rsp_slot( struct hmcsim_t *hmc,
                                              uint32_t dev,
                                              uint32_t quad,
                                              uint32_t vault,
                                              uint32_t slot );
extern int      hmcsim_trace_power_vault_rqst_slot( struct hmcsim_t *hmc,
                                              uint32_t dev,
                                              uint32_t quad,
                                              uint32_t vault,
                                              uint32_t slot );
extern int hmcsim_trace_power_row_access( struct hmcsim_t *hmc,
                                    uint64_t addr,
                                    uint32_t mult );
extern int hmcsim_trace_power_xbar_rsp_slot( struct hmcsim_t *hmc,
                                        uint32_t dev,
                                        uint32_t link,
                                        uint32_t slot );
extern int hmcsim_trace_power_xbar_rqst_slot( struct hmcsim_t *hmc,
                                        uint32_t dev,
                                        uint32_t link,
                                        uint32_t slot );
extern int hmcsim_trace_power_vault_ctrl( struct hmcsim_t *hmc,
                                    uint32_t vault );
extern int hmcsim_trace_power_route_extern( struct hmcsim_t *hmc,
                                      uint32_t srcdev,
                                      uint32_t srclink,
                                      uint32_t srcslot,
                                      uint32_t destdev,
                                      uint32_t destlink,
                                      uint32_t destslot );
extern int hmcsim_trace_power_local_route( struct hmcsim_t *hmc,
                                 uint32_t dev,
                                 uint32_t link,
                                 uint32_t slot,
                                 uint32_t quad,
                                 uint32_t vault );
extern int hmcsim_trace_power_remote_route( struct hmcsim_t *hmc,
                                 uint32_t dev,
                                 uint32_t link,
                                 uint32_t slot,
                                 uint32_t quad,
                                 uint32_t vault );
extern int hmcsim_trace_power_links( struct hmcsim_t *hmc );

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

/* ----------------------------------------------------- HMCSIM_POWER_VAULT_RSP_SLOT */
extern int      hmcsim_power_vault_rsp_slot( struct hmcsim_t *hmc,
                                              uint32_t dev,
                                              uint32_t quad,
                                              uint32_t vault,
                                              uint32_t slot ){
  if( hmc == NULL ){
    return -1;
  }

  hmc->power.t_vault_rsp_slot += hmc->power.vault_rsp_slot;

  hmcsim_trace_power_vault_rsp_slot( hmc, dev, quad, vault, slot );

  return 0;
}

/* ----------------------------------------------------- HMCSIM_POWER_VAULT_RQST_SLOT */
extern int      hmcsim_power_vault_rqst_slot( struct hmcsim_t *hmc,
                                              uint32_t dev,
                                              uint32_t quad,
                                              uint32_t vault,
                                              uint32_t slot ){
  if( hmc == NULL ){
    return -1;
  }

  hmc->power.t_vault_rqst_slot += hmc->power.vault_rqst_slot;

  hmcsim_trace_power_vault_rqst_slot( hmc, dev, quad, vault, slot );

  return 0;
}

/* ----------------------------------------------------- HMCSIM_POWER_ROW_ACCESS */
extern int hmcsim_power_row_access( struct hmcsim_t *hmc,
                                    uint64_t addr,
                                    uint32_t mult ){
  if( hmc == NULL ){
    return -1;
  }

  hmc->power.t_row_access += (hmc->power.row_access*(uint64_t)(mult));

  hmcsim_trace_power_row_access( hmc, addr, mult );

  return 0;
}

/* ----------------------------------------------------- HMCSIM_POWER_XBAR_RSP_SLOT */
extern int hmcsim_power_xbar_rsp_slot( struct hmcsim_t *hmc,
                                        uint32_t dev,
                                        uint32_t link,
                                        uint32_t slot ){
  if( hmc == NULL ){
    return -1;
  }

  hmc->power.t_xbar_rsp_slot += hmc->power.xbar_rsp_slot;

  hmcsim_trace_power_xbar_rsp_slot( hmc, dev, link, slot );

  return 0;
}

/* ----------------------------------------------------- HMCSIM_POWER_XBAR_RQST_SLOT */
extern int hmcsim_power_xbar_rqst_slot( struct hmcsim_t *hmc,
                                        uint32_t dev,
                                        uint32_t link,
                                        uint32_t slot ){
  if( hmc == NULL ){
    return -1;
  }

  hmc->power.t_xbar_rqst_slot += hmc->power.xbar_rqst_slot;

  hmcsim_trace_power_xbar_rqst_slot( hmc, dev, link, slot );

  return 0;
}

/* ----------------------------------------------------- HMCSIM_POWER_VAULT_CTRL_TRANSIENT */
extern int hmcsim_power_vault_ctrl_transient( struct hmcsim_t *hmc,
                                              uint32_t vault,
                                              float p ){
  if( hmc == NULL ){
    return -1;
  }

  hmc->power.t_vault_ctrl += p;

  hmcsim_trace_power_vault_ctrl( hmc, vault );

  return 0;
}

/* ----------------------------------------------------- HMCSIM_POWER_VAULT_CTRL */
extern int hmcsim_power_vault_ctrl( struct hmcsim_t *hmc,
                                    uint32_t vault ){
  if( hmc == NULL ){
    return -1;
  }

  hmc->power.t_vault_ctrl += hmc->power.vault_ctrl;

  hmcsim_trace_power_vault_ctrl( hmc, vault );

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

  hmcsim_trace_power_route_extern( hmc, srcdev, srclink, srcslot,
                                   destdev, destlink, destslot );

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

  hmcsim_trace_power_local_route( hmc, dev, link, slot, quad, vault );

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

  hmcsim_trace_power_remote_route( hmc, dev, link, slot, quad, vault );

  return 0;
}

/* ----------------------------------------------------- HMCSIM_POWER_LINKS */
extern int hmcsim_power_links( struct hmcsim_t *hmc ){

  if( hmc == NULL ){
    return -1;
  }

  hmc->power.t_link_phy += (float)(hmc->num_devs*hmc->num_links)
                            * hmc->power.link_phy;

  hmcsim_trace_power_links( hmc );

  return 0;
}

/* EOF */
