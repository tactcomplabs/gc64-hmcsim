/*
 * _SPMV_H_ 
 * 
 * SPARSE MATRIX VECTOR HMCSIM TEST HEADER FILE 
 * 
 */

#ifndef _SPMV_H_ 
#define _SPMV_H_ 

#include <stdint.h>

/* ------------------------------------------ DATA STRUCTURES */
struct csr_t {
	uint64_t *vals;
	uint64_t *cols;
	uint64_t *rows;
	uint64_t *x_vect;
	uint64_t *product;
};

#endif /* _SPMV_H_ */
/* EOF */
