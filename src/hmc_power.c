/*
 * _HMC_POWER_C_
 *
 * HYBRID MEMORY CUBE SIMULATION LIBRARY
 *
 * POWER CALCULATION ROUTINES
 *
 * COPYRIGHT (c) 2018 TEXAS TECH UNIVERSITY
 * SEE LICENSE FOR DETAILS
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

  if( hmc->num_links == 4 ){
    hmc->power.H4L.vault_rsp_power[quad*vault] += hmc->power.vault_rsp_slot;
    hmc->power.H4L.vault_rsp_btu[quad*vault] += (hmc->power.vault_rsp_slot*HMC_MILLIWATT_TO_BTU);
  }else{
    hmc->power.H8L.vault_rsp_power[quad*vault] += hmc->power.vault_rsp_slot;
    hmc->power.H8L.vault_rsp_btu[quad*vault] += (hmc->power.vault_rsp_slot*HMC_MILLIWATT_TO_BTU);
  }

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

  if( hmc->num_links == 4 ){
    hmc->power.H4L.vault_rqst_power[quad*vault] += hmc->power.vault_rqst_slot;
    hmc->power.H4L.vault_rqst_btu[quad*vault] += (hmc->power.vault_rqst_slot*HMC_MILLIWATT_TO_BTU);
  }else{
    hmc->power.H8L.vault_rqst_power[quad*vault] += hmc->power.vault_rqst_slot;
    hmc->power.H8L.vault_rqst_btu[quad*vault] += (hmc->power.vault_rqst_slot*HMC_MILLIWATT_TO_BTU);
  }

  return 0;
}

/* ----------------------------------------------------- HMCSIM_POWER_ROW_ACCESS */
extern int hmcsim_power_row_access( struct hmcsim_t *hmc,
                                    uint64_t addr,
                                    uint32_t mult ){
  if( hmc == NULL ){
    return -1;
  }

  hmc->power.t_row_access += (hmc->power.row_access*(float)(mult));

  hmcsim_trace_power_row_access( hmc, addr, mult );

  if( hmc->num_links == 4 ){
    hmc->power.H4L.row_access_power += (hmc->power.row_access*(float)(mult));
    hmc->power.H4L.row_access_btu += ((hmc->power.row_access*(float)(mult))*HMC_MILLIWATT_TO_BTU);
  }else{
    hmc->power.H8L.row_access_power += (hmc->power.row_access*(float)(mult));
    hmc->power.H8L.row_access_btu += ((hmc->power.row_access*(float)(mult))*HMC_MILLIWATT_TO_BTU);
  }

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

  if( hmc->num_links == 4 ){
    hmc->power.H4L.xbar_rsp_power[link] += hmc->power.xbar_rsp_slot;
    hmc->power.H4L.xbar_rsp_btu[link] += (hmc->power.xbar_rsp_slot*HMC_MILLIWATT_TO_BTU);
  }else{
    hmc->power.H8L.xbar_rsp_power[link] += hmc->power.xbar_rsp_slot;
    hmc->power.H8L.xbar_rsp_btu[link] += (hmc->power.xbar_rsp_slot*HMC_MILLIWATT_TO_BTU);
  }

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

  if( hmc->num_links == 4 ){
    hmc->power.H4L.xbar_rqst_power[link] += hmc->power.xbar_rqst_slot;
    hmc->power.H4L.xbar_rqst_btu[link] += (hmc->power.xbar_rqst_slot*HMC_MILLIWATT_TO_BTU);
  }else{
    hmc->power.H8L.xbar_rqst_power[link] += hmc->power.xbar_rqst_slot;
    hmc->power.H8L.xbar_rqst_btu[link] += (hmc->power.xbar_rqst_slot*HMC_MILLIWATT_TO_BTU);
  }

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

  if( hmc->num_links == 4 ){
    hmc->power.H4L.vault_ctrl_power[vault] += p;
    hmc->power.H4L.vault_ctrl_btu[vault] += (p*HMC_MILLIWATT_TO_BTU);
  }else{
    hmc->power.H8L.vault_ctrl_power[vault] += p;
    hmc->power.H8L.vault_ctrl_btu[vault] += (p*HMC_MILLIWATT_TO_BTU);
  }

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

  if( hmc->num_links == 4 ){
    hmc->power.H4L.vault_ctrl_power[vault] += hmc->power.vault_ctrl;
    hmc->power.H4L.vault_ctrl_btu[vault] += (hmc->power.vault_ctrl*HMC_MILLIWATT_TO_BTU);
  }else{
    hmc->power.H8L.vault_ctrl_power[vault] += hmc->power.vault_ctrl;
    hmc->power.H8L.vault_ctrl_btu[vault] += (hmc->power.vault_ctrl*HMC_MILLIWATT_TO_BTU);
  }

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

  if( hmc->num_links == 4 ){
    hmc->power.H4L.xbar_route_extern_power[srclink] += hmc->power.xbar_route_extern;
    hmc->power.H4L.xbar_route_extern_btu[srclink] += (hmc->power.xbar_route_extern*HMC_MILLIWATT_TO_BTU);
  }else{
    hmc->power.H8L.xbar_route_extern_power[srclink] += hmc->power.xbar_route_extern;
    hmc->power.H8L.xbar_route_extern_btu[srclink] += (hmc->power.xbar_route_extern*HMC_MILLIWATT_TO_BTU);
  }

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

  if( hmc->num_links == 4 ){
    hmc->power.H4L.link_local_route_power[link] += hmc->power.link_local_route;
    hmc->power.H4L.link_local_route_btu[link] += (hmc->power.link_local_route*HMC_MILLIWATT_TO_BTU);
  }else{
    hmc->power.H8L.link_local_route_power[link] += hmc->power.link_local_route;
    hmc->power.H8L.link_local_route_btu[link] += (hmc->power.link_local_route*HMC_MILLIWATT_TO_BTU);
  }

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

  if( hmc->num_links == 4 ){
    hmc->power.H4L.link_remote_route_power[link] += hmc->power.link_remote_route;
    hmc->power.H4L.link_remote_route_btu[link] += (hmc->power.link_remote_route*HMC_MILLIWATT_TO_BTU);
  }else{
    hmc->power.H8L.link_remote_route_power[link] += hmc->power.link_remote_route;
    hmc->power.H8L.link_remote_route_btu[link] += (hmc->power.link_remote_route*HMC_MILLIWATT_TO_BTU);
  }

  return 0;
}

