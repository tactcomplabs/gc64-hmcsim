/*
 * HMC_SST_STAT.C
 *
 * Implementation file for the new HMC Stat functions that
 * permit us to derive stats from internal HMC-Sim counters
 *
 */


#include <stdlib.h>
#include <stdio.h>
#include "hmc_sim.h"


/* ----------------------------------------------------- HMCSIM_CLEAR_STAT */
extern void hmcsim_clear_stat( struct hmcsim_t *hmc ){
  if( hmc == NULL ){
    return ;
  }

  hmc->istat.xbar_rqst_stall = 0x00ull;
  hmc->istat.xbar_rsp_stall = 0x00ull;
  hmc->istat.vault_rqst_stall = 0x00ull;
  hmc->istat.vault_rsp_stall = 0x00ull;
  hmc->istat.route_rqst_stall = 0x00ull;
  hmc->istat.route_rsp_stall = 0x00ull;
  hmc->istat.undef_stall = 0x00ull;
  hmc->istat.bank_conflict = 0x0ull;
  hmc->istat.xbar_latency = 0x00ull;
}

/* ----------------------------------------------------- HMCSIM_FLOAT_STAT */
extern float hmcsim_float_stat( struct hmcsim_t *hmc, hmc_stat_t stat ){
  if( hmc == NULL){
    return 0.;
  }

  switch( stat ){
    case XBAR_RQST_STALL_STAT:
      return (float)(hmc->istat.xbar_rqst_stall);
      break;
    case XBAR_RSP_STALL_STAT:
      return (float)(hmc->istat.xbar_rsp_stall);
      break;
    case VAULT_RQST_STALL_STAT:
      return (float)(hmc->istat.vault_rqst_stall);
      break;
    case VAULT_RSP_STALL_STAT:
      return (float)(hmc->istat.vault_rsp_stall);
      break;
    case ROUTE_RQST_STALL_STAT:
      return (float)(hmc->istat.route_rqst_stall);
      break;
    case ROUTE_RSP_STALL_STAT:
      return (float)(hmc->istat.route_rsp_stall);
      break;
    case UNDEF_STALL_STAT:
      return (float)(hmc->istat.undef_stall);
      break;
    case BANK_CONFLICT_STAT:
      return (float)(hmc->istat.bank_conflict);
      break;
    case XBAR_LATENCY_STAT:
      return (float)(hmc->istat.xbar_latency);
      break;
    case LINK_PHY_POWER_STAT:
      return hmc->power.t_link_phy;
      break;
    case LINK_LOCAL_ROUTE_POWER_STAT:
      return hmc->power.t_link_local_route;
      break;
    case LINK_REMOTE_ROUTE_POWER_STAT:
      return hmc->power.t_link_local_route;
      break;
    case XBAR_RQST_SLOT_POWER_STAT:
      return hmc->power.t_xbar_rqst_slot;
      break;
    case XBAR_RSP_SLOT_POWER_STAT:
      return hmc->power.t_xbar_rsp_slot;
      break;
    case XBAR_ROUTE_EXTERN_POWER_STAT:
      return hmc->power.t_xbar_route_extern;
      break;
    case VAULT_RQST_SLOT_POWER_STAT:
      return hmc->power.t_vault_rqst_slot;
      break;
    case VAULT_RSP_SLOT_POWER_STAT:
      return hmc->power.t_vault_rsp_slot;
      break;
    case VAULT_CTRL_POWER_STAT:
      return hmc->power.t_vault_ctrl;
      break;
    case ROW_ACCESS_POWER_STAT:
      return hmc->power.t_row_access;
      break;
    case LINK_PHY_THERM_STAT:
      return hmc->power.t_link_phy*HMC_MILLIWATT_TO_BTU;
      break;
    case LINK_LOCAL_ROUTE_THERM_STAT:
      return hmc->power.t_link_local_route*HMC_MILLIWATT_TO_BTU;
      break;
    case LINK_REMOTE_ROUTE_THERM_STAT:
      return hmc->power.t_link_local_route*HMC_MILLIWATT_TO_BTU;
      break;
    case XBAR_RQST_SLOT_THERM_STAT:
      return hmc->power.t_xbar_rqst_slot*HMC_MILLIWATT_TO_BTU;
      break;
    case XBAR_RSP_SLOT_THERM_STAT:
      return hmc->power.t_xbar_rsp_slot*HMC_MILLIWATT_TO_BTU;
      break;
    case XBAR_ROUTE_EXTERN_THERM_STAT:
      return hmc->power.t_xbar_route_extern*HMC_MILLIWATT_TO_BTU;
      break;
    case VAULT_RQST_SLOT_THERM_STAT:
      return hmc->power.t_vault_rqst_slot*HMC_MILLIWATT_TO_BTU;
      break;
    case VAULT_RSP_SLOT_THERM_STAT:
      return hmc->power.t_vault_rsp_slot*HMC_MILLIWATT_TO_BTU;
      break;
    case VAULT_CTRL_THERM_STAT:
      return hmc->power.t_vault_ctrl*HMC_MILLIWATT_TO_BTU;
      break;
    case ROW_ACCESS_THERM_STAT:
      return hmc->power.t_row_access*HMC_MILLIWATT_TO_BTU;
      break;
    default:
      break;
  }
  return 0.;
}

