
# HMCSIM TOP-LEVEL MAKEFILE
# GOBLIN-CORE 64
#

include Makefile.inc

PKGS := ...
LIBNAME := hmcsim
SRCDIR := src
CMCDIR := ./cmc
BUILDDIR := build
SHBUILDDIR := shbuild
CXXBUILDDIR := cxxbuild
CXXSHBUILDDIR := cxxshbuild
LIBS :=
TARGET := lib$(LIBNAME).a
SHTARGET := lib$(LIBNAME).so
LDFLAGS :=
ARFLAGS := rcs
SHLIB := -shared

.PHONY : test tools cmc viz

SRCEXT = c
CXXSRCEXT = cc
CXXSOURCES := $(shell find $(SRCDIR) -type f -name *.$(CXXSRCEXT))
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
SHOBJECTS := $(patsubst $(SRCDIR)/%,$(SHBUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CXXOBJECTS := $(patsubst $(SRCDIR)/%,$(CXXBUILDDIR)/%,$(CXXSOURCES:.$(CXXSRCEXT)=.o))
CXXSHOBJECTS := $(patsubst $(SRCDIR)/%,$(CXXSHBUILDDIR)/%,$(CXXSOURCES:.$(CXXSRCEXT)=.o))
DEPS := $(OBJECTS:.o=.deps)
DEPS := $(CXXOBJECTS:.o=.deps)

all: $(TARGET) $(SHTARGET)

$(SHTARGET): $(SHOBJECTS) $(CXXSHOBJECTS)
	@echo " Linking Shared Lib..."; $(CC) -shared -o $(SHTARGET) $(SHOBJECTS) $(CXXSHOBJECTS) -ldl
	@echo " Building CMC Libs..."; make -C ./cmc/

$(TARGET): $(OBJECTS) $(CXXOBJECTS)
	@echo " Linking Static Lib..."; $(AR) $(AR_OPTS) $(TARGET) $(OBJECTS) $(CXXOBJECTS)
	@echo " Building CMC Libs..."; make -C ./cmc/

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " CC $<"; $(CC) $(CFLAGS) -MD -MF $(@:.o=.deps) -c -o $@ $< -ldl

$(SHBUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(SHBUILDDIR)
	@echo " CC $<"; $(CC) $(CFLAGS) -MD -MF $(@:.o=.deps) -c -fpic -o $@ $< -ldl

$(CXXBUILDDIR)/%.o: $(SRCDIR)/%.$(CXXSRCEXT)
	@mkdir -p $(CXXBUILDDIR)
	@echo " CXX $<"; $(CXX) $(CXXFLAGS) -MD -MF $(@:.o=.deps) -c -o $@ $< -ldl

$(CXXSHBUILDDIR)/%.o: $(SRCDIR)/%.$(CXXSRCEXT)
	@mkdir -p $(CXXSHBUILDDIR)
	@echo " CXX $<"; $(CXX) $(CXXFLAGS) -MD -MF $(@:.o=.deps) -c -fpic -o $@ $< -ldl

docs:
	@echo " Building Docs..."; $(DOXYGEN) ./doxygen/hmc_sim.cfg
doclean:
	@echo " Cleaning Docs..."; $(RM) -Rf ./doxygen/{html,latex,man,rtf,xml}
clean: doclean
	@echo " Cleaning..."; $(RM) -r $(BUILDDIR) $(SHBUILDDIR) $(TARGET) $(SHTARGET)
test: $(TARGET) $(SHTARGET)
	@echo " Building Tests..."; make -C ./test/
viz:
	@echo " Building Viz Tools..."; make -C ./viz/
cmc:
	@echo " Building CMC Libs..."; make -C ./cmc/
tools:
	@echo " Building Tools..."; make -C ./tools/
cmcclean:
	@echo " Cleaning CMC..."; make -C ./cmc/ clean
vizclean:
	@echo " Cleaning Viz..."; make -C ./viz/ clean
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
	install cmc/amo_popcount/libamopopcount.so $(PREFIX)/cmc/
	install cmc/fullempty/ClrXX/libclrxx.so $(PREFIX)/cmc/
	install cmc/fullempty/IncFF/libincff.so $(PREFIX)/cmc/
	install cmc/fullempty/ReadEF/libreadef.so $(PREFIX)/cmc/
	install cmc/fullempty/ReadFE/libreadfe.so $(PREFIX)/cmc/
	install cmc/fullempty/ReadFF/libreadff.so $(PREFIX)/cmc/
	install cmc/fullempty/ReadXX/libreadxx.so $(PREFIX)/cmc/
	install cmc/fullempty/WriteEF/libwriteef.so $(PREFIX)/cmc/
	install cmc/fullempty/WriteFF/libwriteff.so $(PREFIX)/cmc/
	install cmc/fullempty/WriteXE/libwritexe.so $(PREFIX)/cmc/
	install cmc/fullempty/WriteXF/libwritexf.so $(PREFIX)/cmc/
	install cmc/mutex/hmc_lock/libhmc_lock.so $(PREFIX)/cmc/
	install cmc/mutex/hmc_trylock/libhmc_trylock.so $(PREFIX)/cmc/
	install cmc/mutex/hmc_unlock/libhmc_unlock.so $(PREFIX)/cmc/
	install cmc/template/libcmctemplate.so $(PREFIX)/cmc/
	ln -fs $(PREFIX)/lib/libhmcsim.a $(PREFIX)/libhmcsim.a
	ln -fs $(PREFIX)/lib/libhmcsim.so $(PREFIX)/libhmcsim.so

-include $(DEPS)

.PHONY: clean
