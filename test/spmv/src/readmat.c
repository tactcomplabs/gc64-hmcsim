/* 
 * _READMAT_C_ 
 * 
 * HMC-SIM SPMV MATRIX UTILITY FUNCTIONS 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include "spmv.h"

/* ------------------------------------------------- ALLOCATE MEMORY */
/* 
 * allocates the csr spmv structure
 * 
 */
static int allocate_memory( struct csr_t *mat, int nzero, int ncols, int nrows )
{
	if( mat == NULL ){ 
		return -1;
	}

	mat->vals	= NULL;
	mat->cols	= NULL;
	mat->rows	= NULL;
	mat->x_vect	= NULL;

	mat->vals	= malloc( nzero * sizeof( uint64_t ) );
	if( mat->vals == NULL ){ 
		printf( "ERROR : could not allocate vals array\n" );
		return -1;
	}

	mat->cols	= malloc( nzero * sizeof( uint64_t ) );
	if( mat->cols == NULL ){ 
		printf( "ERROR : could not allocate cols array\n" );
		free( mat->vals );
		return -1;
	}

	mat->rows	= malloc( (nrows+1) * sizeof( uint64_t ) );
	if( mat->rows == NULL ){ 
		printf( "ERROR : could not allocate rows array\n" );
		free( mat->vals );
		free( mat->cols );
		return -1;
	}

	mat->x_vect	= malloc( ncols * sizeof( uint64_t ) );
	if( mat->x_vect == NULL ){ 
		printf( "ERROR : could not allocate x_vect array\n" );
		free( mat->vals );
		free( mat->rows );
		free( mat->cols );
		return -1;
	}

	return 0;
}

/* ------------------------------------------------- GENMAT */
/* 
 * generates a sample, synthetic matrix
 * 
 */
extern int genmat( int nzero, int nrows, int ncols, struct csr_t *mat )
{
	/* vars */
	int i		= 0;
	int j		= 0;
	int tmp_i 	= 0;
	int tmp_j 	= 0;
	int tmp_v	= 0;
	int first	= 0;
	uint64_t **t	= NULL;
	/* ---- */

	/* alloate the temp matrix */
	t	= malloc( sizeof( uint64_t * ) * nrows );
	if( t == NULL ){ 
		printf( "ERROR : could not allocate temp matrix\n" );
		return -1;
	}

	for( i = 0; i < nrows; i++ ){ 
		t[i]	= malloc( sizeof( uint64_t ) * ncols );
		for( j = 0; j < ncols; j++ ){ 
			t[i][j] = 0x00ll;
		}
	}

	for( i=0; i<nzero; i++ ){ 
		tmp_i	= rand() % (nrows-1);
		tmp_j	= rand() % (ncols-1);
		tmp_v	= rand()+1;	/* ensure we have a nonzero value */
		t[tmp_i][tmp_j]	= tmp_v;
	}

	/* allocate csr */
	if( allocate_memory( mat, nzero, ncols, nrows ) != 0 ){ 
		printf( "ERROR : could not allocate memory\n" );
		for( i=0; i<nrows; i++ ){ 
			free( t[i] );
		}
		free( t );

		return -1;
	}

	/* 
	 * convert the dense matrix to csr 
	 * 
	 */
	for( i=0; i<nrows; i++ ){ 
		for( j=0; j<ncols; j++ ){ 
			if( t[i][j] != 0x00ll ){ 
				/* 
				 * we found a hit, record it
				 * 
				 */

				/* values */
				mat->vals[tmp_i] = t[i][j];

				/* columns */
				mat->cols[tmp_i] = j;

				/* row start */
				if( first == 0 ){ 
					mat->rows[tmp_j] = tmp_i;
					tmp_j++;
					first = 1;
				}

				tmp_i++;
			}
		}
		
		first = 0;
	}


	/* last value */
	mat->rows[nrows] = mat->vals[nzero-1];

	/* free the matrix */
	for( i=0; i<nrows; i++ ){ 
		free( t[i] );
	}
	free( t );

	/* allocate the vector */
	mat->x_vect	= malloc( sizeof( uint64_t ) * ncols );
	if( mat->x_vect == NULL ){ 
		printf( "ERROR : could not allocate vector\n" );
		return -1;
	}

	/* randomly generate the input vector */
	for( i = 0; i < ncols; i ++ ){ 
		mat->x_vect[i] = rand();
	}

	return 0;
}

/* ------------------------------------------------- READMAT */
/* 
 * reads an interprets the spmv input file 
 * 
 */
extern int readmat( char *matfile, struct csr_t *mat )
{
	/* vars */
	FILE *infile	= NULL;
	/* ---- */

	if( matfile == NULL ){ 
		return -1;
	}else if( mat == NULL ){ 
		return -1;
	}

	infile = fopen( matfile, "r" );
	if( infile == NULL ){ 
		printf( "ERROR : could not open matfile at %s\n", matfile );
		return -1;
	}

	fclose( infile );

	return 0;
}

/* EOF */
