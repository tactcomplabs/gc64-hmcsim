/* 
 * _HMC_RQST_C_
 * 
 * HYBRID MEMORY CUBE SIMULATION LIBRARY 
 * 
 * MEMORY REQUEST HANDLERS
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hmc_sim.h"


/* ----------------------------------------------------- HMCSIM_RQST_GETSEQ */
/* 
 * HMCSIM_RQST_GETSEQ
 * 
 */
static uint8_t hmcsim_rqst_getseq( struct hmcsim_t *hmc, hmc_rqst_t type )
{
	if( (type == PRET) || (type == IRTRY) ){ 
		return hmc->seq;
	}

	hmc->seq++;

	if( hmc->seq > 0x07 ){ 
		hmc->seq = 0x00;
	}

	return 0x01;
}

/* ----------------------------------------------------- HMCSIM_RQST_GETRRP */
/* 
 * HMCSIM_RQST_GETRRP
 * 
 */
static uint8_t hmcsim_rqst_getrrp( struct hmcsim_t *hmc )
{
	return 0x03;
}

/* ----------------------------------------------------- HMCSIM_RQST_GETFRP */
/* 
 * HMCSIM_RQST_GETFRP
 * 
 */
static uint8_t hmcsim_rqst_getfrp( struct hmcsim_t *hmc )
{
	return 0x02;
}

/* ----------------------------------------------------- HMCSIM_RQST_GETRTC */
/* 
 * HMCSIM_RQST_GETRTC
 * 
 */
static uint8_t hmcsim_rqst_getrtc( struct hmcsim_t *hmc )
{
	return 0x01;
}


/* ----------------------------------------------------- HMCSIM_CRC32 */
/* 
 * HMCSIM_CRC32
 * 
 */
static uint32_t hmcsim_crc32( uint64_t addr, uint64_t *payload, uint32_t len )
{
	/* vars */
	uint32_t crc	= 0x11111111;
	/* ---- */

	return crc;
}

/* ----------------------------------------------------- HMCSIM_BUILD_MEMREQUEST */
/* 
 * HMCSIM_BUILD_MEMREQUEST
 * 
 */
