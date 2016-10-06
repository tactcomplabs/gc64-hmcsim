/*
 * HMC_SST_TRACE.CC
 *
 * Implementation file for the SST trace handlers
 *
 */

#ifdef SST_ENABLE_STAT

#include "hmc_sst_trace.h"

// force the use of the SST namespace
using namespace SST;

/* ----------------------------------------------------- HMCSIM_SST_TRACE_INIT */
extern "C" int hmcsim_sst_trace_init();
int hmcsim_sst_trace_init(){
  return 0;
}

/* ----------------------------------------------------- HMCSIM_SST_TRACE_STALL */
extern "C" int hmcsim_sst_trace_stall( struct hmcsim_t *hmc,
                                       uint32_t dev,
                                       uint32_t quad,
                                       uint32_t vault,
                                       uint32_t src,
                                       uint32_t dest,
                                       uint32_t link,
                                       uint32_t slot,
                                       uint32_t type );
int hmcsim_sst_trace_stall( struct hmcsim_t *hmc,
                            uint32_t dev,
                            uint32_t quad,
                            uint32_t vault,
                            uint32_t src,
                            uint32_t dest,
                            uint32_t link,
                            uint32_t slot,
                            uint32_t type ){
  return 0;
}

/* ----------------------------------------------------- HMCSIM_SST_TRACE_RQST */
extern "C" int hmcsim_sst_trace_rqst( struct hmcsim_t *hmc,
                           char *rqst,
                           uint32_t dev,
                           uint32_t quad,
                           uint32_t vault,
                           uint32_t bank,
                           uint64_t addr1,
                           uint32_t size );
int hmcsim_sst_trace_rqst( struct hmcsim_t *hmc,
                           char *rqst,
                           uint32_t dev,
                           uint32_t quad,
                           uint32_t vault,
                           uint32_t bank,
                           uint64_t addr1,
                           uint32_t size ){
  return 0;
}

/* ----------------------------------------------------- HMCSIM_SST_TRACE_BANK_CONFLICT */
extern "C" int hmcsim_sst_trace_bank_conflict( struct hmcsim_t *hmc,
                                    uint32_t dev,
                                    uint32_t quad,
                                    uint32_t vault,
                                    uint32_t bank,
                                    uint64_t addr );
int hmcsim_sst_trace_bank_conflict( struct hmcsim_t *hmc,
                                    uint32_t dev,
                                    uint32_t quad,
                                    uint32_t vault,
                                    uint32_t bank,
                                    uint64_t addr ){
  return 0;
}

/* ----------------------------------------------------- HMCSIM_SST_TRACE_LATENCY */
extern "C" int hmcsim_sst_trace_latency( struct hmcsim_t *hmc,
                              uint32_t dev,
                              uint32_t link,
                              uint32_t slot,
                              uint32_t quad,
                              uint32_t vault );
int hmcsim_sst_trace_latency( struct hmcsim_t *hmc,
                              uint32_t dev,
                              uint32_t link,
                              uint32_t slot,
                              uint32_t quad,
                              uint32_t vault ){
  return 0;
}

#endif /* SST_ENABLE_STAT */
#endif /* _HMC_SST_TRACE_H_ */

/* EOF */
