#!/bin/sh
#
# HMCSIM CMC MAKEFILE
#
# Drives building all the cmc libraries


SUBDIRS :=	$(shell find ./ -maxdepth 1 -mindepth 1 -type d)

CLEANDIRS :=	$(addsuffix .clean, $(SUBDIRS))

INSTALLDIRS :=	$(addsuffix .install, $(SUBDIRS))

all:$(SUBDIRS)
$(SUBDIRS):
	@echo " Building CMC Library in $(@)"; $(MAKE) -C $@
.PHONY: all $(SUBDIRS)

clean: $(CLEANDIRS)
$(CLEANDIRS):
	@echo " Cleaning CMC Library in $(@)"; $(MAKE) -C $(basename $@) clean
.PHONY: clean $(CLEANDIRS)

install: $(INSTALLDIRS)
$(INSTALLDIRS):
	@echo " Installing CMC Library in $(@)"; $(MAKE) -C $(basename $@) clean
.PHONY: install $(INSTALLDIRS)


