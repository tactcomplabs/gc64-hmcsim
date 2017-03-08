#!/bin/sh
#
# HMCSIM SIMPLE_API TEST MAKEFILE
#
# Drives building all the hmcsim simple api tests


SUBDIRS :=	simple4GB simple8GB

CLEANDIRS :=	$(addsuffix .clean, $(SUBDIRS))

all:$(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@
.PHONY: all $(SUBDIRS)

clean: $(CLEANDIRS)
$(CLEANDIRS):
	$(MAKE) -C $(basename $@) clean
.PHONY: clean $(CLEANDIRS)

