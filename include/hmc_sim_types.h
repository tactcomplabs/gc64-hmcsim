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
 * COPYRIGHT (c) 2018 TEXAS TECH UNIVERSITY
 * SEE LICENSE FOR DETAILS
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
#include "hmc_sim_tecplot.h"

/* -------------------------------------------------- ENUMERATED TYPES */
typedef enum{
        XBAR_RQST_STALL_STAT,                /*! HMC-SIM: HMC_STAT_T: CROSSBAR REQUEST STALLS */
        XBAR_RSP_STALL_STAT,                 /*! HMC-SIM: HMC_STAT_T: CROSSBAR RESPONSE STALLS */
        VAULT_RQST_STALL_STAT,               /*! HMC-SIM: HMC_STAT_T: VAULT REQUEST STALLS */
        VAULT_RSP_STALL_STAT,                /*! HMC-SIM: HMC_STAT_T: VAULT RESPONSE STALLS */
        ROUTE_RQST_STALL_STAT,               /*! HMC-SIM: HMC_STAT_T: ROUTE REQUEST STALLS */
        ROUTE_RSP_STALL_STAT,                /*! HMC-SIM: HMC_STAT_T: ROUTE RESPONSE STALLS */
        UNDEF_STALL_STAT,                    /*! HMC-SIM: HMC_STAT_T: UNDEFINED STALL EVENT */
        BANK_CONFLICT_STAT,                  /*! HMC-SIM: HMC_STAT_T: BANK CONFLICTS */
        XBAR_LATENCY_STAT,                   /*! HMC-SIM: HMC_STAT_T: CROSSBAR LATENCY */
        LINK_PHY_POWER_STAT,                 /*! HMC-SIM: HMC_STAT_T: LINK PHY POWER */
        LINK_LOCAL_ROUTE_POWER_STAT,         /*! HMC-SIM: HMC_STAT_T: LINK LOCAL ROUTE POWER */
        LINK_REMOTE_ROUTE_POWER_STAT,        /*! HMC-SIM: HMC_STAT_T: LINK REMOTE ROUTE POWER */
        XBAR_RQST_SLOT_POWER_STAT,           /*! HMC-SIM: HMC_STAT_T: CROSSBAR REQUEST SLOT POWER */
        XBAR_RSP_SLOT_POWER_STAT,            /*! HMC-SIM: HMC_STAT_T: CROSSBAR RESPONSE SLOT POWER */
        XBAR_ROUTE_EXTERN_POWER_STAT,        /*! HMC-SIM: HMC_STAT_T: CROSSBAR ROUTE EXTERN POWER */
        VAULT_RQST_SLOT_POWER_STAT,          /*! HMC-SIM: HMC_STAT_T: VAULT REQUEST SLOT POWER */
        VAULT_RSP_SLOT_POWER_STAT,           /*! HMC-SIM: HMC_STAT_T: VAULT RESPONSE SLOT POWER */
        VAULT_CTRL_POWER_STAT,               /*! HMC-SIM: HMC_STAT_T: VAULT CONTROL POWER */
        ROW_ACCESS_POWER_STAT,               /*! HMC-SIM: HMC_STAT_T: ROW ACCESS POWER */
        LINK_PHY_THERM_STAT,                 /*! HMC-SIM: HMC_STAT_T: LINK PHY THERM */
        LINK_LOCAL_ROUTE_THERM_STAT,         /*! HMC-SIM: HMC_STAT_T: LINK LOCAL ROUTE THERM */
        LINK_REMOTE_ROUTE_THERM_STAT,        /*! HMC-SIM: HMC_STAT_T: LINK REMOTE ROUTE THERM */
        XBAR_RQST_SLOT_THERM_STAT,           /*! HMC-SIM: HMC_STAT_T: CROSSBAR REQUEST SLOT THERM */
        XBAR_RSP_SLOT_THERM_STAT,            /*! HMC-SIM: HMC_STAT_T: CROSSBAR RESPONSE SLOT THERM */
        XBAR_ROUTE_EXTERN_THERM_STAT,        /*! HMC-SIM: HMC_STAT_T: CROSSBAR ROUTE EXTERN THERM */
        VAULT_RQST_SLOT_THERM_STAT,          /*! HMC-SIM: HMC_STAT_T: VAULT REQUEST SLOT THERM */
        VAULT_RSP_SLOT_THERM_STAT,           /*! HMC-SIM: HMC_STAT_T: VAULT RESPONSE SLOT THERM */
        VAULT_CTRL_THERM_STAT,               /*! HMC-SIM: HMC_STAT_T: VAULT CONTROL THERM */
        ROW_ACCESS_THERM_STAT,               /*! HMC-SIM: HMC_STAT_T: ROW ACCESS THERM */
        AVG_PACKET_LATENCY                   /*! HMC-SIM: HMC_STAT_T: AVERAGE PACKET LATENCY */
}hmc_stat_t;

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
	RSP_NONE,			/*! HMC-SIM: HMC_RESPONSE_T: NO RESPONSE COMMAND */
        RSP_CMC                         /*! HMC-SIM: HMC_RESPONSE_T: CUSTOM CMC RESPONSE */
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
        RD256,                          /*! HMC-SIM: HMC_RQST_T: 256-BYTE READ REQUEST */
	MD_RD,				/*! HMC-SIM: HMC_RQST_T: MODE READ REQUEST */
	FLOW_NULL,			/*! HMC-SIM: HMC_RQST_T: NULL FLOW CONTROL */
	PRET,				/*! HMC-SIM: HMC_RQST_T: RETRY POINTER RETURN FLOW CONTROL */
	TRET,				/*! HMC-SIM: HMC_RQST_T: TOKEN RETURN FLOW CONTROL */
	IRTRY,				/*! HMC-SIM: HMC_RQST_T: INIT RETRY FLOW CONTROL */

	/* -- version 2.0 Command Additions */
	WR256,				/*! HMC-SIM: HMC_RQST_T: 256-BYTE WRITE REQUEST */
	P_WR256,			/*! HMC-SIM: HMC_RQST_T: 256-BYTE POSTED WRITE REQUEST */	
	TWOADDS8R,			/*! HMC-SIM: HMC_RQST_T: */
	ADDS16R,			/*! HMC-SIM: HMC_RQST_T: */
	INC8,				/*! HMC-SIM: HMC_RQST_T: 8-BYTE ATOMIC INCREMENT */
	P_INC8,				/*! HMC-SIM: HMC_RQST_T: POSTED 8-BYTE ATOMIC INCREMENT */
	XOR16,				/*! HMC-SIM: HMC_RQST_T: 16-BYTE ATOMIC XOR */
	OR16,				/*! HMC-SIM: HMC_RQST_T: 16-BYTE ATOMIC OR */
	NOR16,				/*! HMC-SIM: HMC_RQST_T: 16-BYTE ATOMIC NOR */
	AND16,				/*! HMC-SIM: HMC_RQST_T: 16-BYTE ATOMIC AND */
	NAND16,				/*! HMC-SIM: HMC_RQST_T: 16-BYTE ATOMIC NAND */
	CASGT8,				/*! HMC-SIM: HMC_RQST_T: 8-BYTE COMPARE AND SWAP IF GT */
	CASGT16,			/*! HMC-SIM: HMC_RQST_T: 16-BYTE COMPARE AND SWAP IF GT */
	CASLT8,				/*! HMC-SIM: HMC_RQST_T: 8-BYTE COMPARE AND SWAP IF LT */
	CASLT16,			/*! HMC-SIM: HMC_RQST_T: 16-BYTE COMPARE AND SWAP IF LT */
	CASEQ8,				/*! HMC-SIM: HMC_RQST_T: 8-BYTE COMPARE AND SWAP IF EQ */
	CASZERO16,			/*! HMC-SIM: HMC_RQST_T: 16-BYTE COMPARE AND SWAP IF ZERO */
	EQ8,				/*! HMC-SIM: HMC_RQST_T: 8-BYTE ATOMIC EQUAL */
	EQ16,				/*! HMC-SIM: HMC_RQST_T: 16-BYTE ATOMIC EQUAL */
	BWR8R,				/*! HMC-SIM: HMC_RQST_T: 8-BYTE ATOMIC BIT WRITE WITH RETURN */
	SWAP16,				/*! HMC-SIM: HMC_RQST_T: 16-BYTE ATOMIC SWAP */

	/* -- CMC Types */
        CMC04,                          /*! HMC-SIM: HMC_RQST_T: CMC CMD=4 */
        CMC05,                          /*! HMC-SIM: HMC_RQST_T: CMC CMD=5 */
        CMC06,                          /*! HMC-SIM: HMC_RQST_T: CMC CMD=6 */
        CMC07,                          /*! HMC-SIM: HMC_RQST_T: CMC CMD=7 */
        CMC20,                          /*! HMC-SIM: HMC_RQST_T: CMC CMD=20 */
        CMC21,                          /*! HMC-SIM: HMC_RQST_T: CMC CMD=21 */
        CMC22,                          /*! HMC-SIM: HMC_RQST_T: CMC CMD=22 */
        CMC23,                          /*! HMC-SIM: HMC_RQST_T: CMC CMD=23 */
        CMC32,                          /*! HMC-SIM: HMC_RQST_T: CMC CMD=32 */
        CMC36,                          /*! HMC-SIM: HMC_RQST_T: CMC CMD=36 */
        CMC37,                          /*! HMC-SIM: HMC_RQST_T: CMC CMD=37 */
        CMC38,                          /*! HMC-SIM: HMC_RQST_T: CMC CMD=38 */
        CMC39,                          /*! HMC-SIM: HMC_RQST_T: CMC CMD=39 */
        CMC41,                          /*! HMC-SIM: HMC_RQST_T: CMC CMD=41 */
        CMC42,                          /*! HMC-SIM: HMC_RQST_T: CMC CMD=42 */
        CMC43,                          /*! HMC-SIM: HMC_RQST_T: CMC CMD=43 */
        CMC44,                          /*! HMC-SIM: HMC_RQST_T: CMC CMD=44 */
        CMC45,                          /*! HMC-SIM: HMC_RQST_T: CMC CMD=45 */
        CMC46,                          /*! HMC-SIM: HMC_RQST_T: CMC CMD=46 */
        CMC47,                          /*! HMC-SIM: HMC_RQST_T: CMC CMD=47 */
        CMC56,                          /*! HMC-SIM: HMC_RQST_T: CMC CMD=56 */
        CMC57,                          /*! HMC-SIM: HMC_RQST_T: CMC CMD=57 */
        CMC58,                          /*! HMC-SIM: HMC_RQST_T: CMC CMD=58 */
        CMC59,                          /*! HMC-SIM: HMC_RQST_T: CMC CMD=59 */
        CMC60,                          /*! HMC-SIM: HMC_RQST_T: CMC CMD=60 */
        CMC61,                          /*! HMC-SIM: HMC_RQST_T: CMC CMD=61 */
        CMC62,                          /*! HMC-SIM: HMC_RQST_T: CMC CMD=62 */
        CMC63,                          /*! HMC-SIM: HMC_RQST_T: CMC CMD=63 */
        CMC69,                          /*! HMC-SIM: HMC_RQST_T: CMC CMD=69 */
        CMC70,                          /*! HMC-SIM: HMC_RQST_T: CMC CMD=70 */
        CMC71,                          /*! HMC-SIM: HMC_RQST_T: CMC CMD=71 */
        CMC72,                          /*! HMC-SIM: HMC_RQST_T: CMC CMD=72 */
        CMC73,                          /*! HMC-SIM: HMC_RQST_T: CMC CMD=73 */
        CMC74,                          /*! HMC-SIM: HMC_RQST_T: CMC CMD=74 */
        CMC75,                          /*! HMC-SIM: HMC_RQST_T: CMC CMD=75 */
        CMC76,                          /*! HMC-SIM: HMC_RQST_T: CMC CMD=76 */
        CMC77,                          /*! HMC-SIM: HMC_RQST_T: CMC CMD=77 */
        CMC78,                          /*! HMC-SIM: HMC_RQST_T: CMC CMD=78 */
        CMC85,                          /*! HMC-SIM: HMC_RQST_T: CMC CMD=85 */
        CMC86,                          /*! HMC-SIM: HMC_RQST_T: CMC CMD=86 */
        CMC87,                          /*! HMC-SIM: HMC_RQST_T: CMC CMD=87 */
        CMC88,                          /*! HMC-SIM: HMC_RQST_T: CMC CMD=88 */
        CMC89,                          /*! HMC-SIM: HMC_RQST_T: CMC CMD=89 */
        CMC90,                          /*! HMC-SIM: HMC_RQST_T: CMC CMD=90 */
        CMC91,                          /*! HMC-SIM: HMC_RQST_T: CMC CMD=91 */
        CMC92,                          /*! HMC-SIM: HMC_RQST_T: CMC CMD=92 */
        CMC93,                          /*! HMC-SIM: HMC_RQST_T: CMC CMD=93 */
        CMC94,                          /*! HMC-SIM: HMC_RQST_T: CMC CMD=94 */
        CMC102,                         /*! HMC-SIM: HMC_RQST_T: CMC CMD=102 */
        CMC103,                         /*! HMC-SIM: HMC_RQST_T: CMC CMD=103 */
        CMC107,                         /*! HMC-SIM: HMC_RQST_T: CMC CMD=107 */
        CMC108,                         /*! HMC-SIM: HMC_RQST_T: CMC CMD=108 */
        CMC109,                         /*! HMC-SIM: HMC_RQST_T: CMC CMD=109 */
        CMC110,                         /*! HMC-SIM: HMC_RQST_T: CMC CMD=110 */
        CMC111,                         /*! HMC-SIM: HMC_RQST_T: CMC CMD=111 */
        CMC112,                         /*! HMC-SIM: HMC_RQST_T: CMC CMD=112 */
        CMC113,                         /*! HMC-SIM: HMC_RQST_T: CMC CMD=113 */
        CMC114,                         /*! HMC-SIM: HMC_RQST_T: CMC CMD=114 */
        CMC115,                         /*! HMC-SIM: HMC_RQST_T: CMC CMD=115 */
        CMC116,                         /*! HMC-SIM: HMC_RQST_T: CMC CMD=116 */
        CMC117,                         /*! HMC-SIM: HMC_RQST_T: CMC CMD=117 */
        CMC118,                         /*! HMC-SIM: HMC_RQST_T: CMC CMD=118 */
        CMC120,                         /*! HMC-SIM: HMC_RQST_T: CMC CMD=120 */
        CMC121,                         /*! HMC-SIM: HMC_RQST_T: CMC CMD=121 */
        CMC122,                         /*! HMC-SIM: HMC_RQST_T: CMC CMD=122 */
        CMC123,                         /*! HMC-SIM: HMC_RQST_T: CMC CMD=123 */
        CMC124,                         /*! HMC-SIM: HMC_RQST_T: CMC CMD=124 */
        CMC125,                         /*! HMC-SIM: HMC_RQST_T: CMC CMD=125 */
        CMC126,                         /*! HMC-SIM: HMC_RQST_T: CMC CMD=126 */
        CMC127                          /*! HMC-SIM: HMC_RQST_T: CMC CMD=127 */

}hmc_rqst_t;

