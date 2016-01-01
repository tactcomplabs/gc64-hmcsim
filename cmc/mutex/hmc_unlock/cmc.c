/*
 * _CMC_C_
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
static char *__op_name      = "HMC_UNLOCK";

/* __rqst : Contains the respective command enum that the simulated
          : application uses to initiate a request for this command.
          : See hmc_rqst_t enums from hmc_sim_types.h
          : MUST BE UNIQUE ACROSS CMC LIBS
*/
static hmc_rqst_t __rqst    = CMC127;

/* __cmd : Contains the respective command code for this CMC operation.
         : This MUST match the __rqst field.  For example, if we have
         : CMC32 as the __rqst, then the __cmd is (uint32_t)(32).
*/
static uint32_t __cmd       = 127;

/* __rqst_len : Contains the respective command request packet len in flits
              : Permissible values are 1->17.  This must include the header
              : and tail flits.  Commands with just an address have 1 flit.
              : Commands with data will include at least two flits.
              : It is up to the implementor to decode the data flits
*/
static uint32_t __rqst_len  = 1;

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
static hmc_response_t __rsp_cmd = WR_RS;


/* __rsp_cmd_code : Contains the command code for RSP_CMC command
                  : responses.  The code must be <= 127 decimal.
                  : Unused response commands are 64->127
*/
static uint8_t __rsp_cmd_code = 0x00;


/* ----------------------------------------------------- HMCSIM_EXECUTE_CMC */
/*
 * Performs the actual CMC operation.  All your custom logic belongs in this
 * function.
 *
 * *hmc is a void pointer to the core hmc structure.  Note that this must
 *    be cast to (struct hmcsim_t *)
 * dev is the respective device where the op is occurring
 * quad is the respective quad where the op is occurring
 * vault is the respective vault where the op is occurring
 * bank is the respective bank where the op is occurring
 * addr is the base address of the incoming request
 * length is the length of the incoming request
 * head is the packet head
 * tail is the packet tail
 * *rqst_payload is the incoming request payload formatted as the maximum
 *    possible packet (256 bytes of data).  Its up to this function to
 *    pull the required bits from this payload.
 * *rsp_payload is the outgoing response data payload formatted as the
 *    maximum possible packet (256 bytes of data).  Its up to this function
 *    to write the required number of output bits in the response payload.
 *    Note that the calling infrastructure will only utilize the number of
 *    bytes as defined by the rsp_len of this CMC operation
 *
 */
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
 * *rqst is a pointer to a valid hmc_rqst_t that defines which CMC operation
 *     command enum that this library will utilize.  See the hmc_rqst_t
 *     enums labeled CMCnn in ~/include/hmc_sim_types.h.
 *
 * *cmd is the respective command code that matches the *rqst command enum.
 *     For example, if *rqst returns CMC32, then the *cmd is "32".
 *
 * *rsp_len is the respective command's response packet length.
 *    This must fit within the standard HMC response packet sizes
 *
 * *rsp_cmd is the respective command's response command type.  See
 *    the values defined in the hmc_response_t enum in ~/include/hmc_sim_types.h
 *
 * *rsp_cmd_code is the respective command's response command code in raw form.
 *
 */
extern int hmcsim_register_cmc( hmc_rqst_t *rqst,
                                uint32_t *cmd,
                                uint32_t *rqst_len,
                                uint32_t *rsp_len,
                                hmc_response_t *rsp_cmd,
                                uint8_t *rsp_cmd_code){
  *rqst         = __rqst;
  *cmd          = __cmd;
  *rqst_len     = __rqst_len;
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
 * *out is the output string that is written to
 *
 */
extern void hmcsim_cmc_str( char *out ){
  sprintf( out, "%s", __op_name );
}

/* EOF */
