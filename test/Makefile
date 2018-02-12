#!/bin/sh
#
# HMCSIM TEST MAKEFILE
#
# Drives building all the hmcsim tests


SUBDIRS :=	simple simple_cmc hmc_physrand decode_physrand stream stream_power mutex mutex_linear sst_init fe_linear fe_tree load_fe gups stream_power_config stream_power_tecplot simple_api dyn_cmc

CLEANDIRS :=	$(addsuffix .clean, $(SUBDIRS))

all:$(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@
.PHONY: all $(SUBDIRS) 

clean: $(CLEANDIRS) 
$(CLEANDIRS): 
	$(MAKE) -C $(basename $@) clean
.PHONY: clean $(CLEANDIRS)

