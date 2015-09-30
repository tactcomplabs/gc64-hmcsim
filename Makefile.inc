#!/bin/sh


#---------------------------------------------------------
# MACROS
#
# Each of the following can be added to the HMCSIM_MACROS
#     option below in order to enable/disable certain
#     options.
# 
# HMCSIM LIBRARY MAJOR VERSION NUMBER
# -DHMC_MAJOR_VERSION=n
#
# HMCSIM LIBRARY MINOR VERSION NUMBER
# -DHMC_MINOR_VERSION=n
#
# HMCSIM LIBRARY VENDOR ID
# -DHMC_VENDOR_ID=n
#
# HMCSIM PRODUCT REVISION [8-bit unsigned int]
# -DHMC_PRODUCT_REVISION=n
#
# HMCSIM PROTOCOL REVISION [8-bit unsigned int]
# -DHMC_PROTOCOL_REVISION=n
#
# HMCSIM PHY REVISION [8-bit unsigned int]
# -DHMC_PHY_REVISION=n
#
# HMCSIM PHYSICAL LINK SPEED
# 0 = 15Gpbs
# 1 = 10Gpbs
# -DHMC_PHY_SPEED={0,1}
#	
#---------------------------------------------------------

#-- HMCSIM_MACROS
#-- HMCSIM BUILD MACROS
HMCSIM_MACROS=

#-- CC
#-- C COMPILER
CC=gcc

#-- CXX
#-- C++ COMPILER
CXX=g++

#-- INC
#-- INCLUDE DIRECTORIES
INC=-I./include/

#-- CFLAGS
#-- C COMPILER FLAGS
CFLAGS=-O0 -Wall -g $(INC) $(HMCSIM_MACROS)

#-- CXXFLAGS
#-- C++ COMPILER FLAGS
CXXFLAGS=-O2 -Wall -g $(INC) $(HMCSIM_MACROS)

#-- AR
#-- ARCHIVE BINARY
AR=ar

#-- AR_OPTS
#-- ARCHIVE OPTIONS
AR_OPTS=rcs

#-- DOXYGEN
#-- DOXYGEN BINARY
DOXYGEN=`which doxygen`

#-- EOF 
