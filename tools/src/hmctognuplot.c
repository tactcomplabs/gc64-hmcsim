/* _HMCTOGNUPLOT_C_
 * 
 * HMC TRACE FILE TO GNUPLOT CONVERSION UTILITY 
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <inttypes.h>

/* ------------------------------------------------------------- MACROS */
#define BUFLEN	1024

/* ------------------------------------------------------------- INTERNAL STRUCTURES */
struct htog_count_t{
	uint64_t *bank_conflict;
	uint64_t *xbar_rqst_stall;
	uint64_t *xbar_latency;
	uint64_t *wr64;
	uint64_t *rd64;
};
struct htog_t{ 
	struct htog_count_t counts;
	uint64_t num_clocks;
	int verbose;
	char *fname;
};
/* --------------------------------------------------------------------------------- */



/* ------------------------------------------------------------- HTOG_FREE */
/* 
 * HTOG_FREE
 * 
 */
static int htog_free( struct htog_t *htog ) {

	if( htog->fname != NULL ){ 
		free( htog->fname );
		htog->fname = NULL;
	}

	if( htog->counts.bank_conflict != NULL ){ 
		free( htog->counts.bank_conflict );
		htog->counts.bank_conflict = NULL;
	}

	if( htog->counts.xbar_rqst_stall != NULL ){ 
		free( htog->counts.xbar_rqst_stall );
		htog->counts.xbar_rqst_stall = NULL;
	}

	if( htog->counts.xbar_latency != NULL ){ 
		free( htog->counts.xbar_latency );
		htog->counts.xbar_latency = NULL;
	}

	if( htog->counts.wr64 != NULL ){ 
		free( htog->counts.wr64 );
		htog->counts.wr64 = NULL;
	}

	if( htog->counts.rd64 != NULL ){ 
		free( htog->counts.rd64 );
		htog->counts.rd64 = NULL;
	}

	return 0;
}

/* ------------------------------------------------------------- PRINT_RESULTS */
/* 
 * PRINT_RESULTS
 * 
 */
static int print_results( struct htog_t *htog ) {

	/* vars */
	FILE *ofile = NULL;
	uint64_t i	= 0;
	/* ---- */

	/* 
	 * bank_conflict 
	 * 
	 */
	ofile = fopen( "bank_conflict.out", "w+" );
	if( ofile == NULL ){ 
		return -1;
	}
	for( i=0; i<htog->num_clocks; i++ ){ 
		fprintf( ofile, "%"PRIu64 " %"PRIu64 "\n", i, htog->counts.bank_conflict[i] );
	}
	fclose( ofile );
	ofile = NULL;

	/* 
	 *  xbar_rqst_stall
	 * 
	 */
	ofile = fopen( "xbar_rqst_stall.out", "w+" );
	if( ofile == NULL ){ 
		return -1;
	}
	for( i=0; i<htog->num_clocks; i++ ){ 
		fprintf( ofile, "%"PRIu64 " %"PRIu64 "\n", i, htog->counts.xbar_rqst_stall[i] );
	}
	fclose( ofile );
	ofile = NULL;

	/* 
	 *  xbar_latency
	 * 
	 */
	ofile = fopen( "xbar_latency.out", "w+" );
	if( ofile == NULL ){ 
		return -1;
	}
	for( i=0; i<htog->num_clocks; i++ ){ 
		fprintf( ofile, "%"PRIu64 " %"PRIu64 "\n", i, htog->counts.xbar_latency[i] );
	}
	fclose( ofile );
	ofile = NULL;

	/* 
	 *  wr64
	 * 
	 */
	ofile = fopen( "wr64.out", "w+" );
	if( ofile == NULL ){ 
		return -1;
	}
	for( i=0; i<htog->num_clocks; i++ ){ 
		fprintf( ofile, "%"PRIu64 " %"PRIu64 "\n", i, htog->counts.wr64[i] );
	}
	fclose( ofile );
	ofile = NULL;

	/* 
	 *  rd64
	 * 
	 */
	ofile = fopen( "rd64.out", "w+" );
	if( ofile == NULL ){ 
		return -1;
	}
	for( i=0; i<htog->num_clocks; i++ ){ 
		fprintf( ofile, "%"PRIu64 " %"PRIu64 "\n", i, htog->counts.rd64[i] );
	}
	fclose( ofile );
	ofile = NULL;



	return 0;
}

/* ------------------------------------------------------------- ALLOC_MEM */
/* 
 * ALLOC_MEM
 * 
 */