/* ----------------------------------------------------- HMCSIM_POWER_LINKS */
extern int hmcsim_power_links( struct hmcsim_t *hmc ){

  int i = 0;

  if( hmc == NULL ){
    return -1;
  }

  hmc->power.t_link_phy += (float)(hmc->num_devs*hmc->num_links)
                            * hmc->power.link_phy;

  hmcsim_trace_power_links( hmc );

  if( hmc->num_links == 4 ){
    for( i=0; i<4; i++ ){
      hmc->power.H4L.link_phy_power[i] += hmc->power.link_phy;
      hmc->power.H4L.link_phy_btu[i] += (hmc->power.link_phy*HMC_MILLIWATT_TO_BTU);
    }
  }else{
    for( i=0; i<8; i++ ){
      hmc->power.H8L.link_phy_power[i] += hmc->power.link_phy;
      hmc->power.H8L.link_phy_btu[i] += (hmc->power.link_phy*HMC_MILLIWATT_TO_BTU);
    }
  }

  return 0;
}

/* ----------------------------------------------------- HMCSIM_POWER_CONFIG */
extern int hmcsim_power_config( struct hmcsim_t *hmc,
                                float link_phy,
                                float link_local_route,
                                float link_remote_route,
                                float xbar_rqst_slot,
                                float xbar_rsp_slot,
                                float xbar_route_extern,
                                float vault_rqst_slot,
                                float vault_rsp_slot,
                                float vault_ctrl,
                                float row_access ){
  if( hmc == NULL ){
    return -1;
  }
  hmc->power.link_phy = link_phy;
  hmc->power.link_local_route = link_local_route;
  hmc->power.link_remote_route = link_remote_route;
  hmc->power.xbar_rqst_slot = xbar_rqst_slot;
  hmc->power.xbar_rsp_slot = xbar_rsp_slot;
  hmc->power.xbar_route_extern = xbar_route_extern;
  hmc->power.vault_rqst_slot = vault_rqst_slot;
  hmc->power.vault_rsp_slot = vault_rsp_slot;
  hmc->power.vault_ctrl = vault_ctrl;
  hmc->power.row_access = row_access;
  return 0;
}


/* EOF */
