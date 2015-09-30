/* 
 * _HMC_SIM_TYPES_H_
 * 
 * HYBRID MEMORY CUBE SIMULATION LIBRARY
 * 
 * HMC TYPES HEADER FILE 
 * 
 * WARNING: DO NOT INCLUDE THIS FILE DIRECTLY
 *	    ALWAYS INCLUDE THE TOP-LEVEL HEADER 
 * 
 */

#ifndef _HMC_SIM_TYPES_H_
#define _HMC_SIM_TYPES_H_


#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>
#include <sys/types.h>
#include "hmc_sim_macros.h"

/* -------------------------------------------------- ENUMERATED TYPES */
typedef enum{
	HMC_LINK_DEV_DEV,		/*! HMC-SIM: HMC_LINK_DEF_T: DEVICE TO DEVICE LINK */ 
	HMC_LINK_HOST_DEV		/*! HMC-SIM: HMC_LINK_DEF_T: HOST TO DEVICE LINK */
}hmc_link_def_t;

typedef enum{
	HMC_RW,				/*! HMC-SIM: HMC_REG_DEF_T: READ+WRITE REGISTER */
	HMC_RO,				/*! HMC-SIM: HMC_REG_DEF_T: READ-ONLY REGISTER */
	HMC_RWS				/*! HMC-SIM: HMC_REG_DEF_T: CLEAR ON WRITE REGISTER */
}hmc_reg_def_t;

typedef enum{
	RD_RS, 				/*! HMC-SIM: HMC_RESPONSE_T: READ RESPONSE */
	WR_RS,				/*! HMC-SIM: HMC_RESPONSE_T: WRITE RESPONSE */
	MD_RD_RS, 			/*! HMC-SIM: HMC_RESPONSE_T: MODE READ RESPONSE */
	MD_WR_RS,			/*! HMC-SIM: HMC_RESPONSE_T: MODE WRITE RESPONSE */
	RSP_ERROR,			/*! HMC-SIM: HMC_RESPONSE_T: ERROR RESPONSE */
	RSP_NONE			/*! HMC-SIM: HMC_RESPONSE_T: NO RESPONSE COMMAND */
}hmc_response_t;

