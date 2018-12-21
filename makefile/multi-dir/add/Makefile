
include ../scripts/Makefile

.PHONY: all

all: $(objs)
	@echo "sources: $(sources)"
	@echo "objs: $(objs)"

$(objs): %.o : %.cc
	$(CXX) -c $(CXXFLAGS) $(INC) $< -o $@

clean:
	$(RM) $(objs)
