
# HMCSIM TOP-LEVEL MAKEFILE
# GOBLIN-CORE 64
#

include Makefile.inc

PKGS := ...
LIBNAME := hmcsim
SRCDIR := src
CMCDIR := cmc
BUILDDIR := build
SHBUILDDIR := shbuild
LIBS :=
TARGET := lib$(LIBNAME).a
SHTARGET := lib$(LIBNAME).so
LDFLAGS :=
ARFLAGS := rcs
SHLIB := -shared

.PHONY : test tools cmc

SRCEXT = c
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
SHOBJECTS := $(patsubst $(SRCDIR)/%,$(SHBUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
DEPS := $(OBJECTS:.o=.deps)

all: $(TARGET) $(SHTARGET)

$(SHTARGET): $(SHOBJECTS)
	@echo " Linking Shared Lib..."; $(CC) -shared -o $(SHTARGET) $(SHOBJECTS) -ldl
	@echo " Building CMC Libs..."; make -C ./cmc/

$(TARGET): $(OBJECTS)
	@echo " Linking Static Lib..."; $(AR) $(AR_OPTS) $(TARGET) $(OBJECTS)
	@echo " Building CMC Libs..."; make -C ./cmc/

$(SHBUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(SHBUILDDIR)
	@echo " CC $<"; $(CC) $(CFLAGS) -MD -MF $(@:.o=.deps) -c -fpic -o $@ $< -ldl

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " CC $<"; $(CC) $(CFLAGS) -MD -MF $(@:.o=.deps) -c -o $@ $< -ldl

docs:
	@echo " Building Docs..."; $(DOXYGEN) ./doxygen/hmc_sim.cfg
doclean:
	@echo " Cleaning Docs..."; $(RM) -Rf ./doxygen/{html,latex,man,rtf,xml}
clean: doclean
	@echo " Cleaning..."; $(RM) -r $(BUILDDIR) $(TARGET)
test: $(TARGET) $(SHTARGET)
	@echo " Building Tests..."; make -C ./test/
cmc:
	@echo " Building CMC Libs..."; make -C ./cmc/
tools:
	@echo " Building Tools..."; make -C ./tools/
cmcclean:
	@echo " Cleaning CMC..."; make -C ./cmc/ clean
toolsclean:
	@echo " Cleaning Tools..."; make -C ./tools/ clean
testclean:
	@echo " Cleaning Tests..."; make -C ./test/ clean
distclean: clean testclean doclean toolsclean cmcclean
install: $(TARGET) $(SHTARGET) cmc 
	@echo " Installing HMC-Sim...";
	@echo " Building Directory Structure...";
	mkdir -p $(PREFIX)/include
	mkdir -p $(PREFIX)/bin
	mkdir -p $(PREFIX)/lib
	mkdir -p $(PREFIX)/cmc
	@echo " Installing libhmcsim.a..."; install ./libhmcsim.a $(PREFIX)/lib/
	@echo " Installing libhmcsim.so..."; install ./libhmcsim.so $(PREFIX)/lib/
	@echo " Installing headers..."; install ./include/*.h $(PREFIX)/include/
	@echo " Installing CMC libs...";
	set -e; for a in $(shell find $(CMCDIR) -type f -name *.so); do install $$a $(PREFIX)/cmc/; done
	ln -s $(PREFIX)/lib/libhmcsim.a $(PREFIX)/libhmcsim.a
	ln -s $(PREFIX)/lib/libhmcsim.so $(PREFIX)/libhmcsim.so

-include $(DEPS)

.PHONY: clean
