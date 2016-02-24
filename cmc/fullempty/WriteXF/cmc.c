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
static char *__op_name      = "WRITEXF";

/* __rqst : Contains the respective command enum that the simulated
          : application uses to initiate a request for this command.
          : See hmc_rqst_t enums from hmc_sim_types.h
          : MUST BE UNIQUE ACROSS CMC LIBS
*/
static hmc_rqst_t __rqst    = CMC78;

/* __cmd : Contains the respective command code for this CMC operation.
         : This MUST match the __rqst field.  For example, if we have
         : CMC32 as the __rqst, then the __cmd is (uint32_t)(32).
*/
static uint32_t __cmd       = 78;

/* __rqst_len : Contains the respective command request packet len in flits
              : Permissible values are 1->17.  This must include the header
              : and tail flits.  Commands with just an address have 1 flit.
              : Commands with data will include at least two flits.
              : It is up to the implementor to decode the data flits
*/
static uint32_t __rqst_len  = 2;

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


/* ----------------------------------------------------- FE_GET_ADDR */
/*
 * get the value of the FE bit for the corresponding addr
 *
 */
uint64_t fe_get_addr( uint64_t addr, uint8_t *bit ){
  uint64_t new  = 0x00ull;
  uint64_t tmp  = 0x00ull;
  uint16_t dram = 0x0000;
  uint8_t bank  = 0x00;
  uint8_t vault = 0x00;
  uint8_t byte  = 0x00;
  uint8_t u7    = 0x00;
  uint8_t nbank = 0x00;
  uint8_t nvault= 0x00;
  uint8_t quad  = 0x00;
  uint8_t nbyte = 0x00;
  uint8_t nbit  = 0x00;
#ifdef _4GB_
  /* 4GB:256byte device config */

  /* grab the lower 32 bits */
  tmp   = (addr&0x1FFFFFFFF);

  /* -- byte address : [7:4] */
  byte  = (uint8_t)((tmp>>4)&0xF);

  /* -- vault address : [10:8] */
  vault = (uint8_t)((tmp>>8)&0x7);

  /* -- bank address : [15:13] */
  bank  = (uint8_t)((tmp>>13)&0x7);

  /*-- dram address : [31:16] */
  dram  = (uint16_t)((tmp>>16)&0xFFFF);

  /* -- grab the upper 6 bits of the dram address */
  u7  = (uint8_t)((dram>>10)&0x3F);
  nvault = (u7&0x7);
  nbank  = ((u7>>3)&0x7);
  if( nvault < 2 ){
    quad = 0x00;
  }else if( nvault < 4 ){
    quad = 0x01;
  }else if( nvault < 6 ){
    quad = 0x02;
  }else{
    quad = 0x03;
  }

  /* -- get the byte address */
  nbyte = u7/8;

  /* -- get the bit address */
  nbit = u7-(nbyte*8);
  *bit = nbit;

  /* -- shift in the DRAM addr */
  new = 0xFFFF;
  new = (new<<16);

  /* -- insert new vault address */
  new |= ((uint64_t)(nvault)<<8);

  /* -- insert quad */
  new |= ((uint64_t)(quad)<<11);

  /* -- insert bank address */
  new |= ((uint64_t)(nbank)<<13);

  /* -- insert the byte address */
  new |= ((uint64_t)(nbyte)<<4);

#else
  /* 8GB:256byte device config */

  /* grab the lower 32 bits */
  tmp   = (addr&0x1FFFFFFFF);

  /* -- byte address : [7:4] */
  byte  = (uint8_t)((tmp>>4)&0xF);

  /* -- vault address : [10:8] */
  vault = (uint8_t)((tmp>>8)&0x7);

  /* -- bank address : [15:13] */
  bank  = (uint8_t)((tmp>>13)&0xF);

  /*-- dram address : [31:16] */
  dram  = (uint16_t)((tmp>>17)&0xFFFF);

  /* -- grab the upper 6 bits of the dram address */
  u7  = (uint8_t)((dram>>10)&0x3F);
  nvault = (u7&0x7);
  nbank  = ((u7>>3)&0x7);
  if( nvault < 2 ){
    quad = 0x00;
  }else if( nvault < 4 ){
    quad = 0x01;
  }else if( nvault < 6 ){
    quad = 0x02;
  }else{
    quad = 0x03;
  }

  /* -- get the byte address */
  nbyte = u7/8;

  /* -- get the bit address */
  nbit = u7-(nbyte*8);
  *bit = nbit;

  /* -- shift in the DRAM addr */
  new = 0xFFFF;
  new = (new<<17);

  /* -- insert new vault address */
  new |= ((uint64_t)(nvault)<<8);

  /* -- insert quad */
  new |= ((uint64_t)(quad)<<11);

  /* -- insert bank address */
  new |= ((uint64_t)(nbank)<<13);

  /* -- insert the byte address */
  new |= ((uint64_t)(nbyte)<<4);

#endif

  return tmp;
}