static int alloc_mem( struct htog_t *htog ) {

	/* vars */
	uint64_t i	= 0x00ll;
	/* ---- */

	htog->counts.bank_conflict = malloc( sizeof( uint64_t ) * htog->num_clocks );
	if( htog->counts.bank_conflict == NULL ){ 
		return -1;
	}

	htog->counts.xbar_rqst_stall = malloc( sizeof( uint64_t ) * htog->num_clocks );
	if( htog->counts.xbar_rqst_stall == NULL ){ 
		return -1;
	}

	htog->counts.xbar_latency = malloc( sizeof( uint64_t ) * htog->num_clocks );
	if( htog->counts.xbar_latency == NULL ){ 
		return -1;
	}

	htog->counts.wr64 = malloc( sizeof( uint64_t ) * htog->num_clocks );
	if( htog->counts.wr64 == NULL ){ 
		return -1;
	}

	htog->counts.rd64 = malloc( sizeof( uint64_t ) * htog->num_clocks );
	if( htog->counts.rd64 == NULL ){ 
		return -1;
	}

	/* 
	 * zero the arrays 
	 * 
	 */
	for( i=0; i<htog->num_clocks; i++ ){ 
		htog->counts.bank_conflict[i]	= 0x00ll;
		htog->counts.xbar_rqst_stall[i]	= 0x00ll;
		htog->counts.xbar_latency[i]	= 0x00ll;
		htog->counts.wr64[i]		= 0x00ll;
		htog->counts.rd64[i]		= 0x00ll;
	}

	return 0;
}

/* ------------------------------------------------------------- SCAN_CLOCKS */
/* 
 * SCAN_CLOCKS
 * 
 */
static int scan_clocks( FILE *infile, struct htog_t *htog ) {

	/* vars */
	char buf[BUFLEN];
	char *pch	= NULL;
	char *pend	= NULL;
	int ne		= 0;
	int flag	= 1;
	uint64_t tc	= 0x00ll;
	/* ---- */


	memset( buf, 0, sizeof( char ) * BUFLEN );

	fseek( infile, 0, SEEK_END );

	/* 
	 * step back three newline chars
	 * 
	 */
	while( (ne < 3) && (flag>0) ){ 
		
		if( fgetc( infile ) == '\n' ){
			ne++;
		}

		if( fseek( infile, -2, SEEK_CUR) == -1 ){ 
			flag = 0;
		}
	}

	/* 
	 * grab a few lines and parse for the clock value
	 * 
 	 */	
	while( !feof( infile ) ){

		/* 
		 * get a line from the file 
		 * 
		 */
		fgets( buf, 1024, infile );

		/*
		 * make sure its not a comment or a partial line
	 	 * 
		 */
		if( (buf[0] != '\n') && (strlen(buf) > 10) ){
			
			/* 
			 * found a valid line, parse it 
		 	 * 
			 */	
			pch = strtok( buf, " " );
	
			/* 
			 * get the second string ":"
			 *
			 */	
			if( pch != NULL ){ 
				pch = strtok( NULL, " " );
			}

			/* 
			 * get the third string "<val>"
			 *
			 */	
			if( pch != NULL ){ 
				pch = strtok( NULL, " " );
			}

			/* 
			 * get the value 
			 *
			 */
			if( pch != NULL ){ 
				tc = strtoull( pch, &pend, 10 );

				if( tc > htog->num_clocks ){ 
					htog->num_clocks = tc;
				}
			}
		}

	}

	if( htog->verbose == 1 ){ 
		printf( "Notice : found %" PRIu64 " clocks\n", htog->num_clocks ); 
	}

	return 0;
}

/* ------------------------------------------------------------- PARSE */
/* 
 * PARSE
 * 
 */
static int parse( FILE *infile, struct htog_t *htog ){ 

	/* vars */
	char buf[BUFLEN];
	char *pch	= NULL;
	char *pend	= NULL;
	uint64_t tc	= 0x00ll;
	/* ---- */


	while( !feof( infile ) ){

		/* 
		 * get a line from the file 
		 * 
		 */
		memset( buf, 0, sizeof( char ) * BUFLEN );
		fgets( buf, 1024, infile );

		/*
		 * make sure its not a comment or a partial line
	 	 * 
		 */
		if( (buf[0] != '\n') && 
			(strlen(buf) > 10) &&
			(buf[0] != '#') ){
			
			/* 
			 * found a valid line, parse it 
		 	 * 
			 */	
			pch = strtok( buf, " " );
	
			/* 
			 * get the second string ":"
			 *
			 */	
			if( pch != NULL ){ 
				pch = strtok( NULL, " " );
			}

			/* 
			 * get the third string "<val>"
			 *
			 */	
			if( pch != NULL ){ 
				pch = strtok( NULL, " " );
			}

			/* 
			 * get the clock value 
			 *
			 */
			if( pch != NULL ){ 
				tc = strtoull( pch, &pend, 10 );
			}

			/* 
			 * get the fourth string ":"
			 *
			 */	
			if( pch != NULL ){ 
				pch = strtok( NULL, " " );
			}

			/* 
			 * get the fourth fifth "<type>"
			 *
			 */	
			if( pch != NULL ){ 
				pch = strtok( NULL, " " );
			}

			/* 
			 * determine the type and update the values
			 * 
			 */
			if( strcmp( pch, "BANK_CONFLICT" ) == 0 ){ 
				htog->counts.bank_conflict[tc]++;
			}else if( strcmp( pch, "XBAR_RQST_STALL" ) == 0 ){ 
				htog->counts.xbar_rqst_stall[tc]++;
			}else if( strcmp( pch, "XBAR_LATENCY" ) == 0 ){
				htog->counts.xbar_latency[tc]++;
			}else if( strcmp( pch, "WR64" ) == 0 ){ 
				htog->counts.wr64[tc]++;
			}else if( strcmp( pch, "RD64" ) == 0 ){
				htog->counts.rd64[tc]++;
			}else{ 
				printf( "Found a bogus value : %s\n", pch );
			}

		}

	}

	return 0;
}

