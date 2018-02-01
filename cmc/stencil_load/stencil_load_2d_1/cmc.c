/*
 * _CMC_C_
 *
 * Implements a single CMC operation
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "hmc_sim.h"

/* ----------------------------------------------------- GLOBALS */
/* These globals define the CMC operation parameters, the request
   enum and the command code.  Modify these values for your
   respective CMC operation.  If more than one CMC library is used
   in a simulation, these values MUST be unique across disparate
   libraries.
*/


/* __op_name : Contains a string representative for the trace logs */
static char *__op_name      = "STENCIL_LOAD_2D_1";

/* __rqst : Contains the respective command enum that the simulated
          : application uses to initiate a request for this command.
          : See hmc_rqst_t enums from hmc_sim_types.h
          : MUST BE UNIQUE ACROSS CMC LIBS
*/
static hmc_rqst_t __rqst    = CMC21;

/* __cmd : Contains the respective command code for this CMC operation.
         : This MUST match the __rqst field.  For example, if we have
         : CMC32 as the __rqst, then the __cmd is (uint32_t)(32).
*/
static uint32_t __cmd       = 21;

/* __rqst_len : Contains the respective command request packet len in flits
              : Permissible values are 1->17.  This must include the header
              : and tail flits.  Commands with just an address have 1 flit.
              : Commands with data will include at least two flits.
              : It is up to the implementor to decode the data flits
*/
static uint32_t __rqst_len  = 3;

/* __rsp_len : Contains the respective command response packet len in flits
             : Permissible values are 0->17.  This must include the header
             : and tail flits.  If __rsp_len is 0, then the operation
             : is assumed to be posted.
*/
static uint32_t __rsp_len   = 1;

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


/* nh_type: type of neighborhood enumerations
 */
enum nh_type{VonNeumann, Moore, NarrowVonNeumann};


/* ----------------------------------------------------- helper functions   */

/* ----------------------------------------------------- PACK_DATA          */
/* Pushes data onto payload truncated to the correct size
 *
 * data is the data from cache line
 * *payload is the payload that the data will be pushed to. Note: It's assumed
 *   that unused bits in payload are zero'd
 * count is the number of items already pushed to the payload
 * data_size is the size in bits of payload data elements
 *
 */
void pack_data(uint64_t data, uint64_t *payload, uint8_t count, uint8_t data_size){
  uint8_t payload_index = count / (64/data_size);
  uint8_t payload_count = count % (64/data_size);
  payload[payload_index] += (data & ((int) pow(2,data_size)-1)) << (payload_count * data_size);
}

/* ----------------------------------------------------- STENCIL_POINT      */
/* Returns the stencil point value for the stencil specified by passed arguments
 * returns 0 if neighborhood not defined as a valid nh_type
 *
 * center_incl is a boolean that is true iff the center cell is included in the stencil
 * neighborhood_type is the layout of the neighborhood and must be chosen a valid
 *   nh_type as defined in the nh_type enum
 * radius is the radius (range) of the stencil
 * dim is the number of dimensions which the stencil is operating over
 *
 */
uint8_t stencil_point(uint8_t center_incl, uint8_t neighborhood_type, uint8_t radius, uint8_t dim){
  switch(neighborhood_type){
    case VonNeumann:
      return 1 + 2*radius*(radius+1) + (center_incl-1);
    case Moore:
      return (pow(1+radius*2,dim)) + (center_incl-1);
    case NarrowVonNeumann:
      return radius*2*dim + center_incl;
    default:
      return 0;
  } 
}

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
  
  struct hmcsim_t *l_hmc = (struct hmcsim_t *)(hmc); 
  const uint8_t data_size = 4;   // in bits
  const uint8_t dims = 2;

  /* extract request payload data */
  /* rqst_payload size : 1 FLIT */
  const uint8_t center_inclusive = rqst_payload[0] & 0x1;
  const uint8_t neighborhood_type = rqst_payload[0] >> 1 & 0x7;
  const uint8_t radius = rqst_payload[0] >> 4 & 0x3F;
  const uint8_t x_width = rqst_payload[0] >> 16 & 0xFFFF;
  
  /* ensure stencil point <= #possible rsp_payload items */
  const uint8_t point = stencil_point(center_inclusive, neighborhood_type, radius, dims);
  if (point > ((__rsp_len*128)/data_size)){
    return -1;
  }


  /* data variable for read ops */
  uint64_t data = 0x00ull;

  /* count variable for number of items in rsp_payload */
  uint8_t rsp_count = 0x0;

  /* address variable for read ops */
  uint64_t fetch_addr = 0x00ull;

  /* function pointer for read op */
  int (*readmem)( struct hmcsim_t *,
		  uint64_t,
		  uint64_t *,
		  uint32_t ) = NULL;
  
  /* init function pointer */
  readmem = l_hmc->readmem;

  uint8_t i, j;
  switch (neighborhood_type){
    case VonNeumann:
      for(i=-radius; i<=radius; i++){
	for(j=-radius; j<=radius;j++){
          // only read values whose Manhattan Distance < the stencil radius
	  if(abs(i)+abs(j) <= radius){
	    // only read center value if it hasn't been excluded
	    if (!(j==0 && i==0)){
              fetch_addr = addr + (i*data_size) + (j*x_width*data_size);
	      if( (*readmem)(l_hmc, fetch_addr, &data, 1) != 0 ){
                /* error in data read */
                return -1;
              }
	      pack_data(data, rsp_payload, rsp_count, data_size);
            }else if(center_inclusive){
	      fetch_addr = addr;
              if( (*readmem)(l_hmc, fetch_addr, &data, 1) != 0 ){
                /* error in data read */
                return -1;
              }
	      pack_data(data, rsp_payload, rsp_count, data_size);
	    }
	  }
	}
      }
      break;
    case Moore:
      for(i=-radius; i<=radius; i++){
	for(j=-radius; j<=radius; j++){
          // only read center value if it hasn't been excluded
          if (i!=0){
            fetch_addr = addr + (i*data_size) + (j*x_width*data_size);
            if( (*readmem)(l_hmc, fetch_addr, &data, 1) != 0 ){
              /* error in data read */
              return -1;
            }
	    pack_data(data, rsp_payload, rsp_count, data_size);
          }else if(center_inclusive){
            fetch_addr = addr;
            if( (*readmem)(l_hmc, fetch_addr, &data, 1) != 0 ){
              /* error in data read */
              return -1;
            }
            pack_data(data, rsp_payload, rsp_count, data_size);
	  }
	}
      }
      break;
    case NarrowVonNeumann:
      for(i=-radius; i<=radius; i++){
        if (i != 0){
          fetch_addr = addr + (i*data_size);
          if( (*readmem)(l_hmc, fetch_addr, &data, 1) != 0 ){
            /* error in data read */
            return -1;
          }
          pack_data(data, rsp_payload, rsp_count, data_size);

	  fetch_addr = addr + (j*x_width*data_size);
          if( (*readmem)(l_hmc, fetch_addr, &data, 1) != 0 ){
            /* error in data read */
            return -1;
          }
	  pack_data(data, rsp_payload, rsp_count, data_size);
	}else if(center_inclusive){
          fetch_addr = addr;
          if( (*readmem)(l_hmc, fetch_addr, &data, 1) != 0 ){
            /* error in data read */
            return -1;
          }
	  pack_data(data, rsp_payload, rsp_count, data_size);
	}
      }
      break;
  }
  
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
