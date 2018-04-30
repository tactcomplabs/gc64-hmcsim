#/bin/bash

ulimit -s unlimited

SREAD=../stencil_read
RUNDIR=`pwd`

BANKS=16
CAPACITY=4
LINKS=4
BSIZE=64
QDEPTH=64
XDEPTH=128
VAULTS=32
DRAMS=20
CLOCKGHZ=2.0
ARRAYWIDTH=24
ARRAYHEIGHT=24
ARRAYDEPTH=24
THREADS=16
CORES=2
LIBSRC=../../../cmc/


echo "Executing $SREAD -L $LIBSRC -b $BANKS -c $CAPACITY -d $DRAMS -l $LINKS -m $BSIZE -n 1 -q $QDEPTH -v $VAULTS -x $XDEPTH -i $ARRAYWIDTH -j $ARRAYHEIGHT -k $ARRAYDEPTH -t $THREADS -C $CLOCKGHZ -o $CORES"


$SREAD -L $LIBSRC -b $BANKS -c $CAPACITY -d $DRAMS -l $LINKS -m $BSIZE -n 1 -q $QDEPTH -v $VAULTS -x $XDEPTH -i $ARRAYWIDTH -j $ARRAYHEIGHT -k $ARRAYDEPTH -t $THREADS -C $CLOCKGHZ -o $CORES

