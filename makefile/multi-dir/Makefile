
include scripts/Makefile

# variable
CXXFLAGS := -g -Wall -O3
OBJPATH := out
INC := 

# source files
sources := $(foreach m,$(modules),$(wildcard $(m)/src/*.cc))
puresrc := $(notdir $(sources))
objects := $(addprefix $(OBJPATH)/, $(puresrc:%.cc=%.o))

# compile function
define create_objs
$$($(1):%.cc=%.o):$(1)
	@echo "$<: $$<"
	@echo "$@: $$@"
	$$(CXX) $$(CXXFLAGS) $$(INC) $$< -o $$(OBJPATH)/$$(notdir $$@)
endef

# link 
.PHONY: all clean

all:
	@echo sources: $(sources)
	@echo puresrc: $(puresrc)
	@echo objects: $(objects)
	@echo 
	$(foreach s,$(sources),$(eval $(call create_objs,$(s))))
	$(CXX) $(CXXFLAGS) -o $@ $(objects) 

clean:
	-rm -rf $(objects) all


