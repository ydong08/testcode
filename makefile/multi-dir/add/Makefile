
include ../scripts/Makefile

.PHONY: all

all: $(objs)
	@echo "$(sources)"
	@echo "$(objs)"

$(objs): %.o : %.cc
	$(CXX) -c $(CXXFLAGS) $(INC) $< -o $@

clean:
	$(RM) $(objs)