extern int	hmcsim_build_memrequest( struct hmcsim_t *hmc, 
					uint8_t  cub, 
					uint64_t addr, 
					uint16_t  tag, 
					hmc_rqst_t type,
					uint8_t link, 
					uint64_t *payload, 
					uint64_t *rqst_head,
					uint64_t *rqst_tail )
{
	/* vars */
	uint8_t cmd	= 0x00;
	uint8_t rrp	= 0x00;
	uint8_t frp	= 0x00;
	uint8_t seq	= 0x00;
	uint8_t rtc	= 0x00;
	uint32_t crc	= 0x00000000;
	uint32_t flits	= 0x00000000;
	uint64_t tmp	= 0x00ll;
	/* ---- */

	if( hmc == NULL ){ 
		return -1;
	}


	/* 
	 * we do no validation of inputs here
	 * users may want to deliberately create bogus
	 * requests
	 * 
	 */

	/* 
	 * get the correct command bit sequence
	 *
	 */
	switch( type )
	{
		case WR16:
			flits	= 2;
			cmd	= 0x08;		/* 001000 */
			break;
		case WR32:
			flits	= 3;
			cmd	= 0x09;		/* 001001 */
			break;
		case WR48:
			flits	= 4;
			cmd	= 0x0A;		/* 001010 */
			break;
		case WR64:
			flits	= 5;
			cmd	= 0x0B;		/* 001011 */
			break;
		case WR80:
			flits	= 6;
			cmd	= 0x0C;		/* 001100 */
			break;
		case WR96:
			flits	= 7;
			cmd	= 0x0D;		/* 001101 */
			break;
		case WR112:
			flits	= 8;
			cmd	= 0x0E;		/* 001110 */
			break;
		case WR128:
			flits	= 9;
			cmd	= 0x0F;		/* 001111 */
			break;
		case MD_WR:
			flits	= 2;
			cmd	= 0x10;		/* 010000 */
			break;
		case BWR:
			flits	= 2;
			cmd	= 0x11;		/* 010001 */
			break;
		case TWOADD8:
			flits	= 2;
			cmd	= 0x12;		/* 010010 */
			break;
		case ADD16:
			flits	= 2;
			cmd	= 0x13;		/* 010011 */
			break;
		case P_WR16:
			flits	= 2;
			cmd	= 0x18;		/* 011000 */
			break;
		case P_WR32:
			flits	= 3;
			cmd	= 0x19;		/* 011001 */
			break;
		case P_WR48:
			flits	= 4;
			cmd	= 0x1A;		/* 011010 */
			break;
		case P_WR64:
			flits	= 5;
			cmd	= 0x1B;		/* 011011 */
			break;
		case P_WR80:
			flits	= 6;
			cmd	= 0x1C;		/* 011100 */
			break;
		case P_WR96:
			flits	= 7;
			cmd	= 0x1D;		/* 011101 */
			break;
		case P_WR112:
			flits	= 8;
			cmd	= 0x1E;		/* 011110 */
			break;
		case P_WR128:
			flits	= 9;
			cmd	= 0x1F;		/* 011111 */
			break;
		case P_BWR:
			flits	= 2;
			cmd	= 0x21;		/* 100001 */
			break;
		case P_2ADD8:
			flits	= 2;
			cmd	= 0x22;		/* 100010 */
			break;
		case P_ADD16:
			flits	= 2;
			cmd	= 0x23;		/* 100011 */
			break;
		case RD16:
			flits	= 1;
			cmd	= 0x30;		/* 110000 */
			break;
		case RD32:
			flits	= 1;
			cmd	= 0x31;		/* 110001 */
			break;
		case RD48:
			flits	= 1;
			cmd	= 0x32;		/* 110010 */
			break;
		case RD64:
			flits	= 1;
			cmd	= 0x33;		/* 110011 */
			break;
		case RD80:
			flits	= 1;
			cmd	= 0x34;		/* 110100 */
			break;
		case RD96:
			flits	= 1;
			cmd	= 0x35;		/* 110101 */
			break;
		case RD112:
			flits	= 1;
			cmd	= 0x36;		/* 110110 */
			break;
		case RD128:
			flits	= 1;
			cmd	= 0x37;		/* 110111 */
			break;
		case MD_RD:
			flits	= 1;
			cmd	= 0x28;		/* 101000 */
			break;
		case FLOW_NULL:
			flits	= 0;
			cmd	= 0x00;		/* 000000 */
			break;
		case PRET:
			flits	= 1;
			cmd	= 0x01;		/* 000001 */
			break;
		case TRET:
			flits	= 1;
			cmd	= 0x02;		/* 000010 */
			break;
		case IRTRY:
			flits	= 1;
			cmd	= 0x03;		/* 000011 */
			break;
		default:
			return -1;
			break;
	}

	/*
	 * build the request packet header
	 * 
	 */
	
	/* -- cmd field : bits 5:0 */
	tmp |= (cmd & 0x3F);
	
	/* -- lng field in flits : bits 10:7 */
	tmp |= ( (uint64_t)(flits & 0xF) << 7 );

	/* -- dln field; duplicate of lng : bits 14:11 */
	tmp |= ( (uint64_t)(flits & 0xF) << 11 );

	/* -- tag field: bits 23:15 */
	tmp |= ( (uint64_t)(tag & 0x1FF) << 15 );

	/* -- address field : bits 57:24 */
	tmp |= ( (addr& 0x3FFFFFFFF) << 24 );

	/* -- cube id field : bits 63:61 */
	tmp |= ( (uint64_t)(cub&0x7) << 61 );

	/* write the request header out */
	*rqst_head	= tmp;	

	tmp = 0x00ll;

	/*
	 * build the request packet tail
	 * 
	 */

	/* -- return retry pointer : bits 7:0 */
	rrp = hmcsim_rqst_getrrp( hmc );
	tmp |= rrp;

	/* -- forward retry pointer : bits 15:8 */
	frp = hmcsim_rqst_getfrp( hmc );
	tmp |= ( (uint64_t)(frp & 0xFF) << 8 );

	/* -- sequence number : bits 18:16 */
	seq = hmcsim_rqst_getseq( hmc, type );
	tmp |= ( (uint64_t)(seq & 0x7) << 16 );

	/* -- source link id : bits 26:24 */
	tmp |= ( (uint64_t)(link & 0x7) << 24 );

	/* -- return token count : bits 31:27 */
	rtc = hmcsim_rqst_getrtc( hmc );
	tmp |= ( (uint64_t)(rtc & 0x1F) << 27 );

	/* -- retrieve the crc : bits 63:32 */
	crc = hmcsim_crc32( addr, payload, (2*flits) );
	tmp |= ( (uint64_t)(crc & 0xFFFFFFFF) << 32 );

	/* write the request tail out */
	*rqst_tail	= tmp;
	
	return 0;
}

/* EOF */
