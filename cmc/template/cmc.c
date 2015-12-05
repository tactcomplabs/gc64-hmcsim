/*
 * _CMC_H_
 *
 * Implements a single CMC operation
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "hmc_sim.h"


/* ----------------------------------------------------- GLOBALS */
/* These globals define the CMC operation parameters, the request
   enum and the command code.  Modify these values for your
   respective CMC operation.  If more than one CMC library is used
   in a simulation, these values MUST be unique across disparate
   libraries.
*/

/* __op : Contains a bitwise OR'd list of operation parameters.
        : See hmc_cmc_opt enums from hmc_sim_types.h
*/
static hmc_cmcop_t __op     = (INARG|OUTARG);

/* __rqst : Contains the respective command enum that the simulated
          : application uses to initiate a request for this command.
          : See hmc_rqst_t enums from hmc_sim_types.h
          : MUST BE UNIQUE ACROSS CMC LIBS
*/
static hmc_rqst_t __rqst    = CMC01;

/* __cmd : Contains the respective command code for this CMC operation.
         : This MUST match the __rqst field.  For example, if we have
         : CMC32 as the __rqst, then the __cmd is (uint32_t)(32).
*/
static uint32_t __cmd       = 1;


/* ----------------------------------------------------- HMCSIM_REGISTER_CMC */
/*
 * Registers the target CMC library instance with the core simulation. This
 * function is loaded via dlopen and called from the HMC-Sim library when
 * the sim makes a call to hmcsim_load_cmc().  Most users will not need
 * to change this function.
 *
 * *op is a pointer to a valid hmc_cmcop_t for which the function returns
 *     its CMC operation type.
 *
 * *rqst is a pointer to a valid hmc_rqst_t that defines which CMC operation
 *     command enum that this library will utilize.  See the hmc_rqst_t
 *     enums labeled CMCnn in ~/include/hmc_sim_types.h.
 *
 * *cmd is the respective command code that matches the *rqst command enum.
 *     For example, if *rqst returns CMC32, then the *cmd is "32".
 *
 */
extern int hmcsim_register_cmc( hmc_cmcop_t *op,
                                hmc_rqst_t *rqst,
                                uint32_t *cmd ){
  *op   = __op;
  *rqst = __rqst;
  *cmd  = __cmd;

  return 0;
}

/* EOF */