/* ----------------------------------------------------- FE_GET_BIT */
/*
 * get the value of the FE bit for the corresponding addr
 *
 */
uint8_t fe_get_bit( void *hmc,
                         uint64_t addr ){
  uint8_t bit   = 0x00;
  uint8_t tbit  = 0x00;
  uint64_t tmp  = fe_get_addr( addr, &bit );
  struct hmcsim_t *l_hmc  = (struct hmcsim_t *)(hmc);

  /* data for the operation */
  uint64_t data = 0x00ll;

  /* function pointer */
  int (*readmem)(struct hmcsim_t *,
                 uint64_t,
                 uint64_t *,
                 uint32_t ) = NULL;

  /* init the function pointers */
  readmem   = l_hmc->readmem;

  /* read the memory */
  (*readmem)(l_hmc, tmp, &data, 1 );

  /* get the bit value */
  tbit = (uint8_t)((data>>bit)&1);

  return tbit;
}

/* ----------------------------------------------------- FE_SET_BIT */
/*
 * set the value of the FE bit for the corresponding addr
 *
 */
void fe_set_bit( void *hmc,
                 uint64_t addr,
                 uint8_t febit ){
  uint8_t bit   = 0x00;
  uint8_t tbit  = 0x00;
  uint64_t tmp  = fe_get_addr( addr, &bit );
  struct hmcsim_t *l_hmc  = (struct hmcsim_t *)(hmc);

  /* data for the operation */
  uint64_t data = 0x00ll;

  /* function pointer */
  int (*readmem)(struct hmcsim_t *,
                 uint64_t,
                 uint64_t *,
                 uint32_t ) = NULL;
  int (*writemem)(struct hmcsim_t *,
                 uint64_t,
                 uint64_t *,
                 uint32_t ) = NULL;

  /* init the function pointers */
  readmem   = l_hmc->readmem;
  writemem  = l_hmc->writemem;

  /* read the memory */
  (*readmem)(l_hmc, tmp, &data, 1 );

  /* get the bit value */
  tbit = (uint8_t)((data>>bit)&1);

  if( (tbit == 1) && (febit == 0) ){
    /* clear the bit */
    data &= ~(1<<bit);
  }else if( (tbit == 0) && (febit == 1) ){
    data |= (1<<bit);
  }/* else, bit matches, do nothing */

  /* write the memory back */
  (*writemem)(l_hmc, addr, &data, 1);
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
  /* hmc struct pointer */
  struct hmcsim_t *l_hmc  = (struct hmcsim_t *)(hmc);

  /* function pointer */
  int (*readmem)(struct hmcsim_t *,
                 uint64_t,
                 uint64_t *,
                 uint32_t ) = NULL;
  int (*writemem)(struct hmcsim_t *,
                 uint64_t,
                 uint64_t *,
                 uint32_t ) = NULL;

  /* init the function pointers */
  readmem   = l_hmc->readmem;
  writemem  = l_hmc->writemem;

  /* set the bit */
  fe_set_bit( hmc, addr, 1 );

  if( (*writemem)(l_hmc, addr, rqst_payload, 1 ) != 0 ){
    return -1;
  }

  rsp_payload[0] = 0x01ull;
  rsp_payload[1] = 0x00ull;

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