/* -------------------------------------------------- DATA STRUCTURES */

struct hmc_token_t{
  uint8_t status;                       /*! HMC-SIM: HMC_TOKEN_T: Status bits
                                          0 = unused; 1=set but no response;
                                          2 = set and response ready
                                          */
  hmc_response_t rsp;                   /*! HMC-SIM: HMC_TOKEN_T: Response type */
  uint32_t rsp_size;                    /*! HMC-SIM: HMC_TOKEN_T: Response data size */
  uint32_t device;                      /*! HMC-SIM: HMC_TOKEN_T: Response device */
  uint32_t link;                        /*! HMC-SIM: HMC_TOKEN_T: Response link */
  uint32_t slot;                        /*! HMC-SIM: HMC_TOKEN_T: Response slot */
  uint64_t en_clock;                    /*! HMC-SIM: HMC_TOKEN_T: Clock cycle of incoming packet */
  uint8_t data[256];                    /*! HMC-SIM: HMC_TOKEN_T: Response data */
};

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

	//struct hmc_dram_t *drams;	    /*! HMC-SIM: HMC_BANK_T: DRAMS */

	uint32_t id;			    /*! HMC-SIM: HMC_BANK_T: BANK ID */
					    /* 16-BYTE BANK INTERLEAVE */
        uint32_t delay;                     /*! HMC-SIM: HMC_BANK_T: CYCLES UNTIL BANK IS AVAILABLE */
        uint32_t valid;                     /*! HMC-SIM: HMC_BANK_T: RESPONSE PACKET IS VALID */
        uint64_t packet[HMC_MAX_UQ_PACKET]; /*! HMC-SIM: HMC_BANK_T: RESPONSE PACKET */
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

        uint64_t cmc_reg[HMC_NUM_CMC_REGS];     /*! HMC-SIM: HMC_DEV_T: CMC CONFIGURATION REGISTERS */

	uint32_t id;				/*! HMC-SIM: HMC_DEV_T: CUBE ID */

	uint8_t seq;				/*! HMC-SIM: HMC_DEV_T: SEQUENCE NUMBER */
};

