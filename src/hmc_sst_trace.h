/*
 * HMC_SST_TRACE.H
 *
 * Header file for the SST trace handlers
 *
 */

#ifndef _HMC_SST_TRACE_H_
#define _HMC_SST_TRACE_H_

#ifdef SST_ENABLE_STAT

#include "sst/core/component.h"
#include "sst_config.h"
#include <stdlib.h>
#include "hmc_sim.h"

/* Global statistic values for SST Statistics tracing */
/* -- stall values */
Statistic<uint64_t>* XbarRqstStall;
Statistic<uint64_t>* VaultRqstStall;
Statistic<uint64_t>* XbarRspStall;
Statistic<uint64_t>* RouteRqstStall;
Statistic<uint64_t>* RouteRspStall;
Statistic<uint64_t>* UndefStall;

/* -- bank conflicts */
Statistic<uint64_t>* BankConflict;

/* -- xbar latency */
Statistic<uint64_t>* XbarLatency;

/* -- power */
Statistic<float>* LinkPhyPower;
Statistic<float>* LinkLocalRoutePower;
Statistic<float>* LinkRemoteRoutePower;
Statistic<float>* XbarRqstSlotPower;
Statistic<float>* XbarRspSlotPower;
Statistic<float>* XbarRouteExternPower;
Statistic<float>* VaultRqstSlotPower;
Statistic<float>* VaultRspSlotPower;
Statistic<float>* VaultCtrlPower;
Statistic<float>* RowAccessPower;

/* -- thermal */
Statistic<float>* LinkPhyTherm;
Statistic<float>* LinkLocalRouteTherm;
Statistic<float>* LinkRemoteRouteTherm;
Statistic<float>* XbarRqstSlotTherm;
Statistic<float>* XbarRspSlotTherm;
Statistic<float>* XbarRouteExternTherm;
Statistic<float>* VaultRqstSlotTherm;
Statistic<float>* VaultRspSlotTherm;
Statistic<float>* VaultCtrlTherm;
Statistic<float>* RowAccessTherm;


#endif /* SST_ENABLE_STAT */
#endif /* _HMC_SST_TRACE_H_ */

/* EOF */
