# installation directory of INET
INETDIR ?= $(PWD)/../inet/

OUT ?= $(PWD)/out/

# the header files from the INET source directory
# are copied into this directory, so that only
# one has to be added to the include path
INETHEADERS := $(OUT)/inet_includes

.PHONY: all
all: checkmakefiles
	cd src && $(MAKE)

.PHONY: clean
clean: checkmakefiles
	cd src && $(MAKE) clean

.PHONY: cleanall
cleanall: checkmakefiles
	cd src && $(MAKE) MODE=release clean
	cd src && $(MAKE) MODE=debug clean
	rm -f src/Makefile
	rm -rf documentation
	rm -rf out
	rm -rf simulation/results

.PHONY: makefiles
makefiles:
	@echo "=========================="
	@echo "collecting INET headers..."
	@echo "=========================="
	rm -rf $(INETHEADERS)
	mkdir -p $(INETHEADERS)
	find $(INETDIR)/src -name '*.h' -exec cp {} $(INETHEADERS) ';'

	@echo "===================="
	@echo "calling opp_makemake"
	@echo "===================="
	cd src && opp_makemake --force --deep --make-so -o ptpsim -O $(OUT) -I$(INETHEADERS) $$NSC_VERSION_DEF

.PHONY: checkmakefiles
checkmakefiles:
	@if [ ! -f src/Makefile ]; then \
	echo; \
	echo '======================================================================='; \
	echo 'src/Makefile does not exist. Please use "make makefiles" to generate it!'; \
	echo '======================================================================='; \
	echo; \
	exit 1; \
	fi

.PHONY: run-sim
run-sim:
	opp_run -l $(INETDIR)/src/inet -l src/ptpsim -n $(INETDIR)/src:src:simulation simulation/omnetpp.ini

.PHONY: doc
doc:
	doxygen Doxyfile

.PHONY: tar
tar:
	git archive --format=tar master | gzip > ../hardwareclock_`date '+%Y-%m-%d_%H_%M'`.tar.gz