struct hmc_cmc_t{

        /* -- data */
        hmc_rqst_t type;                /*! HMC-SIM: HMC_CMC_T: REGISTERED REQUEST TYPE */
        uint32_t cmd;                   /*! HMC-SIM: HMC_CMC_T: COMMAND CODE OF THE REQUEST */
        uint32_t rqst_len;              /*! HMC-SIM: HMC_CMC_T: REQUEST LENGTH */
        uint32_t rsp_len;               /*! HMC-SIM: HMC_CMC_T: RESPONSE LENGTH */
        hmc_response_t rsp_cmd;         /*! HMC-SIM: HMC_CMC_T: RESPONSE COMMAND */
        uint8_t rsp_cmd_code;           /*! HMC-SIM: HMC_CMC_T: RESPONSE COMMAND CODE */

        int track_power;                /*! HMC-SIM: HMC_CMC_T: DOES THIS CMC OP TRACK POWER? */

        uint32_t active;                /*! HMC-SIM: HMC_CMC_T: SIGNALS THAT THE COMMAND IS ACTIVE */
        uint32_t dynamic;               /*! HMC-SIM: HMC_CMC_T: SIGNALS THAT THE COMMAND IS DYNAMIC */
        void *handle;                   /*! HMC-SIM: HMC_CMC_T: DLSYM HANDLE */

