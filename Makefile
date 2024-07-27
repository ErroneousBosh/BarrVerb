###############################
#
# Makefile for BarrVerb
# based on the work of falkTX
# 
# for full licence, see LICENCE in the root of the project
#
###############################

include dpf/Makefile.base.mk

all: plugins gen

plugins: 
	$(MAKE) all -C plugin

ifneq ($(CROSS_COMPILING),true)
gen: plugins dpf/utils/lv2_ttl_generator
	@$(CURDIR)/dpf/utils/generate-ttl.sh
ifeq ($(MACOS),true)
	@$(CURDIR)/dpf/utils/generate-vst-bundles.sh
endif

dpf/utils/lv2_ttl_generator:
	$(MAKE) -C dpf/utils/lv2-ttl-generator
else
gen:
endif

clean:
	$(MAKE) clean -C dpf/utils/lv2-ttl-generator
	 $(MAKE) clean -C plugin
	rm -rf bin build

.PHONY: plugins
