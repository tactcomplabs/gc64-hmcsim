/* 
 * _SHIFTAMT_C_ 
 * 
 * HMCSIM GUPS TEST 
 * 
 * FUNCTIONS TO GET THE APPROPRIATE ADDRESS SHIFT AMOUNT
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/* --------------------------------------------- GETSHIFTAMOUNT */
/* 
 * GETSHIFTAMOUNT
 * 
 */
extern int getshiftamount( 	uint32_t num_links, 
				uint32_t capacity, 
				uint32_t bsize,
				uint32_t *shiftamt )
{

	if( num_links == 4 ){ 
		/* 
		 * 4 link devices
		 * 
		 */
		if( capacity == 2 ){ 
			/* 
			 * 2GB capacity 
	 		 *
			 */
			switch( bsize )	
			{
				case 32 : 
					*shiftamt = 5;
					break;
				case 64 : 
					*shiftamt = 6;
					break;
				case 128:
					*shiftamt = 7;
					break;
				default:
					return -1;
					break;
			}
		}else if( capacity == 4 ){
			/* 
			 * 4GB capacity 
	 		 *
			 */
			switch( bsize )	
			{
				case 32 : 
					*shiftamt = 5;
					break;
				case 64 : 
					*shiftamt = 6;
					break;
				case 128:
					*shiftamt = 7;
					break;
				default:
					return -1;
					break;
			}
		}else{ 
			return -1;
		}

	}else if( num_links == 8 ){
		/* 
		 * 8 link devices
		 * 
		 */
		if( capacity == 4 ){ 
			/* 
			 * 4GB capacity 
	 		 *
			 */
			switch( bsize )	
			{
				case 32 : 
					*shiftamt = 5;
					break;
				case 64 : 
					*shiftamt = 6;
					break;
				case 128:
					*shiftamt = 7;
					break;
				default:
					return -1;
					break;
			}
		}else if( capacity == 8 ){ 
			/* 
			 * 8GB capacity 
	 		 *
			 */
			switch( bsize )	
			{
				case 32 : 
					*shiftamt = 5;
					break;
				case 64 : 
					*shiftamt = 6;
					break;
				case 128:
					*shiftamt = 7;
					break;
				default:
					return -1;
					break;
			}
		}else{
			return -1;
		}
	}else{
		return -1;
	}

	
	return 0;
}

/* EOF */