        /* -- fptrs */
        int (*cmc_register)(hmc_rqst_t *,
                            uint32_t *,
                            uint32_t *,
                            uint32_t *,
                            hmc_response_t *,
                            uint8_t *);

        int (*cmc_execute)(void *,      /* hmc */
                           uint32_t,    /* dev */
                           uint32_t,    /* quad */
                           uint32_t,    /* vault */
                           uint32_t,    /* bank */
                           uint64_t,    /* addr */
                           uint32_t,    /* length */
                           uint64_t,    /* head */
                           uint64_t,    /* tail */
                           uint64_t *,  /* rqst_payload */
                           uint64_t *); /* rsp_payload */
        void (*cmc_str)(char *);
        void (*cmc_power)(uint32_t *,   /* row_ops */
                          float *);     /* transient power */
        uint32_t (*cmc_dyn_rsp)();      /* dynamic response length */
        uint32_t (*cmc_dyn_rqst)();     /* dynamic request length */
        uint32_t (*cmc_mem_ops)(uint64_t,
                                void *);/* memory operation parallelism */
};

struct hmc_internal_stat_t{
  uint64_t xbar_rqst_stall;   /*! HMC_INTERNAL_STAT_T: XBAR REQUEST STALLS */
  uint64_t xbar_rsp_stall;    /*! HMC_INTERNAL_STAT_T: XBAR RESPONSE STALLS */
  uint64_t vault_rqst_stall;  /*! HMC_INTERNAL_STAT_T: VAULT REQUEST STALLS */
  uint64_t vault_rsp_stall;   /*! HMC_INTERNAL_STAT_T: VAULT RESPONSE STALLS */
  uint64_t route_rqst_stall;  /*! HMC_INTERNAL_STAT_T: ROUTE REQUEST STALLS */
  uint64_t route_rsp_stall;   /*! HMC_INTERNAL_STAT_T: ROUTE RESPONSE STALLS */
  uint64_t undef_stall;       /*! HMC_INTERNAL_STAT_T: UNDEFINED STALLS */
  uint64_t bank_conflict;     /*! HMC_INTERNAL_STAT_T: BANK CONFLICTS */
  uint64_t xbar_latency;      /*! HMC_INTERNAL_STAT_T: XBAR LATENCY EVENTS */
  uint64_t packets;           /*! HMC_INTERNAL_STAT_T: NUMBER OF PACKETS */
  uint64_t t_latency;         /*! HMC_INTERNAL_STAT_T: TOTAL PACKET LATENCY */
};

