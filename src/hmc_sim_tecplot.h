/*
 * _HMC_SIM_TECPLOT_H_
 *
 * HMC-Sim Tecplot Data Formats
 *
 */

#ifndef _HMC_SIM_TECPLOT_H_
#define _HMC_SIM_TECPLOT_H_

#include <stdlib.h>

struct HMC4LinkTec{
  // power
  float vault_rsp_power[32];
  float vault_rqst_power[32];
  float vault_ctrl_power[32];
  float xbar_rqst_power[4];
  float xbar_rsp_power[4];
  float xbar_route_extern_power[4];
  float link_local_route_power[4];
  float link_remote_route_power[4];
  float link_phy_power[4];
  float row_access_power;

  // thermal
  float vault_rsp_btu[32];
  float vault_rqst_btu[32];
  float vault_ctrl_btu[32];
  float xbar_rqst_btu[4];
  float xbar_rsp_btu[4];
  float xbar_route_extern_btu[4];
  float link_local_route_btu[4];
  float link_remote_route_btu[4];
  float link_phy_btu[4];
  float row_access_btu;
};

struct HMC8LinkTec{
  float vault_rsp_power[32];
  float vault_rqst_power[32];
  float vault_ctrl_power[32];
  float xbar_rqst_power[8];
  float xbar_rsp_power[8];
  float xbar_route_extern_power[8];
  float link_local_route_power[8];
  float link_remote_route_power[8];
  float link_phy_power[8];
  float row_access_power;

  // thermal
  float vault_rsp_btu[32];
  float vault_rqst_btu[32];
  float vault_ctrl_btu[32];
  float xbar_rqst_btu[8];
  float xbar_rsp_btu[8];
  float xbar_route_extern_btu[8];
  float link_local_route_btu[8];
  float link_remote_route_btu[8];
  float link_phy_btu[8];
  float row_access_btu;
};

#endif /* _HMC_SIM_TECPLOT_H_ */
/* EOF */
