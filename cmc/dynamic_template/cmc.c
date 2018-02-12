/*
 * _CMC_C_
 *
 * Implements a dynamic CMC operation
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
static char *__op_name      = "DYNAMIC_TEMPLATE_OP";

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
static hmc_response_t __rsp_cmd = RD_RS;


/* __rsp_cmd_code : Contains the command code for RSP_CMC command
                  : responses.  The code must be <= 127 decimal.
                  : Unused response commands are 64->127
*/
static uint8_t __rsp_cmd_code = 0x00;

/* __transient_power : Contains the transient power of the respective
                     : CMC operation.  If this field is unknown,
                     : the CMC infrastructure will assume a value of 0.
*/
static float __transient_power = 0.5;

/* __row_ops : Contains the number of row operations for the respective
             : CMC operation.  If this field is unknown, the CMC
             : infrastructure will assume a value of 1.
*/
static uint32_t __row_ops = 2;

/* __dynamic_rsp_len : Contains the instantaneous dynamic response len
                     : for the packet currently being processed in flits
*/
uint32_t __dynamic_rsp_len = 0;

/* __dynamic_rqst_len : Contains the instantaneous dynamic request len
                      : for the packet currently being processed in flits
*/
uint32_t __dynamic_rqst_len = 0;

/* __dynamic_cmc : Determines if dynamic request and response packets
                 : are enabled; 0=no; 1=yes
*/
static uint32_t __dynamic_cmc = 1;

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

  /* relevant function pointers */
  struct hmcsim_t *l_hmc  = (struct hmcsim_t *)(hmc);
  int (*readmem)(struct hmcsim_t *,
                 uint64_t,
                 uint64_t *,
                 uint32_t ) = NULL;
  int (*writemem)(struct hmcsim_t *,
                  uint64_t,
                  uint64_t *,
                  uint32_t ) = NULL;
  int (*read_cmcreg)(struct hmcsim_t *,
                     uint32_t,
                     uint64_t,
                     uint64_t *) = NULL;
  int (*write_cmcreg)(struct hmcsim_t *,
                      uint32_t,
                      uint64_t,
                      uint64_t ) = NULL;

  /* init the function pointers */
  readmem       = l_hmc->readmem;
  writemem      = l_hmc->writemem;
  read_cmcreg   = l_hmc->read_cmcreg;
  write_cmcreg  = l_hmc->write_cmcreg;

  /*
   * !!! perform your operation !!!
   *
   */

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

/* ----------------------------------------------------- HMCSIM_CMC_DYNAMIC */
/*
 * Returns whether or not this packet contains dynamic request or response
 * values. 0=no; 1=yes
 *
 */
uint32_t hmcsim_cmc_dynamic(){
  return __dynamic_cmc;
}

/* ----------------------------------------------------- HMCSIM_CMC_RSP_LEN */
/*
 * Returns the value of the dynamic packet response.
 * If dynamic packets are not enabled, this function returns
 * the value of __rsp_len
 *
 */
uint32_t hmcsim_cmc_dynamic_rsp_len(){
  if( __dynamic_cmc == 1 ){
    return __dynamic_rsp_len;
  }else{
    return __rsp_len;
  }
}

/* ----------------------------------------------------- HMCSIM_CMC_RQST_LEN */
/*
 * Returns the value of the dynamic packet request.
 * If dynamic packets are not enabled, this function returns
 * the value of __rqst_len
 *
 */
uint32_t hmcsim_cmc_dynamic_rqst_len(){
  if( __dynamic_cmc ){
    return __dynamic_rqst_len;
  }else{
    return __rqst_len;
  }
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


/* ----------------------------------------------------- HMCSIM_CMC_POWER */
/*
 * Returns the amount of transient power and the number of row operations
 * for this respective operation.  If these values are not known, then
 * the CMC infrastructure assumes a transient power of 0 and 1 row op.
 * Users can modify these values based upon the runtime of the operation.
 * This function is not called until AFTER the processing is complete
 *
 */
extern void hmcsim_cmc_power( uint32_t *row_ops, float *tpower ){
  *row_ops = __row_ops;
  *tpower  = __transient_power;
}

/* EOF */