/* ----------------------------------------------------- HMCSIM_INT_STAT */
extern uint64_t hmcsim_int_stat( struct hmcsim_t *hmc, hmc_stat_t stat ){
  if( hmc == NULL){
    return 0x00ull;
  }

  switch( stat ){
    case XBAR_RQST_STALL_STAT:
      return (hmc->istat.xbar_rqst_stall);
      break;
    case XBAR_RSP_STALL_STAT:
      return (hmc->istat.xbar_rsp_stall);
      break;
    case VAULT_RQST_STALL_STAT:
      return (hmc->istat.vault_rqst_stall);
      break;
    case VAULT_RSP_STALL_STAT:
      return (hmc->istat.vault_rsp_stall);
      break;
    case ROUTE_RQST_STALL_STAT:
      return (hmc->istat.route_rqst_stall);
      break;
    case ROUTE_RSP_STALL_STAT:
      return (hmc->istat.route_rsp_stall);
      break;
    case UNDEF_STALL_STAT:
      return (hmc->istat.undef_stall);
      break;
    case BANK_CONFLICT_STAT:
      return (hmc->istat.bank_conflict);
      break;
    case XBAR_LATENCY_STAT:
      return (hmc->istat.xbar_latency);
      break;
    case LINK_PHY_POWER_STAT:
      return (uint64_t)hmc->power.t_link_phy;
      break;
    case LINK_LOCAL_ROUTE_POWER_STAT:
      return (uint64_t)hmc->power.t_link_local_route;
      break;
    case LINK_REMOTE_ROUTE_POWER_STAT:
      return (uint64_t)hmc->power.t_link_local_route;
      break;
    case XBAR_RQST_SLOT_POWER_STAT:
      return (uint64_t)hmc->power.t_xbar_rqst_slot;
      break;
    case XBAR_RSP_SLOT_POWER_STAT:
      return (uint64_t)hmc->power.t_xbar_rsp_slot;
      break;
    case XBAR_ROUTE_EXTERN_POWER_STAT:
      return (uint64_t)hmc->power.t_xbar_route_extern;
      break;
    case VAULT_RQST_SLOT_POWER_STAT:
      return (uint64_t)hmc->power.t_vault_rqst_slot;
      break;
    case VAULT_RSP_SLOT_POWER_STAT:
      return (uint64_t)hmc->power.t_vault_rsp_slot;
      break;
    case VAULT_CTRL_POWER_STAT:
      return (uint64_t)hmc->power.t_vault_ctrl;
      break;
    case ROW_ACCESS_POWER_STAT:
      return (uint64_t)hmc->power.t_row_access;
      break;
    case LINK_PHY_THERM_STAT:
      return (uint64_t)hmc->power.t_link_phy*HMC_MILLIWATT_TO_BTU;
      break;
    case LINK_LOCAL_ROUTE_THERM_STAT:
      return (uint64_t)hmc->power.t_link_local_route*HMC_MILLIWATT_TO_BTU;
      break;
    case LINK_REMOTE_ROUTE_THERM_STAT:
      return (uint64_t)hmc->power.t_link_local_route*HMC_MILLIWATT_TO_BTU;
      break;
    case XBAR_RQST_SLOT_THERM_STAT:
      return (uint64_t)hmc->power.t_xbar_rqst_slot*HMC_MILLIWATT_TO_BTU;
      break;
    case XBAR_RSP_SLOT_THERM_STAT:
      return (uint64_t)hmc->power.t_xbar_rsp_slot*HMC_MILLIWATT_TO_BTU;
      break;
    case XBAR_ROUTE_EXTERN_THERM_STAT:
      return (uint64_t)hmc->power.t_xbar_route_extern*HMC_MILLIWATT_TO_BTU;
      break;
    case VAULT_RQST_SLOT_THERM_STAT:
      return (uint64_t)hmc->power.t_vault_rqst_slot*HMC_MILLIWATT_TO_BTU;
      break;
    case VAULT_RSP_SLOT_THERM_STAT:
      return (uint64_t)hmc->power.t_vault_rsp_slot*HMC_MILLIWATT_TO_BTU;
      break;
    case VAULT_CTRL_THERM_STAT:
      return (uint64_t)hmc->power.t_vault_ctrl*HMC_MILLIWATT_TO_BTU;
      break;
    case ROW_ACCESS_THERM_STAT:
      return (uint64_t)hmc->power.t_row_access*HMC_MILLIWATT_TO_BTU;
      break;
    default:
      break;
  }
  return 0x00ull;
}

/* EOF */
