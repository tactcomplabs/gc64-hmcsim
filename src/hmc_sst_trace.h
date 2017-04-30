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
Statistic<uint64_t>* Write16Ops;
Statistic<uint64_t>* Write32Ops;
Statistic<uint64_t>* Write48Ops;
Statistic<uint64_t>* Write64Ops;
Statistic<uint64_t>* Write80Ops;
Statistic<uint64_t>* Write96Ops;
Statistic<uint64_t>* Write112Ops;
Statistic<uint64_t>* Write128Ops;
Statistic<uint64_t>* Write256Ops;
Statistic<uint64_t>* Read16Ops;
Statistic<uint64_t>* Read32Ops;
Statistic<uint64_t>* Read48Ops;
Statistic<uint64_t>* Read64Ops;
Statistic<uint64_t>* Read80Ops;
Statistic<uint64_t>* Read96Ops;
Statistic<uint64_t>* Read112Ops;
Statistic<uint64_t>* Read128Ops;
Statistic<uint64_t>* Read256Ops;
Statistic<uint64_t>* ModeWriteOps;
Statistic<uint64_t>* ModeReadOps;
Statistic<uint64_t>* BWROps;
Statistic<uint64_t>* TwoAdd8Ops;
Statistic<uint64_t>* Add16Ops;
Statistic<uint64_t>* PWrite16Ops;
Statistic<uint64_t>* PWrite32Ops;
Statistic<uint64_t>* PWrite48Ops;
Statistic<uint64_t>* PWrite64Ops;
Statistic<uint64_t>* PWrite80Ops;
Statistic<uint64_t>* PWrite96Ops;
Statistic<uint64_t>* PWrite112Ops;
Statistic<uint64_t>* PWrite128Ops;
Statistic<uint64_t>* PWrite256Ops;
Statistic<uint64_t>* TwoAddS8ROps;
Statistic<uint64_t>* AddS16ROps;
Statistic<uint64_t>* Inc8Ops;
Statistic<uint64_t>* PInc8Ops;
Statistic<uint64_t>* Xor16Ops;
Statistic<uint64_t>* Or16Ops;
Statistic<uint64_t>* Nor16Ops;
Statistic<uint64_t>* And16Ops;
Statistic<uint64_t>* Nand16Ops;
Statistic<uint64_t>* CasGT8Ops;
Statistic<uint64_t>* CasGT16Ops;
Statistic<uint64_t>* CasLT8Ops;
Statistic<uint64_t>* CasLT16Ops;
Statistic<uint64_t>* CasEQ8Ops;
Statistic<uint64_t>* CasZero16Ops;
Statistic<uint64_t>* Eq8Ops;
Statistic<uint64_t>* Eq16Ops;
Statistic<uint64_t>* BWR8ROps;
Statistic<uint64_t>* Swap16Ops;


/* -- bank conflicts */
Statistic<uint64_t>* BankConflict;

/* -- xbar latency */
Statistic<uint64_t>* XbarLatency;

/* -- power */
Statistic<uint64_t>* LinkPhyPower;
Statistic<uint64_t>* LinkLocalRoutePower;
Statistic<uint64_t>* LinkRemoteRoutePower;
Statistic<uint64_t>* XbarRqstSlotPower;
Statistic<uint64_t>* XbarRspSlotPower;
Statistic<uint64_t>* XbarRouteExternPower;
Statistic<uint64_t>* VaultRqstSlotPower;
Statistic<uint64_t>* VaultRspSlotPower;
Statistic<uint64_t>* VaultCtrlPower;
Statistic<uint64_t>* RowAccessPower;

/* -- thermal */
Statistic<uint64_t>* LinkPhyTherm;
Statistic<uint64_t>* LinkLocalRouteTherm;
Statistic<uint64_t>* LinkRemoteRouteTherm;
Statistic<uint64_t>* XbarRqstSlotTherm;
Statistic<uint64_t>* XbarRspSlotTherm;
Statistic<uint64_t>* XbarRouteExternTherm;
Statistic<uint64_t>* VaultRqstSlotTherm;
Statistic<uint64_t>* VaultRspSlotTherm;
Statistic<uint64_t>* VaultCtrlTherm;
Statistic<uint64_t>* RowAccessTherm;


#endif /* SST_ENABLE_STAT */
#endif /* _HMC_SST_TRACE_H_ */

/* EOF */