struct hmc_power_t{
  /* -- local values */
  float link_phy;           /*! HMC_POWER_T: POWER FOR EACH LINK PHY PER CLOCK */
  float link_local_route;   /*! HMC_POWER_T: POWER FOR LOCAL LINK ROUTE TO QUAD */
  float link_remote_route;  /*! HMC_POWER_T: POWER FOR REMOTE LINK ROUTE TO QUAD */
  float xbar_rqst_slot;     /*! HMC_POWER_T: POWER FOR XBAR REQUEST SLOT */
  float xbar_rsp_slot;      /*! HMC_POWER_T: POWER FOR XBAR RESPONSE SLOT */
  float xbar_route_extern;  /*! HMC_POWER_T: POWER FOR ROUTED REQUEST TO EXTERNAL CUBE */
  float vault_rqst_slot;    /*! HMC_POWER_T: POWER FOR VAULT REQUEST SLOT */
  float vault_rsp_slot;     /*! HMC_POWER_T: POWER FOR VAULT RESPONSE SLOT */
  float vault_ctrl;         /*! HMC_POWER_T: POWER FOR VAULT CONTROLLER PER ACTIVE CLOCK */
  float row_access;         /*! HMC_POWER_T: POWER FOR ROW ACCESS */

  /* -- totals */
  float t_link_phy;           /*! HMC_POWER_T: TOTAL POWER FOR EACH LINK PHY PER CLOCK */
  float t_link_local_route;   /*! HMC_POWER_T: TOTAL POWER FOR LOCAL LINK ROUTE TO QUAD */
  float t_link_remote_route;  /*! HMC_POWER_T: TOTAL POWER FOR REMOTE LINK ROUTE TO QUAD */
  float t_xbar_rqst_slot;     /*! HMC_POWER_T: TOTAL POWER FOR XBAR REQUEST SLOT */
  float t_xbar_rsp_slot;      /*! HMC_POWER_T: TOTAL POWER FOR XBAR RESPONSE SLOT */
  float t_xbar_route_extern;  /*! HMC_POWER_T: TOTAL POWER FOR ROUTED REQUEST TO EXTERNAL CUBE */
  float t_vault_rqst_slot;    /*! HMC_POWER_T: TOTAL POWER FOR VAULT REQUEST SLOT */
  float t_vault_rsp_slot;     /*! HMC_POWER_T: TOTAL POWER FOR VAULT RESPONSE SLOT */
  float t_vault_ctrl;         /*! HMC_POWER_T: TOTAL POWER FOR VAULT CONTROLLER PER ACTIVE CLOCK */
  float t_row_access;         /*! HMC_POWER_T: TOTAL POWER FOR ROW ACCESS */

