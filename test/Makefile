#!/bin/sh
# 
# HMCSIM TEST MAKEFILE 
# 
# Drives building all the hmcsim tests


SUBDIRS :=	simple hmc_physrand decode_physrand 1dfft 2dfft stream

CLEANDIRS :=	$(addsuffix .clean, $(SUBDIRS))

all:$(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@
.PHONY: all $(SUBDIRS) 

clean: $(CLEANDIRS) 
$(CLEANDIRS): 
	$(MAKE) -C $(basename $@) clean
.PHONY: clean $(CLEANDIRS)

