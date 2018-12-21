
include scripts/Makefile

make_module_com := $(MAKE) -C $($(1));
make_module_cl := $(MAKE) clean -C $($(1));

.PHONY: all mco mcl clean

all:$(Target)

$(Target):mco
	$(CXX) $(CXXFLAGS) $(INC) -o $@ $(allobjs) $(DEPS)
	@echo "generate target done"

mco:
	@echo "modules: $(modules)"
	$(foreach mo, $(modules), $(eval $(call $(make_module_com), $(mo))))
	@echo "modules compile done"

mcl:
	$(foreach mo, $(modules), $(eval $(call $(make_module_cl), $(mo))))
	@echo "modules clean done"


clean:
	$(RM) $(Target)
	@echo "clean target done"
