
include scripts/Makefile

define make_module_com 
$(MAKE) -C $(1)
endef

define make_module_cl
$(MAKE) clean -C $(1)
endef

.PHONY: all mco mcl clean

all:$(Target)

$(Target):mco
	$(CXX) $(CXXFLAGS) $(INC) -o $@ $(allobjs) $(DEPS)
	@echo "generate target done"

mco:
	@echo "modules: $(modules)"
	@echo $(foreach mo,$(modules),$(eval $(call make_module_com,$(mo))))
	@echo "modules compile done"

mcl:
	@echo $(foreach mo,$(modules),$(eval $(call make_module_cl,$(mo))))
	@echo "modules clean done"


clean:
	$(RM) $(Target)
	@echo "clean target done"