  /* -- output formats */
  int tecplot;                /*! HMC_POWER_T: INDICATES TECPLOT OUTPUT FORMAT */
  char prefix[1024];          /*! HMC_POWER_T: TECPLOT FILE NAME PREFIX */

  /* -- tecplot output data */
  struct HMC4LinkTec H4L;     /*! HMC_POWER_T: 4Link Tecplot data */
  struct HMC8LinkTec H8L;     /*! HMC_POWER_T: 8Link Tecplot data */
};

struct hmcsim_t{

	struct hmc_dev_t *devs;		/*! HMC-SIM: HMCSIM_T: DEVICE STRUCTURES */

        struct hmc_cmc_t *cmcs;         /*! HMC-SIM: HMCSIM_T: REGISTERED CMC OPERATIONS */

	uint32_t num_devs;		/*! HMC-SIM: HMCSIM_T: NUMBER OF DEVICES */ 
	uint32_t num_links;		/*! HMC-SIM: HMCSIM_T: LINKS PER DEVICE */	
	uint32_t num_quads;		/*! HMC-SIM: HCMSIM_T: QUADS PER DEVICE */
	uint32_t num_vaults;		/*! HMC-SIM: HMCSIM_T: VAULTS PER DEVICE */
	uint32_t num_banks;		/*! HMC-SIM: HMCSIM_T: BANKS PER VAULT */
	uint32_t num_drams;		/*! HMC-SIM: HMCSIM_T: DRAMS PER BANK */
	uint32_t capacity;		/*! HMC-SIM: HMCSIM_T: CAPACITY PER DEVICE */

