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


/* __op_name : Contains a string representative for the trace logs */
static char *__op_name      = "TEMPLATE_OP";

/* __op : Contains a bitwise OR'd list of operation parameters.
        : See hmc_cmc_opt enums from hmc_sim_types.h
*/
static hmc_cmcop_t __op     = (INARG|OUTARG);

/* __rqst : Contains the respective command enum that the simulated
          : application uses to initiate a request for this command.
          : See hmc_rqst_t enums from hmc_sim_types.h
          : MUST BE UNIQUE ACROSS CMC LIBS
*/
static hmc_rqst_t __rqst    = CMC04;

/* __cmd : Contains the respective command code for this CMC operation.
         : This MUST match the __rqst field.  For example, if we have
         : CMC32 as the __rqst, then the __cmd is (uint32_t)(32).
*/
static uint32_t __cmd       = 1;

/* __rsp_len : Contains the respective command response packet len in flits
             : Permissible values are 0->17.  This must include the header
             : and tail flits.  If __rsp_len is 0, then the operation
             : is assumed to be posted.
*/
static uint32_t __rsp_len   = 2;

/* __rsp_cmd : Contains the respective response command.  See hmc_response_t
             : enum in hmc_sim_types.h.  All normal commands are permissible.
             : If RSP_CMC is selected, you must also set __rsp_cmd_code
*/
static hmc_response_t __rsp_cmd = RD_RS;


/* __rsp_cmd_code : Contains the command code for RSP_CMC command
                  : responses.  The code must be <= 127 decimal.
                  : Unused response commands are 64->127
*/
static uint8_t __rsp_cmd_code = 0x00;


/* ----------------------------------------------------- HMCSIM_EXECUTE_CMC */
extern int hmcsim_execute_cmc(  void *hmc,
                                uint32_t dev,
                                uint32_t quad,
                                uint32_t vault,
                                uint32_t bank,
                                uint64_t addr,
                                uint32_t length,
                                uint64_t head,
                                uint64_t tail,
                                uint64_t *rqst_payload,
                                uint64_t *rsp_payload ){
  /* perform your operation */

  return 0;
}

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
                                uint32_t *cmd,
                                uint32_t *rsp_len,
                                hmc_response_t *rsp_cmd,
                                uint8_t *rsp_cmd_code){
  *op           = __op;
  *rqst         = __rqst;
  *cmd          = __cmd;
  *rsp_len      = __rsp_len;
  *rsp_cmd      = __rsp_cmd;
  *rsp_cmd_code = __rsp_cmd_code;

  return 0;
}

/* ----------------------------------------------------- HMCSIM_CMC_STR */
/*
 * Returns the name of the CMC operation for use in tracing
 * Most users will not need to change this function
 *
 */
extern void hmcsim_cmc_str( char *out ){
  sprintf( out, "%s", __op_name );
}

/* EOF */
