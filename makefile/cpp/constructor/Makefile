
# := 先定义再使用
# = 可以先使用,后面定义
INC := -I. -Ibase -Isources
CDEFS := -DMAKEFILES -DTEST_MK=6
CXXFLAG := -g -Wall -std=c++11
CXXFLAG += ${CDEFS}
LINKLIB := -lpthread -lrt

vpath % .
vpath %.inc base
vpath %.cc sources
	
#	@echo "$(who)"
#	@echo "$(curpath)"

OBJ= main.o base.o constructor.o
#all: $(OBJ)
#        @echo "$(CXX)"
#        $(CXX) $(CXXFLAG) $^ -o $@ $(LINKLIB)
#$(OBJ): %.o : %.cc
#        $(CXX) -c $(CXXFLAG) $(INC) $< -o $@
#        @echo "Tar: $@, first dep: $<, all dep: $^"

all: $(OBJ)
	@echo "$(CXX)"
	$(CXX) $(CXXFLAG) $(INC) $^ -o $@ $(LINKLIB)

%.o: %.cc
	$(CXX) -c $(CXXFLAG) $(INC) $< -o $@
	@echo "Tar: $@, first dep: $<, all dep: $^"

clean:
	-rm -rf main
	-rm -rf *.o

.PHONY: clean all