        uint32_t num_cmc;               /*! HMC-SIM: HMCSIM_T: NUMBER OF REGISTERED CMC OPERATIONS */

	uint32_t queue_depth;		/*! HMC-SIM: HMCSIM_T: VAULT QUEUE DEPTH */
	uint32_t xbar_depth;		/*! HMC-SIM: HMCSIM_T: CROSSBAR QUEUE DEPTH */

        uint32_t dramlatency;           /*! HMC-sIM: HMCSIM_T: DRAM ACCESS LATENCY IN CYCLES */

	FILE *tfile;			/*! HMC-SIM: HMCSIM_T: TRACE FILE HANDLER */
	uint32_t tracelevel;		/*! HMC-SIM: HMCSIM_T: TRACE LEVEL */

	uint8_t seq;			/*! HMC-SIM: HCMSIM_T: SEQUENCE NUMBER */

	uint64_t clk;			/*! HMC-SIM: HMCSIM_T: CLOCK TICK */

        uint32_t simple_link;           /*! HMC-SIM: HMCSIM_T: SIMPLIFIED API LINK HANDLER */

        struct hmc_power_t power;       /*! HMC-SIM: HMCSIM_T: POWER MEASUREMENT VALUES */

        struct hmc_internal_stat_t istat; /*! HMC-SIM: HMCSIM_T: INTERNAL STATUS COUNTERS */

        struct hmc_token_t tokens[1024];/*! HMC-SIM: HMCSIM_T: SIMPLE API TOKEN HANDLERS */

        int (*readmem)(struct hmcsim_t *,
                       uint64_t,
                       uint64_t *,
                       uint32_t );

        int (*writemem)(struct hmcsim_t *,
                       uint64_t,
                       uint64_t *,
                       uint32_t );

        int (*read_cmcreg)(struct hmcsim_t *hmc,
                           uint32_t dev,
                           uint64_t idx,
                           uint64_t *data);

        int (*write_cmcreg)(struct hmcsim_t *hmc,
                            uint32_t dev,
                            uint64_t idx,
                            uint64_t data);

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
	uint64_t 		*__ptr_end;
};

#ifdef __cplusplus
} /* extern C */
#endif

#endif	/* _HMC_SIM_TYPES_H_ */

/* EOF */
