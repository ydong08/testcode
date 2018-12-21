
include ../scripts/Makefile

#.PHONY: all clean

$(objs): %.o : %.cc
	$(CXX) -c $(CXXFLAGS) $(INC) $< -o $@

clean:
	$(RM) $(objs)