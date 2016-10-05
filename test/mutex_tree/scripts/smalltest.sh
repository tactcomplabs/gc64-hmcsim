#!/bin/bash

ulimit -s unlimited

MUTEX=../mutex_tree
RUNDIR=`pwd`

BANKS=16
CAPACITY=4
LINKS=4
BSIZE=64
QDEPTH=64
XDEPTH=128
VAULTS=16
DRAMS=20
THREADS=10
LIBSRC=../../../cmc/mutex/


echo "Executing $MUTEX -L $LIBSRC -b $BANKS -c $CAPACITY -d $DRAMS -l $LINKS -m $BSIZE -n 1 -q $QDEPTH -v $VAULTS -x $XDEPTH -T $THREADS"


$MUTEX -L $LIBSRC -b $BANKS -c $CAPACITY -d $DRAMS -l $LINKS -m $BSIZE -n 1 -q $QDEPTH -v $VAULTS -x $XDEPTH -T $THREADS
