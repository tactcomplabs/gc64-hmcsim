#!/bin/bash

ulimit -s unlimited

SREAD=../stencil_read
RUNDIR=`pwd`

BANKS=16
CAPACITY=8
LINKS=8
BSIZE=64
QDEPTH=64
XDEPTH=128
VAULTS=64
DRAMS=20
LIBSRC=../../../cmc/


echo "Executing $SREAD -L $LIBSRC -b $BANKS -c $CAPACITY -d $DRAMS -l $LINKS -m $BSIZE -n 1 -q $QDEPTH -v $VAULTS -x $XDEPTH"


$SREAD -L $LIBSRC -b $BANKS -c $CAPACITY -d $DRAMS -l $LINKS -m $BSIZE -n 1 -q $QDEPTH -v $VAULTS -x $XDEPTH
