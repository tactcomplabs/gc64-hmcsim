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

/* -- requests */

/* -- bank conflicts */
Statistic<uint64_t>* BankConflict;

/* -- xbar latency */
Statistic<uint64_t>* XbarLatency;


#endif /* SST_ENABLE_STAT */
#endif /* _HMC_SST_TRACE_H_ */

/* EOF */
