#!/bin/bash
#-- pulls all the trace values from an hmcsim trace output file
#-- exports to individual files for importing into gnuplot
#-- 

FILE=$1

if [ -z $FILE ]; then
	echo "error : usage : $0 /path/to/trace.out"
	exit -1
fi

#-- get the clock values
CLOCKS=`cat $FILE | awk '{print $3}' | uniq`


#-- touch all the output files
touch xbar_latency.out
touch xbar_rqst_stall.out
touch bank_conflict.out
touch wr64.out
touch rd64.out

#-- for each clock value, get the outputs
for a in $CLOCKS; do

	#-- XBAR_LATENCY
	XBAR_LATENCY=`cat $FILE | grep "HMCSIM_TRACE : $a : XBAR_LATENCY" | wc -l`

	#-- XBAR_RQST_STALL
	XBAR_RQST_STALL=`cat $FILE | grep "HMCSIM_TRACE : $a : XBAR_RQST_STALL" | wc -l`

	#-- BANK_CONFLICT
	BANK_CONFLICT=`cat $FILE | grep "HMCSIM_TRACE : $a : BANK_CONFLICT" | wc -l`

	#-- WR64
	WR64=`cat $FILE | grep "HMCSIM_TRACE : $a : WR64" | wc -l`
	
	#-- RD64
	RD64=`cat $FILE | grep "HMCSIM_TRACE : $a : RD64" | wc -l`

	#-- write out the values
	echo "$a $XBAR_LATENCY" >> xbar_latency.out 2>&1
	echo "$a $XBAR_RQST_STALL" >> xbar_rqst_stall.out 2>&1
	echo "$a $BANK_CONFLICT" >> bank_conflict.out 2>&1
	echo "$a $WR64" >> wr64.out 2>&1
	echo "$a $RD64" >> rd64.out 2>&1
done;



