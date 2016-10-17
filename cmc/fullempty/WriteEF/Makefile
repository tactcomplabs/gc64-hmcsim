#!/bin/sh
#
# HMC-SIM 2.0 CMC TEMPLATE MAKEFILE
#
#------------------------------------

include ../Makefile.inc

#-- CHANGE THE NAME OF YOUR LIBRARY HERE
LIBNAME := writeef
#---------------------------------------

SRCDIR := .
BUILDDIR := build
TARGET := lib$(LIBNAME).so

CFLAGS := $(CFLAGS) -fPIC

SRCEXT = c
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
DEPS := $(OBJECTS:.o=.deps)

$(TARGET): $(OBJECTS)
	@echo " Linking... $(TARGET)";  $(CC) -shared -o $(TARGET) $(OBJECTS)
$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " CC $<"; $(CC) $(CFLAGS) -MD -MF $(@:.o=.deps) -c -o $@ $<
clean:
	@echo " Cleaning..."; $(RM) -r $(BUILDDIR) $(TARGET)
install: $(TARGET)
	@echo " Installing $(TARGET)"; install $(TARGET) $(PREFIX)/cmc/
-include $(DEPS)

.PHONY: clean install

#--EOF