typedef enum{
	WR16, 				/*! HMC-SIM: HMC_RQST_T: 16-BYTE WRITE REQUEST */
	WR32, 				/*! HMC-SIM: HMC_RQST_T: 32-BYTE WRITE REQUEST */
	WR48, 				/*! HMC-SIM: HMC_RQST_T: 48-BYTE WRITE REQUEST */
	WR64, 				/*! HMC-SIM: HMC_RQST_T: 64-BYTE WRITE REQUEST */
	WR80, 				/*! HMC-SIM: HMC_RQST_T: 80-BYTE WRITE REQUEST */
	WR96, 				/*! HMC-SIM: HMC_RQST_T: 96-BYTE WRITE REQUEST */
	WR112, 				/*! HMC-SIM: HMC_RQST_T: 112-BYTE WRITE REQUEST */
	WR128, 				/*! HMC-SIM: HMC_RQST_T: 128-BYTE WRITE REQUEST */
	MD_WR, 				/*! HMC-SIM: HMC_RQST_T: MODE WRITE REQUEST */
	BWR, 				/*! HMC-SIM: HMC_RQST_T: BIT WRITE REQUEST */
	TWOADD8, 			/*! HMC-SIM: HMC_RQST_T: DUAL 8-byte ADD IMMEDIATE */
	ADD16,				/*! HMC-SIM: HMC_RQST_T: SINGLE 16-byte ADD IMMEDIATE */
	P_WR16, 			/*! HMC-SIM: HMC_RQST_T: 16-BYTE POSTED WRITE REQUEST */
	P_WR32, 			/*! HMC-SIM: HMC_RQST_T: 32-BYTE POSTED WRITE REQUEST */
	P_WR48, 			/*! HMC-SIM: HMC_RQST_T: 48-BYTE POSTED WRITE REQUEST */
	P_WR64, 			/*! HMC-SIM: HMC_RQST_T: 64-BYTE POSTED WRITE REQUEST */
	P_WR80, 			/*! HMC-SIM: HMC_RQST_T: 80-BYTE POSTED WRITE REQUEST */
	P_WR96, 			/*! HMC-SIM: HMC_RQST_T: 96-BYTE POSTED WRITE REQUEST */
	P_WR112, 			/*! HMC-SIM: HMC_RQST_T: 112-BYTE POSTED WRITE REQUEST */
	P_WR128, 			/*! HMC-SIM: HMC_RQST_T: 128-BYTE POSTED WRITE REQUEST */
	P_BWR,				/*! HMC-SIM: HMC_RQST_T: POSTED BIT WRITE REQUEST */
	P_2ADD8, 			/*! HMC-SIM: HMC_RQST_T: POSTED DUAL 8-BYTE ADD IMMEDIATE */
	P_ADD16, 			/*! HMC-SIM: HMC_RQST_T: POSTED SINGLE 16-BYTE ADD IMMEDIATE */
	RD16, 				/*! HMC-SIM: HMC_RQST_T: 16-BYTE READ REQUEST */	
	RD32, 				/*! HMC-SIM: HMC_RQST_T: 32-BYTE READ REQUEST */	
	RD48, 				/*! HMC-SIM: HMC_RQST_T: 48-BYTE READ REQUEST */	
	RD64, 				/*! HMC-SIM: HMC_RQST_T: 64-BYTE READ REQUEST */	
	RD80, 				/*! HMC-SIM: HMC_RQST_T: 80-BYTE READ REQUEST */	
	RD96, 				/*! HMC-SIM: HMC_RQST_T: 96-BYTE READ REQUEST */	
	RD112, 				/*! HMC-SIM: HMC_RQST_T: 112-BYTE READ REQUEST */	
	RD128, 				/*! HMC-SIM: HMC_RQST_T: 128-BYTE READ REQUEST */	
	MD_RD,				/*! HMC-SIM: HMC_RQST_T: MODE READ REQUEST */
	FLOW_NULL,			/*! HMC-SIM: HMC_RQST_T: NULL FLOW CONTROL */
	PRET,				/*! HMC-SIM: HMC_RQST_T: RETRY POINTER RETURN FLOW CONTROL */
	TRET,				/*! HMC-SIM: HMC_RQST_T: TOKEN RETURN FLOW CONTROL */
	IRTRY				/*! HMC-SIM: HMC_RQST_T: INIT RETRY FLOW CONTROL */
}hmc_rqst_t;

/* -------------------------------------------------- DATA STRUCTURES */

struct hmc_link_t{

	uint32_t id;			/*! HMC-SIM: HMC_LINK_T: LINK ID */
	uint32_t quad;			/*! HMC-SIM: HMC_LINK_T: ASSOCIATED QUADRANT */
	uint32_t src_cub;		/*! HMC-SIM: HMC_LINK_T: SOURCE CUB */
	uint32_t dest_cub;		/*! HMC-SIM: HMC_LINK_T: DESTINATION CUB */

	hmc_link_def_t	type;		/*! HMC-SIM: HMC_LINK_T: LINK TYPE */
};

struct hmc_dram_t{

	uint16_t elems[8];		/*! HMC-SIM: HMC_DRAM_T: DRAM ELEMENTS */

	uint32_t id;			/*! HMC-SIM: HMC_DRAM_T: DRAM ID */
};

struct hmc_bank_t{
	
	struct hmc_dram_t *drams;	/*! HMC-SIM: HMC_BANK_T: DRAMS */

	uint32_t id;			/*! HMC-SIM: HMC_BANK_T: BANK ID */
					/* 16-BYTE BANK INTERLEAVE */	
};

struct hmc_queue_t{
	uint32_t valid;				/*! HMC-SIM: HMC_QUEUE_T: VALID BIT */
	uint64_t packet[HMC_MAX_UQ_PACKET];	/*! HMC-SIM: HMC_QUEUE_T: PACKET */
};

struct hmc_vault_t{ 

	struct hmc_bank_t *banks;	/*! HMC-SIM: HMC_VAULT_T: BANK STRUCTURE */


	struct hmc_queue_t *rqst_queue;	/*! HMC-SIM: HMC_VAULT_T: REQUEST PACKET QUEUE */
	struct hmc_queue_t *rsp_queue;	/*! HMC-SIM: HMC_VAULT_T: REQUEST PACKET QUEUE */

	uint32_t id;			/*! HMC-SIM: HMC_VAULT_T: VAULT ID */
};

struct hmc_quad_t{ 