/* ------------------------------------------------------------- HTOG_CONVERT */
/* 
 * HTOG_CONVERT
 * 
 */
static int htog_convert( struct htog_t *htog ){ 

	/* vars */
	FILE *infile = NULL;
	/* ---- */

	if( htog == NULL ){ 
		return -1;
	}

	/* 
	 * stage 1: open the file 
	 * 
 	 */
	infile = fopen( htog->fname, "r" );
	if( infile == NULL ){ 
		printf( "Error : failed to open file : %s\n", htog->fname );
		return -1;
	}

	/* 
	 * stage 2: scan for the number of clock cycles
	 *          scan from the bottom up
	 */
	if( scan_clocks( infile, htog ) != 0 ){
		printf( "Error : failed to retrieve number of clock cycles\n" );
		fclose( infile );
		infile = NULL;
		return -1;
	}

	/* 
 	 * stage 3: allocate memory for count arrays
	 * 
 	 */
	if( alloc_mem( htog ) != 0 ){
		printf( "Error : failed to allocate memory\n" );
		fclose( infile );
		infile = NULL;
		return -1;
	}

	/* 
	 * stage 4: parse the whole file 
 	 * 
 	 */
	rewind( infile );
	if( parse( infile, htog ) != 0 ){
		printf( "Error : failed to parse the file\n" );
		fclose( infile );
		infile = NULL;
		return -1;
	}

	/* 
	 * stage 5: close the file 
	 * 
 	 */
	fclose( infile );
	infile = NULL;

	/* 
	 * stage 6: print the result files
	 * 
 	 */
	if( print_results( htog ) != 0 ) {
		printf( "Error : failed to print results files\n" );
		return -1;
	}

	return 0;
}

/* ------------------------------------------------------------- INIT STRUCTS */
/* 
 * INIT_STRUCTS
 * 
 */
static int init_structs( struct htog_t *htog ){

	if( htog == NULL ){ 
		return -1;
	}

	htog->counts.bank_conflict	= NULL;
	htog->counts.xbar_rqst_stall	= NULL;
	htog->counts.xbar_latency	= NULL;
	htog->counts.wr64		= NULL;
	htog->counts.rd64		= NULL;

	htog->num_clocks		= 0x00ll;	
	htog->fname			= NULL;
	htog->verbose			= 0;

	return 0;
}

/* ------------------------------------------------------------- SANITY_CHECK */
/* 
 * SANITY_CHECK
 * 
 */
static int sanity_check( struct htog_t *htog ){
	
	if( htog == NULL ){
		return -1;
	}

	if( htog->fname == NULL ){ 
		printf( "Error : filename is null\n" );
		return -1;
	}

	return 0;
}

/* ------------------------------------------------------------- MAIN */
/* 
 * MAIN 
 * 
 */
extern int main( int argc, char **argv ) {

	/* vars */
	int ret		= 0;
	struct htog_t	htog;
	/* ---- */


	/* 
 	 * init the structure 
	 * 
	 */
	if( init_structs( &htog ) != 0 ){
		printf( "Error : failed to init the internal structs\n" );
		return -1;
	}

	/* 
	 * parse the command line args 
	 * 
 	 */
	while(( ret = getopt( argc, argv, "F:hv" )) != -1 )
	{	
		switch( ret )
		{
			case 'F' : 
				htog.fname = malloc( sizeof( char ) * (strlen(optarg)+1) );
				if( htog.fname == NULL ){ 
					printf( "Error : could not allocate memory string filename\n" );
					return -1;
				}

				strcpy( htog.fname, optarg );

				break;
			case 'h' : 
				printf( "usage : %s%s", argv[0], " -F /path/to/tracefile -hv\n" );
				return 0;
				break;
			case 'v' : 	
				printf( "Enabling verbosity\n" );
				htog.verbose = 1;
				break;
			case '?':
			default:
				printf( "Unknown Option!\n" );
				printf( "usage : %s%s", argv[0], " -F /path/to/tracefile -hv\n" );
				return -1;
				break;
		}
	}

	/* 
	 * sanity check 
	 * 
 	 */
	if( sanity_check( &htog ) != 0 ){
		printf( "Error : failed to sanity check args\n" );
		htog_free( &htog );
		return -1;
	}

	/* 
	 * htog_convert
	 * 
 	 */
	if( htog_convert( &htog ) != 0 ){
		printf( "Error : failed to convert file\n" );
		htog_free( &htog );
		return -1;
	}		


	/* 
	 * free the memory 
	 * 
 	 */
	if( htog_free( &htog ) != 0 ){ 
		printf( "Error : failed to free memory\n" );
		return -1;	
	}

	return 0;
} 
