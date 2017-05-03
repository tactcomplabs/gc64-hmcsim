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