	struct hmc_vault_t *vaults;	/*! HMC-SIM: HMC_QUAD_T: VAULT STRUCTURE */

	uint32_t 	id;		/*! HMC-SIM: HMC_QUAD_T: QUADRANT ID */
};

struct hmc_xbar_t{
	struct hmc_queue_t *xbar_rqst;	/*! HMC-SIM: HMC_XBAR_T: CROSSBAR REQUEST QUEUE */
	struct hmc_queue_t *xbar_rsp;	/*! HMC-SIM: HMC_XBAR_T: CROSSBAR RESPONSE QUEUE */
};

struct hmc_reg_t{
	hmc_reg_def_t	type;		/*! HMC-SIM: HMC_REG_T: REGISTER TYPE */
	uint64_t	phy_idx;	/*! HMC-SIM: HMC_REG_T: REGISTER PHYSICAL DEVICE INDEX */
	uint64_t	reg;		/*! HMC-SIM: HMC_REG_T: REGISTER STORAGE */
};

struct hmc_dev_t{

	struct hmc_link_t *links;		/* HMC-SIM: HMC_DEV_T: LINK STRUCTURE */
	
	struct hmc_quad_t *quads;		/*! HMC-SIM: HMC_DEV_T: QUADRANT STRUCTURE */
	
	struct hmc_xbar_t *xbar;		/*! HMC-SIM: HMC_DEV_T: CROSSBAR STRUCTURE */

	struct hmc_reg_t regs[HMC_NUM_REGS];	/*! HMC-SIM: HMC_DEV_T: DEVICE CONFIGURATION REGISTERS */
	
	uint32_t id;				/*! HMC-SIM: HMC_DEV_T: CUBE ID */

	uint8_t seq;				/*! HMC-SIM: HMC_DEV_T: SEQUENCE NUMBER */
};

struct hmcsim_t{

	struct hmc_dev_t *devs;		/*! HMC-SIM: HMCSIM_T: DEVICE STRUCTURES */

	uint32_t num_devs;		/*! HMC-SIM: HMCSIM_T: NUMBER OF DEVICES */ 
	uint32_t num_links;		/*! HMC-SIM: HMCSIM_T: LINKS PER DEVICE */	
	uint32_t num_quads;		/*! HMC-SIM: HCMSIM_T: QUADS PER DEVICE */
	uint32_t num_vaults;		/*! HMC-SIM: HMCSIM_T: VAULTS PER DEVICE */
	uint32_t num_banks;		/*! HMC-SIM: HMCSIM_T: BANKS PER VAULT */
	uint32_t num_drams;		/*! HMC-SIM: HMCSIM_T: DRAMS PER BANK */
	uint32_t capacity;		/*! HMC-SIM: HMCSIM_T: CAPACITY PER DEVICE */

	uint32_t queue_depth;		/*! HMC-SIM: HMCSIM_T: VAULT QUEUE DEPTH */
	uint32_t xbar_depth;		/*! HMC-SIM: HMCSIM_T: VAULT QUEUE DEPTH */

	FILE *tfile;			/*! HMC-SIM: HMCSIM_T: TRACE FILE HANDLER */
	uint32_t tracelevel;		/*! HMC-SIM: HMCSIM_T: TRACE LEVEL */

	uint8_t seq;			/*! HMC-SIM: HCMSIM_T: SEQUENCE NUMBER */

	uint64_t clk;			/*! HMC-SIM: HMCSIM_T: CLOCK TICK */

	struct hmc_dev_t	*__ptr_devs;	
	struct hmc_quad_t	*__ptr_quads;	
	struct hmc_vault_t	*__ptr_vaults;	
	struct hmc_bank_t 	*__ptr_banks;	
	struct hmc_dram_t 	*__ptr_drams;	
	struct hmc_link_t 	*__ptr_links;	
	struct hmc_xbar_t	*__ptr_xbars;
	struct hmc_queue_t	*__ptr_xbar_rqst;
	struct hmc_queue_t	*__ptr_xbar_rsp;
	struct hmc_queue_t	*__ptr_vault_rqst;
	struct hmc_queue_t	*__ptr_vault_rsp;
	uint64_t 		*__ptr_stor;
};



#ifdef __cplusplus
} /* extern C */
#endif

#endif	/* _HMC_SIM_TYPES_H_ */

/* EOF */
